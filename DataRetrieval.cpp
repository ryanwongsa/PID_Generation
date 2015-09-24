#include "DataRetrieval.h"	


	DataRetrieval :: DataRetrieval(string file)	
	{
		vector<string>  lines = dataExtraction(file);
		formatData(lines);
	}

	DataRetrieval :: DataRetrieval(string file, vector<SumPIDCalculation>& pidPart)
	{
		vector<string>  lines = dataExtraction(file);
		pidParticle = pidPart;
	}

	bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }


	std::vector<std::string> & DataRetrieval::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        elems.push_back(item);
	    }
	    return elems;
	}


	vector<string> DataRetrieval:: split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}


	vector<ParticleInformation> DataRetrieval :: getParticles() 
	{
		return particle;
	}

	void DataRetrieval :: formatData(vector<string>& lines) // need to still save the event number and track number
	{
		string particleType ="UNKNOWN";
		for(int i=0;i<lines.size();i++)
		{
			string line = lines[i];


 			if (( (line.find("track") != string::npos) || (line.find("----------------------------------------------------------")!= string::npos) ) && (i+1<lines.size()))
 			{
 				ParticleInformation currParticle;
				if(!(lines[i+1].compare("ELECTRON") ) || !(lines[i+1].compare("PION")))
				{
					particleType =lines[i+1];
					currParticle.helper(false);
					currParticle.setType(particleType); 	 //constructor call
					i=i+2;
				}
				else
				{
					currParticle.helper(true);
					if((i+1<lines.size()) && (lines[i+1].find("sum") != string::npos ) )
					{
						currParticle.setType(particleType); 	 //constructor call
						i=i+1;
					}
					else
						i++;
				}

				int j=i;
				while(lines[j].find("sum") != string::npos)
				{
					//cout<< lines[i] << endl;	// THIS LINE SPOTS FOR ERRORS

					vector<string> parts = split(lines[j], ' ');
					for(int a=1;a<28;a++)
					{
						currParticle.addTimebin(a-1, atoi(parts[a].c_str()));
					}
				  	j++;
				 }
				 i=j-1;
				
				if(currParticle.getType()!=("UNKNOWN"))
					particle.push_back(currParticle);
 			}
		}
	}

	vector<string> DataRetrieval :: dataExtraction(string file)
	{
		vector<string> lines;

		string line;
	  	ifstream myfile (file);

	  	if (myfile.is_open())
	  	{
	    	while ( getline (myfile,line) )
	    	{

	      		lines.push_back(line);
	    	}
	    	myfile.close();
	  	}
		else cout << "Unable to open file"; 


		for(int i=0;i<lines.size();i++)
		{
			std::string::iterator new_end = std::unique(lines[i].begin(), lines[i].end(), BothAreSpaces);
			lines[i].erase(new_end, lines[i].end());   

		}

        return lines;
	}
