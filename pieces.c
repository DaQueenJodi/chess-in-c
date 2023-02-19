#include "pieces.h"
#include "util.h"
#include <ctype.h>
static PieceFlavor get_piece_flavor(char c) {
	switch (tolower(c)) {
		case 'r': return Rook;
		case 'b': return Bishop;
		case 'k': return King;
		case 'n': return Knight;
		case 'p': return Pawn;
		case 'q': return Queen;
		default:
			DIEF("unknown fen: %c", c);
	}
}
Piece piece_from_fen(char c) {
	PieceFlavor flavor = get_piece_flavor(c);
	PieceColor color;
	if (isupper(c)) color = WhitePiece;
	else color = BlackPiece;
	Piece p;
	p.flavor = flavor;
	p.color = color;
	return p;
}

void piece_print(Piece p) {
	char *color_fmt;
	switch (p.color) {
		case WhitePiece: 
			color_fmt = "\033[35m";
			break;
		case BlackPiece: 
			color_fmt = "\033[36m";
			break;
		default:
			DIEF("invalid color: %d\n", p.color);
	}
	printf("%s", color_fmt);
	switch(p.flavor) {
		case Rook:   
			putchar('R');
			break;
		case Knight: 
			putchar('N');
			break;
		case Bishop: 
			putchar('B');
			break;
		case Queen:  
			putchar('Q');
			break;
		case King:   
			putchar('K');
			break;
		case Pawn:   
			putchar('P');
			break;
		default:
			DIEF("invalid piece: %d", p.flavor);
	}
	printf("\033[0m");
}
