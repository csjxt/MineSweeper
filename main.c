// ===== main.c (WASD 이동 + 커서칸 하이라이트 + 정렬 + Enter 오른쪽 이동 + 시간 기록) =====
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "common.h"

// ── 상수 ─────────────────────────────────────────────
// 열/셀 출력 폭: 숫자 간격에 여유를 주기 위해 3칸으로 설정
#define CELL_W 3

// ── 프로토타입 ─────────────────────────────────────────
static void draw_board_with_cursor(const Cursor* cur);
static void get_selection_wasd(int* out_r, int* out_c, Cursor* cur);
static void run_game_loop(void);

// ── 메인 ───────────────────────────────────────────────
int main(void) {
    run_game_loop();
    return 0;
}

// ── (핵심) 커서 위치를 반영하여 보드를 직접 그리는 함수 ──────────
static void draw_board_with_cursor(const Cursor* cur)
{
    // 화면 초기화
    printf("\033[2J");
    printf("\033[H");

    // 상단 타이틀
    printf("현재 보드 상태%*s지뢰 개수: %d\n\n", 12, "", mine_count);

    // 열 번호 — 행번호(2칸) + 공백(1) + '|'(1) = 총 4칸 밀기
    printf("    ");
    for (int j = 0; j < col; ++j) {
        printf("%*d", CELL_W, j + 1); // 폭 3칸으로 여유있게 출력
    }
    printf("\n");

    // 각 행 출력
    for (int i = 0; i < row; ++i) {
        printf("%2d |", i + 1);  // 행 번호 + 구분자
        for (int j = 0; j < col; ++j) {
            char cell;
            if (i == cur->r && j == cur->c) {
                cell = '?';                        // 현재 커서 위치
            }
            else if (show_board[i][j] == HIDDEN) {
                cell = '#';                        // 닫힌 칸
            }
            else if (board[i][j] == MINE) {
                cell = '*';                        // 지뢰 칸
            }
            else {
                int n = board[i][j];
                cell = (n == 0) ? '0' : ('0' + n); // 숫자 칸
            }

            printf("%*c", CELL_W, cell);           // 셀도 같은 폭으로 정렬
        }
        printf("\n");
    }

    // 안내 + 현재 좌표 (1-based로 표시)
    printf("\n이동: W/A/S/D   선택: Enter   종료: Q\n");
    printf("현재 좌표: (r=%d, c=%d)\n", cur->r + 1, cur->c + 1);
}

// ── WASD로 이동 + Enter로 좌표 선택 ───────────────────────
static void get_selection_wasd(int* out_r, int* out_c, Cursor* cur)
{
    while (1) {
        draw_board_with_cursor(cur);

        int key = _getch();
        if (key == 'q' || key == 'Q') {            // 종료
            *out_r = cur->r; *out_c = cur->c;
            return;
        }
        if (key == 13) {                           // Enter → 선택 확정 (현재 위치 유지)
            *out_r = cur->r; *out_c = cur->c;
            return;
        }

        move_cursor(cur, row, col, key);           // W/A/S/D 이동
    }
}

// ── 실제 게임 루프 ─────────────────────────────────────────
static void run_game_loop(void)
{
    int first_row, first_col;

    srand((unsigned)time(NULL));
    levelSelect();
    reset();

    first_row = exception("첫 번째로 열 행", 1, row);
    first_col = exception("첫 번째로 열 열", 1, col);
    first_row--; first_col--; // 0-base로 변환

    mineSet(first_row, first_col);
    near8space();
    open8space(first_row, first_col);

    // 커서 초기 위치: 첫 클릭 위치
    Cursor cur = (Cursor){ first_row, first_col };

    time_t t_start = time(NULL);

    while (1) {
        int user_row, user_col;
        get_selection_wasd(&user_row, &user_col, &cur);

        // ✅ Enter로 선택한 뒤, 커서를 오른쪽 칸으로 자동 이동
        if (cur.c + 1 < col) {
            cur.c += 1;
        }
        else {
            cur.c = 0;
            if (cur.r + 1 < row) cur.r += 1;
            else cur.r = 0; // 마지막 칸이면 맨 위로 돌아감
        }

        // 이미 열린 칸이면 다시 선택
        if (show_board[user_row][user_col] == OPEN) {
            printf("\n이미 열린 칸입니다. 다른 칸을 선택하세요.\n");
            printf("계속하려면 Enter...");
            while (_getch() != 13) { /* wait */ }
            continue;
        }

        // 오픈 처리
        if (board[user_row][user_col] != MINE) {
            show_board[user_row][user_col] = OPEN;
            hidden_count--;
        }

        // 승패 판정
        if (WinOrLose(user_row, user_col) == 0)
            break;
    }

    double sec = difftime(time(NULL), t_start);
    printf("\n게임 종료! 소요 시간: %.1f초\n", sec);
}

