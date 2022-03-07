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
int CPierreDellacherieTetrisController::getLandingHeight(sTetrisBlock& stb)
{
	return stb.nPosY;
}
//����getErodedPieceCellsMetric��ʹ��stbռ�õ�λ�������pbArrTetrisBoardCopy
int CPierreDellacherieTetrisController::getErodedPieceCellsMetric(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb)
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
					nSumWellDepth += nWell;
				}
				else if ((x + 1) >= nTetrisBoardWidth && (pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x - 1] == true))
				{
					nWell++;
					nSumWellDepth += nWell;
				}
				else if ((pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x + 1] == true) && (pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x - 1] == true))
				{
					nWell++;
					nSumWellDepth += nWell;
				}
				else
				{
					nWell = 0;
				}
			}
		}
	}
	return nSumWellDepth;
}

bool CPierreDellacherieTetrisController::canTetrisBlockMovable(sTetrisBlock& stb, bool *pbArrTetrisBoardCopy, int x, int y)
{
	//�жϵ�ǰ�����Ƿ������ת�����ƶ���λ��(x,y),x,y��0��ʼ
	//���ж���ת�����ƶ��Ƿ�ᳬ��TetrisBoard��Ϸ��֮��
	//��ʹ����ת�����ƶ�֮��ķ���2ά�����CTetrisDraw.m_bArrTetris�е�Ԫ�ؽ����߼������
	bool *pbBlockArr = stb.pbBlock;
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			//�ƶ�������ת�������ұ߽���±߽磬���ﲻ�ж��Ƿ񳬳��ϱ߽磬��Ҫ�Ǹ�block�ճ���ʱ�ĳ�ʼλ�ÿ������ϱ߽����й�
			if ((x + j) < 0 || (x + j) >= nTetrisBoardWidth || (y - i) < 0)
				return false;
			//�ƶ�������ת֮��ķ����TetrisArray���κ�һ��λ�ö���ռ�õ�����£�����false
			if (true == (pbBlockArr[i*stb.nBlockWidth +j] && pbArrTetrisBoardCopy[(y - i)*nTetrisBoardWidth + x + j]))
				return false;
		}
	}
	return true;
}

int CPierreDellacherieTetrisController::evaluationFunction(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb)
{
	int lh = getLandingHeight(stb);
	int epcm = getErodedPieceCellsMetric(pbArrTetrisBoardCopy, nHeight, nWidth, stb);
	int brt = getBoardRowTransitions(pbArrTetrisBoardCopy, nHeight, nWidth);
	int bct = getBoardColumnTransitions(pbArrTetrisBoardCopy, nHeight, nWidth);
	int bbh = getBoardBuriedHoles(pbArrTetrisBoardCopy, nHeight, nWidth);
	int bw = getBoardWells(pbArrTetrisBoardCopy, nHeight, nWidth);

	int nScore = -lh + epcm - brt - bct - 4 * bbh - bw;

	return nScore;
}
sPosition CPierreDellacherieTetrisController::pickPositionWithHighestEvalutionScore(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb)
{
	//����ֻ����ֱ����·�����ߴ�ֱ�½�·��(����ͼ���͡���ָʾ�ķ���1,2,3)���������ƶ������б��Σ��궴�ȸ��ӳ���
	//   1����  ����3
	//   1������   ��3
	//   1��  ��   ��3
	//   1��  ��2  ��3
	//
	//����   ������  ����
	//������ ������������
	sPosition sp;
	sp.nPosX = -1;
	sp.nPosY = -1;

	int nTargetX, nTargetY;
	int nTetrisBlockOriginX = stb.nPosX;
	int nTetrisBlockOriginY = stb.nPosY;

	//����˹�����ڳ�Ŀ��λ���ƶ����������赲��ͣ���ڵ�λ��
	int nStoppedX, nStoppedY;

	int nTetrisBlockPreX = nTetrisBlockOriginX;
	int nTetrisBlockPreY = nTetrisBlockOriginY;

	int nHighestEvalutionScore = -99999999;
	//����·��1��·�߽�������
	for (nTargetX = nTetrisBlockOriginX-1; nTargetX >= 0; nTargetX--)
	{
		getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
		//�������ƶ���(nTargetX, nTetrisBlockOriginY)
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTetrisBlockOriginY))
		{
			//
			break;
		}
		//�Զ���˹���鿪ʼ��һ�������ƶ���ʱ�򣬽��д���һ��nTetrisBlockPreY
		nTetrisBlockPreY = nTetrisBlockOriginY;

		//�������ƶ���(nTargetX, 0)
		for (nTargetY = nTetrisBlockOriginY; nTargetY >= 0; nTargetY--)
		{
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
			{
				//ֻ�����²����ƶ���ʱ�򣬲ſ�����TetrisBlockֹͣ�ƶ�
				
				nStoppedX = nTetrisBlockPreX;
				nStoppedY = nTetrisBlockPreY;
				stb.nPosX = nStoppedX;
				stb.nPosY = nStoppedY;
				//����evaluationFunction֮ǰ��Ҫ��stb��λ�������ƶ���tetrisBlock���������ƶ���λ��
				int nScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

				if (nScore >= nHighestEvalutionScore)
				{
					nHighestEvalutionScore = nScore;
					sp.nPosX = stb.nPosX;
					sp.nPosY = stb.nPosY;
				}

				//��stb��������껹ԭ��ԭʼλ�ã��Թ��´�ʹ��
				stb.nPosX = nTetrisBlockOriginX;
				stb.nPosY = nTetrisBlockOriginY;
				break;
			}
				
			nTetrisBlockPreX = nTargetX;
			nTetrisBlockPreY = nTargetY;
		}
	}

	//����·��2��·�߽�������
	nTargetX = nTetrisBlockOriginX;
	nTetrisBlockPreX = nTetrisBlockOriginX;
	nTetrisBlockPreY = nTetrisBlockOriginY;

	getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
	//�������ƶ���(nTargetX, 0)
	for (nTargetY = nTetrisBlockOriginY; nTargetY >= 0; nTargetY--)
	{
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
		{
			//ֻ�����²����ƶ���ʱ�򣬲ſ�����TetrisBlockֹͣ�ƶ�
			nStoppedX = nTetrisBlockPreX;
			nStoppedY = nTetrisBlockPreY;
			stb.nPosX = nStoppedX;
			stb.nPosY = nStoppedY;
			//����evaluationFunction֮ǰ��Ҫ��stb��λ�������ƶ���tetrisBlock���������ƶ���λ��
			int nScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

			if (nScore >= nHighestEvalutionScore)
			{
				nHighestEvalutionScore = nScore;
				sp.nPosX = stb.nPosX;
				sp.nPosY = stb.nPosY;
			}

			//��stb��������껹ԭ��ԭʼλ�ã��Թ��´�ʹ��
			stb.nPosX = nTetrisBlockOriginX;
			stb.nPosY = nTetrisBlockOriginY;
			break;
		}

		nTetrisBlockPreX = nTargetX;
		nTetrisBlockPreY = nTargetY;
	}
	
	//����·��3��·�߽�������
	nTetrisBlockPreX = nTetrisBlockOriginX;
	nTetrisBlockPreY = nTetrisBlockOriginY;
	for (nTargetX = nTetrisBlockOriginX + 1; nTargetX < nTetrisBoardWidth; nTargetX++)
	{
		getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
		//�������ƶ���(nTargetX, nTetrisBlockOriginY)
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTetrisBlockOriginY))
		{
			//
			break;
		}
		//�Զ���˹���鿪ʼ��һ�������ƶ���ʱ�򣬽��д���һ��nTetrisBlockPreY
		nTetrisBlockPreY = nTetrisBlockOriginY;
		//�������ƶ���(nTargetX, 0)
		for (nTargetY = nTetrisBlockOriginY; nTargetY >= 0; nTargetY--)
		{
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
			{
				//ֻ�����²����ƶ���ʱ�򣬲ſ�����TetrisBlockֹͣ�ƶ�

				nStoppedX = nTetrisBlockPreX;
				nStoppedY = nTetrisBlockPreY;
				stb.nPosX = nStoppedX;
				stb.nPosY = nStoppedY;
				//����evaluationFunction֮ǰ��Ҫ��stb��λ�������ƶ���tetrisBlock���������ƶ���λ��
				int nScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

				if (nScore >= nHighestEvalutionScore)
				{
					nHighestEvalutionScore = nScore;
					sp.nPosX = stb.nPosX;
					sp.nPosY = stb.nPosY;
				}

				//��stb��������껹ԭ��ԭʼλ�ã��Թ��´�ʹ��
				stb.nPosX = nTetrisBlockOriginX;
				stb.nPosY = nTetrisBlockOriginY;
				break;
			}

			nTetrisBlockPreX = nTargetX;
			nTetrisBlockPreY = nTargetY;
		}
	}

	return sp;
}



bool CPierreDellacherieTetrisController::generateAICommandListForCurrentTetrisBlock(list<int>& cmdList)
{
	bool *pbArrTetrisBoardCopy = new bool[nTetrisBoardHeight*nTetrisBoardWidth];
	int nHeight = nTetrisBoardHeight;
	int nWidth = nTetrisBoardWidth;
	sTetrisBlock stb;
	CTetrisBlock* pTetrisBlock = getCurTetrisBlock();
	int nBlockHeight = pTetrisBlock->getBlockHeight();
	int nBlockWidth = pTetrisBlock->getBlockWidth();
	stb.pbBlock = new bool[nBlockHeight*nBlockWidth];

	getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
	getCurrentTetrisBlockCopy(stb);

	//��ÿ������˹�Ŀɱ���״����pickPositionWithHighestEvalutionScore���ã�ѡ���evaluationFunction�÷���ߵ�������ת+�����ƶ����

	delete[] stb.pbBlock;
	delete[] pbArrTetrisBoardCopy;

}

//����CTetrisBlock��prepareRotateʵ�ֵĹ���
void CPierreDellacherieTetrisController::RotateTetrisBlock(sTetrisBlock& stb)
{
	//����������е����òο�CTetrisBlock��prepareRotate����
	int nPreBlockHeight = stb.nBlockHeight;
	int nPreBlockWidth = stb.nBlockWidth;
	bool *pbBlockCopy;
	//��ת90��֮�󷽿�ĸ߶ȺͿ�Ȼ���
	stb.nBlockHeight = nPreBlockWidth;
	stb.nBlockWidth = nPreBlockHeight;
	
	//��stb.pbBlock����һ������
	pbBlockCopy = new bool[nPreBlockHeight*nPreBlockHeight];

	for (int m = 0; m < nPreBlockHeight; m++)
	{
		for (int n = 0; n < nPreBlockWidth; n++)
		{
			pbBlockCopy[m*nPreBlockWidth + n] = stb.pbBlock[m*nPreBlockWidth + n];
		}
	}
	
	//���������������ת90��
	for (int i = 0; i < nPreBlockHeight; i++)
	{
		for (int j = 0; j < nPreBlockWidth; j++)
		{
			stb.pbBlock[j*nPreBlockHeight + (nPreBlockHeight - i - 1)] = pbBlockCopy[i*nPreBlockWidth+j];
		}
	}

	stb.nPosX = stb.nPosX + int(nPreBlockWidth - nPreBlockHeight - 1) / 2 + 1;
	stb.nPosY = stb.nPosY + int(nPreBlockWidth - nPreBlockHeight + 1) / 2 - 1;

	delete[] pbBlockCopy;
}