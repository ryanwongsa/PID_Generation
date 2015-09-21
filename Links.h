#pragma once
#include <iostream>
using namespace std;

class Links
{
	float weight;
	float error;

public:
	Links();
	Links(float w);
	
	void setWeight(float w);
	float getWeight() const;
	void setDeltaWeight(float e);
	float getDeltaWeight() const;

};