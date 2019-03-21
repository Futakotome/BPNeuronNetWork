#pragma once
#include<vector>
#include"NeuronLayer.h"

constexpr auto ACTIVE_RESPONSE = 0.7;

class BPNeuronNet
{
public:
	int numInput, numHiddenLayer;
	double learningRate, errorSum = 0;
	std::vector<NeuronLayer> neuronLayers;

	inline double sigmoidActive(double activation, double response) const;
	static inline double backActive(double x);
	void updateNeuronLayer(NeuronLayer& nl, const double inputs[])const;
	void trainNeuronLayer(NeuronLayer& nl, const double activations[], double errorArr[])const;
	void trainUpdate(const double inputs[], const double targets[]);
public:

	double getError()const;
	void training(double inputs[], const double targets[]);
	void process(double inputs[], double * outputs[]);
	void addLayer(int num);

	BPNeuronNet(int numberInput, double learningRate);
	~BPNeuronNet();


};
