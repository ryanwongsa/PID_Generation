#include "ParticleInformation.h"

	ParticleInformation :: ParticleInformation()
	{
		type = "UNKNOWN";
		timebin = new float[27];
		std::fill(timebin, timebin+27,0);
		
	}

	ParticleInformation :: ParticleInformation(string t)
	{
		type = t;
		timebin = new float[27];
		std::fill(timebin, timebin+27,0);

	}

	void ParticleInformation :: setType(string t)
	{
		type = t; 
	}

	string ParticleInformation :: getType() const
	{
		return type;
	}

	void ParticleInformation :: addTimebin(int i, float number)
	{
		timebin[i]+=number;
	}

	float* ParticleInformation :: getTimeBin()
	{
		return timebin;
	}

	void ParticleInformation :: setNeuralNetworkPID(int value)
	{
		pid=value;
	}

	int ParticleInformation :: getNeuralNetworkPID() const
	{
		return pid;
	}

	void ParticleInformation :: setSumPID(int value)
	{
		sumPid=value;
	}

	int ParticleInformation :: getSumPID() const
	{
		return sumPid;
	}

	void ParticleInformation :: helper(bool h)
	{
		help =h;
	}

	bool ParticleInformation :: getHelper()
	{
		return help;
	}

	void ParticleInformation :: setTargetDeviation(float num)
	{
		deviation = num;
	}

	float ParticleInformation :: getTargetDeviation()
	{
		return deviation;
	}


	void ParticleInformation :: setCluster(int num)
	{
		numClusters= num;
	}

	int ParticleInformation :: getCluster()
	{
		return numClusters;
	}

	void ParticleInformation :: setSumTB(int num)
	{
		sumTB=num;
	}

	int ParticleInformation :: getSumTB()
	{
		return sumTB;
	}

	void ParticleInformation :: setTBhighestCharge(int num)
	{
		TBhighestCharge = num;
	}

	int ParticleInformation :: getTBhighestCharge()
	{
		return TBhighestCharge;
	}

	void ParticleInformation :: setChargeSecond(float num)
	{
		chargeSecondCluster=num;
	}

	float ParticleInformation :: getChargeSecond()
	{
		return chargeSecondCluster;
	}

	void ParticleInformation :: setnumTBaboveAVGPion(int num)
	{
		numTBaboveAVGPion=num;
	}

	int ParticleInformation :: getnumTBaboveAVGPion()
	{
		return numTBaboveAVGPion;
	}

	
