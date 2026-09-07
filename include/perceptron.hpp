#pragma once

#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

class Neuron {
public:
	// size is amount of connections
	Neuron(size_t size, double bias, double learningrate)
	    : m_weight(size), m_bias(bias), m_learningrate(learningrate)
	{
		m_size = size;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dist(0.0, 1);
		for (double &w : m_weight)
		{
			w = dist(gen);
		}
	}

	bool activate(double sum)
	{
		if (sum >= 0.5)
			return true;
		return false;
	}

	bool input(std::vector<double> inputs, double goal)
	{
		double sum{0};
		for (int i = 0; i < m_size; i++)
		{
			sum += m_weight[i] * inputs[i];
		}
		sum += m_bias;
		return activate(sum);
	}

	void printWeights() const
	{
		for (size_t i = 0; i < m_weight.size(); i++)
		{
			std::cout << "weight " << (i + 1) << " = "
				  << m_weight[i] << "\n";
		}
		std::cout << "bias " << m_bias << "\n";
	}

	void calibrateWeigh(std::vector<double> inputs, double error)
	{
		if (error == 0)
		{
			std::cout << "Error is Zero\n";
			return;
		}
		for (size_t i = 0; i < m_weight.size(); i++)
		{
			m_weight[i] =
			    m_weight[i] + (m_learningrate * error) * inputs[i];
		}
		m_bias = m_bias + m_learningrate * error;
	};

private:
	std::vector<double> m_weight;
	std::size_t m_size;
	double m_bias;
	double m_learningrate;
};