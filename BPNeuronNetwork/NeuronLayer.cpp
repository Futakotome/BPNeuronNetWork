#include"pch.h"
#include"NeuronUtils.h"
#include"NeuronLayer.h"

void NeuronLayer::reset() const {
	for (int i = 0; i < size; i++) {
		neurons[i].reset();
	}
}
double* NeuronLayer::getActivation()const {
	const int size = this->size;
	for (int i = 0; i < size; i++) {
		activation[i] = neurons[i].activation;

	}
	return activation;
}
double* NeuronLayer::getError()const {
	const int size = this->size;
	for (int i = 0; i < size; i++) {
		error[i] = neurons[i].error;
	}
	return error;
}
NeuronLayer::NeuronLayer(const int inputPerNeuron, Neuron* neurons, const int size) :activation(new double[size]), error(new double[size]), size(size), inputPerNeuron(inputPerNeuron), neurons(neurons) {

}

NeuronLayer::NeuronLayer(const NeuronLayer& nl) {
	this->inputPerNeuron = nl.inputPerNeuron;
	this->neurons = nl.neurons;
	this->size = nl.size;
	this->activation = nl.activation;
	this->error = nl.error;
}

NeuronLayer::NeuronLayer(NeuronLayer&& nl)noexcept :activation(nl.activation), error(nl.error), size(nl.size), inputPerNeuron(nl.inputPerNeuron), neurons(nl.neurons) {
	nl.activation = nullptr;
	nl.error = nullptr;
	nl.neurons = nullptr;
}
NeuronLayer::~NeuronLayer() {
	_delete(error);
	_delete(activation);
}