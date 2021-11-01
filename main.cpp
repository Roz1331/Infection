#include <iostream>
#include <vector>
#include <tuple>
#include "board.h"
#include "infection.h"

using namespace std;
using namespace BOARD;
using namespace INFECTION;

string  toLowerCase(string str)
{
	transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return tolower(c); });
	return str;
}

void initGame(int state[6][6])
{
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
			state[i][j] = 0;
	state[0][0] = 1;
	state[5][5] = 1;
	state[5][0] = 2;
	state[0][5] = 2;
}

int main(int argc, char* argv[])
{
	int state[6][6];

	initGame(state);

	infection game(state);
	//game.startGame(1, 2);

	//game.botBattle(argv[argc - 1][0] - '0' + 1);
	//game.botBattle();
	game.botBattle(5, 5);

}