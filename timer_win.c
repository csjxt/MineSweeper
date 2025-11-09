#include <windows.h>
#include <stdio.h>
#include "common.h"

static LARGE_INTEGER t0, t1, freq;
static int running = 0;

void timer_start(void) {
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);
    running = 1;
}

void timer_stop(void) {
    QueryPerformanceCounter(&t1);
    running = 0;
}

double timer_elapsed_sec(void) {
    LARGE_INTEGER now = running ? (LARGE_INTEGER) { 0 } : t1;
    if (running) QueryPerformanceCounter(&now);
    return (double)(now.QuadPart - t0.QuadPart) / (double)freq.QuadPart;
}

void timer_print_mmss(void) {
    double s = timer_elapsed_sec();
    int mm = (int)(s / 60.0);
    int ss = (int)s % 60;
    printf("%02d:%02d", mm, ss);
}
