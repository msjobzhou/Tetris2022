#include "PDTetrisControllerUsingGeneticAlgorithm.h"
#include<assert.h>
#include <time.h>
#include "FileLogger.h"
#include <sstream>
#include <math.h>

using namespace std;

FileLogger g_fileLoggerPDTCGA("tetrisBlockGALog.txt", debug);

CPDTetrisControllerUsingGeneticAlgorithm::CPDTetrisControllerUsingGeneticAlgorithm()
{
	m_pGA = 0;
	m_nPopulationSize = 100;
	m_nNumWeightInPDController = 6;
	m_nCurrentGeneration = 0;
	m_nTetrisBlockNum = 5000;
	m_pGA = new CGeneticAlgorithm(m_nPopulationSize,
		CParams::dMutationRate,
		CParams::dCrossoverRate,
		m_nNumWeightInPDController);

	
	srand((unsigned int)time(NULL));  // �����������
	int nType;
	//��������ѵ���Ķ���˹��������
	for (int i = 0; i < m_nTetrisBlockNum; i++)
	{
		nType = rand() % 7 + 1;
		m_vecTetrisBlockType.push_back(nType);
	}
	

	//ͨ���Ŵ��㷨�����ʼ����PDController��Ȩ�ز���
	m_vecPopulation = m_pGA->GetChromos();

	for (int i = 0; i < m_nPopulationSize; i++)
	{
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.right = 100;
		rect.bottom = 200;
		//rectû�����壬�Ŵ��㷨���沢����ʹ�ã�CTetrisDraw��һ������HDC���ó�0����ʾ�Ŵ��㷨������̲������ʵ�ʻ�ͼ
		CTetrisDraw* pTetrisDraw = new CTetrisDraw(0, rect);
		CPierreDellacherieTetrisController* pdc1 = new CPierreDellacherieTetrisController(pTetrisDraw);
		m_vecPDTetrisControllersPointer.push_back(pdc1);
	}
	//���Ŵ��㷨�õ���Ȩ�ز�����ֵ����Ӧ��CPierreDellacherieTetrisController
	for (int i = 0; i < m_nPopulationSize; i++)
	{
		sPDTetrisControllerCoefficient pdcCoeff;
		/*pdcCoeff.lh = abs(m_vecPopulation[i].vecWeights[0]);
		pdcCoeff.epcm = abs(m_vecPopulation[i].vecWeights[1]);
		pdcCoeff.brt = abs(m_vecPopulation[i].vecWeights[2]);
		pdcCoeff.bct = abs(m_vecPopulation[i].vecWeights[3]);
		pdcCoeff.bbh = abs(m_vecPopulation[i].vecWeights[4]);
		pdcCoeff.bw = abs(m_vecPopulation[i].vecWeights[5]);*/
		pdcCoeff.lh = m_vecPopulation[i].vecWeights[0];
		pdcCoeff.epcm = m_vecPopulation[i].vecWeights[1];
		pdcCoeff.brt = m_vecPopulation[i].vecWeights[2];
		pdcCoeff.bct = m_vecPopulation[i].vecWeights[3];
		pdcCoeff.bbh = m_vecPopulation[i].vecWeights[4];
		pdcCoeff.bw = m_vecPopulation[i].vecWeights[5];
		m_vecPDTetrisControllersPointer[i]->setPDTetrisControllerCoefficient(pdcCoeff);
		CTetrisBlock* pTetrisBlock = m_vecPDTetrisControllersPointer[i]->getCurTetrisBlock();
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
		CTetrisDraw *pTetrisDraw = m_vecPDTetrisControllersPointer[i]->getTetrisDraw();
		delete pTetrisDraw;
		delete m_vecPDTetrisControllersPointer[i];
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
	stringstream ss;
	for (vector<int>::iterator it = m_vecTetrisBlockType.begin(); it != m_vecTetrisBlockType.end(); it++)
	{
		
		int nBlockType = *it;
		nTetrisBlockUsed++;
		/*ss << "nTetrisBlockUsed:" << nTetrisBlockUsed;
		string debug = ss.str();
		g_fileLoggerPDTCGA.Debug(debug);
		ss.clear();
		ss.str("");*/
		CTetrisBlock* pCurTetrisBlock = new CTetrisBlock(nBlockType);
		pdController.setCurTetrisBlock(pCurTetrisBlock);
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
					if (true == pdController.isGameOver())
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
		if (true == pdController.isGameOver())
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
	stringstream ss;
	while (true)
	{
		double dBestFitness = -999999.0;
		sPDTetrisControllerCoefficient pdtccBest;
		sPDTetrisControllerCoefficient pdtccCurrent;
		for (int i = 0; i < m_nPopulationSize; i++)
		{
			double dFitness=0;
			//clock_t start_time, end_time;
			//start_time = clock();
			RunTetrisGameSimulation(*m_vecPDTetrisControllersPointer[i], dFitness);
			//end_time = clock();
			//double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
			pdtccCurrent = m_vecPDTetrisControllersPointer[i]->getPDTetrisControllerCoefficient();
			m_vecPopulation[i].dFitness = dFitness;
			ss <<"population : " << i << " " << pdtccCurrent.lh << " " << pdtccCurrent.epcm << " " << pdtccCurrent.brt << " " << pdtccCurrent.bct << " " << pdtccCurrent.bbh << " " << pdtccCurrent.bw << " dFitness: " << dFitness <<endl;
			string debug = ss.str();
			g_fileLoggerPDTCGA.Debug(debug);
			ss.clear();
			ss.str("");
			if (dFitness > dBestFitness)
			{
				dBestFitness = dFitness;
				pdtccBest = m_vecPDTetrisControllersPointer[i]->getPDTetrisControllerCoefficient();
			}
		}
		
		m_vecAvFitness.push_back(m_pGA->AverageFitness());
		m_vecBestFitness.push_back(m_pGA->BestFitness());

		++m_nCurrentGeneration;
		ss << "m_nCurrentGeneration: " << m_nCurrentGeneration << " best fitness: " << m_pGA->BestFitness() 
			<< " best index:"<< m_pGA->BestFitnessIndex()<< endl;

		ss << "average  fitness: " << m_pGA->AverageFitness() << endl;

		ss << "best weight: " <<pdtccBest.lh << " " << pdtccBest.epcm << " " << pdtccBest.brt << " " << pdtccBest.bct << " " << pdtccBest.bbh << " " << pdtccBest.bw << endl;


		string debug = ss.str();
		g_fileLoggerPDTCGA.Debug(debug);
		ss.clear();
		ss.str("");

		//run the GA to create a new population
		m_vecPopulation = m_pGA->Epoch(m_vecPopulation);

		//���Ŵ��㷨�õ���Ȩ�ز�����ֵ����Ӧ��CPierreDellacherieTetrisController����������Controller��Ӧ����Ϸ
		for (int i = 0; i < m_nPopulationSize; i++)
		{
			sPDTetrisControllerCoefficient pdcCoeff;
			/*pdcCoeff.lh = abs(m_vecPopulation[i].vecWeights[0]);
		pdcCoeff.epcm = abs(m_vecPopulation[i].vecWeights[1]);
		pdcCoeff.brt = abs(m_vecPopulation[i].vecWeights[2]);
		pdcCoeff.bct = abs(m_vecPopulation[i].vecWeights[3]);
		pdcCoeff.bbh = abs(m_vecPopulation[i].vecWeights[4]);
		pdcCoeff.bw = abs(m_vecPopulation[i].vecWeights[5]);*/
		pdcCoeff.lh = m_vecPopulation[i].vecWeights[0];
		pdcCoeff.epcm = m_vecPopulation[i].vecWeights[1];
		pdcCoeff.brt = m_vecPopulation[i].vecWeights[2];
		pdcCoeff.bct = m_vecPopulation[i].vecWeights[3];
		pdcCoeff.bbh = m_vecPopulation[i].vecWeights[4];
		pdcCoeff.bw = m_vecPopulation[i].vecWeights[5];
			m_vecPDTetrisControllersPointer[i]->setPDTetrisControllerCoefficient(pdcCoeff);
			m_vecPDTetrisControllersPointer[i]->resetGame();
		}
		if (m_nCurrentGeneration >= 30)
			break;
	}
	

	for (vector<double>::iterator it = m_vecBestFitness.begin(); it != m_vecBestFitness.end(); it++)
	{
		double dFitness = *it;
		ss << "dFitness" << dFitness << endl;
	}

	
	string debug = ss.str();
	g_fileLoggerPDTCGA.Debug(debug);
	ss.clear();
	ss.str("");
}

