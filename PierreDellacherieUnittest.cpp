#include "PierreDellacherieUnittest.h"
#include <list>

using namespace std;

PierreDellacherieUnittest::PierreDellacherieUnittest()
{
}


PierreDellacherieUnittest::~PierreDellacherieUnittest()
{
}



void PierreDellacherieUnittest::addTestFunc(FuncPointer fp)
{
	if(fp!=0)
		m_testFuncList.push_back(fp);
}
void PierreDellacherieUnittest::runTest()
{
	list<FuncPointer>::iterator it = m_testFuncList.begin();
	while (it != m_testFuncList.end())
	{
		FuncPointer fp = *it;
		(*fp)();
		it++;
	}
}

void test_getErodedPieceCellsMetric()
{
	int nHeight = 3; 
	int nWidth = 10;
	bool *pbArrTetrisBoardCopy = new bool[nHeight*nWidth];
	
	//测试如下形状落入到TetrisBoard中的得分
	//    ■
	//    ■■
	//      ■
	sTetrisBlock stb;
	stb.nBlockHeight = 3;
	stb.nBlockWidth = 2;
	stb.nPosX = 2;
	stb.nPosY = 2;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	//这里要注意的是方块的左上角坐标是(0,0)，x轴方向往右逐渐增加，y轴方向往下逐渐增加
	stb.pbBlock[0*stb.nBlockWidth+0]=true;
	stb.pbBlock[0*stb.nBlockWidth+1]=false;
	stb.pbBlock[1 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 1] = true;
	stb.pbBlock[2 * stb.nBlockWidth + 0] = false;
	stb.pbBlock[2 * stb.nBlockWidth + 1] = true;
	//
	//■■   ■■■■■■
	//■■■ ■■■■■■

	//这里要注意的是TetrisBoard的左下角坐标是(0,0)，x轴方向往右逐渐增加，y轴方向往上逐渐增加

	//先把前两行都赋值true，然后将第一行第4列、第2行第3、4列都设为false
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			pbArrTetrisBoardCopy[i*nWidth + j] = true;
		}
	}
	pbArrTetrisBoardCopy[0*nWidth + 3] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 2] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 3] = false;

	//再将第3行设置成false

	for (int j1 = 0; j1 < nWidth; j1++)
	{
		pbArrTetrisBoardCopy[2*nWidth + j1] = false;
	}
	CPierreDellacherieTetrisController pdtc(0);
	assert(pdtc.getErodedPieceCellsMetric(pbArrTetrisBoardCopy, nHeight, nWidth, stb) == 6);
	delete []pbArrTetrisBoardCopy;
	delete []stb.pbBlock;
}
void test_getBoardRowTransitions()
{
	int nHeight = 3;
	int nWidth = 10;
	bool *pbArrTetrisBoardCopy = new bool[nHeight*nWidth];
	//    ■■ ■■■
	//■■   ■  ■  ■
	//■  ■ ■■  ■■■
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			pbArrTetrisBoardCopy[i*nWidth + j] = true;
		}
	}
	pbArrTetrisBoardCopy[0 * nWidth + 1] = false;
	pbArrTetrisBoardCopy[0 * nWidth + 3] = false;
	pbArrTetrisBoardCopy[0 * nWidth + 6] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 2] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 3] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 5] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 7] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 9] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 0] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 1] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 4] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 8] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 9] = false;

	CPierreDellacherieTetrisController pdtc(0);
	int nTransition = pdtc.getBoardRowTransitions(pbArrTetrisBoardCopy, nHeight, nWidth);
	assert(nTransition == 20);
	delete[]pbArrTetrisBoardCopy;
}
void test_getBoardColumnTransitions()
{
	int nHeight = 3;
	int nWidth = 10;
	bool *pbArrTetrisBoardCopy = new bool[nHeight*nWidth];
	//    ■■ ■■■
	//■■   ■  ■  ■
	//■  ■ ■■  ■■■
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			pbArrTetrisBoardCopy[i*nWidth + j] = true;
		}
	}
	pbArrTetrisBoardCopy[0 * nWidth + 1] = false;
	pbArrTetrisBoardCopy[0 * nWidth + 3] = false;
	pbArrTetrisBoardCopy[0 * nWidth + 6] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 2] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 3] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 5] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 7] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 9] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 0] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 1] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 4] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 8] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 9] = false;

	CPierreDellacherieTetrisController pdtc(0);
	assert(pdtc.getBoardColumnTransitions(pbArrTetrisBoardCopy, nHeight, nWidth) == 14);
	delete[]pbArrTetrisBoardCopy;
}
void test_getBoardBuriedHoles()
{
	int nHeight = 3;
	int nWidth = 10;
	bool *pbArrTetrisBoardCopy = new bool[nHeight*nWidth];
	//    ■■ ■■■
	//■■   ■  ■  ■
	//■  ■ ■■  ■■■
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			pbArrTetrisBoardCopy[i*nWidth + j] = true;
		}
	}
	pbArrTetrisBoardCopy[0 * nWidth + 1] = false;
	pbArrTetrisBoardCopy[0 * nWidth + 3] = false;
	pbArrTetrisBoardCopy[0 * nWidth + 6] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 2] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 3] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 5] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 7] = false;
	pbArrTetrisBoardCopy[1 * nWidth + 9] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 0] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 1] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 4] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 8] = false;
	pbArrTetrisBoardCopy[2 * nWidth + 9] = false;

	CPierreDellacherieTetrisController pdtc(0);
	assert(pdtc.getBoardBuriedHoles(pbArrTetrisBoardCopy, nHeight, nWidth) == 7);
	delete[]pbArrTetrisBoardCopy;
}
void test_getBoardWells()
{
	int nHeight = 4;
	int nWidth = 10;
	bool *pbArrTetrisBoardCopy = new bool[nHeight*nWidth];
	//    ■■ ■■■■
	//  ■   ■  ■  ■■
	//■  ■ ■■  
	//■  ■ ■  ■
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			pbArrTetrisBoardCopy[i*nWidth + j] = false;
		}
	}
	pbArrTetrisBoardCopy[0 * nWidth + 0] = true;
	pbArrTetrisBoardCopy[0 * nWidth + 2] = true;
	pbArrTetrisBoardCopy[0 * nWidth + 4] = true;
	pbArrTetrisBoardCopy[0 * nWidth + 6] = true;
	pbArrTetrisBoardCopy[1 * nWidth + 0] = true;
	pbArrTetrisBoardCopy[1 * nWidth + 2] = true;
	pbArrTetrisBoardCopy[1 * nWidth + 4] = true;
	pbArrTetrisBoardCopy[1 * nWidth + 5] = true;
	pbArrTetrisBoardCopy[2 * nWidth + 1] = true;
	pbArrTetrisBoardCopy[2 * nWidth + 4] = true;
	pbArrTetrisBoardCopy[2 * nWidth + 6] = true;
	pbArrTetrisBoardCopy[2 * nWidth + 8] = true;
	pbArrTetrisBoardCopy[2 * nWidth + 9] = true;
	pbArrTetrisBoardCopy[3 * nWidth + 2] = true;
	pbArrTetrisBoardCopy[3 * nWidth + 3] = true;
	pbArrTetrisBoardCopy[3 * nWidth + 5] = true;
	pbArrTetrisBoardCopy[3 * nWidth + 6] = true;
	pbArrTetrisBoardCopy[3 * nWidth + 7] = true;
	pbArrTetrisBoardCopy[3 * nWidth + 8] = true;

	CPierreDellacherieTetrisController pdtc(0);
	int nWells = pdtc.getBoardWells(pbArrTetrisBoardCopy, nHeight, nWidth);
	assert(nWells == 13);
	delete[]pbArrTetrisBoardCopy;
}

void test_pickPositionWithHighestEvalutionScore()
{
	//针对如下tetris board布局测试各种block形状
	//
	//    ■   ■
	//  ■■ ■■■  ■
	//■■■ ■■■■■

	//先构造如上的board数据
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 100;
	rect.bottom = 200;
	//CTetrisDraw构造函数的两个参数在此测试函数中并不会用到，所以随便设置一下
	CTetrisDraw* pTetrisDraw = new CTetrisDraw(0, rect);
	CPierreDellacherieTetrisController* pTetrisController = new CPierreDellacherieTetrisController(pTetrisDraw);
	pTetrisDraw->SetTetrisArrayItem(0, 0);
	pTetrisDraw->SetTetrisArrayItem(0, 1);
	pTetrisDraw->SetTetrisArrayItem(0, 2);
	pTetrisDraw->SetTetrisArrayItem(0, 4);
	pTetrisDraw->SetTetrisArrayItem(0, 5);
	pTetrisDraw->SetTetrisArrayItem(0, 6);
	pTetrisDraw->SetTetrisArrayItem(0, 7);
	pTetrisDraw->SetTetrisArrayItem(0, 8);
	
	pTetrisDraw->SetTetrisArrayItem(1, 1);
	pTetrisDraw->SetTetrisArrayItem(1, 2);
	pTetrisDraw->SetTetrisArrayItem(1, 4);
	pTetrisDraw->SetTetrisArrayItem(1, 5);
	pTetrisDraw->SetTetrisArrayItem(1, 6);
	pTetrisDraw->SetTetrisArrayItem(1, 8);
	pTetrisDraw->SetTetrisArrayItem(2, 2);
	pTetrisDraw->SetTetrisArrayItem(2, 5);

	bool *pbArrTetrisBoardCopy = new bool[nTetrisBoardHeight*nTetrisBoardWidth];
	int nHeight = nTetrisBoardHeight;
	int nWidth = nTetrisBoardWidth;
	sTetrisBlock stb;
	//■
	//■■
	//  ■
	stb.nPosX = 4;
	stb.nPosY = 21;
	stb.nBlockHeight = 3;
	stb.nBlockWidth = 2;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth + j] = false;
		}
	}
	stb.pbBlock[0 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 1] = true;
	stb.pbBlock[2 * stb.nBlockWidth + 1] = true;

	sPosition spTmp;
	int nHighestEvalutionScoreRet;
	spTmp = pTetrisController->pickPositionWithHighestEvalutionScore(pbArrTetrisBoardCopy, nHeight, nWidth, stb, nHighestEvalutionScoreRet);
	assert(spTmp.nPosX == 6);
	assert(spTmp.nPosY == 3);
	delete[] stb.pbBlock;


	//  ■
	//■■
	//■
	stb.nPosX = 5;
	stb.nPosY = 21;
	stb.nBlockHeight = 3;
	stb.nBlockWidth = 2;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth + j] = false;
		}
	}
	stb.pbBlock[0 * stb.nBlockWidth + 1] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 1] = true;
	stb.pbBlock[2 * stb.nBlockWidth + 0] = true;

	spTmp = pTetrisController->pickPositionWithHighestEvalutionScore(pbArrTetrisBoardCopy, nHeight, nWidth, stb, nHighestEvalutionScoreRet);
	assert(spTmp.nPosX == 0);
	assert(spTmp.nPosY == 3);
	delete[] stb.pbBlock;

	//■■
	//  ■
	//  ■
	stb.nPosX = 4;
	stb.nPosY = 21;
	stb.nBlockHeight = 3;
	stb.nBlockWidth = 2;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth + j] = false;
		}
	}
	stb.pbBlock[0 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[0 * stb.nBlockWidth + 1] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 1] = true;
	stb.pbBlock[2 * stb.nBlockWidth + 1] = true;

	spTmp = pTetrisController->pickPositionWithHighestEvalutionScore(pbArrTetrisBoardCopy, nHeight, nWidth, stb, nHighestEvalutionScoreRet);
	assert(spTmp.nPosX == 8);
	assert(spTmp.nPosY == 2);
	delete[] stb.pbBlock;


	//■■
	//■
	//■
	stb.nPosX = 4;
	stb.nPosY = 21;
	stb.nBlockHeight = 3;
	stb.nBlockWidth = 2;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth + j] = false;
		}
	}
	stb.pbBlock[0 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[0 * stb.nBlockWidth + 1] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[2 * stb.nBlockWidth + 0] = true;

	spTmp = pTetrisController->pickPositionWithHighestEvalutionScore(pbArrTetrisBoardCopy, nHeight, nWidth, stb, nHighestEvalutionScoreRet);
	assert(spTmp.nPosX == 3);
	assert(spTmp.nPosY == 2);
	delete[] stb.pbBlock;

	//■
	//■
	//■
	//■
	stb.nPosX = 4;
	stb.nPosY = 22;
	stb.nBlockHeight = 4;
	stb.nBlockWidth = 1;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth + j] = false;
		}
	}
	stb.pbBlock[0 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[2 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[3 * stb.nBlockWidth + 0] = true;

	spTmp = pTetrisController->pickPositionWithHighestEvalutionScore(pbArrTetrisBoardCopy, nHeight, nWidth, stb, nHighestEvalutionScoreRet);
	assert(spTmp.nPosX == 9);
	assert(spTmp.nPosY == 3);
	delete[] stb.pbBlock;


	//■■■
	//  ■

	stb.nPosX = 4;
	stb.nPosY = 20;
	stb.nBlockHeight = 2;
	stb.nBlockWidth = 3;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth + j] = false;
		}
	}
	stb.pbBlock[0 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[0 * stb.nBlockWidth + 1] = true;
	stb.pbBlock[0 * stb.nBlockWidth + 2] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 1] = true;

	spTmp = pTetrisController->pickPositionWithHighestEvalutionScore(pbArrTetrisBoardCopy, nHeight, nWidth, stb, nHighestEvalutionScoreRet);
	assert(spTmp.nPosX == 6);
	assert(spTmp.nPosY == 2);
	delete[] stb.pbBlock;

	delete[] pbArrTetrisBoardCopy;
	delete pTetrisDraw;
	delete pTetrisController;
}

void test_RotateTetrisBlock()
{
	//    ■
	//    ■
	//    ■
	//    ■
	sTetrisBlock stb;
	stb.nBlockHeight = 4;
	stb.nBlockWidth = 1;
	stb.nPosX = 4;
	stb.nPosY = 19;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	//这里要注意的是方块的左上角坐标是(0,0)，x轴方向往右逐渐增加，y轴方向往下逐渐增加
	stb.pbBlock[0 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[2 * stb.nBlockWidth + 0] = false;
	stb.pbBlock[3 * stb.nBlockWidth + 0] = true;

	CPierreDellacherieTetrisController pdtc(0);
	pdtc.RotateTetrisBlock(stb);
}

void test_generateAICommandListForCurrentTetrisBlock()
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 100;
	rect.bottom = 200;
	//CTetrisDraw构造函数的两个参数在此测试函数中并不会用到，所以随便设置一下
	CTetrisDraw* pTetrisDraw = new CTetrisDraw(0, rect);
	CPierreDellacherieTetrisController* pTetrisController = new CPierreDellacherieTetrisController(pTetrisDraw);
	//生成一个如下形状的block
	//    ■
	//    ■
	//    ■
	//    ■
	CTetrisBlock* pTetrisBlock = new CTetrisBlock(6, 1);
	pTetrisController->setCurTetrisBlock(pTetrisBlock);

	sPosition sp;
	list<int> AICmdList;
	sp = pTetrisController->generateAICommandListForCurrentTetrisBlock(AICmdList);

	//此时最优的block放置位置的第一步是要旋转，对应的数字是4
	assert(AICmdList.front() == 4);

	//pTetrisBlock会由CPierreDellacherieTetrisController的析构函数来释放，这里不用显式进行释放了
	//delete pTetrisBlock;
	delete pTetrisController;
	delete pTetrisDraw;
}
void test_evaluationFunction()
{
	//先构造如上的board数据
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = 100;
	rect.bottom = 200;
	//CTetrisDraw构造函数的两个参数在此测试函数中并不会用到，所以随便设置一下
	CTetrisDraw* pTetrisDraw = new CTetrisDraw(0, rect);
	CPierreDellacherieTetrisController* pTetrisController = new CPierreDellacherieTetrisController(pTetrisDraw);
	bool *pbArrTetrisBoardCopy = new bool[nTetrisBoardHeight*nTetrisBoardWidth];
	int nHeight = nTetrisBoardHeight;
	int nWidth = nTetrisBoardWidth;

	pTetrisController->getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
	sTetrisBlock stb;
	//■
	//■
	//■
	//■
	stb.nPosX = 9;
	stb.nPosY = 3;
	stb.nBlockHeight = 4;
	stb.nBlockWidth = 1;
	stb.pbBlock = new bool[stb.nBlockHeight*stb.nBlockWidth];
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth + j] = false;
		}
	}
	stb.pbBlock[0 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[1 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[2 * stb.nBlockWidth + 0] = true;
	stb.pbBlock[3 * stb.nBlockWidth + 0] = true;

	int nScore = pTetrisController->evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);
	
	delete[] stb.pbBlock;
	delete[] pbArrTetrisBoardCopy;
}