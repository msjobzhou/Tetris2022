#pragma once
#include "TetrisController.h"
#include<list>
using std::list;

typedef struct TetrisBlock
{
	int nBlockHeight, nBlockWidth;
	int nPosX;
	int nPosY;
	bool *pbBlock;//指向一维数组表示的block，其长度为nBlockHeight*nBlockWidth
}sTetrisBlock;


typedef struct Position
{
public:
	int nPosX;
	int nPosY;
public:
	Position()
	{
		nPosX = -1;
		nPosY = -1;
	}
	Position(const Position &pos)
	{
		nPosX = pos.nPosX;
		nPosY = pos.nPosY;
	}
	Position& operator =(const Position &pos)
	{
		nPosX = pos.nPosX;
		nPosY = pos.nPosY;

		return *this;
	}
}sPosition;

//参考如下博客实现的Pierre Dellacherie算法，纯人工设计的特征和人工设定的各特征权重参数
//https://blog.csdn.net/qq_41882147/article/details/80005763
class CPierreDellacherieTetrisController :
	public CTetrisController
{
public:
	CPierreDellacherieTetrisController(CTetrisDraw* pTetrisDraw, CTetrisBlock* pTetrisBlock = 0);
	~CPierreDellacherieTetrisController();
	void getArrTetrisBoardCopyFromCTetrisDraw(bool *pbArrTetrisBoardCopy);
	void getCurrentTetrisBlockCopy(sTetrisBlock& stb);

public:
	int getLandingHeight(sTetrisBlock& stb);
	int getErodedPieceCellsMetric(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb);
	int getBoardRowTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardColumnTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardBuriedHoles(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardWells(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	bool canTetrisBlockMovable(sTetrisBlock& stb, bool *pbArrTetrisBoardCopy, int x, int y);
	int evaluationFunction(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb);
	sPosition pickPositionWithHighestEvalutionScore(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb, int& nHighestEvalutionScoreRet);
	sPosition generateAICommandListForCurrentTetrisBlock(list<int>& cmdList);
	void RotateTetrisBlock(sTetrisBlock& stb);
	int GetHighestNonEmptyLevel(bool *pbArrTetrisBoardCopy);
	int LevelNumErased(bool *pbArrTetrisBoardCopy, int nLevelStart, int nLevelEnd);
};

