#include <iostream>
#include <vector>
#include <utility>
#include <time.h>
#define ll long long
#define ld long double
#define max_depth 2
using namespace std;
const ll mod = 1e9 + 7;

class plyr
{
public:
    int board[7][7] = {};
    bool is_order = true;
    int score = 0;
	int calculate_score();
	int minimax_chaos(int depth, bool is_max);
	int minimax_order(int depth, bool is_max);
    void execute_order_move(int x1, int y1, int x2, int y2);
    void execute_chaos_move(int x1, int y1, int color);
	void chaos_respond_move(string prev_move, int color);
	void order_respond_move(string prev_move);
	void print_board();
	vector <pair <pair <int, int>, pair <int, int>>> generate_valid_order_moves();
	vector <pair <int, int>> generate_valid_chaos_moves();
};

void plyr::execute_chaos_move(int x1, int y1, int color) {
	board[x1][y1] = color;
}

void plyr::execute_order_move(int x1, int y1, int x2, int y2) 
{
	if (x1 != x2 || y1 != y2) 
	{
		board[x2][y2] = board[x1][y1];
		board[x1][y1] = 0;
	}
}

void plyr::order_respond_move(string prev_move) 
{
	execute_chaos_move(prev_move[1] - 'A', prev_move[2] - 'a', prev_move[0] - '0');
	vector <pair <pair <int, int>, pair <int, int>>> valid_moves = generate_valid_order_moves();
	int n_valid_moves = valid_moves.size(), best_score = INT_MIN,
	best_x1 = valid_moves[0].first.first, 
	best_y1 = valid_moves[0].first.second, 
	best_x2 = valid_moves[0].first.first, 
	best_y2 = valid_moves[0].first.second;
	for (int i = 0; i < n_valid_moves; i++) 
	{
		execute_order_move(valid_moves[i].first.first,
						   valid_moves[i].first.second,
						   valid_moves[i].second.first,
						   valid_moves[i].second.second);
		int best_sce = minimax_chaos(1, false);
		if (best_sce > best_score)
		{
			best_score = best_sce; 
			best_x1 = valid_moves[i].first.first; 
			best_y1 = valid_moves[i].first.second; 
			best_x2 = valid_moves[i].second.first;
			best_y2 = valid_moves[i].second.second;
		}
		execute_order_move(valid_moves[i].second.first,
						   valid_moves[i].second.second,
						   valid_moves[i].first.first,
						   valid_moves[i].first.second);
	}
	execute_order_move(best_x1, best_y1, best_x2, best_y2);
	cout << char(best_x1 + 'A') << char(best_y1 + 'a') << char(best_x2 + 'A') << char(best_y2 + 'a') << endl;
}

void plyr::chaos_respond_move(string prev_move, int color) 
{
	if (!(prev_move == "Start")) execute_order_move(prev_move[0] - 'A', prev_move[1] - 'a', prev_move[2] - 'A', prev_move[3] - 'a');
	vector <pair <int, int>> valid_moves = generate_valid_chaos_moves();
	int n_valid_moves = valid_moves.size(),
	best_x1 = valid_moves[0].first, 
	best_y1 = valid_moves[0].second, 
	best_score = INT_MIN;
	for (int i = 0; i < n_valid_moves; i++) 
	{
		execute_chaos_move(valid_moves[i].first, valid_moves[i].second, color);
		int max_score = minimax_order(1, false);
		if (max_score > best_score)
		{
			best_x1 = valid_moves[i].first;
			best_y1 = valid_moves[i].second;
			best_score = max_score;
		}
		execute_chaos_move(valid_moves[i].first, valid_moves[i].second, 0);
	}
	execute_chaos_move(best_x1, best_y1, color);
	cout << char(best_x1 + 'A') << char(best_y1 + 'a') << endl;
}

int plyr::minimax_chaos(int depth, bool is_max)
{
	if (depth == max_depth) {
		return calculate_score();
	}
	else {
		vector <pair <int, int>> valid_moves = generate_valid_chaos_moves();
		int n_valid_moves = valid_moves.size();
		int best_score;
		if (is_max)
			best_score = INT_MIN;	
		else
			best_score = INT_MAX;
			
		for (int i = 0; i < n_valid_moves; i++) 
		{
			for (int color = 1; color <= 7; color++) {
				execute_chaos_move(valid_moves[i].first, valid_moves[i].second, color);
				int score = minimax_order(depth + 1, !is_max);
				if (is_max) {
					if (score > best_score)
						best_score = score;
				}
				else {
					if (score < best_score)
						best_score = score;
				}
				
				execute_chaos_move(valid_moves[i].first, valid_moves[i].second, 0);
			}
		}
//		cout << "DEPTH " << depth << ":\n";
//		cout << best_score << "\n"; 
		return best_score;
	}
}


int plyr::minimax_order(int depth, bool is_max)
{
	if (depth == max_depth) {
		return calculate_score();
	}
	else {
	vector <pair <pair <int, int>, pair <int, int>>> valid_moves = generate_valid_order_moves();
		int n_valid_moves = valid_moves.size();
		int best_score;
		if (is_max)
			best_score = INT_MIN;
		else
			best_score = INT_MAX;
			
		for (int i = 0; i < n_valid_moves; i++) 
		{
			execute_order_move(valid_moves[i].first.first,
							   valid_moves[i].first.second,
							   valid_moves[i].second.first,
							   valid_moves[i].second.second);
			int score = minimax_chaos(depth + 1, !is_max);
			if (is_max) 
			{
				if (score > best_score)
					best_score = score;
			}
			else 
			{
				if (score < best_score) 
				{
					best_score = score;
				}
			}
			execute_order_move(valid_moves[i].second.first,
							   valid_moves[i].second.second,
							   valid_moves[i].first.first,
							   valid_moves[i].first.second);
		}
//		cout << "DEPTH " << depth << ":\n";
//		cout << best_score << "\n"; 
		return best_score;
	}
}

vector <pair <int, int>> plyr::generate_valid_chaos_moves() 
{
	vector <pair <int, int>> valid_moves;
	for (int i = 0; i < 7; i++) 
	{
		for (int j = 0; j < 7; j++) 
		{
			if (board[i][j] == 0) valid_moves.push_back({i, j});
		}
	}
	return valid_moves;
}
 
vector <pair <pair <int, int>, pair <int, int>>> plyr::generate_valid_order_moves() 
{
	vector <pair <pair <int, int>, pair<int, int>>> valid_moves;
	for (int i = 0; i < 7; i++) 
	{
		for (int j = 0; j < 7; j++) 
		{
			if (board[i][j] > 0) 
			{
				valid_moves.push_back({{i, j}, {i, j}});
				for (int k = i - 1; k >= 0; k--) 
				{
					if (board[k][j] > 0) break;
					valid_moves.push_back({{i, j}, {k, j}});
				}
				
				for (int k = i + 1; k < 7; k++) 
				{
					if (board[k][j] > 0) break;
					valid_moves.push_back({{i, j}, {k, j}});
				}
				
				for (int k = j - 1; k >= 0; k--) 
				{
					if (board[i][k] > 0) break;
					valid_moves.push_back({{i, j}, {i, k}});
				}
				
				
				for (int k = j + 1; k < 7; k++) 
				{
					if (board[i][k] > 0) break;
					valid_moves.push_back({{i, j}, {i, k}});
				}
			}
		}
	}
	return valid_moves;
}

int plyr::calculate_score() 
{
	int palindrome_score = 0;
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
        	if (board[i][j] == 0) continue;
            int left = j - 1, right = j + 1;
            while (left >= 0 && right < 7)
            {
                if (board[i][left] == board[i][right] && board[i][left] != 0 && board[i][right] != 0)
                    palindrome_score += (right - left + 1);
                else
                    break;
                left--;
            	right++;
            }
        }
        for (int j = 0; j < 7; j++)
        {
        	if (board[i][j] == 0) continue;
            int left = j, right = j + 1;
            while (left >= 0 && right < 7)
            {
                if (board[i][left] == board[i][right] && board[i][left] != 0 && board[i][right] != 0)
                    palindrome_score += (right - left + 1);
                else
                    break;
                left--;
                right++;
            }
        }
    }
    
    for (int i = 0; i < 7; i++)
    {
		for (int j = 0; j < 7; j++)
        {
        	if (board[j][i] == 0) continue;
            int top = j - 1, bottom = j + 1;
            while (top >= 0 && bottom < 7)
            {
                if (board[top][i] == board[bottom][i] && board[top][i] != 0 && board[bottom][i] != 0)
                    palindrome_score += (bottom - top + 1);
                else
                    break;
                top--;
                bottom++;
            }
        }
        for (int j = 0; j < 7; j++)
        {
        	if (board[j][i] == 0) continue;
            int top = j, bottom = j + 1;
            while (top >= 0 && bottom < 7)
			{
                if (board[top][i] == board[bottom][i] && board[top][i] != 0 && board[bottom][i] != 0)
                    palindrome_score += (bottom - top + 1);
                else
                    break;
                top--;
                bottom++;
            }
        }
    }
    if (!is_order) {
    	return 280 - palindrome_score;
	} else {
		return 120 + palindrome_score;
	}
}

int main()
{
    //	ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    plyr player1;
    memset(player1.board, 0, sizeof player1.board);
	// Recieve and respond to opponent's move
	string input;
	while (cin >> input && input != "Quit") 
	{
		if (input == "Start") 
		{
			player1.is_order = false;
		}
		
		if (!player1.is_order) 
		{
			int color;
			cin >> color;
    		player1.chaos_respond_move(input, color);
		}
		 
		else 
		{
			player1.order_respond_move(input);
		}
	}
}
