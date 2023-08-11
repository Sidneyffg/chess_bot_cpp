#pragma once
#include <string>
#include <map>
#include <iostream>
#include "main.h"


class Board
{
public:
	Board(const Board&) = delete;
	Board() = default;
	Board(string board) {
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 13; j++) {
				if ((char)board[i] != charPieces[j])
					continue;
				pieces[i] = (P)j;
				break;
			}
		}

		pastMoves = new uint64_t[400];
	};
	unsigned totalMoves = 0;
	bool shouldLog = false;
	P pieces[64];
	bool isWhiteTurn = true;
	bool lastMoveCapture = false;
	bool canWhiteCastle[2]{ true,true };
	bool canBlackCastle[2]{ true,true };
	Square enPassantSquare = 64;
	uint64_t *pastMoves;


	void logBoard() {
		for (int y = 0; y < 8; y++) {
			cout << "  +---+---+---+---+---+---+---+---+\n";
			cout << abs(y - 7) + 1 << " ";
			for (int x = 0; x < 8; x++) {
				char piece = charPieces[pieces[x + y * 8]];
				if (piece == 'e')
					cout << "|   ";
				else
					cout << "| " << piece << " ";
			}

			cout << "|\n";
		}
		cout << "  +---+---+---+---+---+---+---+---+\n";
		cout << "    a   b   c   d   e   f   g   h  \n";
	}

	void setFen(const string& fen) {
		if(this->shouldLog)
			cout << fen << endl;
		Square pos = 0;
		for (int i = 0; i < fen.size(); i++) {
			if (isdigit(fen[i])) {
				for (int j = 0; j < fen[i] - '0'; j++) {
					this->pieces[pos] = EMPTY;
					pos++;
				}
			}
			else {
				if (fen[i] == '/')
					continue;
				if (fen[i] == ' ') {
					this->isWhiteTurn = fen[i + 1] == 'w';
					i += 3;
					this->canWhiteCastle[0] = false;
					this->canWhiteCastle[1] = false;
					this->canBlackCastle[0] = false;
					this->canBlackCastle[1] = false;
					while (fen[i] != ' ') {
						switch (fen[i]) {
						case 'K':
							this->canWhiteCastle[1] = true;
							break;
						case 'Q':
							this->canWhiteCastle[0] = true;
							break;
						case 'k':
							this->canBlackCastle[1] = true;
							break;
						case 'q':
							this->canBlackCastle[0] = true;
							break;
						}
						i++;
					}
					i++;
					if(fen[i] == '-')
						break;
					
					char charPos[2] = { fen[i], fen[i + 1] };
					this->enPassantSquare = posToSquare(charPos);
					break;
				}
				for (int j = 0; j < 12; j++) {
					if (charPieces[j] == fen[i]) {
						this->pieces[pos] = (P)j;
						pos++;
						break;
					}
				}
			}
		}
		if(this->shouldLog)
			this->logBoard();
	}
};



/*106743106 +
137077698 +
133233975 +
134087476 +
144074944 +
157756443 +
227598692 +
269605599 +
306139086 +
309478878 +
102021008 +
119614891 +
135987938 +
130293688 +
106678423 +
138495340 +
120142144 +
148527161 +
147678952 +
120669851*/