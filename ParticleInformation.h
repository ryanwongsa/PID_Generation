#pragma once
#include <iostream>
#include <algorithm>    // std::fill
using namespace std;

class ParticleInformation
{
	string type;
	float  *timebin;
	int pid;
	int sumPid;
	bool help;
	float deviation;

	//features
	int numTBaboveMAXPion;
	int numTBaboveAVGElectron;

	int numClusters;
	int sumTB;
	int TBhighestCharge;
	float chargeSecondCluster;
	int numTBaboveAVGPion;



public:

	ParticleInformation();
	ParticleInformation(string type);
	void setType(string t);
	string getType() const;
	void addTimebin(int i, float number);
	float*  getTimeBin();
	void setNeuralNetworkPID(int value);
	int getNeuralNetworkPID() const;
	void setSumPID(int value);
	int getSumPID() const;
	void helper(bool h);
	bool getHelper();
	void setTargetDeviation(float num);
	float getTargetDeviation();

	void setCluster(int num);
	int getCluster();

	void setSumTB(int num);
	int getSumTB();

	void setTBhighestCharge(int num);
	int getTBhighestCharge();

	void setChargeSecond(float num);
	float getChargeSecond();

	void setnumTBaboveAVGPion(int num);
	int getnumTBaboveAVGPion();

};