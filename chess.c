#include "board.h"
#include "util.h"

int main(void) {
	Board *board = board_new();
	board_start(board);
	board_print(board);
	board_move_piece(board, src, dest);
	board_print(board);

	return 0; 
}
