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

		int numP=0;
		int histogramLayerNo=0;
		for(int i=0;i<maxPIDvalue;i++)
		{
			numP+=sumpidHistogramPion[i];
			if(numP>=numPions*0.9)
			{
				histogramLayerNo=i;
				break;
			}
		}

		int numE=0;
		for(int i=0;i<histogramLayerNo;i++)
		{
			numE+=sumpidHistogramElectron[i];
		}

		cout << "Efficiency: " << numE / (float)numElectrons << endl;

	}

	void displayFeatures(vector<ParticleInformation>& particles)
	{
		int numElectronClusters[10];
		std::fill(numElectronClusters, numElectronClusters+10,0);

		int numPionClusters[10];
		std::fill(numPionClusters, numPionClusters+10,0);

		int numElectronSum[100];
		std::fill(numElectronSum, numElectronSum+100,0);

		int numPionSum[100];
		std::fill(numPionSum, numPionSum+100,0);

		int electronTB[27];
		std::fill(electronTB, electronTB+27,0);

		int pionTB[27];
		std::fill(pionTB, pionTB+27,0);

		for(int i=0;i<particles.size();i++)
		{
		//	cout << particles[i].getType() << ":"; //<< endl;
		//	cout << "\t"<<particles[i].getnumTBaboveAVGPion() << endl;
			if(particles[i].getType()=="ELECTRON")
			{
				numElectronClusters[particles[i].getCluster()]++;
				numElectronSum[particles[i].getSumTB()]++;
				electronTB[particles[i].getTBhighestCharge()]++;
//				cout << "hred->Fill("<<particles[i].getTBhighestCharge()<<");"<< endl;

			}
			else
			{
				numPionClusters[particles[i].getCluster()]++;
				numPionSum[particles[i].getSumTB()]++;
				pionTB[particles[i].getTBhighestCharge()]++;
			//	cout << "hblue->Fill("<<particles[i].getTBhighestCharge()<<");"<< endl;

			}
		}	

		cout << "====================CLUSTER INFORMATION==========================" << endl;
		cout << "ELECTRON"<< endl;
		for(int i=0;i<5;i++)
		{
			cout << numElectronClusters[i] << " ";
		}
		cout << endl;

		cout << "PION"<< endl;
		for(int i=0;i<5;i++)
		{
			cout << numPionClusters[i] << " ";
		}
		cout << endl;

		cout <<"====================SUMMATION INFORMATION==========================" << endl;
		cout << "ELECTRON"<< endl;
		for(int i=0;i<100;i++)
		{
			cout << numElectronSum[i] << " ";
		}
		cout << endl;

		cout << "PION"<< endl;
		for(int i=0;i<100;i++)
		{
			cout << numPionSum[i] << " ";
		}
		cout << endl;

		cout <<"====================TIME BIN WITH HIGHEST CHARGE INFORMATION==========================" << endl;
		cout << "ELECTRON"<< endl;
		for(int i=0;i<27;i++)
		{
			cout << electronTB[i] << " ";
		}
		cout << endl;

		cout << "PION"<< endl;
		for(int i=0;i<27;i++)
		{
			cout << pionTB[i] << " ";
		}
		cout << endl;

	}

	void addFeatures(vector<ParticleInformation>& particles)
	{

		for(int j=0;j<particles.size();j++)
		{
			int numTBaboveAVGPion=0;
			int numTBaboveMAXPion=0;
			int numTBaboveAVGElectron=0;
			int TBhighestCharge=0;
			int numClusters=0;

			float highCharge=0;
			for(int i=0;i<27;i++)
			{
				if(particles[j].getTimeBin()[i]>maxPIONvalue)	//input 0
				{
					numTBaboveMAXPion++;
				}

				if(particles[j].getTimeBin()[i]>averagePIONvalue)	//input 2
				{
					numTBaboveAVGPion++;
				}

				if(highCharge<particles[j].getTimeBin()[i])	// input 1
				{
					highCharge=particles[j].getTimeBin()[i];
					TBhighestCharge=i;
				}

				if(averageElectronValue<particles[j].getTimeBin()[i])	//input 3
				{
					numTBaboveAVGElectron++;
				}

			}

			float clusterFactor = maxELECTRONvalue *0.1;
			float thresholdCluster = maxELECTRONvalue - clusterFactor;
			float chargeOnSecondCluster=0;
			for(int i=1;i<27;i++)
			{
				if(particles[j].getTimeBin()[i]>thresholdCluster && particles[j].getTimeBin()[i-1]<thresholdCluster)
				{
					numClusters++;
					if(numClusters>1)
					{
						while(particles[j].getTimeBin()[i]>thresholdCluster && particles[j].getTimeBin()[i-1]<thresholdCluster && i<27)
						{
							if(chargeOnSecondCluster<particles[j].getTimeBin()[i])
								chargeOnSecondCluster=particles[j].getTimeBin()[i];
							i++;
						}
					}
				}
			}	

			particles[j].setCluster(numClusters);//divide by 10
			particles[j].setSumTB(v[j].getPID());//divide by 100
			particles[j].setTBhighestCharge(TBhighestCharge);	// not helpful (divide by 27)
			particles[j].setChargeSecond(chargeOnSecondCluster); // divide by 1023
			particles[j].setnumTBaboveAVGPion(numTBaboveAVGPion);	//helpful divide by 27

		}
	}

	// EDIT THIS METHOD TO CHANGE THE INPUT NEURON INFORMATION
	void fillInputs(ParticleInformation &particle, NeuronLayers& neuronLayers)
	{
		for(int j=0;j<9;j++)
		{
			float sum=0;
			for(int i=j*3;i<j*3+3;i++)
			{
				sum+=particle.getTimeBin()[i]/ (float)( MaxTimeBinValue);
			}
			neuronLayers.fillInputNeuron(j, sum);
		}


		// INPUT 0
		neuronLayers.fillInputNeuron(9, particle.getCluster()/(float)10);

		// INPUT 1
		neuronLayers.fillInputNeuron(10, particle.getSumTB()/(float)100);

		// INPUT 2
		neuronLayers.fillInputNeuron(11, particle.getTBhighestCharge()/(float)27);

		// INPUT 3
		neuronLayers.fillInputNeuron(12, particle.getChargeSecond()/(float)1023);

		// INPUT 4
		neuronLayers.fillInputNeuron(13, particle.getnumTBaboveAVGPion()/(float)27);

		//INPUT 5

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
			//particles[i].setTargetDeviation(deviation);
		}

	}
	//TOOK OUT DEVIATION

	void timebinCalculationAverages(vector<ParticleInformation>& particles)
	{
		std::fill(time_bin_index_electron, time_bin_index_electron+27,0);
		std::fill(time_bin_index_pion, time_bin_index_pion+27,0);		

		int countE=0;
		int countP=0;
		for(int i=0;i<particles.size();i++)
		{
			for(int j=0;j<27;j++)
			{
				if(particles[i].getType()=="ELECTRON")
				{
					time_bin_index_electron[j]+=particles[i].getTimeBin()[j];
				}
				else
				{
					time_bin_index_pion[j]+=particles[i].getTimeBin()[j];
				}

			}
			if(particles[i].getType()=="ELECTRON")
				countE++;
			else
				countP++;
		}

		for(int i=0;i<27;i++)
		{
			time_bin_index_pion[i]=time_bin_index_pion[i]/(float)(countP);
			time_bin_index_electron[i]=time_bin_index_electron[i]/(float)(countE);
		}


		maxPIONvalue=0;
		maxELECTRONvalue=0;
		averagePIONvalue=0;
		averageElectronValue=0;
		//cout<< "ELECTRON AVERAGE TIME BIN" << endl;
		for(int i=0;i<27;i++)
		{
			//for(int j=0;j<(int)time_bin_index_electron[i];j++)
			//{
			// cout<<"for(int i=0;i<"<<(int)time_bin_index_electron[i]<<";i++)"<< endl;
			// 	cout <<"hred->Fill("<<i<<");"<<endl;
			//}
			averageElectronValue+=time_bin_index_electron[i];
			if(maxELECTRONvalue<time_bin_index_electron[i])
				maxELECTRONvalue=time_bin_index_electron[i];
		//	cout << " " << (int)time_bin_index_electron[i];
		}
	//	cout << endl;

		averageElectronValue=averageElectronValue/(float)27;

//		cout<< "PION AVERAGE TIME BIN" << endl;
		for(int i=0;i<27;i++)
		{
			// cout<<"for(int i=0;i<"<<(int)time_bin_index_pion[i]<<";i++)"<< endl;
			// cout <<"hblue->Fill("<<i<<");"<<endl;

			averagePIONvalue+=time_bin_index_pion[i];
			if(maxPIONvalue<time_bin_index_pion[i])
				maxPIONvalue=time_bin_index_pion[i];
		//	cout << " " << (int)time_bin_index_pion[i];
		}
		averagePIONvalue=averagePIONvalue/(float)(27);
		//cout << endl;
	}

	int main(int argc, char const *argv[])
	{

	 	
	 	std::srand ( unsigned ( std::time(0) ) );
	// 	cout << "Creating PID Generator Using Neural Networks" << endl;

	 	DataRetrieval dr(argv[1]);
	 	particles = dr.getParticles();
	

		deviationDistribution(particles);	// sum as a percentage distribution is in vector v


	 	timebinCalculationAverages(particles);

	 	addFeatures(particles);

	 	displayFeatures(particles);



	 	calculateNumbersOfParticles(particles,0, particles.size());

	 	//SHUFFLE WILL MESS OUTPUT UP.. BUT STILL NEED TO SHUFFLE FOR THE NEURAL NETWORK
	 	std::random_shuffle ( particles.begin(), particles.end() );

	 	cout << "==========================================================" << endl;
	 	cout << "Beginning PID Generation"<< endl;
	 	cout << "Total Number of Pions: "<< numPions << endl;
		cout << "Total Number of Electrons: "<< numElectrons << endl;

	 //	cout << particles.size() << endl;

		cout << "=======================Summation Method==================" << endl;
	//	displayParticles();
		summationMethod();

		cout << "===================End Summation Method==================" << endl;

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

