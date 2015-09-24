#include "RunProgram.h"

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
			pidParticle[u].setPID(min, max,maxPIDvalue-1);
		}

		int sumpidHistogramElectron[maxPIDvalue];
		std::fill(sumpidHistogramElectron, sumpidHistogramElectron+maxPIDvalue,0);
		int sumpidHistogramPion[maxPIDvalue];
		std::fill(sumpidHistogramPion, sumpidHistogramPion+maxPIDvalue,0);


		for(int i=0;i<particles.size();i++)
		{
			ParticleInformation particle = particles[i];

			string type = particle.getType();
			if(type==("ELECTRON"))
			{
				// if(!particle.getHelper())
				// 	cout << "ELECTRON: " <<endl<< "  "<< pidParticle[i].getPID()<< endl;
				// else
				// 	cout << "  " << pidParticle[i].getPID()<< endl;
				//cout <<"hred->Fill("<<pidParticle[i].getPID()<<");"<< endl;
				sumpidHistogramElectron[pidParticle[i].getPID()]++;
			} 
			else if(type==("PION"))
			{
				// if(!particle.getHelper())
				// 	cout << "PION: " <<endl<< "  "<< pidParticle[i].getPID()<< endl;
				// else
				// 	cout <<"  " << pidParticle[i].getPID()<< endl;
				//cout << "hblue->Fill("<<pidParticle[i].getPID()<<");"<< endl;
				sumpidHistogramPion[pidParticle[i].getPID()]++;
			}
		}

		cout << "Electron distribution: "<< endl;
		for(int i=0;i<maxPIDvalue;i++)
		{
			cout<< " "<< sumpidHistogramElectron[i];
		}
		cout << endl;

		cout << "Pion distribution: "<< endl;
		for(int i=0;i<maxPIDvalue;i++)
		{
			cout<<" "<<sumpidHistogramPion[i];
		}
		cout << endl;

	}

	// EDIT THIS METHOD TO CHANGE THE INPUT NEURON INFORMATION
	void fillInputs(ParticleInformation &particle, NeuronLayers& neuronLayers)
	{
		for(int j=0;j<numInputs;j++)
		{
			neuronLayers.fillInputNeuron(j, particle.getTimeBin()[j]/ (float)( MaxTimeBinValue));
		}
	}

	// EDIT THIS METHOD TO CHANGE THE TARGET VALUES
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

	float fillNeuralNetworkPIDs(vector<ParticleInformation>& particles, NeuronLayers& neuronLayers, int begin, int end)
	{		
		float sumError=0;
		for(int u=begin;u<end;u++)
		{
			ParticleInformation particle = particles[u];
			
			fillInputs(particle, neuronLayers);
			fillTargets(particle, neuronLayers);
			neuronLayers.forwardPass();

			for(int i=0; i<numOutputs;i++)
			{
				particles[u].setNeuralNetworkPID( (int) (neuronLayers.getOutput(i)*(maxPIDvalue-1)) );
				sumError +=neuronLayers.getErrorOutput(i);
			}
		}

		sumError = (float) sqrt(sumError/(float)(end-begin));
		return sumError;
	}

	void makeNeuralNetworkHistogram(vector<ParticleInformation>& particles, int begin, int end)
	{
		std::fill(pidNNHistogramElectron, pidNNHistogramElectron+maxPIDvalue,0);
		std::fill(pidNNHistogramPion, pidNNHistogramPion+maxPIDvalue,0);

		for(int i=begin;i<end;i++)
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
		for(int i=0;i<maxPIDvalue;i++)
		{
			cout << " " << pidNNHistogramElectron[i];
		}
		cout<< endl;

		cout << "Pion Histogram" << endl;
		for(int i=0;i<maxPIDvalue;i++)
		{
			cout << " " << pidNNHistogramPion[i];
		}
		cout<< endl;
	}

	int calcEfficiency(int nintyPercentPions)
	{
		int numP=0;
		int histogramLayerNo=0;
		for(int i=0;i<maxPIDvalue;i++)
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

	void calculateNumbersOfParticles(vector<ParticleInformation>& particles, int begin, int end)
	{
		numElectrons=0;
		numPions=0;
		for(int i=begin;i<end;i++)
		{
			if(particles[i].getType()=="ELECTRON")
				numElectrons++;
			else
				numPions++;
		}
	}


	int main(int argc, char const *argv[])
	{

	 	
	 	std::srand ( unsigned ( std::time(0) ) );
	// 	cout << "Creating PID Generator Using Neural Networks" << endl;

	 	DataRetrieval dr(argv[1]);
	 	particles = dr.getParticles();
	 	calculateNumbersOfParticles(particles,0, particles.size());

	 	//SHUFFLE WILL MESS OUTPUT UP.. BUT STILL NEED TO SHUFFLE FOR THE NEURAL NETWORK
	 	std::random_shuffle ( particles.begin(), particles.end() );

	 	cout << "==========================================================" << endl;
	 	cout << "Beginning PID Generation"<< endl;
	 	cout << "Total Number of Pions: "<< numPions << endl;
		cout << "Total Number of Electrons: "<< numElectrons << endl;

	 //	cout << particles.size() << endl;

	//	displayParticles();
	//	summationMethod();

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

		// FOR TRAINED DATA
		calculateNumbersOfParticles(particles,0, particles.size()*trainPercent);


		int generations = atoi(argv[5]);
		for(int i=0;i<generations;i++)
		{

			clog << "Generation No: "<< i<< " out of " << generations ;//<< endl;
			for(int u=0;u<particles.size()*trainPercent;u++) 
			{
				ParticleInformation particle = particles[u];

				fillInputs(particle, neuronLayers);
				fillTargets(particle, neuronLayers);

				neuronLayers.forwardPass();

				neuronLayers.backwardPass();
			}

			// extra information
			float rmsError = fillNeuralNetworkPIDs(particles,neuronLayers, 0,particles.size()* trainPercent);
			makeNeuralNetworkHistogram(particles, 0,particles.size()* trainPercent);	
			int eNo = calcEfficiency( (int) (numPions*0.9));
			clog << " Effiency Trained: " << eNo /(double) numElectrons; //<<endl;
			clog << " Error: " << rmsError <<endl;
			// end of extra information
		}

		// TRAINED RESULTS
		cout << "============================="<< endl;
		cout << "Trained Results" << endl;


		float rmsError =fillNeuralNetworkPIDs(particles,neuronLayers, 0,particles.size()* trainPercent);

		makeNeuralNetworkHistogram(particles, 0,particles.size()* trainPercent);
		printNeuralNetworkHistogram();

		cout << "Trained Number of Pions: "<< numPions << endl;
		cout << "Trained Number of Electrons: "<< numElectrons << endl;

		int electronNo = calcEfficiency( (int) (numPions*0.9));
		
		cout<< "Effiency Trained: " << electronNo /(double) numElectrons <<endl;
		cout << "Error Trained: " << rmsError <<endl;


		// UNTRAINED RESULTS
		cout << "============================="<< endl;
		cout << "Untrained Results" << endl;
		calculateNumbersOfParticles(particles,particles.size()*trainPercent, particles.size());

		rmsError =fillNeuralNetworkPIDs(particles,neuronLayers, particles.size()*trainPercent, particles.size());

		makeNeuralNetworkHistogram(particles, particles.size()* trainPercent, particles.size());
		printNeuralNetworkHistogram();

		cout << "Untrained Number of Pions: "<< numPions << endl;
		cout << "Untrained Number of Electrons: "<< numElectrons << endl;

		electronNo = calcEfficiency((int)(numPions*0.9));

		cout<< "Effiency Untrained: " << electronNo /(double) numElectrons <<endl;
		cout << "Error Untrained: " << rmsError <<endl;
		return 0;
	}

