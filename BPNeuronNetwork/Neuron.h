#pragma once
#include<vector>

class Neuron
{
public:
	int numInputPerNerous;
	double activation = 0;
	double error = 0;
	double* weight;
	double* momentum;
	void reset();
	Neuron() = default;
	Neuron(Neuron&& n) noexcept;

	explicit Neuron(int numInputPerNerous);
	~Neuron();

};
