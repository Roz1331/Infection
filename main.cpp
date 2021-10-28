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

int main(string args[])
{
	int state[6][6];

	initGame(state);

	infection game(state);
	//game.startGame(1, 1);

	game.botBattle(1);
}