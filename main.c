#include <windows.h>
#include <conio.h>   // _getch()
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#pragma warning (disable:4996)

/* 콘솔 기본 커서(흰색 |) 숨기기/보이기 */
static void hide_console_caret(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(h, &ci);
    ci.bVisible = FALSE;
    SetConsoleCursorInfo(h, &ci);
}
static void show_console_caret(void) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    GetConsoleCursorInfo(h, &ci);
    ci.bVisible = TRUE;
    SetConsoleCursorInfo(h, &ci);
}

/* Enter 키를 누를 때까지 대기 (다른 키는 무시) */
static void wait_for_enter(void) {
    for (;;) {
        int ch = _getch();
        if (ch == '\r' || ch == '\n') break;   // Enter
        // 그 외 키는 무시
    }
}

int main(void) {
    int first_row, first_col;

    srand((unsigned)time(NULL));

    levelSelect();
    reset();

    // 첫 클릭 좌표 입력 (지뢰 배치에 반영)
    first_row = exception("첫 번째로 열 행", 1, row);
    first_col = exception("첫 번째로 열 열", 1, col);
    first_row--; first_col--;

    mineSet(first_row, first_col);
    near8space();
    open8space(first_row, first_col);

    cursor_init(0, 0);
    timer_start();

    setvbuf(stdout, NULL, _IONBF, 0); // 출력 지연 제거
    hide_console_caret();             // 시스템 커서 숨김

    // 시작 화면 1회 출력
    draw_board_with_cursor_and_status();

    for (;;) {
        int dx = 0, dy = 0;
        Action act = read_action(&dx, &dy, col, row);

        if (act == ACT_MOVE) {
            cursor_move(dx, dy, col, row);
            draw_board_with_cursor_and_status();
            continue;
        }
        else if (act == ACT_OPEN) {
            int y = g_cursor.y;
            int x = g_cursor.x;

            
            if (show_board[y][x] == OPEN) {
                ui_set_message("이미 연 칸입니다. 다른 칸을 선택하세요.\n계속하시려면 Enter를 누르세요.");
                draw_board_with_cursor_and_status();
                wait_for_enter();      // Enter를 누를 때까지 멈춤
                ui_clear_message();    // 계속 진행할 때 메시지 해제
                draw_board_with_cursor_and_status();
                continue;
            }

            // 아직 안 연 칸이면 열기
            ui_clear_message();        // 이전 경고 메시지 있으면 제거
            open8space(y, x);
            draw_board_with_cursor_and_status();

            // 게임 종료/승패 판정
            if (WinOrLose(y, x) == 0) {
                timer_stop();
                break;
            }
            if (hidden_count == mine_count) {
                timer_stop();
                ui_set_message("모든 지뢰를 찾았습니다! 게임 종료!");
                draw_board_with_cursor_and_status();
                break;
            }
            continue;
        }

        // 다른 액션은 없으니 루프 계속
    }

    show_console_caret(); // 커서 복원
    return 0;
}


