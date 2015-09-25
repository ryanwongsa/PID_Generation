#include "NeuronLayers.h"

NeuronLayers :: NeuronLayers(int nI, int nHL, int nHN, int nO)
{
	srand (static_cast <unsigned> (time(0)));
	numInput = nI;
	numHiddenLayers=nHL;
	numHiddenNodes=nHN;
	numOutput=nO;

	numNeuronLayers = 2+ numHiddenLayers;

	initialiseNeuronLayers();
	initialiseInput();
	initialiseHidden();
	initialiseOutput();
	initialiseLinks();
}

void NeuronLayers :: initialiseNeuronLayers()
{
	for(int i=0;i<numNeuronLayers;i++)
	{
		neuron.push_back(vector<Neuron> ());
	}
}

void NeuronLayers :: initialiseInput()
{
	for(int i=0;i<numInput;i++)
	{
		neuron[0].push_back(Neuron ());
	}
}

void NeuronLayers :: initialiseHidden()
{
	for(int i=1;i<1+numHiddenLayers;i++)
	{
		for(int j=0;j<numHiddenNodes;j++)
		{
			neuron[i].push_back(Neuron ());
		}
	}
}

void NeuronLayers :: initialiseOutput()
{
	for(int i=0;i<numOutput;i++)
	{
		neuron[numNeuronLayers-1].push_back(Neuron ());
	}
}

void NeuronLayers :: initialiseLinks()
{
	for(int i=0;i<numHiddenLayers+1;i++)
	{
		links.push_back(vector<vector<Links>> ());
	}

	// for first layer : input -> hidden
	for(int i=0;i<numInput;i++)
	{
		links[0].push_back(vector<Links> ());
		for(int j=0;j<numHiddenNodes;j++)
		{
			links[0][i].push_back(Links (float (-1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2))))));
		}
	}

	for(int a=1;a<numHiddenLayers;a++)
	{
		for(int i=0;i<numHiddenNodes;i++)
		{
			links[a].push_back(vector<Links> ());
			for(int j=0; j<numHiddenNodes;j++)
			{
				links[a][i].push_back(Links (float (-1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2))))));
			}
		}
	}

	for(int i=0;i<numHiddenNodes;i++)
	{
		links[numHiddenLayers].push_back(vector<Links> ());
		for(int j=0;j<numOutput;j++)
		{
			links[numHiddenLayers][i].push_back(Links (float (-1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2))))));
		}
	}
}

void NeuronLayers :: fillInputNeuron(int i, float value)
{
	neuron[0][i].setInput(value);

}

void NeuronLayers :: setTarget(float target, int i)
{
	neuron[numNeuronLayers-1][i].setTarget(target);
}

void NeuronLayers :: setDeviation(float dev, int i)
{
	neuron[numNeuronLayers-1][i].setTargetDeviation(dev);
}

void NeuronLayers :: displayNeuronNetwork()
{
	cout << "======================================================================" << endl;
	cout << "Displaying Neuron Neural Network" << endl;

	for(int i=0;i<neuron.size();i++)
	{
		cout << "Neuron Layer " << i << endl;
		for(int j=0;j<neuron[i].size();j++)
		{
			cout << " "<<neuron[i][j].getA();
		}
		cout << endl;
	}
}

void NeuronLayers :: displayLinkNetwork()
{
	cout << "======================================================================" << endl;
	cout << "Displaying Link Neural Network" << endl;

	for(int i=0;i<links.size();i++)
	{
		cout << "Link Layer " << i << endl;
		for(int j=0;j<links[i].size();j++)
		{
			for(int k=0; k<links[i][j].size();k++)
			{
				cout << " "<<links[i][j][k].getWeight();
			}
		}
		cout << endl;
	}
}

float NeuronLayers :: getOutput(int i)
{
	return neuron[numNeuronLayers-1][i].getA();
}

void NeuronLayers :: forwardPass()
{
	for(int i=1;i<numNeuronLayers;i++)
	{
		for(int j=0;j<neuron[i].size();j++)	//out neuron
		{
			float sum=0;
			for(int k=0;k<neuron[i-1].size();k++) // in neuron
			{
				sum+=links[i-1][k][j].getWeight()*neuron[i-1][k].getA();
			}
			neuron[i][j].setIn(sum);
			neuron[i][j].setA(sum);
		}
	}	
}

float NeuronLayers :: getErrorOutput(int i)
{
	float difference = (neuron[numNeuronLayers-1][i].getTarget() - neuron[numNeuronLayers-1][i].getA());
		
	if( abs(difference)<neuron[numNeuronLayers-1][i].getTargetDeviation() )
			difference=0;	//Can possibly Modify this section

	return SQR(difference);
}

void NeuronLayers :: backwardPass()
{
	for(int i=0;i<neuron[numNeuronLayers-1].size();i++)
	{
		float difference = (neuron[numNeuronLayers-1][i].getTarget() - neuron[numNeuronLayers-1][i].getA());

		//------MODIFICATIONS-----
		if( abs(difference)<neuron[numNeuronLayers-1][i].getTargetDeviation() )
			difference=0;	// can possiby modify this section
		//------MODIFICATIONS END-----

		float error = neuron[numNeuronLayers-1][i].getDerivIn() * difference; //(neuron[numNeuronLayers-1][i].getTarget() - neuron[numNeuronLayers-1][i].getA());
		neuron[numNeuronLayers-1][i].setError(error);
	}

	for(int l=numNeuronLayers-2;l>=0;l--)
	{
		for(int i=0;i<neuron[l].size();i++)
		{
			float sum=0;
			for(int j=0;j<neuron[l+1].size();j++)
			{
				sum+=links[l][i][j].getWeight()*neuron[l+1][j].getError();
			}

			float error = neuron[l][i].getDerivIn()*sum;
			neuron[l][i].setError(error);
		}
	}

	for(int l=0;l<links.size();l++)
	{
		for(int i=0;i<links[l].size();i++)
		{
			for(int j=0;j<links[l][i].size();j++)
			{
				float newWeight = links[l][i][j].getWeight() + LEARNING_RATE *neuron[l][i].getA() * neuron[l+1][j].getError()+ MOMENTUM*links[l][i][j].getDeltaWeight();
				links[l][i][j].setWeight(newWeight);
				links[l][i][j].setDeltaWeight(LEARNING_RATE *neuron[l][i].getA() * neuron[l+1][j].getError());
			}
		}
	}

}

