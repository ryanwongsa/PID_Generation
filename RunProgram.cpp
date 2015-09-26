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
			cout <<  "Deviation: " << particles[j].getTargetDeviation() <<endl;
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
			neuronLayers.setDeviation(particle.getTargetDeviation(), i);
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

	void deviationDistribution(vector<ParticleInformation>& particles)
	{
		vector<SumPIDCalculation> v;

		int max=0;
		int min =27*1024*4;
		int midPointE;
		int midPointP;

		for(int i=0;i<particles.size();i++)
		{
			ParticleInformation particle = particles[i];
			SumPIDCalculation sumCal(particle);
			v.push_back(sumCal);

			if(max<sumCal.getSum())
				max=sumCal.getSum();
			
			if(min>sumCal.getSum())
				min=sumCal.getSum();
		}

		for(int u=0;u<particles.size();u++)
		{
			v[u].setPID(min, max,99);
		}

		int sumpidHistogramElectron[100];
		std::fill(sumpidHistogramElectron, sumpidHistogramElectron+100,0);
		int sumpidHistogramPion[100];
		std::fill(sumpidHistogramPion, sumpidHistogramPion+100,0);



		for(int i=0;i<particles.size();i++)
		{
			ParticleInformation particle = particles[i];

			string type = particle.getType();
			if(type==("ELECTRON"))
			{
				sumpidHistogramElectron[v[i].getPID()]++;
			} 
			else if(type==("PION"))
			{
				sumpidHistogramPion[v[i].getPID()]++;
			}
		}

		int maxElectron=0;
		int maxPion=0;
		int electronMP=0;
		int pionMP=0;

		for(int i=0;i<100;i++)
		{
			if(sumpidHistogramElectron[i]>maxElectron)
			{
				maxElectron=sumpidHistogramElectron[i];
				electronMP=i;
			}
		}

		for(int i=0;i<100;i++)
		{
			if(sumpidHistogramPion[i]>maxPion)
			{
				maxPion=sumpidHistogramPion[i];
				pionMP=i;
			}
		}

		for(int i=0;i<particles.size();i++)
		{
			ParticleInformation particle = particles[i];
			float deviation=0;
			string type = particle.getType();
			if(type==("ELECTRON"))
			{
				deviation = 1 - sumpidHistogramElectron[v[i].getPID()]/(float) maxElectron;
			} 
			else if(type==("PION"))
			{
				deviation = 1 - sumpidHistogramPion[v[i].getPID()]/(float) maxPion;
			}

			deviation = deviation*deviationScaleFactor;
			particles[i].setTargetDeviation(deviation);
		}

	}

	int main(int argc, char const *argv[])
	{

	 	
	 	std::srand ( unsigned ( std::time(0) ) );
	// 	cout << "Creating PID Generator Using Neural Networks" << endl;

	 	DataRetrieval dr(argv[1]);
	 	particles = dr.getParticles();

	 	deviationDistribution(particles);	//NEW METHOD HERE

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
		int numTrainedParticles =particles.size()*trainPercent;

		calculateNumbersOfParticles(particles,0, numTrainedParticles);


		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
		cout << "Total Number of Pions Pre-Trained: "<< numPions << endl;
		cout << "Total Number of Electrons Pre-Trained: "<< numElectrons << endl;
		cout << "NumTrained Pre-Trained: "<< numTrainedParticles << endl;
		//=================GET 50/50 PION ELECTRON RATIO============
		
		int difference = numPions - numElectrons;
		int differenceOriginal =difference;
		cout << "Difference: "<< difference << endl;

		if(difference >0)
		{
			int count=0;
			while(difference >0)
			{
				//cout<< particles[count].getType() << count <<endl;
				if(particles[count].getType()=="PION")
				{
					//cout<< particles[count].getType() << " REMOVED "<< count<< endl;
					particles.erase(particles.begin()+count);
					difference--;
				}
				else
					count++;
			}
		}
		else if(difference <0)
		{
			int count=0;
			while(difference <0)
			{
				if(particles[count].getType()=="ELECTRON")
				{
					particles.erase(particles.begin()+count);
					difference++;
				}
				else
					count++;
			}
		}
		numTrainedParticles=numTrainedParticles-abs(differenceOriginal);
		calculateNumbersOfParticles(particles,0, numTrainedParticles);

		cout << "Total Number of Pions Trained: "<< numPions << endl;
		cout << "Total Number of Electrons Trained: "<< numElectrons << endl;
		cout << "NumTrained: "<< numTrainedParticles << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
		//==========================================================

		int generations = atoi(argv[5]);
		for(int i=0;i<generations;i++)
		{
			std::random_shuffle ( particles.begin(), particles.begin()+numTrainedParticles );

			clog << "Generation No: "<< i<< " out of " << generations ;//<< endl;
			for(int u=0;u<numTrainedParticles;u++) 
			{
				ParticleInformation particle = particles[u];

				fillInputs(particle, neuronLayers);
				fillTargets(particle, neuronLayers);

				neuronLayers.forwardPass();

				neuronLayers.backwardPass();
			}

			// extra information
			float rmsError = fillNeuralNetworkPIDs(particles,neuronLayers, 0,numTrainedParticles);
			makeNeuralNetworkHistogram(particles, 0,numTrainedParticles);	
			int eNo = calcEfficiency( (int) (numPions*0.9));
			clog << " Effiency Trained: " << eNo /(double) numElectrons; //<<endl;
			clog << " Error: " << rmsError <<endl;
			// end of extra information
		}

		// TRAINED RESULTS
		cout << "============================="<< endl;
		cout << "Trained Results" << endl;


		float rmsError =fillNeuralNetworkPIDs(particles,neuronLayers, 0,numTrainedParticles);

		makeNeuralNetworkHistogram(particles, 0,numTrainedParticles);
		printNeuralNetworkHistogram();

		cout << "Trained Number of Pions: "<< numPions << endl;
		cout << "Trained Number of Electrons: "<< numElectrons << endl;

		int electronNo = calcEfficiency( (int) (numPions*0.9));
		
		cout<< "Effiency Trained: " << electronNo /(double) numElectrons <<endl;
		cout << "Error Trained: " << rmsError <<endl;


		// UNTRAINED RESULTS
		cout << "============================="<< endl;
		cout << "Untrained Results" << endl;
		calculateNumbersOfParticles(particles,numTrainedParticles, particles.size());

		rmsError =fillNeuralNetworkPIDs(particles,neuronLayers, numTrainedParticles, particles.size());

		makeNeuralNetworkHistogram(particles, numTrainedParticles, particles.size());
		printNeuralNetworkHistogram();

		cout << "Untrained Number of Pions: "<< numPions << endl;
		cout << "Untrained Number of Electrons: "<< numElectrons << endl;

		electronNo = calcEfficiency((int)(numPions*0.9));

		cout<< "Effiency Untrained: " << electronNo /(double) numElectrons <<endl;
		cout << "Error Untrained: " << rmsError <<endl;
		return 0;
	}

