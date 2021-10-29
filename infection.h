#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>

using namespace BOARD;

namespace INFECTION
{
	struct infection
	{
		board state;
		int winner;
		int curPlayer;
		int firstMove;
		int maxDepth = 4;
		string bestFrom;
		string bestTo;

		infection(int st[6][6], int pl = 1, int fm = 1)
		{
			state = board(st);
			curPlayer = pl;
			firstMove = fm;
		}

		void initArray(int copyfrom[6][6], int copyto[6][6])
		{
			for (int i = 0; i < 6; i++)
				for (int j = 0; j < 6; j++)
					copyto[i][j] = copyfrom[i][j];
		}

		void infectOpponent(int field[6][6], int x, int y, int player)
		{
			for (int i = 0; i < 6; i++)
				for (int j = 0; j < 6; j++)
					if (abs(i - x) <= 1 && abs(y - j) <= 1 && field[i][j] != player && field[i][j] != 0)
						field[i][j] = player;
		}

		vector<board> moves(board st, int player)
		{
			bestFrom = "";
			vector<board> result;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					// для "своего" элемента на доске
					if (st.state[i][j] == player)
					{
						for (int ii = 0; ii < 6; ii++)
						{
							for (int jj = 0; jj < 6; jj++)
							{
								if (st.state[ii][jj] == 0)
								{
									if (abs(i - ii) <= 1 && abs(j - jj) <= 1) // обрабатываем первый слой
									{
										char charFrom = j + 'a';
										char intFrom = i + '0' + 1;

										bestFrom += charFrom;
										bestFrom += intFrom;

										char charTo = jj + 'a';
										char intTo = ii + '0' + 1;

										bestTo += charTo;
										bestTo += intTo;

										int tmp[6][6];
										initArray(st.state, tmp);

										tmp[ii][jj] = player;

										// "заражаем" ячейки противника
										infectOpponent(tmp, ii, jj, player);
										result.push_back(board(tmp));
									}
									else if (abs(i - ii) <= 2 && abs(j - jj) <= 2) //обрабатываем второй слой
									{
										int tmp[6][6];
										initArray(st.state, tmp);

										tmp[i][j] = 0;
										tmp[ii][jj] = player;

										// "заражаем" ячейки противника
										infectOpponent(tmp, ii, jj, player);
										result.push_back(board(tmp));
									}
								}
							}
						}
					}
				}
			}
			return result;
		}

		tuple<board, int> findBestMoveForFirstPlayer(board brd, int player = 1, int depth = 4, int currentStep = 1, int alpha = INT_MIN, int beta = INT_MAX)
		{
			if (depth == 0)
				return tuple<board, int>(brd, brd.heur);

			int cur;
			int localBest = INT_MAX;
			board localBestBoard = state;

			if (currentStep % 2 == 0) // четный слой - слой min, обновляем beta
			{
				for (board move : moves(brd, player))
				{
					cur = get<1>(findBestMoveForFirstPlayer(move.state, player == 1 ? 2 : 1, depth - 1, currentStep + 1, alpha, beta));

					beta = min(cur, beta);

					if (cur < localBest)
					{
						localBest = cur;
						localBestBoard = move;
					}

					if (beta < alpha) // отсечение
						break;
				}
			}
			else // нечетный слой - слой max, обновляем alpha
			{
				localBest = INT_MIN;
				for (board move : moves(brd, player))
				{
					cur = get<1>(findBestMoveForFirstPlayer(move.state, player == 1 ? 2 : 1, depth - 1, currentStep + 1, alpha, beta));

					alpha = max(alpha, cur);

					if (cur > localBest)
					{
						localBest = cur;
						localBestBoard = move;
					}

					if (beta < alpha) // отсечение
						break;
				}
			}

			return make_tuple(localBestBoard, localBest);
		}

		tuple<board, int> findBestMoveForSecondPlayer(board brd, int player = 2, int depth = 4, int currentStep = 1, int alpha = INT_MIN, int beta = INT_MAX)
		{
			if (depth == 0)
				return tuple<board, int>(brd, -brd.heur);

			int cur;
			int localBest = INT_MAX;
			board localBestBoard = state;

			if (currentStep % 2 == 0) // четный слой - слой min, обновляем beta
			{
				for (board move : moves(brd, player))
				{
					cur = get<1>(findBestMoveForSecondPlayer(move.state, player == 1 ? 2 : 1, depth - 1, currentStep + 1, alpha, beta));

					beta = min(cur, beta);

					if (cur < localBest)
					{
						localBest = cur;
						localBestBoard = move;
					}

					if (beta < alpha) // отсечение
						break;
				}
			}
			else // нечетный слой - слой max, обновляем alpha
			{
				localBest = INT_MIN;
				for (board move : moves(brd, player))
				{
					cur = get<1>(findBestMoveForSecondPlayer(move.state, player == 1 ? 2 : 1, depth - 1, currentStep + 1, alpha, beta));

					alpha = max(alpha, cur);

					if (cur > localBest)
					{
						localBest = cur;
						localBestBoard = move;
					}

					if (beta < alpha) // отсечение
						break;
				}
			}

			return make_tuple(localBestBoard, localBest);
		}

		void makeBestMove(int player, int d)
		{
			if (player == 1)
				state = get<0>(findBestMoveForFirstPlayer(state, 1, d));
			else if(player == 2) state = get<0>(findBestMoveForSecondPlayer(state, 2, d));
		}
		
		bool isGameOver()
		{
			int score1 = 0;
			int score2 = 0;
			int emptyCells = 0;

			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					if (state.state[i][j] == 1)
						score1++;
					else if (state.state[i][j] == 2)
						score2++;
					else emptyCells++;
				}
			}
			
			// определяем победителя

			if (score1 == 0 )
			{
				winner = 2;
				return true;
			}
			else if (score2 == 0)
			{
				winner = 1;
				return true;
			}
			else if (emptyCells ==0 || (moves(state, 1).size() == 0 && moves(state, 2).size() == 0))
			{
				if (state.heur > 0)
					winner = 1;
				else if (state.heur < 0)
					winner = 2;
				else
					winner = 0; // draw
				return true;
			}
			return false;
		}

		string toLowerCase(string str)
		{
			transform(str.begin(), str.end(), str.begin(),
				[](unsigned char c) { return tolower(c); });
			return str;
		}

		int cointPoints(int pl)
		{
			int res = 0;

			for (int i = 0; i < 6; i++)
				for (int j = 0; j < 6; j++)
					if (state.state[i][j] == pl)
						res++;
			return res;
		}

		void playerMove()
		{
			string input1, input2;
			bool startFlag = true;

			// считываем ходы с консоли
			while (startFlag)
			{
				cin >> input1 >> input2;

				if (input1.size() != 2 || input2.size() != 2)
				{
					cout << "invalid input! try againt" << endl;
					continue;
				}

				input1 = toLowerCase(input1);
				input2 = toLowerCase(input2);
				
				if (!isalpha(input1[0]) || !isalpha(input2[0]) || !isdigit(input1[1]) || !isdigit(input2[1]))
				{
					cout << "invalid input! try againt" << endl;
					continue;
				}

				int charFrom = input1[0] - 'a';
				int intFrom = input1[1] - '0' - 1;
				int charTo = input2[0] - 'a';
				int intTo = input2[1] - '0' - 1;

				// проверяем, корректно ли задан ход
				if(state.state[intFrom][charFrom] != curPlayer || state.state[intTo][charTo] != 0)
				{
					cout << "invalid input! try againt" << endl;
					continue;
				}
				if (abs(charFrom - charTo) > 2 || abs(intFrom - intTo) > 2)
				{
					cout << "invalid input! try againt" << endl;
					continue;
				}
				if (charFrom > 5 || intFrom > 5 || charTo > 5 || intTo > 5) 
				{
					cout << "invalid input! try againt" << endl;
					continue;
				}

				if (abs(charFrom - charTo) <= 1 && abs(intFrom - intTo) <= 1)
				{
					state.state[intTo][charTo] = curPlayer;
					// infecting
					for (int i = 0; i < 6; i++)
					{
						for (int j = 0; j < 6; j++)
						{
							if (abs(i - intTo) <= 1 && abs(j - charTo) <= 1 && state.state[i][j] != 0)
								state.state[i][j] = curPlayer;
						}
					}
				}
				else if (abs(charFrom - charTo) <= 2 && abs(intFrom - intTo) <= 2)
				{
					state.state[intFrom][charFrom] = 0;
					state.state[intTo][charTo] = curPlayer;

					// infecting
					for (int i = 0; i < 6; i++)
					{
						for (int j = 0; j < 6; j++)
						{
							if (abs(i - intTo) <= 1 && abs(j - charTo) <= 1 && state.state[i][j] != 0)
								state.state[i][j] = curPlayer;
						}
					}
				}

				startFlag = false;
			}
		}
		
		void startGame(int pl = 1, int fm = 1)
		{
			curPlayer = pl;
			firstMove = fm;

			print();

			if (firstMove != curPlayer)
			{
				if (!moves(state, 2).empty())
				{
					/*makeBestMove(curPlayer == 1 ? 2 : 1, maxDepth);*/
					makeBestMove(2, maxDepth);
					cout << "opponent move" << endl;
					print();
				}
			}

			while (!isGameOver())
			{
				if (!moves(state, 1).empty())
				{
					cout << "your turn" << endl;
					playerMove();
					print();
				}
				if (!moves(state, 2).empty())
				{
					cout << "opponent move" << endl;
					makeBestMove(2, maxDepth);
					print();
				}
			}

			switch (winner)
			{
			case 1:
				cout << "Well done! You won!" << endl;
				break;
			case 2:
				cout << "Sorry, you lost(((" << endl;
				break;
			case 0:
				cout << "Not bad. Draw!" << endl;
				break;
			}
		}

		void botBattle(int depth1 = 4, int depth2 = 4, int turn = 1)
		{
			firstMove = turn;

			print();

			if (firstMove != curPlayer)
			{
				if (!moves(state, 2).empty())
				{
					cout << "player 2 move" << endl;
					makeBestMove(curPlayer == 1 ? 2 : 1, depth1);
					print();
				}
			}

			while (!isGameOver())
			{
				if (!moves(state, 1).empty())
				{
					cout << "player 1 move" << endl;
					makeBestMove(curPlayer, depth1);
					print();
				}
				if (!moves(state, 2).empty())
				{
					cout << "player 2 move" << endl;
					makeBestMove(2, depth2);
					print();
				}
			}

			switch (winner)
			{
			case 1:
				cout << "Player 1 won" << endl;
				break;
			case 2:
				cout << "Player 2 won" << endl;
				break;
			case 0:
				cout << "Not bad. Draw!" << endl;
				break;
			}
		}
		
		void print()
		{
			cout << "   ";
			for (int i = 0; i < 6; i++)
				cout << (char)('a' + i) << ' ';
			cout << endl << "  ";
			for (int i = 0; i < 6; i++)
				cout << "__";
			cout << endl;
			for (int i = 0; i < 6; i++)
			{
				cout << i + 1 << "| ";
				for (int j = 0; j < 6; j++)
					cout << state.state[i][j] << ' ';
				cout << endl;
			}
			cout << endl;
		}
	};
}
