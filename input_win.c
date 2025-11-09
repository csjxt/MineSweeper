#include <stdio.h>
#include <conio.h>   // _getch() 사용
#include "common.h"

Cursor g_cursor = { 0, 0 };

void cursor_init(int sx, int sy) { g_cursor.x = sx; g_cursor.y = sy; }

void cursor_move(int dx, int dy, int W, int H) {
    int nx = g_cursor.x + dx, ny = g_cursor.y + dy;
    if (nx < 0) nx = 0;
    if (ny < 0) ny = 0;
    if (nx >= W) nx = W - 1;
    if (ny >= H) ny = H - 1;
    g_cursor.x = nx; g_cursor.y = ny;
}

Action read_action(int* dx, int* dy, int W, int H) {
    int c = _getch(); // 한 글자 입력
    switch (c) {
    case 'w': case 'W': *dx = 0; *dy = -1; return ACT_MOVE;
    case 's': case 'S': *dx = 0; *dy = +1; return ACT_MOVE;
    case 'a': case 'A': *dx = -1; *dy = 0; return ACT_MOVE;
    case 'd': case 'D': *dx = +1; *dy = 0; return ACT_MOVE;
    case ' ': return ACT_OPEN;
    default:  return ACT_NONE;
    }
}