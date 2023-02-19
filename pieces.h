#ifndef C_PIECES_HH
#define C_PIECES_HH

#include "util.h"
typedef enum {
	WhitePiece,
	BlackPiece
} PieceColor;

typedef enum {
	Rook,
	Knight,
	Bishop,
	Queen,
	King,
	Pawn,
	PieceNone,
} PieceFlavor;

typedef struct {
	PieceColor color;
	PieceFlavor flavor;
} Piece;

#define PIECE_NONE (Piece){.flavor = PieceNone}
Piece piece_from_fen(char c);
void piece_print(Piece p);

#endif
