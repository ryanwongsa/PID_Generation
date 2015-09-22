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
};