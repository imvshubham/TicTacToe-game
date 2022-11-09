#include<iostream>
#include<fstream>
#define BOARD_CONSTANT 3
#define AI_MOVE 0
#define USER_MOVE 1
#define USER2_MOVE 2

using namespace std;

class Board {

protected:
	Board() {}
	char board[BOARD_CONSTANT][BOARD_CONSTANT];

	struct Move {
		int x;
		int y;
	};

	void InitializeBoard() {
		for (int i = 0; i < BOARD_CONSTANT; i++)
			for (int j = 0; j < BOARD_CONSTANT; j++)
				board[i][j] = '.';
	}

	void ShowBoard() {
		for (int i = 0; i < BOARD_CONSTANT; i++)
		{
			for (int j = 0; j < BOARD_CONSTANT; j++)
			{
				cout << "   " << board[i][j];
			}
			cout << "\n\n";
		}

	}

	void UpdateBoard(int i, int j, int CurrentPlayer, char Player_Sign, char Player_2Sign) {
		if (CurrentPlayer == USER_MOVE) {

			board[i][j] = Player_Sign;
		}
		else if (CurrentPlayer == AI_MOVE || CurrentPlayer == USER2_MOVE) {
			board[i][j] = Player_2Sign;
		}
	}

	char checkVictory() {

		if (board[0][0] != '.'
			&& ((board[0][0] == board[0][1] && board[0][1] == board[0][2])
				|| (board[0][0] == board[1][0] && board[1][0] == board[2][0]))) {
			return board[0][0];
		}

		else if (board[1][1] != '.'
			&& ((board[1][1] == board[0][0] && board[1][1] == board[2][2])
				|| (board[1][1] == board[0][2] && board[1][1] == board[2][0])
				|| (board[1][1] == board[0][1] && board[1][1] == board[2][1])
				|| (board[1][1] == board[1][0] && board[1][1] == board[1][2]))) {
			return board[1][1];
		}

		else if (board[2][2] != '.'
			&& ((board[2][2] == board[1][2] && board[2][2] == board[0][2])
				|| (board[2][2] == board[2][1] && board[2][2] == board[2][0]))) {
			return board[2][2];
		}

		return '~';
	}

};

class AI : protected Board {

private:
	bool isComputer;
	char player_sign, comp_sign;

	int max(int a, int b) {
		return a > b ? a : b;
	}

	int min(int a, int b) {
		return a > b ? b : a;
	}

	int MiniMax(int search_depth, bool isComputer, int counter) {

		if (checkVictory() == comp_sign)
			return 10 - search_depth;

		else if (checkVictory() == player_sign)
			return -10 + search_depth;

		else if (counter == 10)
			return 0;

		if (isComputer) {
			int best = -1000;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (board[i][j] == '.') {
						board[i][j] = comp_sign;
						best = max(best, MiniMax(search_depth + 1, !isComputer, counter + 1));
						board[i][j] = '.';
					}
				}
			}
			return best;
		}
		else {
			int best = 1000;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					if (board[i][j] == '.') {
						board[i][j] = player_sign;
						best = min(best, MiniMax(search_depth + 1, !isComputer, counter + 1));
						board[i][j] = '.';
					}
				}
			}
			return best;
		}
	}

protected:
	AI() {}

	Move AIMove(char comp_sign, char player_sign, int counter) {

		this->player_sign = player_sign;
		this->comp_sign = comp_sign;

		int bestVal = -1000;
		Move bestmove;
		bestmove.x = bestmove.y = -1;

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (board[i][j] == '.') {
					board[i][j] = comp_sign;
					int moveVal = MiniMax(0, false, counter + 1);
					board[i][j] = '.';

					if (moveVal >= bestVal) {
						bestmove.x = i;
						bestmove.y = j;
						bestVal = moveVal;
					}
				}
			}
		}
		return bestmove;
	}
};

class Game : private AI {

private:
	Move move, aiMove;
	int counter;
	char play_with_comp, Player_Sign, Player_2Sign, victory, first_move;
	bool vict, change;

	void playerTurn(int CurrentPlayer) {
	inception:
		// );
		ShowBoard();

		if (CurrentPlayer == USER_MOVE)
			cout << "PlayerI: ";
		else
			cout << "Player II: ";

		cout << "Enter the Co-ordinates: ";
		cin >> move.x;

		if (move.x == 10)
			counter = 10;

		else {
			cin >> move.y;
			move.x--; move.y--;

			if (move.x > 3 || move.x < 0 || move.y > 3 || move.y < 0) {           // Checking validity of Coordinates
				cout << "\n Invalid Co-Ordinates!Please Enter Again.\n";
				system("pause");
				goto inception;
			}

			if (board[move.x][move.y] != '.') {
				cout << "\nCo-ordinate is Pre-Occupied.Please ENTER again.\n";
				system("pause");
				goto inception;
			}

			UpdateBoard(move.x, move.y, CurrentPlayer, Player_Sign, Player_2Sign);     // Updating the Board
		}
	}

	void Victory() {
		victory = checkVictory();
		if (victory != '~')
		{
			 //);
			ShowBoard();
			vict = true;
			if (victory == Player_Sign)
				if (play_with_comp == 'y' || play_with_comp == 'Y')
					cout << "Cheers! You won.\n";
				else
					cout << "Cheers! PlayerI won.\n";
			else
				if (play_with_comp == 'y' || play_with_comp == 'Y') {

					cout << "Yeah! I won\n";
					cout << "Gear up and try again next time.\n";
				}
				else
					cout << "PlayerII won.\n";
			system("pause");
		}
	}

public:

	Game() :counter(1), first_move('y'), victory('~'), vict(false), change(false)
	{
		InitializeBoard();
	}

	void startGame() {

	play_with_comp:
		cout << "\nDo you want to play with me(Computer)?(y/n) ";
		cin >> play_with_comp;

		if (play_with_comp != 'Y' && play_with_comp != 'y' && play_with_comp != 'N' && play_with_comp != 'n')
			goto play_with_comp;

		if (play_with_comp == 'y' || play_with_comp == 'Y') {

		first_move:
			cout << "\nDo you want to play first?(y/n) ";
			cin >> first_move;

			if (first_move != 'Y' && first_move != 'y' && first_move != 'N' && first_move != 'n')
				goto first_move;

		}

	take_userone_symbols:
		cout << "\nEnter PlayerI's Symbol : ";                                      // Taking Signs.
		cin >> Player_Sign;

		if (Player_Sign == '.') {
			cout << "\nDue to some Algorithmic issues, You have to take some other Symbol.\n";
			goto take_userone_symbols;
		}

	take_usertwo_symbols:
		if (play_with_comp == 'y' || play_with_comp == 'Y')
			cout << "\nEnter my Symbol : ";
		else
			cout << "\nEnter PlayerII's Symbol : ";
		cin >> Player_2Sign;

		if (Player_2Sign == '.' || Player_2Sign == Player_Sign) {
			cout << "\nDue to some Algorithmic issues, You have to take some other Symbol.\n";
			goto take_usertwo_symbols;
		}

		while (counter < 10)
		{
			if (vict) break;

			if (play_with_comp == 'y' || play_with_comp == 'Y')
			{
				if ((first_move == 'y' || first_move == 'Y'))
				{
					if (change == false) {
						playerTurn(USER_MOVE);

						change = true;
						Victory();
						if (vict) break;
					}

					else {

						aiMove = AIMove(Player_2Sign, Player_Sign, counter);
						UpdateBoard(aiMove.x, aiMove.y, AI_MOVE, Player_Sign, Player_2Sign);

						change = false;
						Victory();
						if (vict) break;
					}
				}
				else if ((first_move == 'n' || first_move == 'N'))
				{
					if (change == false) {

						aiMove = AIMove(Player_2Sign, Player_Sign, counter);
						UpdateBoard(aiMove.x, aiMove.y, AI_MOVE, Player_Sign, Player_2Sign);

						change = true;
						Victory();
						if (vict) break;
					}

					else {
						playerTurn(USER_MOVE);

						change = false;
						Victory();
						if (vict) break;
					}

				}
			}
			else
			{
				if (change == false) {
					change = true;  playerTurn(USER_MOVE); Victory();
					if (vict) break;
				}
				else {
					change = false;  playerTurn(USER2_MOVE); Victory();
					if (vict) break;
				}
			}

			counter++;
		} // End of While

		if ((counter == 10) & (!vict)) {
			// );
			ShowBoard();
			cout << "Cat's Game.\n";
			system("pause");
		}
	} // End of Function

};

int main() {
	char ch = 'y';
	while (ch == 'y' || ch == 'Y') {
		Game game;
		char get_info;

	start:
		cout << "Do you want to see the Instructions (y/n)? ";
		cin >> get_info;

		if (get_info != 'Y' && get_info != 'y' && get_info != 'N' && get_info != 'n')
			goto start;

		if (get_info == 'y') {
			fstream fout;
			fout.open("T3-The game.txt", ios::binary | ios::in | ios::app);

			if (!fout) {
				cout << "OOPS! Cannot open file";
				goto play;
			}

			char ch;
			while (!fout.eof()) {

				fout.get(ch);
				cout << ch;
			}
		}
		else {
			cout << "All the best!";
		}

		play:
		game.startGame();
		// );

		cout << "Want to play again?(y/n) ";
		cin >> ch;
	}
	system("pause");
	return 0;
}
