#include "PierreDellacherieTetrisController.h"
#include "assert.h"



CPierreDellacherieTetrisController::CPierreDellacherieTetrisController(CTetrisDraw* pTetrisDraw, CTetrisBlock* pTetrisBlock):
	CTetrisController(pTetrisDraw, pTetrisBlock)
{

}


CPierreDellacherieTetrisController::~CPierreDellacherieTetrisController()
{
	CTetrisController::~CTetrisController();
}

void CPierreDellacherieTetrisController::getArrTetrisBoardCopyFromCTetrisDraw(bool *pbArrTetrisBoardCopy)
{
	CTetrisDraw* pTetrisDraw = getTetrisDraw();
	for (int y = 0; y < nTetrisBoardHeight; ++y)
	{
		for (int x = 0; x < nTetrisBoardWidth; ++x)
		{
			pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x] = pTetrisDraw->GetTetrisArrayItem(y, x);
		}
	}
}
void CPierreDellacherieTetrisController::getCurrentTetrisBlockCopy(sTetrisBlock& stb)
{
	CTetrisBlock* pTetrisBlock = getCurTetrisBlock();
	stb.nBlockHeight = pTetrisBlock->getBlockHeight();
	stb.nBlockWidth = pTetrisBlock->getBlockWidth();
	stb.nPosX = pTetrisBlock->getBlockPosX();
	stb.nPosY = pTetrisBlock->getBlockPosY();
	//��stb��������и�ֵ
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth+j]= pTetrisBlock->getBlockValue(i,j);
		}
	}
}
//���¼�����������Ҫ�ڵ�ǰTetrisBlock�䶨�����ƶ�֮����ִ��setBlockOccupiedTetrisArray(true)֮����ܵ���
//����
int CPierreDellacherieTetrisController::getLandingHeight(sTetrisBlock stb)
{
	return stb.nPosY;
}
//����getErodedPieceCellsMetric��ʹ��stbռ�õ�λ�������pbArrTetrisBoardCopy
int CPierreDellacherieTetrisController::getErodedPieceCellsMetric(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock stb)
{
	int nLevelErasedAfterTetrisBlockDownTillItCannotMove = 0;
	int nBlockElementContributeToLevelErased = 0;
	//�õ�ǰ����˹�������ڵ�λ�����pbArrTetrisBoardCopy
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		int j;
		//nBlockHeightTmp��ʾ��ǰ�������λ�õ��·��߶ȣ�Ҳ����tetris board���У�
		int nBlockHeightTmp = stb.nPosY - stb.nBlockHeight + 1;
		//�������ǰ��������tetris board�����У���Ԫ��ռ�ݵ�λ�ø���
		int nOccupiedNum = 0;
		for (j = 0; j < nTetrisBoardWidth; j++)
		{
			if (true == pbArrTetrisBoardCopy[(nBlockHeightTmp + i)*nTetrisBoardWidth + j])
			{
				nOccupiedNum++;
			}
		}
		assert(nOccupiedNum < nTetrisBoardWidth);
		//�������pbArrTetrisBoardCopy
		for (j = 0; j < stb.nBlockWidth; j++)
		{
			if(true == stb.pbBlock[i*stb.nBlockWidth + j])
			{
				pbArrTetrisBoardCopy[(nBlockHeightTmp + i)*nTetrisBoardWidth + stb.nPosX+j]
					= stb.pbBlock[i*stb.nBlockWidth + j];
			}
		}

		//�������󷽿�����tetris board�����У���Ԫ��ռ�ݵ�λ�ø���
		int nOccupiedNumAfterTetrisBlockDownTillItCannotMove = 0;
		for (j = 0; j < nTetrisBoardWidth; j++)
		{
			if (true == pbArrTetrisBoardCopy[(nBlockHeightTmp + i)*nTetrisBoardWidth + j])
			{
				nOccupiedNumAfterTetrisBlockDownTillItCannotMove++;
			}
		}
		//�����ǰ����ÿ��λ�ö���ռ���Ļ�������Ҫ��������������������Լ��������������׵ķ���element��
		//��ν��elementָ�������tetris block����СԪ�أ����ֱ���ͷ�����������е�ÿ��������һ��element
		if (nTetrisBoardWidth == nOccupiedNumAfterTetrisBlockDownTillItCannotMove)
		{
			nLevelErasedAfterTetrisBlockDownTillItCannotMove++;
			nBlockElementContributeToLevelErased+=(nOccupiedNumAfterTetrisBlockDownTillItCannotMove- nOccupiedNum);
		}
		
	}
	//����PierreDellacherie�������������ֵΪnLevelErasedAfterTetrisBlockDownTillItCannotMove*nBlockElementContributeToLevelErased
	return nLevelErasedAfterTetrisBlockDownTillItCannotMove*nBlockElementContributeToLevelErased;
}
int CPierreDellacherieTetrisController::getBoardRowTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth)
{
	int nTransitions = 0;
	for (int y = 0; y < nHeight; ++y)
	{
		for (int x = 0; x < nWidth - 1; ++x)
		{
			//����transition��������������£�����������������жϣ�
			//1��pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x]Ϊtrue��pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x+1]Ϊfalse
			//2��pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x]Ϊfalse��pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x+1]Ϊtrue
			if ((pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x] ^ pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x + 1]) == true)
			{
				nTransitions++;
			}
		}
	}

	return nTransitions;
}
int CPierreDellacherieTetrisController::getBoardColumnTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth)
{
	//�����߼��ο�getBoardRowTransitions
	int nTransitions = 0;
	for (int x = 0; x < nWidth; ++x)
	{
		for (int y = 0; y < nHeight - 1; ++y)
		{
			if ((pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x] ^ pbArrTetrisBoardCopy[(y + 1)*nTetrisBoardWidth + x]) == true)
			{
				nTransitions++;
			}
		}
	}

	return nTransitions;
}
int CPierreDellacherieTetrisController::getBoardBuriedHoles(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth)
{

	return 0;
}
int CPierreDellacherieTetrisController::getBoardWells(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth)
{

	return 0;
}