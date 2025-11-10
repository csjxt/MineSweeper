#pragma once

#define MAX_ROW 70 //최대행 70
#define MAX_COL 70 //최대열 70

#define HIDDEN 0 //아직 열리지 않은 칸
#define OPEN 1 //열린칸
#define FLAGGED 2 //깃발을 꽂은 칸

#define MINE -1 //지뢰가 있는칸
#define Clicked_MINE -2 //사용자가 연 지뢰칸

extern int row; //행
extern int col; //열
extern int mine_count; //지뢰개수
extern int hidden_count; //열리지 않은 칸 개수

extern int board[MAX_ROW][MAX_COL];	
extern int show_board[MAX_ROW][MAX_COL]; //사용자에게 보여줄 보드

void ClearBuffer(void);    //입력 버퍼 비우는 함수
void levelSelect(void);	//난이도 선택 함수
void reset(void);	//초기화 함수
void mineSet(int first_row, int first_col);	//지뢰배치 함수(첫클릭 무조건 지뢰 배치 안함)
void near8space(void);	//주변 8칸의 지뢰 개수 계산 함수(board배열에 저장)
int exception(const char* message, int min, int max);	//입력할때 특정범위 정수만 입력받음

void boardPrint(void); //보드 출력하는 함수
int WinOrLose(int user_row, int user_col); //승패 판정 함수
void open8space(int user_row, int user_col); //빈칸인 주변 8칸 여는 함수

void boardPrintWithCursor(int cur_r, int cur_c);



