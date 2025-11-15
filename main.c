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

    for (;;) {
        printf("\033[2J");
        printf("\033[H");
        boardPrint();

        getUserInput(&user_row, &user_col);

        user_row--; 
        user_col--;

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