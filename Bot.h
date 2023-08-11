#pragma once
#include "Board.h";
#include "Pieces.h";
#include "main.h";

class Bot {
public:
	string genNewMove(Board& board, unsigned depth, bool shouldMove,Square bestMoveStartPos = 64,Square bestMoveEndPos = 64) {
		int score = board.isWhiteTurn ? -inf : inf;
		int a = -inf;
		int b = inf;
		Square startMove = 65;
		Square endMove = 65;
		P promotion = DEBUG;
		if (bestMoveStartPos != 64) {

		}
		for (Square i = 0; i < 64; i++) {
			if (board.pieces[i] == EMPTY || Pieces::isPieceWhite(board.pieces[i]) != board.isWhiteTurn)
				continue;
			Square moves[56];
			fill_n(moves, 56, 64);
			Pieces::getValidMovesForPiece(i, board, moves);
			Square oldEnPassantSquare = board.enPassantSquare;
			for (int j = 0; j < 56; j++) {
				if (moves[j] == 64)
					break;
				if ((moves[j] < 8 && board.pieces[i] == PAWN_W) || (moves[j] > 55 && board.pieces[i] == PAWN_B)) {
					for (int k = 0; k < 4; k++) {
						this->doMove(board, i, moves[j], depth, a, b, score, PUpgradePieces[k], true, &startMove, &endMove, &promotion);
					}
				}
				else {
					this->doMove(board, i, moves[j], depth, a, b, score, DEBUG, true, &startMove, &endMove, &promotion);
				}
			}
		}
		if (board.shouldLog)
			cout << "Score: " << score << endl;
		if (shouldMove)
			Pieces::movePiece(startMove, endMove, board);
		if (promotion != DEBUG) {
			for (int i = 0; i < 4; i++) {
				if (promotion == PUpgradePieces[i])
					return Pieces::square_to_string(startMove) + Pieces::square_to_string(endMove) + charUpgradePieces[i];
			}
		}
		return Pieces::square_to_string(startMove) + Pieces::square_to_string(endMove);
	}

	void doMove(Board& board,Square startPos, Square endPos,int depth, int& a, int& b, int& score, P PUpgradePiece, bool saveMoves = false,Square *startMove = NULL, Square* endMove = NULL, P* promotion = NULL) {
		Square oldEnPassantSquare = board.enPassantSquare;
		Square cs[4];
		P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
		bool cv[4]{ false,false,false,false };
		Pieces::movePiece(startPos, endPos, board, cs, cp, cv, PUpgradePiece);
		int newScore = this->newMove(board, depth - 1, a, b);
		Pieces::unmovePiece(cs, cp, cv, oldEnPassantSquare, board);
		if (board.isWhiteTurn) {
			if (newScore > score) {
				score = newScore;
				if (saveMoves) {
					*startMove = startPos;
					*endMove = endPos;
					*promotion = PUpgradePiece;
				}
			}
			if (newScore > a)
				a = newScore;
		}
		else {
			if (newScore < score) {
				score = newScore;
				if (saveMoves) {
					*startMove = startPos;
					*endMove = endPos;
					*promotion = PUpgradePiece;
				}
			}
			if (newScore < b)
				b = newScore;
		}
	}


	int newMove(Board& board, int depth, int a, int b) {
		if (depth <= 0) {
			if (!board.lastMoveCapture || depth <= -2)
				return this->evalBoard(board);
		}
		int score = board.isWhiteTurn ? -inf : inf;
		for (Square i = 0; i < 64; i++) {
			if (board.pieces[i] == EMPTY || Pieces::isPieceWhite(board.pieces[i]) != board.isWhiteTurn)
				continue;
			Square moves[56];
			fill_n(moves, 56, 64);
			Pieces::getValidMovesForPiece(i, board, moves);
			for (int j = 0; j < 56; j++) {
				if (moves[j] == 64)
					break;
				Square oldEnPassantSquare = board.enPassantSquare;
				if ((moves[j] < 8 && board.pieces[i] == PAWN_W) || (moves[j] > 55 && board.pieces[i] == PAWN_B)) {
					for (int k = 0; k < 4; k++) {
						this->doMove(board, i,moves[j],depth,a,b,score,PUpgradePieces[k]);
						/*Square cs[4];
						P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
						bool cv[4]{ false,false,false,false };
						Pieces::movePiece(i, moves[j], board, cs, cp, cv, PUpgradePieces[k]);
						int newScore = this->newMove(board, depth - 1, a, b);
						Pieces::unmovePiece(cs, cp, cv, oldEnPassantSquare, board);
						if (board.isWhiteTurn) {
							if (newScore >= b)
								return b;
							if (newScore > score)
								score = newScore;
							if (newScore > a)
								a = newScore;
						}
						else {
							if (newScore <= a)
								return a;
							if (newScore < score)
								score = newScore;
							if (newScore < b)
								b = newScore;
						}*/
					}
				}
				else {
					this->doMove(board, i, moves[j], depth, a, b, score, DEBUG);
					/*Square cs[4];
					P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
					bool cv[4]{ false,false,false,false };
					Pieces::movePiece(i, moves[j], board, cs, cp, cv);
					int newScore = this->newMove(board, depth - 1, a, b);
					Pieces::unmovePiece(cs, cp, cv, oldEnPassantSquare, board);
					if (board.isWhiteTurn) {
						if (newScore >= b)
							return b;
						if (newScore > score)
							score = newScore;
						if (newScore > a)
							a = newScore;
					}
					else {
						if (newScore <= a)
							return a;
						if (newScore < score)
							score = newScore;
						if (newScore < b)
							b = newScore;
					}*/
				}
			}
		}
		if (score == inf || score == -inf) {
			if (Pieces::isInCheck(board.isWhiteTurn, board))
				return board.isWhiteTurn ? -inf + 100 - depth : inf - 100 + depth;
			return 0;
		}
		return score;
	}

	int evalBoard(Board& board) {
		int value = 0;
		unsigned totalPiecesValue = 0;
		Square wKingPos;
		Square bKingPos;
		for (Square i = 0; i < 64; i++) {
			if (board.pieces[i] == EMPTY)
				continue;
			if (board.pieces[i] == KING_W)
				wKingPos = i;
			if (board.pieces[i] == KING_B)
				bKingPos = i;
			value += this->pieceValues[board.pieces[i]];
			if (board.pieces[i] == PAWN_W) {
				value += (6-(i >> 3)) * 150;
			}
			else if (board.pieces[i] == PAWN_B) {
				value -= ((i >> 3) - 1) * 150;
			}
			totalPiecesValue += abs(this->pieceValues[board.pieces[i]]);
			bool iswhite = Pieces::isPieceWhite(board.pieces[i]);
			Square moves[56];
			fill_n(moves, 56, 64);
			Pieces::getMovesForPiece(i, board, moves);
			for (int j = 0; j < 14; j++) {
				if (moves[j] == 64)
					break;
				value += 50 * (iswhite ? 1 : -1);
			}
		}
		if (totalPiecesValue <= 9000) {
			value += this->kingPlaceValue[wKingPos] * 1000;
			value -= this->kingPlaceValue[bKingPos] * 1000;
		}
		return value;
	}

	unsigned short kingPlaceValue[64]{
		0,0,0,0,0,0,0,0,
		0,1,1,1,1,1,1,0,
		0,1,2,2,2,2,1,0,
		0,1,2,3,3,2,1,0,
		0,1,2,3,3,2,1,0,
		0,1,2,2,2,2,1,0,
		0,1,1,1,1,1,1,0,
		0,0,0,0,0,0,0,0
	};

	void perft(short depth, Board& board, bool isWhite) {
		/*/for (Square i = 0; i < 64; i++) {
			if (board.pieces[i] == EMPTY || Pieces::isPieceWhite(board.pieces[i]) != isWhite)
				continue;
			Square moves[56];
			fill_n(moves, 56, 64);
			Pieces::getValidMovesForPiece(i, board, moves);
			for (short j = 0; j < 56; j++) {
				if (moves[j] == 64)
					break;
				Square enPassants[2]{ board.whiteEnPassantSquare, board.blackEnPassantSquare };
				if ((moves[j] < 8 && board.pieces[i] == PAWN_W) || (moves[j] > 55 && board.pieces[i] == PAWN_B)) {
					for (int k = 0; k < 4; k++) {
						Square cs[4];
						P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
						bool cv[4]{ false,false,false,false };
						Pieces::movePiece(i, moves[j], board, cs, cp, cv,PUpgradePieces[k]);
						cout << Pieces::square_to_string(i) << " -> " << Pieces::square_to_string(moves[j]) << " : " << this->perftLoop(depth - 1, !isWhite, board) << endl;
						Pieces::unmovePiece(cs, cp, cv, enPassants, board);
					}
				}
				else {
					Square cs[4];
					P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
					bool cv[4]{ false,false,false,false };
					Pieces::movePiece(i, moves[j], board, cs, cp, cv);
					cout << Pieces::square_to_string(i) << " -> " << Pieces::square_to_string(moves[j]) << " : " << this->perftLoop(depth - 1, !isWhite, board) << endl;
					Pieces::unmovePiece(cs, cp, cv, enPassants, board);
				}
			}
		}*/
		for (int i = 0; i < depth; i++) {
			cout << "Perft " << i + 1 << ": " << this->perftLoop(i + 1, isWhite, board) << endl;
		}

	}
private:
	short pieceValues[13]{ 0,9000,5000,3000,3000,1000,0,-9000,-5000,-3000,-3000,-1000,0 };
	unsigned perftLoop(short depth, bool isWhite, Board& board) {
		if (depth == 0)
			return 1;
		unsigned count = 0;
		for (Square i = 0; i < 64; i++) {
			if (board.pieces[i] == EMPTY || Pieces::isPieceWhite(board.pieces[i]) != isWhite)
				continue;
			Square moves[56];
			fill_n(moves, 56, 64);
			Pieces::getValidMovesForPiece(i, board, moves);
			for (short j = 0; j < 56; j++) {
				if (moves[j] == 64)
					break;

				Square oldEnPassantSquare = board.enPassantSquare;
				if ((moves[j] < 8 && board.pieces[i] == PAWN_W) || (moves[j] > 55 && board.pieces[i] == PAWN_B)) {
					for (int k = 0; k < 4; k++) {
						Square cs[4];
						P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
						bool cv[4]{ false,false,false,false };
						Pieces::movePiece(i, moves[j], board, cs, cp, cv, PUpgradePieces[k]);
						count += this->perftLoop(depth - 1, !isWhite, board);
						Pieces::unmovePiece(cs, cp, cv, oldEnPassantSquare, board);
					}
				}
				else {
					Square cs[4];
					P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
					bool cv[4]{ false,false,false,false };
					Pieces::movePiece(i, moves[j], board, cs, cp, cv);
					count += this->perftLoop(depth - 1, !isWhite, board);
					Pieces::unmovePiece(cs, cp, cv, oldEnPassantSquare, board);
				}
			}
		}
		return count;
	}

	int inf = 2147483647;
};