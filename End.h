#pragma once
#include "main.h"
#include "Pieces.h"

class End {
public:
	bool isEnd(Board &board) {
		unsigned short pieces[6]{ 0 };
		for (int i = 0; i < 64; i++) {
			if (board.pieces[i] == EMPTY)
				continue;
			P newPiece = board.pieces[i];
			Pieces::toWhitePiece(&newPiece);
			if (Pieces::isPieceWhite(board.pieces[i]) == board.isWhiteTurn)
				pieces[newPiece]++;
			else if (board.pieces[i] != P(board.isWhiteTurn * 6))
				return false;
		}
		bool correct[3]{ true,true,true};
		for (int i = 0; i < 5; i++) {
			if (pieces[i + 1] != queenEndGame[i])
				correct[0] = false;
			if (pieces[i + 1] != rookEndGame[i])
				correct[1] = false;
			if (pieces[i + 1] != bishopEndGame[i])
				correct[2] = false;
		}
		return correct[0] || correct[1] || correct[2];
	}

	void queenEnd(Board& board, Square* startPos, Square* endPos) {
		Square ourKingPos;

	}

private:
	unsigned short queenEndGame[5]{ 1,0,0,0,0 };
	unsigned short rookEndGame[5]{ 0,1,0,0,0 };
	unsigned short bishopEndGame[5]{ 0,0,2,0,0 };
};