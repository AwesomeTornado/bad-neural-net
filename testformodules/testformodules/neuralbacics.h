#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

enum class activations { sigmoid, tanh, binstep, relu, lrelu, arctanh, guder, gelu, softplus, swish, VOID };

enum class momentumType { standard, nesterov};

class config {
public:

	activations hiddenActivation = activations::VOID;

	activations outputActivation = activations::VOID;

	int inputs = NULL;

	int outputs = NULL;

	int layers = NULL;

	int nodesPerLayer = NULL;

	float learningRate = NULL;

	bool complete();

	bool isStochastic = 0;

	int batchSize = 0;

	float momentum = 0;

	momentumType typeOfMomentum = momentumType::standard;
};

class ANN {
public:

	bool init(config settings);

	std::vector<double> fprop(std::vector<double> inputs);

	bool updateNetwork();

	void train(std::vector<double> inputs, std::vector<double> outputs);


private:

	double activate(activations, double input);

	double derive(activations, double input);

	double errorCorrection(double risk);


protected:

	double neuron(int layer, int node);

	void bprop(std::vector<double> expected);

	double neuronnoactivate(int layer, int node);


	float learningrate = 0;

	int maxinputs = 1;

	int maxlayers = 3;

	int nodesperlayer = 1;

	int outputs = 1;

	bool stochastic = 0;

	int batchsize = 0;

	float momentum = 0;

	momentumType typeOfMomentum = momentumType::standard;

	// this variable stores how many times this network has calculated improvements.
	//in order to make sure that the learningrate var has a constant effect regardless of training examples the changes are divided by this to get the average tangent across all training examples. also used for batch number.
	int learningRounds = 0;



	activations outputactivation;

	activations activation;


	//QOL vars
	const int vWEIGHT = 0;

	const int vBIAS = 1;

	const int vOUTPUT = 2;

	const int vGOAL = 2;

	const int vDEFAULT = 0;



	/*
	this is an array to store all of the info for all of the nodes in the neural network.

	the array has three first parts
	>weight
	>bias
	>output
	the weight and bias will be changed once per generation, while the output will change every time the network is running,

	2nd part
	this part is the layer that the node is on
	3rd part
	the third part is the nodes themselves
	*/
	std::vector < std::vector < std::vector < std::vector<double>>>> network = {};


	/*
	this is an array of all the changes that need to be made to improve the network.

	the array has three first parts
	>weight
	>bias
	>goal
	the weight bias and goal will change once every training example

	2nd part
	this part is the layer that the node is on
	3rd part
	the third part is the nodes themselves
*/
	std::vector < std::vector < std::vector < std::vector<double>>>> tangents = {};



	//this is what tangents was before backpropagation changed them
	std::vector < std::vector < std::vector < std::vector<double>>>> oldTangents = {};


	/*
	this holds the inputs.
	*/
	std::vector<double> inputs = {};

};