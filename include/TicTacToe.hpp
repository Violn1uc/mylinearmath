#pragma once

#include <array>
#include <vector>
#include <limits>
#include <algorithm>

using Board = std::array<int, 9>; // 1 = bot, -1 = opponent, 0 = empty

inline bool hasWon(const Board &b, int player)
{
	static const int lines[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
					{0, 3, 6}, {1, 4, 7}, {2, 5, 8},
					{0, 4, 8}, {2, 4, 6}};
	for (const auto &line : lines)
		if (b[line[0]] == player && b[line[1]] == player &&
		    b[line[2]] == player)
			return true;
	return false;
}

inline bool isFull(const Board &b)
{
	return std::none_of(b.begin(), b.end(), [](int c) { return c == 0; });
}

inline std::vector<int> availableMoves(const Board &b)
{
	std::vector<int> moves;
	for (int i = 0; i < 9; ++i)
		if (b[i] == 0)
			moves.push_back(i);
	return moves;
}

inline int minimax(Board b, int player, int depth, bool maximizing)
{
	if (hasWon(b, player))
		return 10 - depth;
	if (hasWon(b, -player))
		return depth - 10;
	if (isFull(b))
		return 0;

	int best = maximizing ? std::numeric_limits<int>::min()
			      : std::numeric_limits<int>::max();

	for (int move : availableMoves(b))
	{
		b[move] = maximizing ? player : -player;
		int score = minimax(b, player, depth + 1, !maximizing);
		b[move] = 0;
		best =
		    maximizing ? std::max(best, score) : std::min(best, score);
	}
	return best;
}

inline int bestMove(const Board &b, int player)
{
	int bestScore = std::numeric_limits<int>::min();
	int move = -1;
	for (int candidate : availableMoves(b))
	{
		Board copy = b;
		copy[candidate] = player;
		int score = minimax(copy, player, 0, false);
		if (score > bestScore)
		{
			bestScore = score;
			move = candidate;
		}
	}
	return move;
}