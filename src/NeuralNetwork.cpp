#include "NeuralNetwork.hpp"

#include <cmath>
#include <iostream>
#include <random>
#include <sstream>

namespace {
std::mt19937 &rng()
{
	static std::mt19937 engine(std::random_device{}());
	return engine;
}

double randomWeight()
{
	static std::uniform_real_distribution<double> dist(-1.0, 1.0);
	return dist(rng());
}
} // namespace

// ----------------------------- Neuron -----------------------------

Neuron::Neuron(std::size_t size, double learningRate)
    : m_size(size), m_bias(randomWeight()), m_learningRate(learningRate)
{
	m_weight.resize(size);
	for (auto &w : m_weight)
		w = randomWeight();
}

double Neuron::activate(double sum) const
{
	return 1.0 / (1.0 + std::exp(-sum));
}

double Neuron::activateDerivative(double activatedOutput) const
{
	return activatedOutput * (1.0 - activatedOutput);
}

double Neuron::input(const std::vector<double> &inputs)
{
	double sum = m_bias;
	for (std::size_t i = 0; i < m_size; ++i)
		sum += m_weight[i] * inputs[i];

	m_lastInputs = inputs;
	m_lastOutput = activate(sum);
	return m_lastOutput;
}

std::string Neuron::saveWeights() const
{
	std::ostringstream output;

	output << "bias=" << m_bias;

	for (std::size_t i = 0; i < m_size; ++i)
	{
		output << " w" << i << "=" << m_weight[i];
	}

	output << '\n';

	return output.str();
}

void Neuron::printWeights() const
{
	std::cout << "bias=" << m_bias;
	for (std::size_t i = 0; i < m_size; ++i)
		std::cout << " w" << i << "=" << m_weight[i];
	std::cout << '\n';
}

double Neuron::delta(double error) const
{
	return error * activateDerivative(m_lastOutput);
}

void Neuron::applyUpdate(double delta)
{
	for (std::size_t i = 0; i < m_size; ++i)
		m_weight[i] += m_learningRate * delta * m_lastInputs[i];
	m_bias += m_learningRate * delta;
}

void Neuron::calibrateWeight(const std::vector<double> &inputs, double error)
{

	m_lastInputs = inputs;
	applyUpdate(delta(error));
}

double Neuron::weight(std::size_t index) const { return m_weight.at(index); }

std::size_t Neuron::size() const { return m_size; }

double Neuron::bias() const { return m_bias; }


Layer::Layer(std::size_t numNeurons, std::size_t inputsPerNeuron,
	     double learningRate)
{
	m_neurons.reserve(numNeurons);
	for (std::size_t i = 0; i < numNeurons; ++i)
		m_neurons.emplace_back(inputsPerNeuron, learningRate);
}

std::vector<double> Layer::input(const std::vector<double> &inputs)
{
	std::vector<double> outputs;
	outputs.reserve(m_neurons.size());
	for (auto &neuron : m_neurons)
		outputs.push_back(neuron.input(inputs));
	return outputs;
}

void Layer::printWeights() const
{
	for (std::size_t i = 0; i < m_neurons.size(); ++i)
	{
		std::cout << "    neuron " << i << ": ";
		m_neurons[i].printWeights();
	}
}

std::string Layer::saveWeights() const
{
	std::ostringstream output;

	for (std::size_t i = 0; i < m_neurons.size(); ++i)
	{
		output << "    neuron " << i << ": ";
		output << m_neurons[i].saveWeights();
	}

	return output.str();
}

std::vector<double> Layer::calibrateWeight(const std::vector<double> &errors)
{
	std::vector<double> deltas(m_neurons.size());
	for (std::size_t i = 0; i < m_neurons.size(); ++i)
		deltas[i] = m_neurons[i].delta(errors[i]);
	std::size_t inputSize =
	    m_neurons.empty() ? 0 : m_neurons.front().size();
	std::vector<double> propagated(inputSize, 0.0);
	for (std::size_t i = 0; i < m_neurons.size(); ++i)
		for (std::size_t j = 0; j < inputSize; ++j)
			propagated[j] += m_neurons[i].weight(j) * deltas[i];

	for (std::size_t i = 0; i < m_neurons.size(); ++i)
		m_neurons[i].applyUpdate(deltas[i]);

	return propagated;
}

std::size_t Layer::size() const { return m_neurons.size(); }

const Neuron &Layer::getNeuron(std::size_t index) const
{
	return m_neurons.at(index);
}

NeuralNetwork::NeuralNetwork(std::size_t inputSize,
			     std::vector<std::size_t> layerSizes,
			     double learningRate)
{
	std::size_t prevSize = inputSize;
	m_layers.reserve(layerSizes.size());
	for (std::size_t layerSize : layerSizes)
	{
		m_layers.emplace_back(layerSize, prevSize, learningRate);
		prevSize = layerSize;
	}
}

std::vector<double> NeuralNetwork::input(const std::vector<double> &inputs)
{
	std::vector<double> current = inputs;
	for (auto &layer : m_layers)
		current = layer.input(current);
	return current;
}

void NeuralNetwork::train(const std::vector<double> &inputs,
			  const std::vector<double> &goal)
{
	std::vector<double> output = input(inputs);

	std::vector<double> errors(output.size());
	for (std::size_t i = 0; i < output.size(); ++i)
		errors[i] = goal[i] - output[i];
	for (std::size_t idx = m_layers.size(); idx-- > 0;)
		errors = m_layers[idx].calibrateWeight(errors);
}

void NeuralNetwork::printWeights() const
{
	for (std::size_t i = 0; i < m_layers.size(); ++i)
	{
		std::cout << "  Layer " << i << ":\n";
		m_layers[i].printWeights();
	}
}

std::string NeuralNetwork::saveWeights() const
{
	std::ostringstream output;

	for (std::size_t i = 0; i < m_layers.size(); ++i)
	{
		output << "  Layer " << i << ":\n";
		output << m_layers[i].saveWeights();
	}

	return output.str();
}

std::size_t NeuralNetwork::layerCount() const { return m_layers.size(); }

std::size_t NeuralNetwork::layerSize(std::size_t layerIndex) const
{
	return m_layers.at(layerIndex).size();
}

double NeuralNetwork::neuronWeight(std::size_t layerIndex,
				   std::size_t neuronIndex,
				   std::size_t weightIndex) const
{
	return m_layers.at(layerIndex)
	    .getNeuron(neuronIndex)
	    .weight(weightIndex);
}

double NeuralNetwork::neuronBias(std::size_t layerIndex,
				 std::size_t neuronIndex) const
{
	return m_layers.at(layerIndex).getNeuron(neuronIndex).bias();
}