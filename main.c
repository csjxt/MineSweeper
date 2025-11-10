#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif
#include "common.h"
#pragma warning (disable:4996)

int main(void) {

    int first_row, first_col; // 첫 번째 클릭

    int cur_r = 0, cur_c = 0;        // 현재 커서(0-based)
    time_t start_time = 0;           // 기준 시각(프로그램 시작)
    int last_space_sec = 0;          // 마지막 Space 시각(초) — 흐르지 않음
    int show_msg = 0;                // 이미 연 칸 알림 표시 여부

    srand((unsigned)time(NULL));

    levelSelect();
    reset();

    // 첫 클릭 입력
    first_row = exception("첫 번째로 열 행", 1, row);
    first_col = exception("첫 번째로 열 열", 1, col);
    first_row--; first_col--;

    mineSet(first_row, first_col);
    near8space();
    open8space(first_row, first_col);

    // 커서 시작 위치
    cur_c = 0;

    // 기준 시각 저장
    start_time = time(NULL);

    // 화면 정리
    printf("\033[2J\033[H");

    // 화면 위치 보정값 
    const int TOP = 5;  
    const int LEFT = 6;  
    const int CELLW = 3;

    for (;;) {
        // 1) 화면 그리기 시작
        printf("\033[H\x1b[?25l");
        boardPrint();

        // 승리 조기 종료 처리
        if (hidden_count == mine_count) {
            printf("\033[%d;1H", TOP + row + 2);
            printf("지뢰 위치가 특정되어 게임이 끝났습니다.\n");
            // 마지막에 커서만 보드 칸으로 보여주고 종료
            int screenRow = TOP + cur_r;
            int screenCol = LEFT + cur_c * CELLW + 2;
            printf("\033[%d;%dH\x1b[?25h", screenRow, screenCol);
            break;
        }

        // 2) HUD 출력
        int elapsed = last_space_sec; 
        printf("\033[%d;1H", TOP + row + 2);
        printf("Cursor : (%d, %d)\n", cur_r + 1, cur_c + 1);
        printf("Time   : %02d:%02d\n", elapsed / 60, elapsed % 60);
        printf("[WASD] 이동  [Space] 열기\n");
        if (show_msg) {
            printf("이미 연 칸입니다. 다른 칸을 선택하세요.\n계속 하려면 Enter를 누르세요.\n");
        }
        else {
            printf("\n\n"); 
        }

        // 3) 커서를 보드 칸의 숫자 오른쪽에 보이게
        int screenRow = TOP + cur_r;
        int screenCol = LEFT + cur_c * CELLW + 2;
        printf("\033[%d;%dH\x1b[?25h", screenRow, screenCol);
        fflush(stdout);

        // 4) 입력 처리
#ifdef _WIN32
        if (_kbhit()) {
            int ch = _getch();

            // 알림 중이면 Enter만 받기 (즉시 지우기)
            if (show_msg) {
                if (ch == '\r' || ch == '\n') {
                    show_msg = 0;
                    // 알림이 출력된 두 줄을 즉시 지움
                    printf("\033[%d;1H\033[2K\033[1B\033[2K", TOP + row + 5);
                    // 커서를 다시 보드 칸 위치로 복귀
                    int screenRow = TOP + cur_r;
                    int screenCol = LEFT + cur_c * CELLW + 2;
                    printf("\033[%d;%dH", screenRow, screenCol);
                    fflush(stdout);
                }
                continue;

            }

            if (ch == 'w' || ch == 'W') { if (cur_r > 0)        cur_r--; }
            else if (ch == 's' || ch == 'S') { if (cur_r < row - 1) cur_r++; }
            else if (ch == 'a' || ch == 'A') { if (cur_c > 0)        cur_c--; }
            else if (ch == 'd' || ch == 'D') { if (cur_c < col - 1) cur_c++; }
            else if (ch == ' ') {
                // Space 누른 순간의 시간 저장
                last_space_sec = (int)(time(NULL) - start_time);

                if (show_board[cur_r][cur_c] == OPEN) {
                    show_msg = 1;
                }
                else {
                    if (board[cur_r][cur_c] != MINE) {
                        open8space(cur_r, cur_c);
                    }
                    if (WinOrLose(cur_r, cur_c) == 0)
                        break;
                }
            }
        }
        Sleep(10);
#endif
    }

    //커서 보이게 복구
    printf("\x1b[?25h");
    return 0;
}
