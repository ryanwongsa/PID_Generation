#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>    
#include "Links.h"
#include "Neuron.h"
#include <stdlib.h>    
#include <stdio.h>   
#include <cstdlib>
#include <ctime> 

class NeuronLayers
{
	int numInput;
	int numHiddenLayers;
	int numHiddenNodes;
	int numOutput;
	int numNeuronLayers;

	float LEARNING_RATE = (float) 1;
	float MOMENTUM = (float) 0.5;

	vector<vector<Neuron>> neuron;
 
	vector<vector<vector< Links >>> links;
	
public:
	NeuronLayers(int numInput, int numHiddenLayers, int numHiddenNodes, int numOutput);
	void initialiseNeuronLayers();
	void initialiseInput();
	void initialiseHidden();
	void initialiseOutput();
	void initialiseLinks();
	void fillInputNeuron(int i, float value);
	void setTarget(float target, int i);
	void displayNeuronNetwork();
	void displayLinkNetwork();
	void forwardPass();
	void backwardPass();
	float getOutput(int i);
};

