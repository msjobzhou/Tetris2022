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
		//ע�ⷽ�������������(0,0)��x���������ӣ�y���������ӣ� Tetrisboard������(0,0)��y����������
		//nBlockHeightTmp��ʾ��ǰ����ĵ�i�ж�Ӧ��tetrisBoard�ĸ߶ȣ�Ҳ����tetris board���У�
		int nBlockHeightTmp = stb.nPosY -i;
		//�������ǰ��������tetris board�����У���Ԫ��ռ�ݵ�λ�ø���
		int nOccupiedNum = 0;
		for (j = 0; j < nTetrisBoardWidth; j++)
		{
			if (true == pbArrTetrisBoardCopy[nBlockHeightTmp*nTetrisBoardWidth + j])
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
				pbArrTetrisBoardCopy[nBlockHeightTmp*nTetrisBoardWidth + stb.nPosX+j]
					= stb.pbBlock[i*stb.nBlockWidth + j];
			}
		}

		//�������󷽿�����tetris board�����У���Ԫ��ռ�ݵ�λ�ø���
		int nOccupiedNumAfterTetrisBlockDownTillItCannotMove = 0;
		for (j = 0; j < nTetrisBoardWidth; j++)
		{
			if (true == pbArrTetrisBoardCopy[nBlockHeightTmp*nTetrisBoardWidth + j])
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
	//����ÿһ�У��������¼�⣬������һ��Ϊtrue��element֮�����������֮�µ�ÿ��Ϊfalse��element������Ϊ1��Buried hole
	int nBuriedHoles = 0;
	for (int x = 0; x < nWidth; ++x)
	{
		bool bFirstOccupiedElementofThisColumn = false;
		for (int y = nHeight - 1; y >= 0; y--)
		{
			if (!bFirstOccupiedElementofThisColumn && (pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x] == true))
			{
				bFirstOccupiedElementofThisColumn = true;
			}
			if (bFirstOccupiedElementofThisColumn && (pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x] == false))
			{
				nBuriedHoles++;
			}
		}
	}
	return nBuriedHoles;
}
int CPierreDellacherieTetrisController::getBoardWells(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth)
{
	//����ÿһ�У��������¼�⣬�����ǰ��elementΪfalse��������������element�Ƿ�Ϊtrue��������board�߽�
	int nSumWellDepth = 0;
	for (int x = 0; x < nWidth; ++x)
	{
		int nWell = 0;
		for (int y = nHeight - 1; y >= 0; y--)
		{
			if (pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x] == false)
			{
				if ((x - 1 < 0) && (pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x +1] == true))
				{
					nWell++;
				}
				else if ((x + 1) >= nTetrisBoardWidth && (pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x - 1] == true))
				{
					nWell++;
				}
				else if ((pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x + 1] == true) && (pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x - 1] == true))
				{
					nWell++;
				}
				else
				{
					int nWellDepthFrom1to_nWell = ((1 + nWell)*nWell) / 2;
					nSumWellDepth += nWellDepthFrom1to_nWell; //����Ϊɶ������ο�����ע�����ᵽ�Ĳ���
					nWell = 0;
				}
			}
		}
	}
	return nSumWellDepth;
}