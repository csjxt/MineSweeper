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


	/*출력 테스트 



	1,2,3번 역할 담당이 밑내용 삭제후 코드 작성 



	*/

	printf("\n정답보드\n\n");
	printf("    ");
	for (int j = 0; j < col; j++) {
		printf("%2d ", j + 1);
	}
	printf("\n");

	for (int i = 0; i < row; i++) {

		printf("%2d | ", i + 1);

		for (int j = 0; j < col; j++) {

			if (board[i][j] == MINE) {
				printf(" * "); // 지뢰 표시
			}
			else {
				printf(" %d ", board[i][j]);
			}
		}
		printf("\n");
	}

	printf("\n");
	printf("선택한 좌표 (%d행 %d열)의 값: %d\n\n", first_row + 1, first_col + 1, board[first_row][first_col]);

	return 0;
}