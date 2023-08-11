#pragma once
#include <sstream>
#include "main.h"
#include <chrono>

inline void uci(Board &board, Bot bot) {
	cout << "id name best_bot_ever author sidney_de_beste" << endl;
	cout << "uciok" << endl;
	string token, cmd;
	do {
		if (!std::getline(std::cin, cmd))
			cmd = "quit";

		std::istringstream is(cmd);

		token.clear();
		is >> std::skipws >> token;
		if (token == "isready") {
			cout << "readyok" << endl;
		}
		else if (token == "ucinewgame") {
			board.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		}
		else if (token == "position") {
			is >> token;
			if (token == "fen") {
				string fen = "";

				while (true) {
					is >> token;
					if (token == "moves" || !is) break;
					fen += " " + token;
				}

				fen.erase(0, 1);
				board.setFen(fen);
			}
			else if(token == "startpos") {
				board.setFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
				is >> token;
			}
			if (token == "moves") {
				while (true) {
					is >> token;
					if (!is) break;
					Pieces::uciMovePiece(token, board);
				}
			}
		}
		else if(token == "go") {
			string lookingFor = board.isWhiteTurn ? "wtime" : "btime";
			cout << "info string " << "looking for: " << lookingFor << endl;
			while (is >> token) {
				if (token == lookingFor || token == "movetime")
					break;
			}
			if (!is) {
				is >> token;
				int timeLeft = 15000;

				auto startTime = std::chrono::high_resolution_clock::now();
				unsigned short depth = 0;
				string lastMove;
				while (true) {
					depth++;
					lastMove = bot.genNewMove(board, depth, false);
					if (depth <= 4) continue;
					auto endTime = std::chrono::high_resolution_clock::now();
					auto time = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
					cout << "info string " << "time: " << time.count() << " timeLeft: " << timeLeft << endl;
					if (timeLeft - time.count() * 15 > 15000) continue;
					cout << "bestmove " << lastMove << endl;
					Pieces::uciMovePiece(lastMove, board);
					break;
				}
			}
			else {
				is >> token;
				int timeLeft = stoi(token);

				auto startTime = std::chrono::high_resolution_clock::now();;
				unsigned short depth = 0;
				string lastMove;
				unsigned int lastTime;
				while (true) {
					startTime = std::chrono::high_resolution_clock::now();
					depth++;
					lastMove = bot.genNewMove(board, depth, false);
					if(depth == 3)
						lastTime = chrono::duration_cast<chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
					if (depth < 4) continue;
					auto endTime = std::chrono::high_resolution_clock::now();
					unsigned time = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
					short branchingFactor = time / max((int)lastTime,1);
					if (timeLeft - time * branchingFactor > 10000 && time * branchingFactor < timeLeft / 10) {
						lastTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
						continue;
					}
					cout << "info string branchingFactor: " << (int)branchingFactor << endl;
					cout << "info string depth: " << depth << endl;
					cout << "bestmove " << lastMove << endl;
					Pieces::uciMovePiece(lastMove, board);
					break;
				}
			}
		}
		else if (token == "logboard") {
			board.logBoard();
		}

	} while (token != "quit");
}