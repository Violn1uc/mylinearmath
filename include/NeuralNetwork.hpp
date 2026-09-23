#pragma once

#include <cstddef>
#include <vector>
#include <string>

// A single neuron in a neural network layer.
//
// Uses a sigmoid activation function so that a derivative exists
// for backpropagation.
class Neuron {
public:
	Neuron(std::size_t size, double learningRate);

	double activate(double sum) const;
	double activateDerivative(double activatedOutput) const;

	// Forward propagation.
	double input(const std::vector<double> &inputs);

	void printWeights() const;
	std::string saveWeights() const;

	// Backpropagation helpers.
	double delta(double error) const;
	void applyUpdate(double delta);

	// Convenience method for updating a single neuron.
	void calibrateWeight(const std::vector<double> &inputs, double error);

	// Read-only accessors.
	double weight(std::size_t index) const;
	std::size_t size() const;
	double bias() const;

private:
	std::vector<double> m_weight;
	std::size_t m_size;
	double m_bias;
	double m_learningRate;

	// Values saved during forward propagation and used during
	// backpropagation.
	std::vector<double> m_lastInputs;
	double m_lastOutput{0.0};
};

// One fully-connected layer of neurons.
class Layer {
public:
	Layer(std::size_t numNeurons, std::size_t inputsPerNeuron,
	      double learningRate);

	// Forward propagation through the layer.
	std::vector<double> input(const std::vector<double> &inputs);

	void printWeights() const;

	std::string saveWeights() const;

	// Backpropagation through the layer.
	//
	// Returns the error that should be propagated to the
	// previous layer.
	std::vector<double> calibrateWeight(const std::vector<double> &errors);

	std::size_t size() const;

	// Read-only access to a neuron for logging/debugging.
	const Neuron &getNeuron(std::size_t index) const;

private:
	std::vector<Neuron> m_neurons;
};

// Multi-layer perceptron.
//
// The network consists of a stack of fully-connected layers.
// Training uses backpropagation with sigmoid activation.
class NeuralNetwork {
public:
	NeuralNetwork(std::size_t inputSize,
		      std::vector<std::size_t> layerSizes, double learningRate);

	// Forward propagation.
	std::vector<double> input(const std::vector<double> &inputs);

	// Train on one input/target pair.
	void train(const std::vector<double> &inputs,
		   const std::vector<double> &goal);

	void printWeights() const;
	std::string saveWeights() const;

	// Read-only accessors used by the logger/tests.
	std::size_t layerCount() const;

	std::size_t layerSize(std::size_t layerIndex) const;

	double neuronWeight(std::size_t layerIndex, std::size_t neuronIndex,
			    std::size_t weightIndex) const;

	double neuronBias(std::size_t layerIndex,
			  std::size_t neuronIndex) const;

private:
	std::vector<Layer> m_layers;
};