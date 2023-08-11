#include "Pieces.h"

short Pieces::piecesMoves[6][56][2] = {
		{
			{ -1, -1 },
			{ 0, -1 },
			{ 1, -1 },
			{ -1, 0 },
			{ 1, 0 },
			{ -1, 1 },
			{ 0, 1 },
			{ 1, 1 }
		},
		{
			{ -1, 0 },
			{ -2, 0 },
			{ -3, 0 },
			{ -4, 0 },
			{ -5, 0 },
			{ -6, 0 },
			{ -7, 0 },
			{ 1, 0 },
			{ 2, 0 },
			{ 3, 0 },
			{ 4, 0 },
			{ 5, 0 },
			{ 6, 0 },
			{ 7, 0 },
			{ 0, -1 },
			{ 0, -2 },
			{ 0, -3 },
			{ 0, -4 },
			{ 0, -5 },
			{ 0, -6 },
			{ 0, -7 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 },
			{ 0, 5 },
			{ 0, 6 },
			{ 0, 7 },
			{ -1, -1 },
			{ -2, -2 },
			{ -3, -3 },
			{ -4, -4 },
			{ -5, -5 },
			{ -6, -6 },
			{ -7, -7 },
			{ -1, 1 },
			{ -2, 2 },
			{ -3, 3 },
			{ -4, 4 },
			{ -5, 5 },
			{ -6, 6 },
			{ -7, 7 },
			{ 1, 1 },
			{ 2, 2 },
			{ 3, 3 },
			{ 4, 4 },
			{ 5, 5 },
			{ 6, 6 },
			{ 7, 7 },
			{ 1, -1 },
			{ 2, -2 },
			{ 3, -3 },
			{ 4, -4 },
			{ 5, -5 },
			{ 6, -6 },
			{ 7, -7 },
		},
		{
			{ -1, 0 },
			{ -2, 0 },
			{ -3, 0 },
			{ -4, 0 },
			{ -5, 0 },
			{ -6, 0 },
			{ -7, 0 },
			{ 1, 0 },
			{ 2, 0 },
			{ 3, 0 },
			{ 4, 0 },
			{ 5, 0 },
			{ 6, 0 },
			{ 7, 0 },
			{ 0, -1 },
			{ 0, -2 },
			{ 0, -3 },
			{ 0, -4 },
			{ 0, -5 },
			{ 0, -6 },
			{ 0, -7 },
			{ 0, 1 },
			{ 0, 2 },
			{ 0, 3 },
			{ 0, 4 },
			{ 0, 5 },
			{ 0, 6 },
			{ 0, 7 }
		},
		{
			{ -1, -1 },
			{ -2, -2 },
			{ -3, -3 },
			{ -4, -4 },
			{ -5, -5 },
			{ -6, -6 },
			{ -7, -7 },
			{ -1, 1 },
			{ -2, 2 },
			{ -3, 3 },
			{ -4, 4 },
			{ -5, 5 },
			{ -6, 6 },
			{ -7, 7 },
			{ 1, 1 },
			{ 2, 2 },
			{ 3, 3 },
			{ 4, 4 },
			{ 5, 5 },
			{ 6, 6 },
			{ 7, 7 },
			{ 1, -1 },
			{ 2, -2 },
			{ 3, -3 },
			{ 4, -4 },
			{ 5, -5 },
			{ 6, -6 },
			{ 7, -7 }
		},
		{
			{ -2, -1 },
			{ -1, -2 },
			{ 2, -1 },
			{ 1, -2 },
			{ -2, 1 },
			{ -1, 2 },
			{ 2, 1 },
			{ 1, 2 }
		}
};
unsigned short Pieces::piecesMovesLength[5]{ 8, 56, 28,28,8 };

Square Pieces::castlePiecesPos[6]{ 0, 4, 7,56, 60, 63 };


string Pieces::FileToChar("abcdefgh");
string Pieces::RankToChar("87654321");

uint64_t Pieces::randomNums[64][13];