#pragma once

#include <vector>
#include <algorithm>

using namespace std;

class CGenome
{
public:
	vector <double>	vecWeights;

	double          dFitness;

	CGenome() :dFitness(0) {}

	CGenome(vector <double> w, double f) : vecWeights(w), dFitness(f) {}

	//overload '<' used for sorting
	friend bool operator<(const CGenome& lhs, const CGenome& rhs)
	{
		return (lhs.dFitness < rhs.dFitness);
	}
};


class CGeneticAlgorithm
{

private:

	//this holds the entire population of chromosomes
	vector <CGenome>	 m_vecPop;

	//size of population
	int m_iPopSize;

	//amount of weights per chromo
	int m_iChromoLength;

	//total fitness of population
	double m_dTotalFitness;

	//best fitness this population
	double m_dBestFitness;

	//average fitness
	double m_dAverageFitness;

	//worst
	double m_dWorstFitness;

	//keeps track of the best genome
	int		m_iFittestGenome;

	//probability that a chromosones bits will mutate.
	//Try figures around 0.05 to 0.3 ish
	double m_dMutationRate;

	//probability of chromosones crossing over bits
	//0.7 is pretty good
	double m_dCrossoverRate;

	//generation counter
	int	  m_cGeneration;


	void    Crossover(const vector<double> &mum,
		const vector<double> &dad,
		vector<double>       &baby1,
		vector<double>       &baby2);


	void    Mutate(vector<double> &chromo);

	CGenome	GetChromoRoulette();

	void	  GrabNBest(int	            NBest,
		const int       NumCopies,
		vector<CGenome>	&vecPop);

	void	  FitnessScaleRank();

	void	  CalculateBestWorstAvTot();

	void	  Reset();



public:

	CGeneticAlgorithm(int 		popsize,
		double	MutRat,
		double	CrossRat,
		int	  	numweights);

	//this runs the GA for one generation.
	vector<CGenome>	Epoch(vector<CGenome> &old_pop);


	//-------------------accessor methods
	vector<CGenome>	GetChromos()const { return m_vecPop; }
	double			    AverageFitness()const { return m_dTotalFitness / m_iPopSize; }
	double		    	BestFitness()const { return m_dBestFitness; }

public:
	
	~CGeneticAlgorithm();
};

