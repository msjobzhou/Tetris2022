#include "PDTetrisControllerUsingGeneticAlgorithm.h"



CPDTetrisControllerUsingGeneticAlgorithm::CPDTetrisControllerUsingGeneticAlgorithm()
{
}


CPDTetrisControllerUsingGeneticAlgorithm::~CPDTetrisControllerUsingGeneticAlgorithm()
{
}
//针对当前CPierreDellacherieTetrisController模拟运行俄罗斯方块游戏，并返回其对应的fitness值
void CPDTetrisControllerUsingGeneticAlgorithm::RunTetrisGameSimulation(CPierreDellacherieTetrisController &pdController, double &dFitness)
{
	//用于训练的俄罗斯方块序列为空时，不做任何动作
	if (m_vecTetrisBlockType.empty())
		return;

	for (vector<int>::iterator it = m_vecTetrisBlockType.begin(); it != m_vecTetrisBlockType.end(); it++)
	{
		int nBlockType = *it;

	}
}