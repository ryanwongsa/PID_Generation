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
	int numHiddenLayers;
	int numHiddenNodes;
	int numInputs;
	int numOutputs;
	int pidNNHistogramElectron[256];
	int pidNNHistogramPion[256];

	int numElectrons=0;
	int numPions=0;

	
	int MaxTimeBinValue;
	vector<SumPIDCalculation> pidParticle;

	void displayParticles()
	{

		for(int j=0;j<particles.size();j++)
		{
			cout << particles[j].getType()<<": "<< endl;
			for(int i=0;i< 27;i++)
			{
				cout << " " << particles[j].getTimeBin()[i];
			}
			cout << endl;
		}
	}

	void summationMethod()
	{
		int max =0;
		int min =27*1024*4;
		for(int u=0;u<particles.size();u++)
		{
			SumPIDCalculation pidsum(particles[u]);

			//cout << pidsum.getSum() << endl;

			pidParticle.push_back(pidsum);
			if(max<pidsum.getSum())
				max=pidsum.getSum();
			
			if(min>pidsum.getSum())
				min=pidsum.getSum();
		}

		for(int u=0;u<particles.size();u++)
		{
			pidParticle[u].setPID(min, max);
		}

		// DISPLAYS SUM PID NUMBERS
		// for(int u=0;u<particles.size();u++)
		// {
		// 	cout << particles[u].getType() << ": " << pidParticle[u].getPID() << endl;
		// }


		int sumpidHistogramElectron[256];
		std::fill(sumpidHistogramElectron, sumpidHistogramElectron+256,0);
		int sumpidHistogramPion[256];
		std::fill(sumpidHistogramPion, sumpidHistogramPion+256,0);


		for(int i=0;i<particles.size();i++)
		{
			ParticleInformation particle = particles[i];

			string type = particle.getType();
			if(type==("ELECTRON"))
			{
			//	cout <<"hred->Fill("<<pidParticle[i].getPID()<<");"<< endl;
				sumpidHistogramElectron[pidParticle[i].getPID()]++;
			}
			else if(type==("PION"))
			{
			//	cout << "hblue->Fill("<<pidParticle[i].getPID()<<");"<< endl;
				sumpidHistogramPion[pidParticle[i].getPID()]++;
			}
		}

		cout << "Electron distribution: "<< endl;
		for(int i=0;i<256;i++)
		{
			cout<< " "<< sumpidHistogramElectron[i];
		}
		cout << endl;

		cout << "Pion distribution: "<< endl;
		for(int i=0;i<256;i++)
		{
			cout<<" "<<sumpidHistogramPion[i];
		}
		cout << endl;

	}



	void fillInputs(ParticleInformation &particle, NeuronLayers& neuronLayers)
	{
		for(int j=0;j<27;j++)
		{
			//cout << " "<<particle.getTimeBin()[j]/ (float)( MaxTimeBinValue);
			neuronLayers.fillInputNeuron(j, particle.getTimeBin()[j]/ (float)( MaxTimeBinValue));
		}
		//cout <<endl;
	}

	void fillTargets(ParticleInformation &particle, NeuronLayers& neuronLayers)
	{
		float target;
		if(particle.getType()==("ELECTRON"))
			target=(float) 1;
		else
			target=(float) 0;

		for(int i=0;i<numOutputs; i++)
		{
			neuronLayers.setTarget(target, i);
		}
	}

	void fillNeuralNetworkPIDs(vector<ParticleInformation>& particles, NeuronLayers& neuronLayers, int begin)
	{		

		for(int u=begin;u<particles.size();u++)
		{
			ParticleInformation particle = particles[u];
			
			fillInputs(particle, neuronLayers);
			neuronLayers.forwardPass();

			for(int i=0; i<numOutputs;i++)
			{
				particles[u].setNeuralNetworkPID( (int) (neuronLayers.getOutput(i)*255) );
			}
		}
	}

	void makeNeuralNetworkHistogram(vector<ParticleInformation>& particles, int begin)
	{
		std::fill(pidNNHistogramElectron, pidNNHistogramElectron+256,0);
		std::fill(pidNNHistogramPion, pidNNHistogramPion+256,0);

		for(int i=begin;i<particles.size();i++)
		{
			ParticleInformation particle = particles[i];

			string type = particle.getType();
			if(type==("ELECTRON"))
			{
				pidNNHistogramElectron[particle.getNeuralNetworkPID()]++;
			}
			else if(type==("PION"))
			{
				pidNNHistogramPion[particle.getNeuralNetworkPID()]++;
			}
		}
	}

	void printNeuralNetworkHistogram()
	{
		cout << "Electron Histogram" << endl;
		for(int i=0;i<256;i++)
		{
			cout << " " << pidNNHistogramElectron[i];
		}
		cout<< endl;

		cout << "Pion Histogram" << endl;
		for(int i=0;i<256;i++)
		{
			cout << " " << pidNNHistogramPion[i];
		}
		cout<< endl;
	}

	int calcEfficiency(int nintyPercentPions)
	{
		int numP=0;
		int histogramLayerNo=0;
		for(int i=0;i<256;i++)
		{
			numP+=pidNNHistogramPion[i];
			if(numP>=nintyPercentPions)
			{
				histogramLayerNo=i;
				break;
			}
		}

		int numE=0;
		for(int i=0;i<histogramLayerNo;i++)
		{
			numE+=pidNNHistogramElectron[i];
		}

		return numE;
	}

	void calculateNumbersOfParticles(vector<ParticleInformation>& particles, int begin)
	{
		numElectrons=0;
		numPions=0;
		for(int i=begin;i<particles.size();i++)
		{
			if(particles[i].getType()=="ELECTRON")
				numElectrons++;
			else
				numPions++;
		}
	}


	int main(int argc, char const *argv[])
	{
	 	MaxTimeBinValue = 4*1024;	
	 	std::srand ( unsigned ( std::time(0) ) );
	// 	cout << "Creating PID Generator Using Neural Networks" << endl;

	 	DataRetrieval dr(argv[1]);
	 	particles = dr.getParticles();
	 	calculateNumbersOfParticles(particles,0);

	 	std::random_shuffle ( particles.begin(), particles.end() );

	 	cout << "==========================================================" << endl;
	 	cout << "Beginning PID Generation"<< endl;
	 	cout << "Total Number of Pions: "<< numPions << endl;
		cout << "Total Number of Electrons: "<< numElectrons << endl;

	 //	cout << particles.size() << endl;

	//	displayParticles();
		summationMethod();

		numInputs=27;
		numHiddenLayers =atoi(argv[2]);
		numHiddenNodes = atoi(argv[3]);
		numOutputs = atoi(argv[4]);

		double trainPercent =0.8;
		double untrainedPercent = 1-trainPercent;

		cout << "Starting Neural Network" << endl;
		cout << "Number of Inputs: " << numInputs << endl;
		cout << "Number of Hidden Layers: " << numHiddenLayers << endl;
		cout << "Number of Hidden Nodes: " << numHiddenNodes << endl;
		cout << "Number of Outputs: " << numOutputs << endl; 
	
		NeuronLayers neuronLayers(numInputs, numHiddenLayers, numHiddenNodes, numOutputs);

		// neuronLayers.displayNeuronNetwork();
		// neuronLayers.displayLinkNetwork();

		int generations = atoi(argv[5]);
		for(int i=0;i<generations;i++)
		{
			//cout << "Generation No: "<< i<< " out of " << generations << endl;
			for(int u=0;u<particles.size()*trainPercent;u++) 
			{
				ParticleInformation particle = particles[u];

				fillInputs(particle, neuronLayers);
				fillTargets(particle, neuronLayers);

				// neuronLayers.displayNeuronNetwork();
				// neuronLayers.displayLinkNetwork();

				neuronLayers.forwardPass();

				// neuronLayers.displayNeuronNetwork();
				// neuronLayers.displayLinkNetwork();

				neuronLayers.backwardPass();
			}
			// neuronLayers.displayNeuronNetwork();
			// neuronLayers.displayLinkNetwork();
				
		}

		
		
		calculateNumbersOfParticles(particles,particles.size()* trainPercent);

		

		fillNeuralNetworkPIDs(particles,neuronLayers, particles.size()*trainPercent);

		makeNeuralNetworkHistogram(particles, particles.size()* trainPercent);
		printNeuralNetworkHistogram();

		cout << "Untrained Number of Pions: "<< numPions << endl;
		cout << "Untrained Number of Electrons: "<< numElectrons << endl;

		int electronNo = calcEfficiency(numPions*0.9);

		cout<< "Effiency: " << electronNo / (double) numElectrons<<endl;
		return 0;
	}

