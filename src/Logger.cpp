#include "Logger.hpp"

#include <deque>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct EpochLog {
	std::size_t epoch;
	double mse;

	std::vector<std::vector<double>> outputs;
	std::vector<std::vector<double>> errors;
	std::string weights;
};

void printEpoch(const EpochLog &log, const std::vector<TrainingSample> &dataset)
{
	std::cout << "\nEpoch: " << log.epoch << " ------------------------\n";

	std::cout << std::fixed << std::setprecision(6);

	for (std::size_t i = 0; i < dataset.size(); ++i)
	{
		const auto &inputs = dataset[i].first;
		const auto &goals = dataset[i].second;

		for (double input : inputs)
			std::cout << input << " ";

		std::cout << "GOAL:";
		for (double goal : goals)
			std::cout << " " << goal;

		std::cout << "  output:";
		for (double output : log.outputs[i])
			std::cout << " " << output;

		std::cout << "  error:";
		for (double error : log.errors[i])
			std::cout << " " << error;

		std::cout << '\n';
	}

	std::cout << "MSE: " << log.mse << '\n';
	std::cout << "Weights:\n" << log.weights;
}

} // namespace

std::size_t trainWithEpochLogging(NeuralNetwork &network,
				  const std::vector<TrainingSample> &dataset,
				  std::size_t maxEpochs, double threshold)
{
	constexpr std::size_t epochsToStore = 5;
	// How often to print a live progress line. Adjust to taste.
	constexpr std::size_t progressEvery = 100;

	std::vector<EpochLog> firstEpochs;
	firstEpochs.reserve(epochsToStore);

	std::deque<EpochLog> lastEpochs;

	std::size_t epochsRun = 0;

	for (std::size_t epoch = 0; epoch < maxEpochs; ++epoch)
	{
		epochsRun = epoch + 1;

		// TRAIN
		for (const auto &sample : dataset)
		{
			network.train(sample.first, sample.second);
		}

		// EVALUATE
		EpochLog log;
		log.epoch = epoch;
		log.outputs.reserve(dataset.size());
		log.errors.reserve(dataset.size());

		double sumSquaredError = 0.0;

		for (const auto &sample : dataset)
		{
			const auto &inputs = sample.first;
			const auto &goals = sample.second;

			std::vector<double> output = network.input(inputs);
			std::vector<double> errors(output.size());

			double sampleSquaredError = 0.0;

			for (std::size_t i = 0; i < output.size(); ++i)
			{
				errors[i] = goals[i] - output[i];
				sampleSquaredError += errors[i] * errors[i];
			}

			sumSquaredError += sampleSquaredError;

			log.outputs.push_back(std::move(output));
			log.errors.push_back(std::move(errors));
		}

		log.mse = sumSquaredError / static_cast<double>(dataset.size());

		const bool willKeepAsFirst = firstEpochs.size() < epochsToStore;
		const bool reachedThreshold = log.mse < threshold;
		const bool isLastEpoch = (epoch + 1 == maxEpochs);

		// We always push into the rolling "last N epochs" window, and
		// we don't know in advance which epoch will end up being the
		// true last one kept, so this still has to run every epoch. The
		// real fix if this is the bottleneck is making saveWeights()
		// itself cheap (reserve string capacity up front, avoid
		// repeated reallocation) rather than skipping it.
		log.weights = network.saveWeights();
		(void)willKeepAsFirst;
		(void)reachedThreshold;
		(void)isLastEpoch;

		// FIRST 5
		if (willKeepAsFirst)
		{
			firstEpochs.push_back(log);
		}

		// LAST 5 (rolling window)
		lastEpochs.push_back(log);
		if (lastEpochs.size() > epochsToStore)
		{
			lastEpochs.pop_front();
		}

		// LIVE PROGRESS -- this is what was missing before. Without it,
		// the program does real work silently for potentially tens of
		// thousands of epochs and looks frozen even though it isn't.
		if (epoch % progressEvery == 0 || reachedThreshold ||
		    isLastEpoch)
		{
			std::cout << "Epoch " << epoch << "  MSE=" << std::fixed
				  << std::setprecision(6) << log.mse
				  << std::endl; // endl flushes, so it's visible
						// immediately
		}

		// STOP
		if (reachedThreshold)
		{
			std::cout << "\nMSE threshold reached: " << log.mse
				  << " at epoch " << epoch << '\n';
			break;
		}
	}

	// PRINT FIRST 5
	std::cout << "\n========== FIRST 5 EPOCHS ==========\n";
	for (const EpochLog &log : firstEpochs)
	{
		printEpoch(log, dataset);
	}

	// PRINT LAST 5
	if (epochsRun > epochsToStore)
	{
		std::cout << "\n========== LAST 5 EPOCHS ==========\n";
		for (const EpochLog &log : lastEpochs)
		{
			printEpoch(log, dataset);
		}
	}

	return epochsRun;
}