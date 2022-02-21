#pragma once
#include "TetrisController.h"

//�ο����²���ʵ�ֵ�Pierre Dellacherie�㷨�����˹���Ƶ��������˹��趨�ĸ�����Ȩ�ز���
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

