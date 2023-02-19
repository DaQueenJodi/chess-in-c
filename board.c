#include "board.h"
#include "pieces.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "util.h"

#define FEN_START "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

Board *board_new(void) {
	Board *b = malloc(sizeof(Board));
	if (b == NULL) DIE("failed to allocate board");
	memset(b->board, 0, sizeof(b->board));
	return b;
}

void board_start(Board *b) {
	board_from_fen(b, FEN_START, sizeof(FEN_START) - 1);
}
void board_from_fen(Board *b, char *fen, size_t len) {
	size_t curry = 0;
	size_t currx = 0;
	for (size_t i = 0; i < len; i++) {
		char c = fen[i];
		if (c == '/')  { 
			curry += 1;
		 	currx = 0; 
		}
		else if (isdigit(c)) {
			for (size_t i = 0; i < (c & 0xF); i++) {
				b->board[currx++][curry] = PIECE_NONE;
			}
		}
		else b->board[currx++][curry] = piece_from_fen(c);
	}
}

void board_print(Board *b) {
	for (size_t y = 0; y < BOARD_H; y++) {
		for (size_t x = 0; x < BOARD_W; x++) {
			Piece p = b->board[x][y];
			if (p.flavor == PieceNone) {
			 	putchar(' ');
			}
			else piece_print(p);
		}
		putchar('\n');
	}
}
static PointVec *rook_get_moves(Board *b, Point p) {
	PointVec *pv = pointvec_new();
	Point t;
	t.y = p.y;
	// all moves to the left
	for (size_t x = p.x + 1; x > 0; x--) {
		t.x = x;
		if (b->board[t.x][t.y].flavor == PieceNone)
			pointvec_append(pv, t);
		if (b->board[t.x][t.y].flavor != PieceNone) break;
	}
	// all moves to the right
	t.x = p.x;
	for (size_t x = p.x + 1; x < BOARD_W; x++) {
		t.x = x;
		if (b->board[t.x][t.y].flavor == PieceNone)
			pointvec_append(pv, t);
		if (b->board[t.x][t.y].flavor != PieceNone) break;
	}
	// all moves up
	t.x = p.x;
	for (size_t y = p.y + 1; y > 0; y--) {
		t.y = y;
		if (b->board[t.x][t.y].flavor == PieceNone)
			pointvec_append(pv, t);
		if (b->board[t.x][t.y].flavor != PieceNone) break;
	}
	// all moves down
	t.y = p.y;
	for (size_t y = p.y + 1; y < BOARD_H; y++) {
		t.y = y;
		if (b->board[t.x][t.y].flavor == PieceNone)
			pointvec_append(pv, t);
		if (b->board[t.x][t.y].flavor != PieceNone) break;
	}
	return pv;
}
#define PAWN_START_BLACK 1
#define PAWN_START_WHITE 6
static PointVec *pawn_get_moves(Board *b, Point p) {
	PieceColor col = b->board[p.x][p.y].color;
	PointVec *pv = pointvec_new();
	Point t;
	t.x = p.x;
	t.y = p.y;
	size_t spaces;
	bool is_at_start;
	if (col == WhitePiece) is_at_start = p.y == PAWN_START_WHITE;
	else is_at_start = p.y == PAWN_START_BLACK;
	if (is_at_start) spaces = 2;
	else spaces = 1;

	if (col == WhitePiece) t.y = p.y - spaces;
	else t.y = p.y + spaces;
	for (size_t i = 0; i < spaces; i++) {
		t.y = p.y + i + 1;
		if (b->board[t.x][t.y].flavor == PieceNone)
			pointvec_append(pv, t);
		else break;
	}
	if (col == WhitePiece)
		t.y = p.y + 1;
	else t.y = p.y - 1;
	t.x = p.x - 1;
	if (t.x > 0 && b->board[t.x][t.y].flavor != PieceNone)
		pointvec_append(pv, t);
	t.x = p.x + 1;
	if (t.x < BOARD_W && b->board[t.x][t.y].flavor != PieceNone)
		pointvec_append(pv, t);
	
	return pv;
}

static PointVec *bishop_get_moves(Board *b, Point loc) {
	PointVec *pv = pointvec_new();
	#define DIAG_NUM 4
	int diagx[] = {1, -1, 1, -1};
	int diagy[] = {1, -1, -1, 1};
	for (size_t i = 0; i < DIAG_NUM; i++) {
		int y = diagy[i];
		for (size_t i = 0; i < DIAG_NUM; i++) {
			int x = diagx[i];
			Point p = loc;
			while (true) {
				p.x += x;
				p.y += y;
				if (p.x >= 0 && p.y >= 0 && 
						p.x < BOARD_W && p.y < BOARD_H &&
						b->board[p.x][p.y].flavor == PieceNone)
					pointvec_append(pv, p);
				else break;
			}
		}
	}
	return pv;
}
static PointVec *knight_get_moves(Point loc) {
	PointVec *pv = pointvec_new();
	Point offsets[] = { 
		{2, 1},
		{-2, 1},
		{2, -1},

		{1, 2},
		{-1, 2},
		{1, -2},
	};
	#define OFFSET_NUM (sizeof(offsets) / sizeof(Point))
	for (size_t i = 0; i < OFFSET_NUM; i++) {
		Point off = offsets[i];
		Point p = point_add(off, loc);
		if (p.x >= 0 && p.y >= 0 &&
				p.x < BOARD_W && p.y)
			pointvec_append(pv, p);
	}
	return pv;
}

static PointVec *queen_get_moves(Board *b, Point p) {
	PointVec *bpv = bishop_get_moves(b, p);
	PointVec *rpv = rook_get_moves(b, p);
	for (size_t i = 0; i < rpv->len; i++) {
		Point p = rpv->buff[i];
		pointvec_append(bpv, p);
	}
	return bpv;
}

static PointVec *king_get_moves(Point loc) {
	PointVec *pv = pointvec_new();
	Point offsets[] = {
		{0, 1},
		{0, -1},
		{1, 0},
		{1, 1},
		{1, -1},
		{-1, 0},
		{-1, 1},
		{-1, -1},
	};

	#define OFFSET_NUM (sizeof(offsets) / sizeof(Point))
	for (size_t i = 0; i < OFFSET_NUM; i++) {
		Point off = offsets[i];
		Point p = point_add(loc, off);
		if (p.x >= 0 && p.y >= 0 &&
				p.x < BOARD_W && p.y < BOARD_H)
			pointvec_append(pv, p);
	}
	return pv;
}

PointVec *board_get_moves(Board *b, Point p) {
	Piece pi = b->board[p.x][p.y];
	switch (pi.flavor) {
		case Rook: return rook_get_moves(b, p);
		case Pawn: return pawn_get_moves(b, p);
		case Bishop: return bishop_get_moves(b, p);
		case Knight: return knight_get_moves(p);
		case Queen: return queen_get_moves(b, p);
		case King: return king_get_moves(p);
		default: DIEF("not yet implemented\n");
	}
}

int board_move_piece(Board *b, Point src, Point dest) {
	PointVec *pv = board_get_moves(b, src);
	bool found = false;
	for (size_t i = 0; i < pv->len; i++) {
		Point p = pv->buff[i];
		if (p.x == dest.x && p.y == dest.y) {
			found = true;
			break;
		}
	}
	if (!found) return -1;
	Piece t = b->board[src.x][src.y];
	b->board[src.x][src.y] = PIECE_NONE;
	b->board[dest.x][dest.y] = t;
	return 0;
}
