#ifndef C_BOARD_HH
#define C_BOARD_HH

#define BOARD_H 8
#define BOARD_W 8

#include <stdlib.h>
#include "pieces.h"
#include "util.h"

typedef struct {
	Piece board[8][8];
} Board;
Board *board_new(void);
void board_start(Board *b);
void board_from_fen(Board *b, char *fen, size_t len);
PointVec *board_get_moves(Board *b, Point p);
int board_move_piece(Board *b, Point src, Point dest);
void board_print(Board *b);
#endif
