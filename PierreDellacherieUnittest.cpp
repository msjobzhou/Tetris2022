#include "PierreDellacherieUnittest.h"


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
	assert(pdtc.getBoardRowTransitions(pbArrTetrisBoardCopy, nHeight, nWidth) == 17);
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
	assert(nWells == 17);
	delete[]pbArrTetrisBoardCopy;
}