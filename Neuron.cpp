#include "Neuron.h"

	Neuron :: Neuron()
	{
		in =0;
		a=0;
		deviation=0;
	}

	Neuron :: Neuron(int v)
	{
		a = v;
		in =0;
		deviation=0;
	}

	float Neuron :: getA() const
	{
		return a;
	}


	void Neuron :: setA(float x)
	{
		a = 1 / ( (float)(1 + exp(- x)) );
	}

	void Neuron :: setInput(float x)
	{
		a = x;
	}

	float Neuron :: getIn() const
	{
		return in;
	}


	void Neuron :: setIn(float num)
	{
		in = num;
	}

	float Neuron :: getActivation(float x)
	{
		return 1 / ( (float)(1 + exp(- x)) );
	}

	float Neuron :: getDerivIn()
	{
		return a * (1 - a);
	}

	void Neuron :: setTarget(float num)
	{
		target=num;
	}

	float Neuron :: getTarget() const
	{
		return target;
	}

	void Neuron :: setError(float e)
	{
		signalError = e;
	}

	float Neuron :: getError() const
	{
		return signalError;
	}

	void Neuron :: setTargetDeviation(float num)
	{
		deviation = num;
	}

	float Neuron :: getTargetDeviation()
	{
		return deviation;
	}

