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
	RECT m_rectGameArea;
	RECT m_rectScoreArea;
	RECT m_rectNextBlockHintArea;
	

public:
	CTetrisDraw(HDC hdcDraw, RECT rectWhole);
	~CTetrisDraw();
	void DrawGameArea(int nLow= 0, int nHigh= nTetrisBoardHeight -1);
	void DrawScoreArea(long score);
	void DrawNextBlockHintArea();
	HDC SetHDC(HDC hdcDraw);
	void SetArea(RECT rect);
	void SetTetrisArrayItem(int nHeight, int nWidth, bool bValue=true);
	bool GetTetrisArrayItem(int nHeight, int nWidth);
	void DrawCurrentTetrisBlock(CTetrisBlock* pTetrisBlock);
	void ClearCurrentTetrisBlock(CTetrisBlock* pTetrisBlock);
	int LevelNumErased(int nLevelStart, int nLevelEnd);
	void ClearLevel(int nLevel);
	int GetHighestNonEmptyLevel();
	
};

