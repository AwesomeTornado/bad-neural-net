#include <vector>
#include <iostream>
#include "neuralbacics.h"
#include <random>
#include "idx.h"
#include <string>
#include <fstream>
#include <ctime>


int main() {
	std::random_device rnd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rnd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> rando(0, 60000); // guaranteed unbiased


	srand(static_cast<unsigned int>(time(0)));

	dynamicdimentions images;
	dynamicdimentions labels;

	IDXio file;
	std::string imagefile = { "train" "-images.idx3-ubyte" };
	std::string labelfile = { "train" "-labels.idx1-ubyte" };

	imagefile.insert(0, "C:\\Users\\harle\\Downloads\\mnist\\");
	labelfile.insert(0, "C:\\Users\\harle\\Downloads\\mnist\\");

	images = file.recallIDX(imagefile);
	labels = file.recallIDX(labelfile);

	std::vector<double> inputs;
	std::vector<double> outputs;
	inputs.resize(784, 0);
	outputs.resize(10, 0);
	std::vector<int> params;


	config ANNsettings;

	ANNsettings.hiddenActivation = activations::guder;
	ANNsettings.outputActivation = activations::sigmoid;
	ANNsettings.inputs = 784;
	ANNsettings.outputs = 10;
	ANNsettings.layers = 4;
	ANNsettings.learningRate = .2;
	ANNsettings.nodesPerLayer = 50;
	ANNsettings.momentum = .5;
	ANNsettings.batchSize = 1000;
	ANNsettings.typeOfMomentum = momentumType::nesterov;


	ANN ALEXIS;
	ALEXIS.init(ANNsettings);


	int loop = 1;
	int generations = 0;
	while (loop == 1) {
		std::cout << "0 = test\nanything else to train\n";
		std::cin >> generations;
		if (generations == 0) {
			int trainingNum = 0;
			int temp = 0;
			for (int i = 0; i < 20; i = i + 1) {
				trainingNum = rando(rng);

				temp = images.to_true_value(trainingNum, 0);

				for (int x = 0; x < 784; x = x + 1) {
					inputs[x] = images.gettruelocation((long long)temp + x);
				}
				xtra::mnistout(inputs);
				xtra::voutlable(ALEXIS.fprop(inputs), 1);
				std::cout << "\n\n" << xtra::max(ALEXIS.fprop(inputs)) << "\n\n\n\n";
			}
		}
		else {
			int trainingNum = 0;
			int temp = 0;
			for (int i = 0; i < generations; i = i + 1) {
				trainingNum = rando(rng);

				temp = images.to_true_value(trainingNum, 0);

				for (int x = 0; x < 784; x = x + 1) {
					inputs[x] = images.gettruelocation((long long)temp + x);
				}

				xtra::fill(&outputs, 0);
				outputs[labels.gettruelocation((long long)trainingNum)] = 1;

				ALEXIS.train(inputs, outputs);
			}
			//ALEXIS.updateNetwork();
		}
	}


	/*
	ANN net1;
	net1.init(5, 5, 3, 5, 10, activations::guder, activations::sigmoid);


	std::vector<double> inputs;
	inputs.resize(10, 0);


	int loop = 1;
	while (loop > 0) {

		for (int i = 0; i < 5; i = i + 1) {
			for (int x = 0; x < 5; x = x + 1) {
				if (i == x) {
					inputs[x] = 1;
				}
				else {
					inputs[x] = 0;
				}
			}
			net1.train(inputs, inputs);
			xtra::vout(net1.fprop(inputs), 1);
			std::cout << "\n\n\n\n";
		}

		
		net1.updateNetwork();
		if (loop == 1) {
			std::cin >> loop;
		}
		loop = loop - 1;
	}


	*/


	//std::vector<double> inputs;
	//inputs.resize(5, 0);

	//int loop = 1;
	//while (loop > 0) {
	//		for (int x = 0; x < 5; x = x + 1) {
	//			if (x == 1) {
	//				inputs[x] = 1;
	//			}
	//			else {
	//				inputs[x] = 0;
	//			}
	//		}
	//		net1.train(inputs, inputs);

	//		for (int x = 0; x < 5; x = x + 1) {
	//			if (x == 1) {
	//				inputs[x] = 1;
	//			}
	//			else {
	//				inputs[x] = 0;
	//			}
	//		}
	//		std::cout << "\n\n\n\n";
	//		xtra::vout(net1.fprop(inputs), 1);
	//		net1.updateNetwork();
	//	if (loop == 1) {
	//		std::cin >> loop;
	//	}
	//	loop = loop - 1;
	//}



	/*

	srand(static_cast<unsigned int>(time(0)));

	IDXio file;
	std::string imagefile = { "train" "-images.idx3-ubyte" };
	std::string labelfile = {"train" "-labels.idx1-ubyte"};

	imagefile.insert(0, "C:\\Users\\harle\\Downloads\\mnist\\");
	labelfile.insert(0, "C:\\Users\\harle\\Downloads\\mnist\\");

	

	dynamicdimentions images;
	dynamicdimentions labels;

	images = file.recallIDX(imagefile);
	labels = file.recallIDX(labelfile);

	ANN jeff;
	jeff.init(784, 10, 6, 28, 3.5, activations::relu, activations::sigmoid, rand());

	std::vector<double> inputs;
	std::vector<double> outputs;
	inputs.resize(784, 0);
	outputs.resize(10, 0);
	std::vector<int> params;

	int loops = 0;
	bool justforaloop = 1;
	std::cout << "=> started\n";
	while (justforaloop == true) {
		std::cout << "=> type 0 for testing\n=> type anything higher for training\n";
		std::cin >> loops;
		if (loops > 0) {
			for (int i = 0; i < loops; i = i + 1) {
				std::cout << "\n\n\n\nA new generation of JEFF has started. generation " << i + 1 << " out of " << loops << "\n\n";
				int temp = 0;
				int location = 0;
				for (int p = 0; p < 6000; p = p + 1) {
					location = rand() % 60000;
					//location = p;
					temp = images.to_true_value(location, 0);
					for (int x = 0; x < 784; x = x + 1) {
						inputs[x] = images.gettruelocation((long long)temp + x);
					}
					xtra::fill(&outputs, 0);
					outputs[labels.gettruelocation((long long)location)] = 100;
					jeff.train(inputs, outputs);
					if (p % 6000 == 0) {
						std::cout << "\nPatience is a virtue.\n" << p << "\n";
					}
					if (p % 6000 == 2000) {
						std::cout << "\nPatience is not the ability to wait, but the ability to keep a good attitude while waiting.\n" << p << "\n";
					}
					if (p % 6000 == 4000) {
						std::cout << "\nThe secret of patience is to do something else in the meantime.\n" << p << "\n";
					}
				}
				jeff.updateNetwork();
			//	jeff.learningrate = jeff.learningrate / 1.2;
			}
		}
		else if (loops < 0) {
			int temp = 0;
			int location = 0;
			location = rand() % 6000;
			temp = images.to_true_value(location, 0);
			for (int x = 0; x < 784; x = x + 1) {
				inputs[x] = images.gettruelocation((long long)temp + x);
			}
			xtra::fill(&outputs, 0);
			outputs[labels.gettruelocation((long long)location)] = 1;
			xtra::mnistout(inputs);
			xtra::vout(outputs, 1);
			outputs = jeff.fprop(inputs);
			xtra::vout(outputs, 1);
		}
		else {
			int temp = 0;
			int correct = 0;
			for (int p = 0; p < 20; p = p + 1) {
				temp = images.to_true_value(p, 0);
				for (int x = 0; x < 784; x = x + 1) {
					inputs[x] = images.gettruelocation((long long)temp + x);
				}
				xtra::fill(&outputs, 0);
				int num = labels.gettruelocation((long long)p);
				outputs[num] = 1;
				outputs = jeff.fprop(inputs);
				if(xtra::max(outputs) == outputs[num] && outputs[num] != 0){
					correct = correct + 1;
				}
				if (p%30000 == 0) {
					std::cout << "\nPatience is a virtue.\n" << p << "\n";
					xtra::mnistout(inputs, 1);
					std::cout << "\n";
					xtra::vout(outputs, 1);
					std::cout << "\n";
				}
				if (p % 30000 == 10000) {
					std::cout << "\nPatience is not the ability to wait, but the ability to keep a good attitude while waiting.\n" << p << "\n";
					xtra::mnistout(inputs, 1);
					std::cout << "\n";
					xtra::vout(outputs, 1);
					std::cout << "\n";
				}
				if (p % 30000 == 20000) {
					std::cout << "\nThe secret of patience is to do something else in the meantime.\n" << p << "\n";
					xtra::mnistout(inputs, 1);
					std::cout << "\n";
					xtra::vout(outputs, 1);
					std::cout << "\n";
				}
			}
			std::cout << "\n\n\n\n\ntotal correct\n" << correct << "\npercentage correct\n" << ((float)((float)correct/60000)*100) << "%\n\n";
		}
	}
	*/
}