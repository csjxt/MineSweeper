#include <stdio.h>
#include "common.h"

extern Cursor g_cursor;
extern void boardPrint(void);

/* ====== 화면 배치 설정 ====== */
#define HEADER_TOP_LINES  4   // 열 번호 줄이 몇 줄 위에 있는지 (보통 1)
#define LEFT_SEP_CHARS    3   // 행 번호 뒤의 구분자 폭
#define CELL_W            3   // 셀 가로폭
#define CELL_H            1   // 셀 세로폭

/* ─────────────────────────────────────────────
   상태 메시지(예: "이미 연 칸입니다") 유지용
────────────────────────────────────────────── */
static char g_info_msg[128] = "";   // 메시지 저장 (비어 있으면 표시 안함)

/* 메시지 설정/해제 함수 */
void ui_set_message(const char* s) {
    if (!s) { g_info_msg[0] = '\0'; return; }
    snprintf(g_info_msg, sizeof(g_info_msg), "%s", s);
}

void ui_clear_message(void) {
    g_info_msg[0] = '\0';
}

/* 보조 함수 */
static int row_label_width(void) {
    if (row >= 100) return 3;
    if (row >= 10)  return 2;
    return 1;
}

static inline void gotorc(int r, int c) { printf("\033[%d;%dH", r, c); }

/* ====== 커서와 상태줄 포함 전체 보드 출력 ====== */
void draw_board_with_cursor_and_status(void)
{
    // 0) 화면 상단으로 이동 후 현재 커서부터 끝까지 지우기
    printf("\033[H\033[J");

    // 1) 팀의 보드 전체 출력
    boardPrint();

    // 2) 콘솔 상 실제 보드의 시작 위치 계산
    const int origin_row = 1 + HEADER_TOP_LINES;
    const int origin_col = 1 + row_label_width() + LEFT_SEP_CHARS;

    // 3) 커서 위치 계산 (보드 좌표 → 콘솔 좌표)
    int scr_r = origin_row + g_cursor.y * CELL_H;
    int scr_c = origin_col + (g_cursor.x + 1) * CELL_W - 1;

    // 4) 아이콘 출력
    gotorc(scr_r, scr_c);
    putchar('@');

    // 5) 상태줄 출력 (보드 바로 아래)
    int status_r = origin_row + row * CELL_H + 2;
    gotorc(status_r, 1);
    printf("\033[K"); // 상태줄 지우고
    printf("선택: (행 %d, 열 %d)  ", g_cursor.y + 1, g_cursor.x + 1);
    timer_print_mmss();

    // 6) 안내문 출력 (다음줄)
    gotorc(status_r + 1, 1);
    printf("\033[K(WASD 이동 / Space=OPEN)");

    // 7) 상태 메시지 출력 (그 아래줄)
    gotorc(status_r + 2, 1);
    printf("\033[K"); // 한 줄 지움
    if (g_info_msg[0] != '\0') {
        printf("%s", g_info_msg);
    }

    fflush(stdout);
}

