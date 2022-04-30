#include "PDTetrisControllerUsingGeneticAlgorithm.h"
#include<assert.h>


CPDTetrisControllerUsingGeneticAlgorithm::CPDTetrisControllerUsingGeneticAlgorithm()
{
	m_pGA = 0;
	m_nPopulationSize = 100;
	m_nNumWeightInPDController = 6;
	m_pGA = new CGeneticAlgorithm(m_nPopulationSize,
		CParams::dMutationRate,
		CParams::dCrossoverRate,
		m_nNumWeightInPDController);

	//ͨ���Ŵ��㷨�����ʼ����PDController��Ȩ�ز���
	m_vecPopulation = m_pGA->GetChromos();

	for (int i = 0; i < m_nPopulationSize; i++)
	{
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.right = 100;
		rect.bottom = 200;
		//rectû�����壬�Ŵ��㷨���沢����ʹ��
		CTetrisDraw* pTetrisDraw = new CTetrisDraw(0, rect);
		m_vecPDTetrisControllers.push_back(CPierreDellacherieTetrisController(pTetrisDraw));
	}
	//���Ŵ��㷨�õ���Ȩ�ز�����ֵ����Ӧ��CPierreDellacherieTetrisController
	for (int i = 0; i < m_nPopulationSize; i++)
	{
		sPDTetrisControllerCoefficient pdcCoeff;
		pdcCoeff.lh = m_vecPopulation[i].vecWeights[0];
		pdcCoeff.epcm = m_vecPopulation[i].vecWeights[1];
		pdcCoeff.brt = m_vecPopulation[i].vecWeights[2];
		pdcCoeff.bct = m_vecPopulation[i].vecWeights[3];
		pdcCoeff.bbh = m_vecPopulation[i].vecWeights[4];
		pdcCoeff.bw = m_vecPopulation[i].vecWeights[5];
		m_vecPDTetrisControllers[i].setPDTetrisControllerCoefficient(pdcCoeff);
	}

}


CPDTetrisControllerUsingGeneticAlgorithm::~CPDTetrisControllerUsingGeneticAlgorithm()
{
	if (0 != m_pGA)
	{
		delete m_pGA;
	}
	for (int i = 0; i < m_nPopulationSize; i++)
	{
		CTetrisDraw *pTetrisDraw = m_vecPDTetrisControllers[i].getTetrisDraw();
		delete pTetrisDraw;
	}
}
//��Ե�ǰCPierreDellacherieTetrisControllerģ�����ж���˹������Ϸ�����������Ӧ��fitnessֵ
void CPDTetrisControllerUsingGeneticAlgorithm::RunTetrisGameSimulation(CPierreDellacherieTetrisController &pdController, double &dFitness)
{
	dFitness = 0;
	//����ѵ���Ķ���˹��������Ϊ��ʱ�������κζ���
	if (m_vecTetrisBlockType.empty())
		return;
	int nTetrisBlockUsed = 0;
	for (vector<int>::iterator it = m_vecTetrisBlockType.begin(); it != m_vecTetrisBlockType.end(); it++)
	{
		int nBlockType = *it;
		nTetrisBlockUsed++;
		CTetrisBlock* pCurTetrisBlock = new CTetrisBlock(nBlockType);
		list<int> AICmdList;
		pdController.generateAICommandListForCurrentTetrisBlock(AICmdList);
		while (!AICmdList.empty())
		{
			if (pCurTetrisBlock->getBlockPosY() <= nTetrisBoardHeight - 1)
			{
				int cmd = AICmdList.front();
				pdController.executeCommand(cmd);
				AICmdList.pop_front();
			}
			else {
				bool bMoveDown = pdController.executeCommand(3);
				if (false == bMoveDown)
				{
					if (false == pdController.isGameOver())
					{
						//�����Ҫ���벻ͬԭ��ֵ���ж�
						dFitness = nTetrisBlockUsed;
						return;
					}
				}
			}
		}
		//AICmdListΪ��ʱ�������е����ִ����ʱ,ֱ���ƶ�����ǰ���鴹ֱ����������λ��
		bool bMoveDown = true;
		while (bMoveDown)
		{
			bMoveDown = pdController.executeCommand(3);
		}
		if (false == pdController.isGameOver())
		{
			//�����Ҫ���벻ͬԭ��ֵ���ж�
			dFitness = nTetrisBlockUsed;
			return;
		}
	}
	assert(nTetrisBlockUsed<=m_nTetrisBlockNum);
	//��������ѵ���Ķ���˹���鶼ʹ�����ˣ���Ϸ��Ȼ������
	dFitness = nTetrisBlockUsed;
	return;
}

void CPDTetrisControllerUsingGeneticAlgorithm::MainProcess()
{
	while (true)
	{
		for (int i = 0; i < m_nPopulationSize; i++)
		{
			double dFitness;
			RunTetrisGameSimulation(m_vecPDTetrisControllers[i], dFitness);
			m_vecPopulation[i].dFitness = dFitness;
		}

		m_vecAvFitness.push_back(m_pGA->AverageFitness());
		m_vecBestFitness.push_back(m_pGA->BestFitness());

		++m_nCurrentGeneration;

		//run the GA to create a new population
		m_vecPopulation = m_pGA->Epoch(m_vecPopulation);

		//���Ŵ��㷨�õ���Ȩ�ز�����ֵ����Ӧ��CPierreDellacherieTetrisController����������Controller��Ӧ����Ϸ
		for (int i = 0; i < m_nPopulationSize; i++)
		{
			sPDTetrisControllerCoefficient pdcCoeff;
			pdcCoeff.lh = m_vecPopulation[i].vecWeights[0];
			pdcCoeff.epcm = m_vecPopulation[i].vecWeights[1];
			pdcCoeff.brt = m_vecPopulation[i].vecWeights[2];
			pdcCoeff.bct = m_vecPopulation[i].vecWeights[3];
			pdcCoeff.bbh = m_vecPopulation[i].vecWeights[4];
			pdcCoeff.bw = m_vecPopulation[i].vecWeights[5];
			m_vecPDTetrisControllers[i].setPDTetrisControllerCoefficient(pdcCoeff);
			m_vecPDTetrisControllers[i].resetGame();
		}
	}
	

}