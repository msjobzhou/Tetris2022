#pragma once

#include <vector>

#include "PierreDellacherieTetrisController.h"
#include "GeneticAlgorithm.h"
#include "CParams.h"

using namespace std;

//使用遗传算法计算PierreDellacherie俄罗斯方块控制算法的特征系数(权重)
class CPDTetrisControllerUsingGeneticAlgorithm
{
private:
	//storage for the population of genomes
	//每一代人口的基因，每一个个体基因对应着每一个PDTetrisControllers的特征系数(权重)
	vector<CGenome>	     m_vecPopulation;
	int m_nPopulationSize;
	//m_vecPDTetrisControllers和m_vecPopulation一一对应
	vector<CPierreDellacherieTetrisController*> m_vecPDTetrisControllersPointer;
	int m_nNumWeightInPDController;

	//用于训练的俄罗斯方块序列，所有的个体都使用这一个序列进行训练
	//具体的序列取值在构造函数中随机生成，数量由m_nTetrisBlockNum进行控制
	vector<int> m_vecTetrisBlockType;
	int m_nTetrisBlockNum;
	//记录遗传算法已经进行到第多少代
	int	m_nCurrentGeneration;

	//指向遗传算法的指针
	CGeneticAlgorithm* m_pGA;
	vector<double>		   m_vecAvFitness;
	vector<double>		   m_vecBestFitness;

public:
	CPDTetrisControllerUsingGeneticAlgorithm();
	~CPDTetrisControllerUsingGeneticAlgorithm();
	
	void RunTetrisGameSimulation(CPierreDellacherieTetrisController &pdController, double& dFitness);
	void MainProcess();
};

