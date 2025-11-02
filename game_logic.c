#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#pragma warning (disable:4996)

int row;
int col;
int mine_count;
int board[MAX_ROW][MAX_COL];
int show_board[MAX_ROW][MAX_COL];

void ClearBuffer(void) {
	int input;
	while ((input = getchar()) != '\n' && input != EOF) {

	}
}

int exception(const char* message, int min, int max) {

	int value;	//사용자 입력값
	int next;	//사용자 입력 다음값

	for (;;) {
		printf("%s %d부터 %d까지: ", message, min, max);
		if (scanf("%d", &value) != 1) {	//정수이면 1반환
			printf("잘못된 입력입니다. 숫자만 입력하세요.\n");
			ClearBuffer();
		}

		else if ((next = getchar()) != '\n') {
			printf("잘못된 입력입니다. 숫자 뒤에 문자가 붙었습니다.\n");
			ClearBuffer();
		}

		else if (value < min || value > max) {
			printf("범위를 벗어났습니다. %d부터 %d까지의 값만 입력하세요.\n", min, max);
		}

		else {
			return value;
		}
	}
}

void levelSelect(void) {

	int level;

	for (;;) {

		level = exception("난이도를 선택하시오(1.쉬움 2.보통 3.어려움 4.커스텀)", 1, 4);

		if (level == 1) {
			row = 9;
			col = 9;
			mine_count = 10;
			break;
		}
		else if (level == 2) {
			row = 16;
			col = 16;
			mine_count = 40;
			break;
		}
		else if (level == 3) {
			row = 16;
			col = 30;
			mine_count = 99;
			break;
		}
		else if (level == 4) {

			row = exception("행을 입력하시오", 1, MAX_ROW);
			col = exception("열을 입력하시오", 1, MAX_COL);

			if (row * col < 10) {
				printf("보드 크기가 너무 작습니다.\n");
				printf("최소 1개의 지뢰를 배치할 공간이 없습니다.\n");
				printf("(총 10칸 이상 필요) 난이도를 다시 선택해주세요.\n");
				continue;
			}

			int max_mines = (row * col) - 9;	//지뢰를 배치할수있는 최대 개수			
			mine_count = exception("지뢰 개수를 입력하시오", 1, max_mines);

			break;
		}
	}
}

void reset(void) {

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			board[i][j] = 0;	//정답보드 0으로 초기화
			show_board[i][j] = HIDDEN;	//보여줄 보드는 숨김(0)으로 초기화
		}
	}
}

void mineSet(int first_row, int first_col) {

	int count = 0;	//지뢰 설치 개수

	while (count < mine_count) {

		int r = rand() % row;
		int c = rand() % col;

		// 첫 클릭 좌표와 주변 8칸 체크
		int near_first = 0;	//0이면 지뢰 설치 가능 1이면 지뢰 설치 불가능

		for (int x = -1; x <= 1; x++) {	//위,아래칸
			for (int y = -1; y <= 1; y++) {	//왼,오른쪽칸

				if (r == first_row + x && c == first_col + y) {
					near_first = 1;
					break;
				}
			}
			if (near_first == 1) break;
		}

		if (board[r][c] != MINE && near_first == 0) {
			board[r][c] = MINE;
			count++;
		}
	}
}

void near8space(void) {

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (board[i][j] == MINE) continue;	//지뢰면 숫자 계산 안함

			int near_mine_count = 0;	//주변 8칸 지뢰개수

			for (int x = -1; x <= 1; x++) {	//위,아래칸
				for (int y = -1; y <= 1; y++) {	//왼,오른쪽칸

					if (x == 0 && y == 0) continue;	//자기 자신 제외

					int real_row = i + x;	//실제 행
					int real_col = j + y;	//실제 열

					if (real_row >= 0 && real_row < row && real_col >= 0 && real_col < col) {
						if (board[real_row][real_col] == MINE) {
							near_mine_count++;
						}
					}

				}
			}
			board[i][j] = near_mine_count;
		}
	}
}
