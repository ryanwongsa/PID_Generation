#pragma once
#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;

class Neuron
{
	float in;
	float a;
	float target;
	float signalError;
	float activationValue;
	float deviation;

public:
	Neuron();
	Neuron(int v);

	float getA() const;
	void setA(float num);
	float getIn() const;
	void setIn(float num);
	void setTarget(float num);
	float getTarget() const;
	void setError(float e);
	float getError() const;
	float getActivation(float x);
	float getDerivIn();
	void setInput(float x);
	void setTargetDeviation(float num);
	float getTargetDeviation();
};