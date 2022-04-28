#pragma once

#include <vector>

#include "PierreDellacherieTetrisController.h"
#include "GeneticAlgrithom.h"

using namespace std;

//使用遗传算法计算PierreDellacherie俄罗斯方块控制算法的特征系数(权重)
class CPDTetrisControllerUsingGeneticAlgorithm
{
private:
	//storage for the population of genomes
	//每一代人口的基因，对应着每一个PDTetrisControllers的特征系数(权重)
	vector<CGenome>	     m_vecPopulation;
	//m_vecPDTetrisControllers和m_vecPopulation一一对应
	vector<CPierreDellacherieTetrisController> m_vecPDTetrisControllers;

public:
	CPDTetrisControllerUsingGeneticAlgorithm();
	~CPDTetrisControllerUsingGeneticAlgorithm();
};

