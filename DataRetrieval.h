#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>    
#include "ParticleInformation.h"
#include "SumPIDCalculation.h"
#include <stdlib.h>     /* atoi */
#include <stdio.h>      /* printf, fgets */


using namespace std;

class DataRetrieval
{
	vector<ParticleInformation> particle;
	vector<SumPIDCalculation> pidParticle;

public:
	DataRetrieval(string file);
	DataRetrieval(string file, vector<SumPIDCalculation>& pidParticle);

	vector<string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	vector<string> split(const std::string &s, char delim);
	vector<ParticleInformation> getParticles();
	void formatData(vector<string>& lines);
	vector<string> dataExtraction(string file);

};
