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

	
	srand((unsigned int)time(NULL));  // 产生随机种子
	int nType;
	//产生用于训练的俄罗斯方块数量
	for (int i = 0; i < m_nTetrisBlockNum; i++)
	{
		nType = rand() % 7 + 1;
		m_vecTetrisBlockType.push_back(nType);
	}
	

	//通过遗传算法随机初始化个PDController的权重参数
	m_vecPopulation = m_pGA->GetChromos();

	for (int i = 0; i < m_nPopulationSize; i++)
	{
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.right = 100;
		rect.bottom = 200;
		//rect没有意义，遗传算法里面并不会使用，CTetrisDraw第一个参数HDC设置成0，表示遗传算法计算过程不会进行实际绘图
		CTetrisDraw* pTetrisDraw = new CTetrisDraw(0, rect);
		CPierreDellacherieTetrisController* pdc1 = new CPierreDellacherieTetrisController(pTetrisDraw);
		m_vecPDTetrisControllersPointer.push_back(pdc1);
	}
	//将遗传算法得到的权重参数赋值给对应的CPierreDellacherieTetrisController
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
//针对当前CPierreDellacherieTetrisController模拟运行俄罗斯方块游戏，并返回其对应的fitness值
void CPDTetrisControllerUsingGeneticAlgorithm::RunTetrisGameSimulation(CPierreDellacherieTetrisController &pdController, double &dFitness)
{
	dFitness = 0;
	//用于训练的俄罗斯方块序列为空时，不做任何动作
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
						//这里待要加入不同原因值的判断
						dFitness = nTetrisBlockUsed;
						return;
					}
				}
			}
		}
		//AICmdList为空时或者所有的命令都执行完时,直接移动到当前方块垂直能下落的最低位置
		bool bMoveDown = true;
		while (bMoveDown)
		{
			bMoveDown = pdController.executeCommand(3);
		}
		if (true == pdController.isGameOver())
		{
			//这里待要加入不同原因值的判断
			dFitness = nTetrisBlockUsed;
			return;
		}
	}
	assert(nTetrisBlockUsed<=m_nTetrisBlockNum);
	//所有用于训练的俄罗斯方块都使用完了，游戏依然还活着
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

		//将遗传算法得到的权重参数赋值给对应的CPierreDellacherieTetrisController，且重置其Controller对应的游戏
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

