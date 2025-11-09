#pragma once

#ifndef COMMON_H 
#define COMMON_H 

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

// 메시지 API
void ui_set_message(const char* s);
void ui_clear_message(void);

typedef struct { int x, y; } Cursor;
extern Cursor g_cursor;

typedef enum { ACT_NONE = 0, ACT_MOVE, ACT_OPEN } Action;  
void cursor_init(int sx, int sy);
void cursor_move(int dx, int dy, int W, int H);
Action read_action(int* dx, int* dy, int W, int H);

void   timer_start(void);
void   timer_stop(void);
double timer_elapsed_sec(void);
void   timer_print_mmss(void);

void draw_board_with_cursor_and_status(void);

#endif




