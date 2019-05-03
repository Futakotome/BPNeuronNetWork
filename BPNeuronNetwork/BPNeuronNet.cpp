#include"pch.h"
#include"BPNeuronNet.h"


double BPNeuronNet::sigmoidActive(const double activation, const double response) {
	return 1.0 / (1.0 + exp(-activation * response));
}
double tanhActive(const double activation) {
	return (exp(activation) - exp(-activation)) / (exp(activation) + exp(-activation));
}
double reluActive(const double activation) {
	return std::max(0.0, activation);
	
}
double tanhbackactive(const double x) {
	return 1 - pow(tanhActive(x), 2);
}
double reluBackActive(const double x) {
	if (x > 0)
		return 1;
	else if (x <= 0);
		return 0;

}
double BPNeuronNet::backActive(const double x,const double response) {
	return sigmoidActive(x, response)*sigmoidActive(1 - x, response);
}
//���򴫲� ���� ���ÿ����Ԫ�����������ļ���ֵ ��Ϊ��һ�������
void BPNeuronNet::updateNeuronLayer(NeuronLayer& nl, const double inputs[])const {
	const int numNerous = nl.size;
	const int numInputPerNerous = nl.inputPerNeuron;

	for (int i = 0; i < numNerous; i++) {
		double* curWeight = nl.neurons[i].weight;//ÿ����Ԫ�� ����Ȩ�� ����
		double netInput = 0;

		int k;
		for (k = 0; k < numInputPerNerous; k++) {//����ÿ����Ԫ�� ����Ȩ�� ����
			netInput += curWeight[k] * inputs[k];//���
		}
		netInput += curWeight[k];
		nl.neurons[i].activation = sigmoidActive(netInput, ACTIVE_RESPONSE);//ÿ����Ԫ ͨ�� �����
		//nl.neurons[i].activation = reluActive(netInput);
		//nl.neurons[i].activation = tanhActive(netInput);
	}
}
//���ķ��򴫲�
void BPNeuronNet::trainNeuronLayer(NeuronLayer& nl, const double preActivations[], double preErrorArr[])const {
	const int numNeurous = nl.size;
	const int numInputPerNeurous = nl.inputPerNeuron;
	Neuron* neuronArr = nl.neurons;

	for (int i = 0; i < numNeurous; i++) {
		double* curWeight = neuronArr[i].weight;
		const double error = neuronArr[i].error * backActive(neuronArr[i].activation,ACTIVE_RESPONSE);
		//const double error = neuronArr[i].error*reluBackActive(neuronArr[i].activation);
		//const double error = neuronArr[i].error*tanhbackactive(neuronArr[i].activation);

		int j;
		for (j = 0; j < numInputPerNeurous; j++) {
			if (preErrorArr) {
				preErrorArr[j] += curWeight[j] * error;
			}
			neuronArr[i].momentum[j] = 0.2*neuronArr[i].momentum[j] + error * learningRate * preActivations[j];//���¶���
			neuronArr[i].weight[j] += neuronArr[i].momentum[j];//����Ȩ��
		}
		neuronArr[i].weight[j] += error * learningRate;
	}
}
//���򴫲�
void BPNeuronNet::trainUpdate(const double inputs[], const double targets[]) {
	for (int i = 0; i <= numHiddenLayer; i++) {//����ÿһ��������
		updateNeuronLayer(neuronLayers[i], inputs);
		inputs = neuronLayers[i].getActivation();//����һ���ü���ֵ��Ϊ���������
	}
	NeuronLayer& outLayer = neuronLayers[numHiddenLayer];//����� 
	double * outActivation = outLayer.getActivation();
	const int numNeurons = outLayer.size;

	errorSum = 0;
	for (int i = 0; i < numNeurons; i++) {
		const double err = targets[i] - outActivation[i];
		outLayer.neurons[i].error = err;
		errorSum += err * err;
	}
}
double BPNeuronNet::getError()const {
	return errorSum;
}
void BPNeuronNet::training(double inputs[], const double targets[]) {
	double * preOutActivations = nullptr;
	double * preError = nullptr;
	//���򴫲�
	trainUpdate(inputs, targets);
	//���򴫲�
	for (int i = numHiddenLayer; i >= 0; i--) {
		NeuronLayer& curLayer = neuronLayers[i];
		if (i > 0) {
			NeuronLayer& preLayer = neuronLayers[i - 1];
			preOutActivations = preLayer.getActivation();
			preError = preLayer.getError();
			memset(preError, 0, preLayer.size * sizeof(double));
		}
		else {
			preOutActivations = inputs;
			preError = nullptr;
		}
		trainNeuronLayer(curLayer, preOutActivations, preError);
	}
}
void BPNeuronNet::predict(double inputs[], double* outputs[]) {
	for (int i = 0; i <= numHiddenLayer; i++) {
		updateNeuronLayer(neuronLayers[i], inputs);
		inputs = neuronLayers[i].getActivation();
	}
	*outputs = neuronLayers[numHiddenLayer].getActivation();
}
void BPNeuronNet::addLayer(const int num) {
	int inputNum = !neuronLayers.empty() ? neuronLayers[numHiddenLayer].size : numInput;

	void * raw = operator new[](num * sizeof(Neuron));
	Neuron * neurons = static_cast<Neuron*>(raw);
	for (int i = 0; i < num; i++) {
		new (&neurons[i])Neuron(inputNum);
	}
	neuronLayers.emplace_back(inputNum, neurons, num);
	numHiddenLayer = neuronLayers.empty() ? 0 : neuronLayers.size() - 1;
}
BPNeuronNet::BPNeuronNet(const int numberInput, const double learningRate) :numInput(numberInput), numHiddenLayer(0), learningRate(learningRate) {

}
BPNeuronNet::~BPNeuronNet() = default;