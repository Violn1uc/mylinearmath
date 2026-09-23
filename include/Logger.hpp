#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include "NeuralNetwork.hpp"

    // A single training example: an input vector paired with its
    // expected output ("goal") vector.
    using TrainingSample = std::pair<std::vector<double>, std::vector<double>>;

// Trains `network` on `dataset` for up to `maxEpochs` epochs (or until
// the mean squared error drops below `threshold`), logging the first
// and last 5 epochs to stdout. Returns the number of epochs actually run.
std::size_t trainWithEpochLogging(NeuralNetwork &network,
				  const std::vector<TrainingSample> &dataset,
				  std::size_t maxEpochs, double threshold);