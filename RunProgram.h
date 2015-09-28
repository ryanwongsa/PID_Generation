#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>    
#include "ParticleInformation.h"
#include "SumPIDCalculation.h"
#include "DataRetrieval.h"
#include "NeuronLayers.h"
#include <stdlib.h>     /* atoi */
#include <stdio.h>      /* printf, fgets */
#include <cstdlib> 
#include <algorithm> 
#include <ctime>


using namespace std;

	vector<ParticleInformation> particles;
	vector<SumPIDCalculation> v;
	int numHiddenLayers;
	int numHiddenNodes;
	int numInputs=14; //27
	int numOutputs;

	float time_bin_index_electron[27];
	float time_bin_index_pion[27];

	int sumpidHistogramGeneral[100];

	const int maxPIDvalue=256;

	int pidNNHistogramElectron[maxPIDvalue];
	int pidNNHistogramPion[maxPIDvalue];


	int numElectrons=0;
	int numPions=0;

	float deviationScaleFactor =0.4;

	int MaxTimeBinValue = 4*1024;
	vector<SumPIDCalculation> pidParticle;


	//INPUT DATA

	float maxPIONvalue;
	float maxELECTRONvalue;
	float averagePIONvalue;
	float averageElectronValue;


	void displayParticles();
	void summationMethod();
	void calculateNumbersOfParticles(vector<ParticleInformation>& particles, int begin, int end);
	void fillInputs(ParticleInformation &particle, NeuronLayers& neuronLayers);
	void fillTargets(ParticleInformation &particle, NeuronLayers& neuronLayers);
	float fillNeuralNetworkPIDs(vector<ParticleInformation>& particles, NeuronLayers& neuronLayers, int begin, int end);
	void makeNeuralNetworkHistogram(vector<ParticleInformation>& particles, int begin, int end);
	void printNeuralNetworkHistogram();
	int calcEfficiency(int nintyPercentPions);
	int main(int argc, char const *argv[]);
	float getNeuralNetworkError(NeuronLayers neuronLayers);
	void deviationDistribution(vector<ParticleInformation>& particles);
	void timebinCalculationAverages(vector<ParticleInformation>& particles);

