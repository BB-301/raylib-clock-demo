/*
Copyright (c) 2024 BB-301 <fw3dg3@gmail.com> [Official repository](https://github.com/BB-301/raylib-clock-demo)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <raylib.h>
#include <time.h>

#define TITLE_AM "Clock (AM)"
#define TITLE_PM "Clock (PM)"

typedef struct
{
    time_t raw;
    struct tm *info;
} my_time;

typedef struct
{
    Camera2D seconds;
    Camera2D minutes;
    Camera2D hours;
    my_time time_tracker;
    int am;
} my_clock_hands;

static void my_time_update(my_time *t);
static void my_clock_hands_init(my_clock_hands *h, int screen_center);
static void my_clock_hands_refresh(my_clock_hands *h);

int main(void)
{
    const int screen = 400;
    const int center = screen / 2;
    const int radius_screw = 6;
    const int hand_offset = 30;

    const int radius_tick_big = 4;
    const int radius_tick_small = 2;

    const int hand_length_seconds = center;
    const int hand_thickness_seconds = 2;

    const int hand_length_minutes = center;
    const int hand_thickness_minutes = 4;

    const int hand_length_hours = hand_length_minutes * 0.7;
    const int hand_thickness_hours = 6;

    my_clock_hands hands;
    my_clock_hands_init(&hands, center);

    InitWindow(screen, screen, hands.am ? TITLE_AM : TITLE_PM);
    SetTargetFPS(60);

    Camera2D ticks = {
        .zoom = 1.0f,
        .rotation = 0,
        .offset = (Vector2){.x = center, .y = center},
        .target = (Vector2){.x = 0, .y = 0}};

    while (!WindowShouldClose())
    {
        my_clock_hands_refresh(&hands);

        BeginDrawing();
        ClearBackground(WHITE);

        SetWindowTitle(hands.am ? TITLE_AM : TITLE_PM);

        for (int i = 0; i < 60; i++)
        {
            ticks.rotation = i * 6;
            int radius = i % 5 == 0 ? radius_tick_big : radius_tick_small;
            BeginMode2D(ticks);
            DrawCircle(0, center - (hand_offset / 2), radius, BLACK);
            EndMode2D();
        }

        BeginMode2D(hands.hours);
        DrawRectangle(
            center - hand_thickness_hours / 2,
            center - hand_thickness_hours / 2 - hand_length_hours + hand_offset,
            hand_thickness_hours,
            hand_length_hours,
            BLACK);
        EndMode2D();

        BeginMode2D(hands.minutes);
        DrawRectangle(
            center - hand_thickness_minutes / 2,
            center - hand_thickness_minutes / 2 - hand_length_minutes + hand_offset,
            hand_thickness_minutes,
            hand_length_minutes,
            BLACK);
        EndMode2D();

        BeginMode2D(hands.seconds);
        DrawRectangle(
            center - hand_thickness_seconds / 2,
            center - hand_thickness_seconds / 2 - hand_length_seconds + hand_offset,
            hand_thickness_seconds,
            hand_length_seconds,
            ORANGE);
        EndMode2D();

        DrawCircle(center, center, radius_screw, BLACK);
        DrawCircle(center, center, radius_screw * 0.5, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

static void my_time_update(my_time *t)
{
    time(&t->raw);
    t->info = localtime(&t->raw);
}

static void my_clock_hands_init(my_clock_hands *h, int screen_center)
{
    h->seconds = (Camera2D){
        .zoom = 1.0f,
        .rotation = 0,
        .offset = (Vector2){.x = screen_center, .y = screen_center},
        .target = (Vector2){.x = screen_center, .y = screen_center}};
    h->minutes = (Camera2D){
        .zoom = 1.0f,
        .rotation = 0,
        .offset = (Vector2){.x = screen_center, .y = screen_center},
        .target = (Vector2){.x = screen_center, .y = screen_center}};
    h->hours = (Camera2D){
        .zoom = 1.0f,
        .rotation = 0,
        .offset = (Vector2){.x = screen_center, .y = screen_center},
        .target = (Vector2){.x = screen_center, .y = screen_center}};
}

static void my_clock_hands_refresh(my_clock_hands *h)
{
    my_time_update(&h->time_tracker);
    float sec = h->time_tracker.info->tm_sec;
    float min = h->time_tracker.info->tm_min;
    float hour = h->time_tracker.info->tm_hour;
    h->seconds.rotation = sec * 6;
    h->minutes.rotation = (min * 60 + sec) / 10;
    h->hours.rotation = (((float)((int)hour % 12) * 60 * 60) + (min * 60) + sec) / 120;
    h->am = hour < 12 ? 1 : 0;
}
