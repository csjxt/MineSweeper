#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#pragma warning (disable:4996)

int main(void) {

    int first_row, first_col;

    srand((unsigned int)time(NULL)); // 경고 제거용

    levelSelect();
    reset();

    first_row = exception("첫 번째로 열 행", 1, row);
    first_col = exception("첫 번째로 열 열", 1, col);

    first_row--;  
    first_col--;

    mineSet(first_row, first_col);
    near8space();

    open8space(first_row, first_col); 

    startTimer(); // 첫 클릭 후부터 시간을 재기 시작

    int user_row, user_col; 
    int action; //선택 변수 (1: 열기, 2: 깃발)

    for (;;) {
        printf("\033[2J");
        printf("\033[H");
        boardPrint();

        action = exception("행동 선택 (1.열기 2.깃발)", 1, 2);

        getUserInput(&user_row, &user_col);

        user_row--; 
        user_col--;

        //깃발모드
        if (action == 2) {
            toggleFlag(user_row, user_col);
            // 깃발만 꽂고 루프 다시 시작 (승패 판정 불필요)
            continue;
        }

        //열기모드
        // 깃발이 꽂혀있는 칸은 열지 못하도록 보호
        if (show_board[user_row][user_col] == FLAGGED) {
            printf("깃발이 꽂힌 칸입니다. 깃발을 해제하고 여세요.\n");
            printf("계속 하려면 Enter를 누르세요.");
            ClearBuffer();
            continue;
        }

        if (show_board[user_row][user_col] == OPEN) {
            printf("이미 연 칸입니다. 다른 칸을 선택하세요.\n");
            printf("계속 하려면 Enter를 누르세요.");
            ClearBuffer();
            continue;
        }

        open8space(user_row, user_col);

        if (WinOrLose(user_row, user_col) == 0) {
            int time_used = getElapsedTime();
            printf("총 걸린 시간: %d초\n", time_used);
            break;
        }

        else {
            int time_used = getElapsedTime();
            printf("현재 시간: %d초\n", time_used);
            printf("계속 하려면 Enter를 누르세요.");
            ClearBuffer();
        }
    }
    return 0;
}