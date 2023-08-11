#include <iostream>
#include <string>
#include "Board.h"
#include "Pieces.h"
#include "Bot.h"
#include <sstream>
#include "uci.h"
using namespace std;
typedef uint8_t Square;

int main()
{
	Pieces::init();
	//rebqkbnrpeppppppnpeeeeeeeeeeeeeeeeeeeePeeeeeeeeePPPPPPBPRNBQKeNR
	Board board("rebqkbnrpeppppppnpeeeeeeeeeeeeeeeeeeeePeeeeeeeeePPPPPPBPRNBQKeNR");
	Bot bot;
	string token, cmd;
	//board.setFen("4r1r1/p1pk1n2/1p1p1P2/5P1P/P1PP3R/5P2/1B2pQB1/2KR4 b - - 1 39");
	board.setFen("8/8/2K5/8/8/8/7P/k7 w - - 0 1");
	do {
		if (!std::getline(std::cin, cmd))
			cmd = "quit";

		std::istringstream is(cmd);

		token.clear();
		is >> std::skipws >> token;
		if (token == "uci") {
			board.shouldLog = false;
			uci(board,bot);
			return 69ull;
		}else if (token == "move") {
			is >> token;
			Pieces::uciMovePiece(token, board);
			board.logBoard();
			bot.genNewMove(board, 5, true);
			board.logBoard();
		}
		else if (token == "bot") {
			if (board.isWhiteTurn) {
				cout << "It's your turn..." << endl;
				//continue;
			}
			bot.genNewMove(board,9,true);
			board.logBoard();
		}
		else if (token == "getmoves") {
			is >> token;
			Square pos = posToSquare(token);
			Square moves[56];
			fill_n(moves, 56, 64);
			Pieces::getMovesForPiece(pos, board, moves);
			Board tempBoard;
			memcpy(&tempBoard, &board, sizeof(Board));
			for (int i = 0; i < 56; i++) {
				if (moves[i] == 64)
					break;
				tempBoard.pieces[moves[i]] = DEBUG;
			}
			tempBoard.logBoard();
		}
		else if (token == "fen") {
			string fen = "";

			std::string fenPart;
			while (true) {
				is >> fenPart;
				if (!is) break;
				fen += " " + fenPart;
			}

			fen.erase(0, 1);
			board.setFen(fen);
		}
		else if (token == "perft") {
			is >> token;
			int depth = stoi(token);
			bot.perft(depth, board, board.isWhiteTurn);
		}
		else if (token == "logboard") {
			board.logBoard();
		}

	} while (token != "quit");
}