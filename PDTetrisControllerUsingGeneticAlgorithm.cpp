#include "PDTetrisControllerUsingGeneticAlgorithm.h"



CPDTetrisControllerUsingGeneticAlgorithm::CPDTetrisControllerUsingGeneticAlgorithm()
{
}


CPDTetrisControllerUsingGeneticAlgorithm::~CPDTetrisControllerUsingGeneticAlgorithm()
{
}
//��Ե�ǰCPierreDellacherieTetrisControllerģ�����ж���˹������Ϸ�����������Ӧ��fitnessֵ
void CPDTetrisControllerUsingGeneticAlgorithm::RunTetrisGameSimulation(CPierreDellacherieTetrisController &pdController, double &dFitness)
{
	//����ѵ���Ķ���˹��������Ϊ��ʱ�������κζ���
	if (m_vecTetrisBlockType.empty())
		return;

	for (vector<int>::iterator it = m_vecTetrisBlockType.begin(); it != m_vecTetrisBlockType.end(); it++)
	{
		int nBlockType = *it;

	}
}