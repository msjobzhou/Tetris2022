#pragma once
#include "TetrisController.h"

//参考如下博客实现的Pierre Dellacherie算法，纯人工设计的特征和人工设定的各特征权重参数
//https://blog.csdn.net/qq_41882147/article/details/80005763
class CPierreDellacherieTetrisController :
	public CTetrisController
{
public:
	CPierreDellacherieTetrisController(CTetrisDraw* pTetrisDraw, CTetrisBlock* pTetrisBlock = 0);
	~CPierreDellacherieTetrisController();

private:
	int getLandingHeight();
	int getErodedPieceCellsMetric();
	int getBoardRowTransitions();
	int getBoardColumnTransitions();
	int getBoardBuriedHoles();
	int getBoardWells();

};

