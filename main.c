#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#pragma warning (disable:4996)

int main(void) {

	int first_row, first_col;	//첫번째 클릭(바로 패배 방지)

	srand(time(NULL));

	levelSelect();
	reset();

	first_row = exception("첫 번째로 열 행", 1, row);
	first_col = exception("첫 번째로 열 열", 1, col);

	first_row--;	//0부터 시작하게 변경
	first_col--;

	mineSet(first_row, first_col);
	near8space();

	open8space(first_row, first_col); // 첫 클릭 시 주변 비어있는 칸 오픈

	int user_row, user_col; //행 열 입력받는 변수

	for (;;) {
		printf("\033[2J");
		printf("\033[H");
		boardPrint();

		if (hidden_count == mine_count) { // 커스텀 난이도에서 첫 클릭으로 인한 빈칸 열림 때문에 지뢰 위치가 바로 특정되는 경우
			printf("지뢰 위치가 특정되어 게임이 끝났습니다.\n");
			break;
		}

		user_row = exception("열 행 입력", 1, row);
		user_col = exception("열 열 입력", 1, col);

		user_row--;	//인덱스 값으로 변경
		user_col--;

		if (show_board[user_row][user_col] == OPEN) {
			printf("이미 연 칸입니다. 다른 칸을 선택하세요.\n");
			printf("계속 하려면 Enter를 누르세요.");
			while (getchar() != '\n');
			continue;
		}
		if (board[user_row][user_col] != MINE) {
			show_board[user_row][user_col] = OPEN;
			hidden_count--;
		}

		if (WinOrLose(user_row, user_col) == 0)
			break;

		printf("\n");
	}


		return 0;
}