#pragma once
#include "TetrisController.h"

typedef struct TetrisBlock
{
	int nBlockHeight, nBlockWidth;
	int nPosX;
	int nPosY;
	bool *pbBlock;//指向一维数组表示的block，其长度为m_nBlockHeight*m_nBlockWidth
}sTetrisBlock;

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

private:
	int getLandingHeight(sTetrisBlock stb);
	int getErodedPieceCellsMetric(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock stb);
	int getBoardRowTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardColumnTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardBuriedHoles(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardWells(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);

};

