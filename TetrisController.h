#pragma once
#include "TetrisBlock.h"
#include "TetrisDraw.h"
#include "defines.h"

class CTetrisController
{
private:
	CTetrisDraw* m_pTetrisDraw;
	CTetrisBlock* m_pCurTetrisBlock;
	long m_lScore;

	bool commandLeft();
	bool commandRight();
	bool commandRotate();
	bool commandDown();

public:
	CTetrisController(CTetrisDraw* pTetrisDraw, CTetrisBlock* pTetrisBlock=0);
	~CTetrisController();
	void setCurTetrisBlock(CTetrisBlock* pCurTetrisBlock=0);
	bool executeCommand(int nCmd);
	CTetrisBlock* getCurTetrisBlock();
	CTetrisDraw* getTetrisDraw();
	bool isGameOver();
	long GetScore();
	void resetGame();

protected:
	bool canTetrisBlockMovable(CTetrisBlock* pBlock, int x, int y);
	bool canTetrisBlockMovable(bool **ppbBlockArr, int nHeight, int nWidth, int x, int y);
	void setBlockOccupiedTetrisArray(bool bVal);
	
};

