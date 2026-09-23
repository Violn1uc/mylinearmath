#include <iomanip>
#include <iostream>
#include <vector>

#include "NeuralNetwork.hpp"
#include "Logger.hpp"
#include "TicTacToe.hpp"
#include "GenerateData.hpp"

void printBoard(const Board &b)
{
	auto symbol = [](int v) {
		return v == 1 ? 'X' : (v == -1 ? 'O' : '.');
	};
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
			std::cout << symbol(b[row * 3 + col]) << ' ';
		std::cout << '\n';
	}
}

int networkMove(NeuralNetwork &network, const Board &board, int player)
{
	std::vector<double> input(9);
	for (int i = 0; i < 9; ++i)
		input[i] = static_cast<double>(board[i] * player);

	std::vector<double> output = network.input(input);

	int best = -1;
	double bestScore = -1.0;
	for (int i = 0; i < 9; ++i)
	{
		if (board[i] == 0 && output[i] > bestScore)
		{
			bestScore = output[i];
			best = i;
		}
	}
	return best;
}

int main()
{
	std::cout << "Generating training data via minimax...\n";
	std::vector<TrainingSample> dataset = generateTrainingData(100);
	std::cout << "Dataset size: " << dataset.size() << " samples\n";

	NeuralNetwork network(9, {18, 9}, 0.1);

	const std::size_t maxEpochs = 5000;

	trainWithEpochLogging(network, dataset, maxEpochs, 0.01);

	std::cout
	    << "\nPlay against the bot! You are 'O' (-1), bot is 'X' (1).\n";
	std::cout
	    << "Enter a cell number 0-8 (top-left = 0, bottom-right = 8).\n\n";

	Board board{};
	int currentPlayer = 1;

	while (!isFull(board) && !hasWon(board, 1) && !hasWon(board, -1))
	{
		printBoard(board);

		if (currentPlayer == 1)
		{
			int move = networkMove(network, board, 1);
			std::cout << "\nBot plays: " << move << "\n\n";
			board[move] = 1;
		}
		else
		{
			int move;
			std::cout << "Your move: ";
			std::cin >> move;
			if (move < 0 || move > 8 || board[move] != 0)
			{
				std::cout << "Invalid move, try again.\n";
				continue;
			}
			board[move] = -1;
		}
		currentPlayer = -currentPlayer;
	}

	printBoard(board);

	if (hasWon(board, 1))
		std::cout << "\nBot wins!\n";
	else if (hasWon(board, -1))
		std::cout << "\nYou win!\n";
	else
		std::cout << "\nDraw!\n";

	return 0;
}