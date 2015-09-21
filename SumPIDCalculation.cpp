#include "SumPIDCalculation.h"

	SumPIDCalculation :: SumPIDCalculation(ParticleInformation p)
	{
		type = p.getType();
		timebin = p.getTimeBin();
		particle = p;
		sum=0;

		setSum();
	}	

	int SumPIDCalculation :: getSum()
	{
		return sum;
	}

	void SumPIDCalculation :: setSum()
	{
		for(int i=0;i<27;i++)
		{
	//		cout << timebin[i] << " ";
			sum+=timebin[i];
		}
//		cout << sum<< " " << getSum()<< endl;
	}

	void SumPIDCalculation :: setPID(int min, int max) 
	{
		int total = max-min;
		pid = (int) ( ((sum-min)/((double)total)) * 255 );

	}

	int SumPIDCalculation :: getPID() const
	{
		return pid;
	}
