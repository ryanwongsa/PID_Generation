#pragma once
#include <iostream>
#include <string>
#include "ParticleInformation.h"
using namespace std;

class SumPIDCalculation
{
	ParticleInformation particle;
	string type;
	float* timebin;
	int sum;
	int pid;

public:
	SumPIDCalculation(ParticleInformation p);

	int getSum();
	void setSum();
	void setPID(int min, int max, int maxPID);
	int getPID() const;


};