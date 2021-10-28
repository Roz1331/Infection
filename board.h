#include <iostream>

using namespace std;

namespace BOARD
{
	struct board
	{
		int state[6][6];
		int heur;

		board(int st[6][6])
		{
			for (int i = 0; i < 6; i++)
				for (int j = 0; j < 6; j++)
					state[i][j] = st[i][j];
			heur = heuristic(1);
		}

		board() {}

		int heuristic(int player)
		{
			int playerScore = 0;
			int compScore = 0;
			for (int i = 0; i < 6; i++)
				for (int j = 0; j < 6; j++)
					if (state[i][j] == player)
						playerScore++;
					else if (state[i][j] != 0)
						compScore++;
			return playerScore - compScore;
		}
	};
}