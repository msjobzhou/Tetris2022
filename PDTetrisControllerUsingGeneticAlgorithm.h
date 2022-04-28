#pragma once

#include <vector>

#include "PierreDellacherieTetrisController.h"
#include "GeneticAlgrithom.h"

using namespace std;

//ʹ���Ŵ��㷨����PierreDellacherie����˹��������㷨������ϵ��(Ȩ��)
class CPDTetrisControllerUsingGeneticAlgorithm
{
private:
	//storage for the population of genomes
	//ÿһ���˿ڵĻ��򣬶�Ӧ��ÿһ��PDTetrisControllers������ϵ��(Ȩ��)
	vector<CGenome>	     m_vecPopulation;
	//m_vecPDTetrisControllers��m_vecPopulationһһ��Ӧ
	vector<CPierreDellacherieTetrisController> m_vecPDTetrisControllers;

public:
	CPDTetrisControllerUsingGeneticAlgorithm();
	~CPDTetrisControllerUsingGeneticAlgorithm();
};

