
#include <vector>
#include "neuralbacics.h"
#include <algorithm>
#include <cmath>
#include <random>
#include <ctime>
#include <omp_llvm.h>
#include <iostream>

double ANN::errorCorrection(double risk)
{
	if (risk == risk) {
		if (std::isinf(risk) == false) {
			if (std::isnan(risk) == false) {
				return risk;
			}
		}
	}
	try {
		throw std::runtime_error("\nBAD NUMBER ");
	}
	catch (std::runtime_error& e) {
		std::cerr << e.what() << risk << "\n";
	}
	return 0;
}

bool ANN::updateNetwork()
{

	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	int average = 0;
	for (b = 1; b < maxlayers; b = b + 1) {
		if (b == maxlayers - 1) {
			average = outputs;
		}
		else {
			average = nodesperlayer;
		}

		//figure out if average is working correctly.
		for (c = 0; c < nodesperlayer; c = c + 1) {
			for (d = 0; d < maxinputs; d = d + 1) {

				//compute averages over all nodes and examples.
				tangents[vWEIGHT][b][c][d] = ((tangents[vWEIGHT][b][c][d] / learningRounds) / average);

				//fudge the gradient by the learningrate.
				tangents[vWEIGHT][b][c][d] = tangents[vWEIGHT][b][c][d] * learningrate;




				if (typeOfMomentum == momentumType::standard) {
					//update network
					network[vWEIGHT][b][c][d] = network[vWEIGHT][b][c][d] + tangents[vWEIGHT][b][c][d];

					//implement momentum
					tangents[vWEIGHT][b][c][d] = momentum * tangents[vWEIGHT][b][c][d];
				}
				else {
					//update network
					network[vWEIGHT][b][c][d] = network[vWEIGHT][b][c][d] + (((-momentum) * oldTangents[vWEIGHT][b][c][d]) + ((1 + momentum) * tangents[vWEIGHT][b][c][d]));
				}
				try {
					if (errorCorrection(network[vWEIGHT][b][c][d]) == 0 && network[vWEIGHT][b][c][d] != 0) throw std::exception("  BAD NUMBERS  ");
				}
				catch (std::exception& e) {
					std::cerr << e.what() << network[vWEIGHT][b][c][d] << "\n";
				}
			}
			//compute averages over all nodes and examples.
			tangents[vBIAS][b][c][vDEFAULT] = ((tangents[vBIAS][b][c][vDEFAULT] / learningRounds) / average);//might not need average here, it might be needlessly decreacing computed value

			//fudge the gradient by the learningrate.
			tangents[vBIAS][b][c][vDEFAULT] = tangents[vBIAS][b][c][vDEFAULT] * learningrate;

			//update network
			network[vBIAS][b][c][vDEFAULT] = network[vBIAS][b][c][vDEFAULT] + tangents[vBIAS][b][c][vDEFAULT];
			if (typeOfMomentum == momentumType::standard) {
				//implement momentum
				tangents[vBIAS][b][c][vDEFAULT] = momentum * tangents[vBIAS][b][c][vDEFAULT];
			}
		}
	}
	for (b = 1; b < maxlayers; b = b + 1) {
		for (c = 0; c < nodesperlayer; c = c + 1) {
			tangents[vGOAL][b][c][vDEFAULT] = 0;//does momentum or nesterov momentum need the outputs (goals) to stay un-reset too?
		}
	}


	learningRounds = 0;


	return true;
}

void ANN::train(std::vector<double> inputs, std::vector<double> outputs)
{
	fprop(inputs);
	bprop(outputs);
	if (stochastic == true) {
		updateNetwork();
	}
	if (learningRounds == batchsize) {
		updateNetwork();
	}
}

double ANN::activate(activations activation, double input) {
	if (activation == activations::sigmoid) {
		double output = (1 / (1 + (exp(-(input)))));
		return output;
	}
	if (activation == activations::tanh) {
		double output = ((exp(input) - exp(-(input))) / ((exp(input)) + (exp(-(input)))));
		return output;
	}
	if (activation == activations::binstep) {
		if (input > 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
	if (activation == activations::relu) {
		double output = (std::max(static_cast<double>(0), input));
		return output;
	}
	if (activation == activations::arctanh) {
		double output = (atan(input));
		return output;
	}
	if (activation == activations::guder) {
		double output = (2 * (atan(tan(0.5 * input))));
		return output;
	}
	if (activation == activations::gelu) {
		double output = (0.5 * input * (1 + tanh(sqrt(2 / 3.1415 * (input + 0.044715 * 3)))));
		return output;
	}
	if (activation == activations::lrelu) {
		double output = (std::max(input * 0.01, input));
		return output;
	}
	if (activation == activations::softplus) {
		double output = (log(1 + (exp(input))));
		return output;
	}
	if (activation == activations::swish) {
		double output = (input / (1 + (exp(-input))));
		return output;
	}
	return NULL;
}

double ANN::derive(activations activation, double input)
{
	if (activation == activations::sigmoid) {
		double output = (activate(activations::sigmoid, input) * (1 - activate(activations::sigmoid, input)));
		return output;
	}
	if (activation == activations::tanh) {
		double output = (1 - (pow((exp(input) - exp(-(input))) / ((exp(input)) + (exp(-(input)))), 2)));
		return output;
	}
	if (activation == activations::binstep) {
		return 0;
	}
	if (activation == activations::relu) {
		if (input > 0) {
			return 1;
		}
		return 0;
	}
	if (activation == activations::arctanh) {
		double output = (1 / 1 + pow(input, 2));
		return output;
	}
	if (activation == activations::guder) {
		double output = (2 / (exp(input) + exp(-input)));
		return output;
	}
	if (activation == activations::gelu) {
		double output = (0.5 * tanh(0.0356774 * pow(input, 3) + 0.797885 * input));
		return output;
	}
	if (activation == activations::lrelu) {
		if (input > 0) {
			return 1;
		}
		return 0.01;
	}
	if (activation == activations::softplus) {
		double output = (activate(activations::sigmoid, input));
		return output;
	}
	if (activation == activations::swish) {
		double output = (input + activate(activations::sigmoid, input) * (1 - input));
		return output;
	}
	return NULL;
}

bool ANN::init(config settings)
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(-9999999, 9999999); // guaranteed unbiased

	if (settings.complete() == 0) {
		exit(EXIT_FAILURE);
	}

	//define the shape and main features of the ann
	maxinputs = settings.inputs;
	maxlayers = settings.layers;
	nodesperlayer = settings.nodesPerLayer;
	outputs = settings.outputs;
	outputactivation = settings.outputActivation;
	activation = settings.hiddenActivation;
	learningrate = settings.learningRate;
	momentum = settings.momentum;
	batchsize = settings.batchSize;
	stochastic = settings.isStochastic;
	typeOfMomentum = settings.typeOfMomentum;

	// set the max size of the network -> 3(for output, weight, and bias) layers, inputs, inputs.
	network.resize(3, std::vector < std::vector<std::vector<double>>>(maxlayers, std::vector < std::vector<double>>(maxinputs, std::vector<double>(maxinputs))));

	// set the max size of the network -> 3(for output, weight, and bias) layers, inputs, inputs.
	tangents.resize(3, std::vector < std::vector<std::vector<double>>>(maxlayers, std::vector < std::vector<double>>(maxinputs, std::vector<double>(maxinputs))));

	if (typeOfMomentum == momentumType::nesterov) {
		oldTangents.resize(3, std::vector < std::vector<std::vector<double>>>(maxlayers, std::vector < std::vector<double>>(maxinputs, std::vector<double>(maxinputs))));
	}
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	for (a = 0; a < 3; a = a + 1) {
		for (b = 1; b < maxlayers; b = b + 1) {
			for (c = 0; c < nodesperlayer; c = c + 1) {
				for (d = 0; d < maxinputs; d = d + 1) {
					network[a][b][c][d] = (uni(rng) / static_cast<double>(9999999)); //randomize net
				}
			}
		}
	}
	return true; //its finished
}

double ANN::neuron(int layer, int node)
{
	int loops = 0;
	if (layer == 1) {// if the layer is at the beginning it will have to accept all of the inputs as parameters
		loops = maxinputs;
	}
	else {//else it will have only the normal amount
		loops = nodesperlayer;
	}
	int x;//for variable	
	double temptotal = 0;//holds the temporary total while we are still adding up the inputs
	for (x = 0; x < loops; x = x + 1) {// run through all the connections
		temptotal = (network[vOUTPUT][long long(layer - 1)][x][0] * network[vWEIGHT][layer][node][x]) + temptotal;//weight * input
	}
	temptotal = temptotal + network[vBIAS][layer][node][0];// add bias
	if (layer == maxlayers - 1) {
		network[vOUTPUT][layer][node][0] = errorCorrection(activate(outputactivation, temptotal));//final result
		return activate(outputactivation, temptotal);
	}
	else {
		network[vOUTPUT][layer][node][0] = errorCorrection(activate(activation, temptotal));//final result
		return errorCorrection(activate(activation, temptotal));
	}
}

double ANN::neuronnoactivate(int layer, int node)
{
	int loops = 0;
	if (layer == 1) {// if the layer is at the beginning it will have to accept all of the inputs as parameters
		loops = maxinputs;
	}
	else {//else it will have only the normal amount
		loops = nodesperlayer;
	}
	int x;//for variable	
	double temptotal = 0;//holds the temporary total while we are still adding up the inputs
	for (x = 0; x < loops; x = x + 1) {// run through all the connections
		temptotal = (network[vOUTPUT][long long(layer - 1)][x][0] * network[vWEIGHT][layer][node][x]) + temptotal;//weight * input
	}
	temptotal = temptotal + network[vBIAS][layer][node][0];// add bias
	return temptotal;//final result
}

std::vector<double> ANN::fprop(std::vector<double> inputs)
{
	std::vector<double> fpropoutput;
	fpropoutput.resize(outputs);
	int x;
	//#pragma omp parallel for
	for (x = 0; x < maxinputs; x = x + 1) {//turn the first layer of the network into the inputs
		network[vOUTPUT][0][x][0] = inputs[x];
	}
	//#pragma omp parallel for
	for (x = 0; x < maxinputs; x = x + 1) {// run the second layer of the network with all of the inputs
		neuron(1, x);
	}
	int i;
	for (i = 2; i < maxlayers; i = i + 1) {//run the rest of the layers normally...
		if (i < maxlayers - 1) {
			int x;
			//#pragma omp parallel for
			for (x = 0; x < nodesperlayer; x = x + 1) {
				neuron(i, x);
			}
		}
		else {//  ...unless its the final layer, in which case only run the outputs.
			int x;
			//#pragma omp parallel for
			for (x = 0; x < outputs; x = x + 1) {
				fpropoutput[x] = neuron(i, x);
			}
		}
	}
	return fpropoutput;
}

void ANN::bprop(std::vector<double> expected)
{
	learningRounds = learningRounds + 1;

	//set some temporary variables
	double commonmath = 0;
	double z = 0;
	int layer = 0;
	double correct = 0;
	int numConnections = 0;
	int numNodes = 0;

	for (layer = maxlayers - 1; layer > 0; layer = layer - 1) {//run through all of the layers backwards
		numNodes = nodesperlayer;
		numConnections = nodesperlayer;
		if (layer == maxlayers - 1) {
			numNodes = outputs;
		}
		if (layer == 1) {
			numConnections = maxinputs;
		}
		//#pragma omp parallel for
		for (int node = 0; node < numNodes; node = node + 1) {//run through every node
			double temp[3] = {};
			if (layer == maxlayers - 1) {
				correct = expected[node];
			}
			else {
				correct = tangents[vGOAL][layer + 1][node][vDEFAULT] + network[vOUTPUT][layer][node][vDEFAULT];
			}

			z = neuronnoactivate(layer, node);

			if (layer == maxlayers - 1) {
				commonmath = derive(outputactivation, z) * 2 * (network[vOUTPUT][layer][node][vDEFAULT] - correct);
			}
			else {
				commonmath = derive(activation, z) * 2 * (network[vOUTPUT][layer][node][vDEFAULT] - correct);
			}
			commonmath = errorCorrection(commonmath);
			for (int connection = 0; connection < numConnections; connection = connection + 1) {//run through every connection

				//first focus on the weights

				//what is the value of the prev. layers output at this connection?
				double netPrevLayerOutput = network[vOUTPUT][long long(layer - 1)][connection][vDEFAULT];
				//what is the deriv. of the cost function in relation to the prev. nodes weight? 
				double derivOfCostToWeight = (netPrevLayerOutput * commonmath);
				//what is the average so far (from other training examples)
				double tanConnectionWeight = tangents[vWEIGHT][layer][node][connection];
				//temporaraly store the final value
				temp[vWEIGHT] = tanConnectionWeight - derivOfCostToWeight;
				//add the derivitave that was just calculated to the array for changes to be made and check for error nums.
				if (typeOfMomentum == momentumType::nesterov) {
					oldTangents[vWEIGHT][layer][node][connection] = tangents[vWEIGHT][layer][node][connection];
				}

				tangents[vWEIGHT][layer][node][connection] = errorCorrection(temp[vWEIGHT]);



				//then focus on the goals

				//what is the value of the weight at this connection?
				double netConnectionWeight = network[vWEIGHT][layer][node][connection];
				//what is the deriv. of the cost function in relation to the prev. nodes output?
				double derivOfCostToInput = (netConnectionWeight * commonmath);
				//what is the average so far (from other training examples)
				double tanPrevLayerGoal = tangents[vGOAL][long long(layer - 1)][connection][vDEFAULT];
				//temporaraly store the final value
				temp[vGOAL] = tanPrevLayerGoal - derivOfCostToInput;
				//add the derivitave that was just calculated to the array for changes to be made and check for error nums.
				if (typeOfMomentum == momentumType::nesterov) {
					oldTangents[vGOAL][layer][connection][vDEFAULT] = tangents[vGOAL][layer][connection][vDEFAULT];
				}
				tangents[vGOAL][layer][connection][vDEFAULT] = errorCorrection(temp[vGOAL]);
				/*why do we use the weight to determine the deriv. of the output of the prev. neuron? glad you asked!
				* well, what happens if we increace the weight?
				*	thats right, the value coming out of the connection gets larger.
				* now what happens if we increace the output of the prev. layers node?
				*	right again, the value coming out of the connection gets larger.
				*
				* and, if you imagine that somehow you swapped the weight and the prev. layers output, would the math change at all?
				*	no, it wouldent, because of the communative property.
				*
				* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
				*				 ____________																							|
				*				/		A	 \			 																				|
				*			   /	B	|	  \																							|
				*			  / 	 \	W	   \																						|
				*		     |		  \	|		|																						|
				*			 |  	   \|		|																						|
				*			 |  		Z		|																						|
				*			 \  		|		/																						|
				*			  \ 	sigmoid	   /																						|
				*			   ````````````````																							|
				* A (activation or output) feeds directly into W (weight) and can be swapped with W without changing the result			|
				*																														|
				* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
				*
				* weight and output are related. to find the derivitave of weight, we use output as a constant. to find the derivitave of output, we use weight as a constant.
				* (constant is relative, i havent checked to make sure that 'constant' is the correct termonology here.)
				*
				*/
			}
			temp[vBIAS] = tangents[vBIAS][layer][node][vDEFAULT] - commonmath;
			if (typeOfMomentum == momentumType::nesterov) {
				oldTangents[vBIAS][layer][node][vDEFAULT] = tangents[vBIAS][layer][node][vDEFAULT];
			}
			tangents[vBIAS][layer][node][vDEFAULT] = errorCorrection(temp[vBIAS]);
		}
	}

}

bool config::complete()
{
	try {
		if (hiddenActivation != activations::VOID) {
			if (outputActivation != activations::VOID) {
				if (inputs != NULL) {
					if (outputs != NULL) {
						if (layers != NULL) {
							if (learningRate != NULL) {
								if (nodesPerLayer != NULL) {
									if (!(isStochastic == true && batchSize != 0)) {
										return true;
									}
									else {
										throw std::runtime_error("\"isStochastic\" cannot be true while \"batchSize\" is not zero\neither use batchSize or use isStochastic, you cant use both at the same time.");
									}
								}
								else {
									throw std::runtime_error("parameter \"nodesPerLayer\" is VOID.\n\"nodesPerLayer\" is a required parameter to run the network.");
								}
							}
							else {
								throw std::runtime_error("parameter \"learningRate\" is VOID.\n\"learningRate\" is a required parameter to run the network.");
							}
						}
						else {
							throw std::runtime_error("parameter \"layers\" is VOID.\n\"layers\" is a required parameter to run the network.");
						}
					}
					else {
						throw std::runtime_error("parameter \"outputs\" is VOID.\n\"outputs\" is a required parameter to run the network.");
					}
				}
				else {
					throw std::runtime_error("parameter \"inputs\" is VOID.\n\"inputs\" is a required parameter to run the network.");
				}
			}
			else {
				throw std::runtime_error("parameter \"outputActivation\" is VOID.\n\"outputActivation\" is a required parameter to run the network.");
			}
		}
		else {
			throw std::runtime_error("parameter \"hiddenActivation\" is VOID.\n\"hiddenActivation\" is a required parameter to run the network.");
		}
	}
	catch (std::runtime_error& e) {
		std::cerr << e.what() << "\nplease correct parameter(s) and try again.\n";
		return false;
	}
	return false;
}
