#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Board.h"
#include "main.h"
#include <string>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <cmath>
#include <bitset>



class Pieces {
public:
	static short piecesMoves[6][56][2];
	static unsigned short piecesMovesLength[5];

	static Square castlePiecesPos[6];

	static void init() {
		random_device rd;

		mt19937_64 e2(rd());

		uniform_int_distribution<long long unsigned> dist(llround(pow(2, 0 )), llround(pow(2, 64)));

		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 13; j++) {
				Pieces::randomNums[i][j] = dist(e2);
			}
			cout << (bitset<64>)dist(e2) << endl;
		}
		
	}

	static void uciMovePiece(string move, Board& board) {
		char dest[2]{ move[2], move[3] };
		Square startPos = posToSquare(move);
		Square endPos = posToSquare(dest);
		bool isValidPos = board.pieces[startPos] != EMPTY && Pieces::isPieceWhite(board.pieces[startPos]) == board.isWhiteTurn;
		Square moves[56];
		fill_n(moves, 56, 64);
		if (isValidPos) {
			isValidPos = false;
			Pieces::getValidMovesForPiece(startPos, board, moves);
			for (int i = 0; i < 56; i++) {
				if (moves[i] == 64)
					break;
				if (moves[i] == endPos) {
					isValidPos = true;
					break;
				}
			}
		}
		if (isValidPos) {
			P pawnUpgrade = DEBUG;
			if (move[4] != NULL) {
				for (int i = 0; i < 4; i++) {
					if (move[4] == charUpgradePieces[i]) {
						pawnUpgrade = PUpgradePieces[i];
					}
				}
			}
			Square cs[4];
			P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
			bool cv[4]{ false,false,false,false };
			Pieces::movePiece(startPos, endPos, board, cs, cp, cv, pawnUpgrade);
		}
		else {
			cout << "Invalid move..." << endl;
		}
	}

	static void movePiece(Square startPos, Square endPos, Board& board, Square changedSquares[4] = NULL, P changedPieces[4] = NULL, bool changedVars[4] = NULL, P pawnUpgrade = DEBUG) {
		P piece = board.pieces[startPos];
		bool isPieceWhite = Pieces::isPieceWhite(piece);
		Pieces::toWhitePiece(&piece);
		bool resetEnPassant = true;
		switch (piece)
		{
		case KING_W:
			if (isPieceWhite) {
				if (board.canWhiteCastle[0] && endPos == 58) {
					Pieces::setBoardPiece(59, ROOK_W, changedSquares, changedPieces, board);
					Pieces::setBoardPiece(56, EMPTY, changedSquares, changedPieces, board);
				}
				else if (board.canWhiteCastle[1] && endPos == 62) {
					Pieces::setBoardPiece(61, ROOK_W, changedSquares, changedPieces, board);
					Pieces::setBoardPiece(63, EMPTY, changedSquares, changedPieces, board);
				}
			}
			else {
				if (board.canBlackCastle[0] && endPos == 2) {
					Pieces::setBoardPiece(3, ROOK_B, changedSquares, changedPieces, board);
					Pieces::setBoardPiece(0, EMPTY, changedSquares, changedPieces, board);
				}
				else if (board.canBlackCastle[1] && endPos == 6) {
					Pieces::setBoardPiece(5, ROOK_B, changedSquares, changedPieces, board);
					Pieces::setBoardPiece(7, EMPTY, changedSquares, changedPieces, board);
				}
			}
			break;
		case PAWN_W:
			Square endY = endPos >> 3;
			Square startY = startPos >> 3;
			if (endY == 0 || endY == 7) {
				Pieces::setBoardPiece(startPos, P(pawnUpgrade + !isPieceWhite * 6), changedSquares, changedPieces, board);
			}
			else if (startY == 1 && endY == 3) {
				short changePos[2] = { -1,0 };
				if (Pieces::isValidPos(endPos, changePos) && board.pieces[endPos - 1] == PAWN_W) {
					board.enPassantSquare = endPos - 8;
					resetEnPassant = false;
				}
				else {
					changePos[0] = 1;
					if (Pieces::isValidPos(endPos, changePos) && board.pieces[endPos + 1] == PAWN_W) {
						board.enPassantSquare = endPos - 8;
						resetEnPassant = false;
					}
				}
			}
			else if (startY == 6 && endY == 4) {
				short changePos[2] = { -1,0 };
				if (Pieces::isValidPos(endPos, changePos) && board.pieces[endPos - 1] == PAWN_B) {
					board.enPassantSquare = endPos + 8;
					resetEnPassant = false;
				}
				else {
					changePos[0] = 1;
					if (Pieces::isValidPos(endPos, changePos) && board.pieces[endPos + 1] == PAWN_B) {
						board.enPassantSquare = endPos + 8;
						resetEnPassant = false;
					}
				}
			}
			else if ((startPos & 7) != (endPos & 7) && board.pieces[endPos] == EMPTY)
				Pieces::setBoardPiece(endPos + (isPieceWhite * 2 - 1) * 8, EMPTY, changedSquares, changedPieces, board);
			break;
		}

		for (int i = 0; i < 6; i++) {
			if (startPos != castlePiecesPos[i] && endPos != castlePiecesPos[i])
				continue;
			if (i == 0 || i == 1)
				changeBoardVar(false, true, changedVars, board);
			if (i == 1 || i == 2)
				changeBoardVar(false, false, changedVars, board);
			if (i == 3 || i == 4)
				changeBoardVar(true, true, changedVars, board);
			if (i == 4 || i == 5)
				changeBoardVar(true, false, changedVars, board);
		}

		if (resetEnPassant)
			board.enPassantSquare = 64;

		if (board.pieces[endPos] != EMPTY && board.pieces[endPos] != PAWN_W && board.pieces[endPos] != PAWN_B)
			board.lastMoveCapture = true;
		else
			board.lastMoveCapture = false;

		Pieces::setBoardPiece(endPos, board.pieces[startPos], changedSquares, changedPieces, board);
		Pieces::setBoardPiece(startPos, EMPTY, changedSquares, changedPieces, board);
		board.isWhiteTurn = !board.isWhiteTurn;
		board.totalMoves++;
	};

	static void setBoardPiece(Square pos, P piece, Square changedSquares[4], P changedPieces[4], Board& board) {
		if (changedSquares != NULL) {
			unsigned short changeNum;
			for (int i = 0; i < 4; i++) {
				if (changedPieces[i] == DEBUG) {
					changeNum = i;
					break;
				}
			}
			changedSquares[changeNum] = pos;
			changedPieces[changeNum] = board.pieces[pos];
		}
		board.pieces[pos] = piece;
	}

	static void changeBoardVar(bool isWhite, bool isLeft, bool changedVars[4], Board& board) {
		if (isWhite) {
			if (!board.canWhiteCastle[!isLeft])
				return;
			board.canWhiteCastle[!isLeft] = false;
			if (changedVars == NULL)
				return;
			changedVars[!isLeft] = true;
		}
		else {
			if (!board.canBlackCastle[!isLeft])
				return;
			board.canBlackCastle[!isLeft] = false;
			if (changedVars == NULL)
				return;
			changedVars[!isLeft + 2] = true;
		}
	}

	static void unmovePiece(Square changedSquares[4], P changedPieces[4], bool changedVars[4], Square oldEnPassantsSquare, Board& board) {
		for (int i = 3; i >= 0; i--) {
			if (changedVars[i]) {
				if (i < 2) {
					board.canWhiteCastle[i] = !board.canWhiteCastle[i];
				}
				else {
					board.canBlackCastle[i - 2] = !board.canBlackCastle[i - 2];
				}
			}
			if (changedPieces[i] != DEBUG) {
				board.pieces[changedSquares[i]] = changedPieces[i];
			}
		}
		board.enPassantSquare = oldEnPassantsSquare;
		board.isWhiteTurn = !board.isWhiteTurn;
		board.totalMoves--;
	}

	static void getMovesForPiece(Square pos, Board& board, Square moves[56]) {
		P piece = board.pieces[pos];
		bool isPieceWhite = Pieces::isPieceWhite(piece);
		Pieces::toWhitePiece(&piece);
		unsigned short totalMoves = 0;
		switch (piece)
		{
		case KING_W:
			for (int i = 0; i < piecesMovesLength[0]; i++) {
				if (!Pieces::isValidPos(pos, piecesMoves[0][i]))
					continue;
				Square newPos = pos + piecesMoves[0][i][0] + piecesMoves[0][i][1] * 8;
				if (board.pieces[newPos] != EMPTY &&
					Pieces::isPieceWhite(board.pieces[newPos]) == isPieceWhite)
					continue;
				moves[totalMoves] = newPos;
				totalMoves++;
			}
			if (isPieceWhite) {

				if (board.canWhiteCastle[0] &&
					board.pieces[57] == EMPTY &&
					board.pieces[58] == EMPTY &&
					board.pieces[59] == EMPTY &&
					!Pieces::isInCheck(true, board))
				{
					board.pieces[59] = KING_W;
					board.pieces[60] = EMPTY;
					if (!Pieces::isInCheck(true, board)) {
						moves[totalMoves] = (Square)58;
						totalMoves++;
					}
					board.pieces[59] = EMPTY;
					board.pieces[60] = KING_W;
				}
				if (board.canWhiteCastle[1] &&
					board.pieces[61] == EMPTY &&
					board.pieces[62] == EMPTY &&
					!Pieces::isInCheck(true, board))
				{
					board.pieces[61] = KING_W;
					board.pieces[60] = EMPTY;
					if (!Pieces::isInCheck(true, board)) {
						moves[totalMoves] = (Square)62;
						totalMoves++;
					}
					board.pieces[61] = EMPTY;
					board.pieces[60] = KING_W;
				}
			}
			else {
				if (board.canBlackCastle[0] &&
					board.pieces[1] == EMPTY &&
					board.pieces[2] == EMPTY &&
					board.pieces[3] == EMPTY &&
					!Pieces::isInCheck(false, board))
				{
					board.pieces[3] = KING_B;
					board.pieces[4] = EMPTY;
					if (!Pieces::isInCheck(false, board)) {
						moves[totalMoves] = (Square)2;
						totalMoves++;
					}
					board.pieces[3] = EMPTY;
					board.pieces[4] = KING_B;
				}
				if (board.canBlackCastle[1] &&
					board.pieces[5] == EMPTY &&
					board.pieces[6] == EMPTY &&
					!Pieces::isInCheck(false, board))
				{
					board.pieces[5] = KING_B;
					board.pieces[4] = EMPTY;
					if (!Pieces::isInCheck(false, board)) {
						moves[totalMoves] = (Square)6;
						totalMoves++;
					}
					board.pieces[5] = EMPTY;
					board.pieces[4] = KING_B;
				}
			}
			break;
		case QUEEN_W:
			for (int s = 0; s < 8; s++) {
				for (int i = 0; i < 7; i++) {
					unsigned short varPos = s * 7 + i;
					if (!Pieces::isValidPos(pos, Pieces::piecesMoves[1][varPos]))
						break;
					Square movePos = pos + piecesMoves[1][varPos][0] + piecesMoves[1][varPos][1] * 8;
					if (board.pieces[movePos] != EMPTY) {
						if (Pieces::isPieceWhite(board.pieces[movePos]) == isPieceWhite)
							break;
						moves[totalMoves] = (Square)movePos;
						totalMoves++;
						break;
					}
					moves[totalMoves] = (Square)movePos;
					totalMoves++;
				}
			}
			break;
		case ROOK_W:
			for (int s = 0; s < 4; s++) {
				for (int i = 0; i < 7; i++) {
					unsigned short varPos = s * 7 + i;
					if (!Pieces::isValidPos(pos, Pieces::piecesMoves[2][varPos]))
						break;
					Square movePos = pos + piecesMoves[2][varPos][0] + piecesMoves[2][varPos][1] * 8;
					if (board.pieces[movePos] != EMPTY) {
						if (Pieces::isPieceWhite(board.pieces[movePos]) == isPieceWhite)
							break;
						moves[totalMoves] = (Square)movePos;
						totalMoves++;
						break;
					}
					moves[totalMoves] = (Square)movePos;
					totalMoves++;
				}
			}
			break;
		case BISHOP_W:
			for (int s = 0; s < 4; s++) {
				for (int i = 0; i < 7; i++) {
					unsigned short varPos = s * 7 + i;
					if (!Pieces::isValidPos(pos, Pieces::piecesMoves[3][varPos]))
						break;
					Square movePos = pos + piecesMoves[3][varPos][0] + piecesMoves[3][varPos][1] * 8;
					if (board.pieces[movePos] != EMPTY) {
						if (Pieces::isPieceWhite(board.pieces[movePos]) == isPieceWhite)
							break;
						moves[totalMoves] = (Square)movePos;
						totalMoves++;
						break;
					}
					moves[totalMoves] = (Square)movePos;
					totalMoves++;
				}
			}
			break;
		case KNIGHT_W:
			for (int i = 0; i < 8; i++) {
				if (!Pieces::isValidPos(pos, Pieces::piecesMoves[4][i]))
					continue;
				Square movePos = pos + piecesMoves[4][i][0] + piecesMoves[4][i][1] * 8;
				if (board.pieces[movePos] != EMPTY && Pieces::isPieceWhite(board.pieces[movePos]) == isPieceWhite)
					continue;
				moves[totalMoves] = (Square)movePos;
				totalMoves++;
			}
			break;
		case PAWN_W:
			if (isPieceWhite) {
				if ((pos >> 3) == 3 && (pos - 7 == board.enPassantSquare || pos - 9 == board.enPassantSquare)) {
					moves[totalMoves] = board.enPassantSquare;
					totalMoves++;
				}

				short movePos[2]{ -1,-1 };
				if (Pieces::checkNode(false, pos, movePos, board)) {
					moves[totalMoves] = Square(pos + movePos[0] + movePos[1] * 8);
					totalMoves++;
				}
				movePos[0] += 2;
				if (Pieces::checkNode(false, pos, movePos, board)) {
					moves[totalMoves] = Square(pos + movePos[0] + movePos[1] * 8);
					totalMoves++;
				}
				movePos[0]--;
				if (board.pieces[pos + movePos[0] + movePos[1] * 8] != EMPTY)
					break;
				moves[totalMoves] = Square(pos + movePos[0] + movePos[1] * 8);
				totalMoves++;
				movePos[1]--;
				if ((pos >> 3) != 6 || board.pieces[pos + movePos[0] + movePos[1] * 8] != EMPTY)
					break;
				moves[totalMoves] = Square(pos + movePos[0] + movePos[1] * 8);
				totalMoves++;
			}
			else {
				if ((pos >> 3) == 4 && (pos + 9 == board.enPassantSquare || pos + 7 == board.enPassantSquare)) {
					moves[totalMoves] = board.enPassantSquare;
					totalMoves++;
				}
				short movePos[2]{ -1,1 };
				if (Pieces::checkNode(true, pos, movePos, board)) {
					moves[totalMoves] = Square(pos + movePos[0] + movePos[1] * 8);
					totalMoves++;
				}
				movePos[0] += 2;
				if (Pieces::checkNode(true, pos, movePos, board)) {
					moves[totalMoves] = Square(pos + movePos[0] + movePos[1] * 8);
					totalMoves++;
				}
				movePos[0]--;
				if (board.pieces[pos + movePos[0] + movePos[1] * 8] != EMPTY)
					break;
				moves[totalMoves] = Square(pos + movePos[0] + movePos[1] * 8);
				totalMoves++;
				movePos[1]++;
				if ((pos >> 3) != 1 || board.pieces[pos + movePos[0] + movePos[1] * 8] != EMPTY)
					break;
				moves[totalMoves] = Square(pos + movePos[0] + movePos[1] * 8);
				totalMoves++;
			}
			break;
		default:
			board.logBoard();
			cout << "Piece: " << (int)piece << endl;
			cout << RED << "\n\n\nInvalid piece in getMovesForPiece...\n\n\n" << RESET;
			throw;
			break;
		}
	}

	static void getValidMovesForPiece(Square pos, Board& board, Square validMoves[56]) {
		Square moves[56];
		fill_n(moves, 56, 64);
		bool isPieceWhite = Pieces::isPieceWhite(board.pieces[pos]);
		unsigned short totalValidMoves = 0;
		Pieces::getMovesForPiece(pos, board, moves);
		for (int i = 0; i < 56; i++) {
			if (moves[i] == 64)
				return;
			Square cs[4];
			P cp[4]{ DEBUG,DEBUG ,DEBUG ,DEBUG };
			bool cv[4]{ false,false,false,false };
			Square oldEnPassantSquare = board.enPassantSquare;
			Pieces::movePiece(pos, moves[i], board, cs, cp, cv, board.isWhiteTurn ? QUEEN_W : QUEEN_B);
			if (!Pieces::isInCheck(isPieceWhite, board)) {
				validMoves[totalValidMoves] = moves[i];
				totalValidMoves++;
			}
			Pieces::unmovePiece(cs, cp, cv, oldEnPassantSquare, board);
		}
	}

	static bool isInCheck(bool isWhite, Board& board) {
		P kingPiece = isWhite ? KING_W : KING_B;
		Square kingPos;
		for (int i = 0; i < 64; i++) {
			if (board.pieces[i] == kingPiece) {
				kingPos = i;
				break;
			}
		}
		for (int i = 0; i < 64; i++) {
			if (board.pieces[i] == EMPTY || Pieces::isPieceWhite(board.pieces[i]) == isWhite)
				continue;
			if (board.pieces[i] == isWhite ? KING_B : KING_W) {
				int8_t thisKingX = kingPos & 7;
				int8_t thisKingY = kingPos >> 3;
				int8_t otherKingX = i & 7;
				int8_t otherKingY = i >> 3;
				if ((otherKingX >= thisKingX - 1 && otherKingX <= thisKingX + 1) && (otherKingY >= thisKingY - 1 && otherKingY <= thisKingY + 1))
					return true;
				continue;
			}
			Square moves[56];
			fill_n(moves, 56, 64);
			Pieces::getMovesForPiece(i, board, moves);
			for (int j = 0; j < 56; j++) {
				if (moves[j] == 64)
					break;
				if (moves[j] != kingPos)
					continue;
				return true;
			}
		}
		return false;
	}
	static string FileToChar;
	static string RankToChar;
	static int file_of(Square s) {
		return (int)s & 7;
	}

	static int rank_of(Square s) {
		return s >> 3;
	}
	static string square_to_string(Square s) {
		return string(1, FileToChar[file_of(s)]) + RankToChar[rank_of(s)];
	}
	static bool checkNode(bool shouldBeWhite, Square pos, short changePos[2], Board& board) {
		if (!Pieces::isValidPos(pos, changePos))
			return false;
		Square movePos = pos + changePos[0] + changePos[1] * 8;
		if (board.pieces[movePos] == EMPTY)
			return false;
		return Pieces::isPieceWhite(board.pieces[movePos]) == shouldBeWhite;
	}
	static bool isValidPos(Square pos, short changePos[2]) {
		uint8_t posY = pos >> 3,
			posX = pos & 7;
		if (posX + changePos[0] >= 8 ||
			posX + changePos[0] < 0 ||
			posY + changePos[1] >= 8 ||
			posY + changePos[1] < 0)
			return false;
		return true;
	}
	static bool isPieceWhite(P piece) {
		return piece < 6;
	}
	static bool isPieceBlack(P piece) {
		return piece > 5;
	}
	static void toWhitePiece(P* piece) {
		if (*piece < 6)
			return;
		*piece = P(*piece - 6);
	}
	static uint64_t genHash(Board &board) {
		
	}
	static uint64_t randomNums[64][13];
};