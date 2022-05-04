#pragma once
#include <windows.h>  

#include "defines.h"
#include "TetrisBlock.h"

class CTetrisDraw
{
private:
	HDC m_hdcDraw;
	RECT m_rectWhole;
	bool m_bArrTetris[nTetrisBoardHeight][nTetrisBoardWidth];
	int m_bArrTetrisColor[nTetrisBoardHeight][nTetrisBoardWidth];
	RECT m_rectGameArea;
	RECT m_rectScoreAndNextBlockArea;
	RECT m_rectGameHintArea;
	

public:
	CTetrisDraw(HDC hdcDraw, RECT rectWhole);
	~CTetrisDraw();
	void DrawGameArea(int nLow= 0, int nHigh= nTetrisBoardHeight -1);
	void DrawScoreAndNextBlockArea(long score, long lTetrisBlockNumUsed, CTetrisBlock* pNextTetrisBlock = 0);
	void DrawGameHintArea();
	HDC SetHDC(HDC hdcDraw);
	void SetArea(RECT rect);
	void SetTetrisArrayItem(int nHeight, int nWidth, bool bValue=true);
	void SetTetrisArrayItemColorIndex(int nHeight, int nWidth, int nColorIndex);
	bool GetTetrisArrayItem(int nHeight, int nWidth);
	int GetTetrisArrayItemColorIndex(int nHeight, int nWidth);
	void DrawCurrentTetrisBlock(CTetrisBlock* pTetrisBlock);
	void ClearCurrentTetrisBlock(CTetrisBlock* pTetrisBlock);
	int LevelNumErased(int nLevelStart, int nLevelEnd);
	void ClearLevel(int nLevel);
	int GetHighestNonEmptyLevel();
	
	void ClearTetrisArray();
};

