#pragma once

#include "TicTacToe.hpp"
#include "Logger.hpp"
#include <random>

inline std::vector<TrainingSample> generateTrainingData(int numGames)
{
	std::vector<TrainingSample> dataset;
	std::mt19937 rng(std::random_device{}());

	for (int game = 0; game < numGames; ++game)
	{
		Board board{};
		int player = 1;

		while (!isFull(board) && !hasWon(board, 1) &&
		       !hasWon(board, -1))
		{
			auto moves = availableMoves(board);
			if (moves.empty())
				break;

			int move = bestMove(board, player);

			std::vector<double> input(9);
			for (int i = 0; i < 9; ++i)
				input[i] =
				    static_cast<double>(board[i] * player);

			std::vector<double> target(9, 0.0);
			target[move] = 1.0;

			dataset.emplace_back(input, target);

			board[move] = player;
			player = -player;

			if (!isFull(board) && !hasWon(board, 1) &&
			    !hasWon(board, -1))
			{
				auto remaining = availableMoves(board);
				if (!remaining.empty() && rng() % 4 == 0)
				{
					std::uniform_int_distribution<
					    std::size_t>
					    dist(0, remaining.size() - 1);
					int randomMove = remaining[dist(rng)];
					board[randomMove] = player;
					player = -player;
				}
			}
		}
	}
	return dataset;
}