#pragma once
#include "TetrisController.h"
#include<list>
using std::list;

typedef struct TetrisBlock
{
	int nBlockHeight, nBlockWidth;
	int nPosX;
	int nPosY;
	bool *pbBlock;//ָ��һά�����ʾ��block���䳤��ΪnBlockHeight*nBlockWidth
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


typedef struct PDTetrisControllerCoefficient
{
public:
	//��ϵ������ο�evaluationFunction����
	double lh;
	double epcm;
	double brt;
	double bct;
	double bbh;
	double bw;
public:
	PDTetrisControllerCoefficient()
	{
		lh = 0.0;
		epcm = 0.0;
		brt = 0.0;
		bct = 0.0;
		bbh = 0.0;
		bw = 0.0;
	}
	PDTetrisControllerCoefficient(const PDTetrisControllerCoefficient & eff)
	{
		lh = eff.lh;
		epcm = eff.epcm;
		brt = eff.brt;
		bct = eff.bct;
		bbh = eff.bbh;
		bw = eff.bw;
	}

	PDTetrisControllerCoefficient& operator =(const PDTetrisControllerCoefficient &eff)
	{
		lh = eff.lh;
		epcm = eff.epcm;
		brt = eff.brt;
		bct = eff.bct;
		bbh = eff.bbh;
		bw = eff.bw;

		return *this;
	}
}sPDTetrisControllerCoefficient;


//�ο����²���ʵ�ֵ�Pierre Dellacherie�㷨�����˹���Ƶ��������˹��趨�ĸ�����Ȩ�ز���
//https://blog.csdn.net/qq_41882147/article/details/80005763
class CPierreDellacherieTetrisController :
	public CTetrisController
{
private:
	sPDTetrisControllerCoefficient m_PDCoff;

public:
	CPierreDellacherieTetrisController(CTetrisDraw* pTetrisDraw, CTetrisBlock* pTetrisBlock = 0);
	~CPierreDellacherieTetrisController();
	void getArrTetrisBoardCopyFromCTetrisDraw(bool *pbArrTetrisBoardCopy);
	void getCurrentTetrisBlockCopy(sTetrisBlock& stb);
	void setPDTetrisControllerCoefficient(sPDTetrisControllerCoefficient& coff);

public:
	int getLandingHeight(sTetrisBlock& stb);
	int getErodedPieceCellsMetric(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb);
	int getBoardRowTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardColumnTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardBuriedHoles(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	int getBoardWells(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth);
	bool canTetrisBlockMovable(sTetrisBlock& stb, bool *pbArrTetrisBoardCopy, int x, int y);
	float evaluationFunction(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb);
	sPosition pickPositionWithHighestEvalutionScore(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb, float& fHighestEvalutionScoreRet);
	sPosition generateAICommandListForCurrentTetrisBlock(list<int>& cmdList);
	void RotateTetrisBlock(sTetrisBlock& stb);
	int GetHighestNonEmptyLevel(bool *pbArrTetrisBoardCopy, int nArrayHeight, int nArrayWidth);
	int LevelNumErased(bool *pbArrTetrisBoardCopy, int nArrayHeight, int nArrayWidth, int nLevelStart, int nLevelEnd);
};

