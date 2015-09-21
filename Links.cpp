#include "Links.h"

	Links :: Links() 
	{
		error = 0;
	}

	Links :: Links(float w)
	{
		weight = w;
		error = 0;
	}

	void Links :: setWeight(float w)
	{
		weight = w;
	}

	float Links :: getWeight() const
	{
		return weight;
	}

	void Links :: setDeltaWeight(float e)	
	{
		error = e;
	}

	float Links :: getDeltaWeight() const
	{
		return error;
	}

