#include "PierreDellacherieTetrisController.h"
#include "assert.h"

#include "FileLogger.h"
#include <sstream>

#include <exception>

#include "ArrayIndexRangeCheck.h"

using namespace std;

FileLogger g_fileLogger("tetrisBlockLog.txt", error);


CPierreDellacherieTetrisController::CPierreDellacherieTetrisController(CTetrisDraw* pTetrisDraw, CTetrisBlock* pTetrisBlock):
	CTetrisController(pTetrisDraw, pTetrisBlock)
{
	m_PDCoff.lh = -1.0f;
	m_PDCoff.epcm = 1.0f;
	m_PDCoff.brt = -1.0f;
	m_PDCoff.bct = -1.0f;
	m_PDCoff.bbh = -4.0f;
	m_PDCoff.bw = -1.0f;
}


CPierreDellacherieTetrisController::~CPierreDellacherieTetrisController()
{
	CTetrisController::~CTetrisController();
}

void CPierreDellacherieTetrisController::getArrTetrisBoardCopyFromCTetrisDraw(bool *pbArrTetrisBoardCopy)
{
	CTetrisDraw* pTetrisDraw = getTetrisDraw();
	for (int y = 0; y < nTetrisBoardHeight; y++)
	{
		for (int x = 0; x < nTetrisBoardWidth; x++)
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
	//stringstream ss;
	//ss << "getErodedPieceCellsMetric para stb.nPosX " << stb.nPosX << " stb.nPosY" << stb.nPosY << endl;

	//string debug = ss.str();
	//g_fileLogger.Debug(debug);

	ArrayIndexRangeCheck arrIndxRngCheck(nHeight, nWidth);

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
			arrIndxRngCheck.IndexRangeCheck(nBlockHeightTmp, j);
			if (true == pbArrTetrisBoardCopy[nBlockHeightTmp*nTetrisBoardWidth + j])
			{
				nOccupiedNum++;
			}
		}
		assert(nOccupiedNum < nTetrisBoardWidth);
		//�������pbArrTetrisBoardCopy
		for (j = 0; j < stb.nBlockWidth; j++)
		{
			arrIndxRngCheck.IndexRangeCheck(nBlockHeightTmp, stb.nPosX + j);
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
			arrIndxRngCheck.IndexRangeCheck(nBlockHeightTmp, j);
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
		//��������Ż�һ�£����һ�ж���ռ���Ļ�����ǰû�ж�pbArrTetrisBoardCopy���������������Ϊ�����getBoardRowTransitions
		//�Ⱥ�������Ҫʹ�ñ�����ռ�ݺ��pbArrTetrisBoardCopy���м��㣬�������ķ��÷�ʽ���������γɡ��ն������ڻ��getBoardBuriedHoles
		//�Ⱥ�����ɱȽϴ�ĸ�ֵ����ʵ���ϱ�ռ����һ��ᱻ���������ˡ��ն���������󲢲������ˡ�
		LevelNumErased(pbArrTetrisBoardCopy, nHeight, nWidth, stb.nPosY - stb.nBlockHeight + 1, stb.nPosY);
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
			//1��pbArrTetrisBoardCopy[y*nWidth + x]Ϊtrue��pbArrTetrisBoardCopy[y*nWidth + x+1]Ϊfalse
			//2��pbArrTetrisBoardCopy[y*nWidth + x]Ϊfalse��pbArrTetrisBoardCopy[y*nWidth + x+1]Ϊtrue
			if ((pbArrTetrisBoardCopy[y*nWidth + x] ^ pbArrTetrisBoardCopy[y*nWidth + x + 1]) == true)
			{
				nTransitions++;
			}
		}
		//tetris board�ı߽�������ʯ�鱻ռ��
		//�ȿ���߽�ͱ��е�һ��ʯ������
		//if (false == pbArrTetrisBoardCopy[y*nWidth + 0])
		//	nTransitions++;
		//�ٿ��������һ��ʯ����ұ߽�����
		//if (false == pbArrTetrisBoardCopy[y*nWidth + nWidth - 1])
		//	nTransitions++;
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
			if ((pbArrTetrisBoardCopy[y*nWidth + x] ^ pbArrTetrisBoardCopy[(y + 1)*nWidth + x]) == true)
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
			
			//if ((y - i)*nTetrisBoardWidth + x + j >= nTetrisBoardHeight*nTetrisBoardWidth)
			//{
			//	g_fileLogger.Error("pbArrTetrisBoardCopy out of range in function canTetrisBlockMovable");
			//}
			//�ƶ�������ת֮��ķ����TetrisArray���κ�һ��λ�ö���ռ�õ�����£�����false
			//AIģʽ�£����ܶ��ڸճ�����block������ת�����ƶ���������ʱy��ֵ�ǳ���nTetrisBoardHeight-1��
			//�����ٷ�������ǰ�Ӹ��жϣ���ֹ����pbArrTetrisBoardCopy����ı߽磬������delete []�쳣
			//ͬʱ���ڴ���Ļ�Ϸ��ճ�����tetris block����nTetrisBoardHeight�м����ϵģ��϶������г�ͻ�������������ж���
			if ((y - i) < nTetrisBoardHeight)
			{
				if (true == (pbBlockArr[i*stb.nBlockWidth + j] && pbArrTetrisBoardCopy[(y - i)*nTetrisBoardWidth + x + j]))
				{
					/*stringstream ss;
					string debug= "pbBlockArr ";
					string tmp;
					ss << "pbBlockArr " << i << " " << j << " " << pbBlockArr[i*stb.nBlockWidth + j] << endl;
					ss << "pbArrTetrisBoardCopy " << (y - i) << " " << (x + j) << " " << pbArrTetrisBoardCopy[(y - i)*nTetrisBoardWidth + x + j] << endl;
					debug = ss.str();
					g_fileLogger.Debug(debug);*/
					return false;
				}
			}	
		}
	}
	return true;
}

float CPierreDellacherieTetrisController::evaluationFunction(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb)
{
	int lh = getLandingHeight(stb);
	int epcm = getErodedPieceCellsMetric(pbArrTetrisBoardCopy, nHeight, nWidth, stb);
	int brt = getBoardRowTransitions(pbArrTetrisBoardCopy, nHeight, nWidth);
	int bct = getBoardColumnTransitions(pbArrTetrisBoardCopy, nHeight, nWidth);
	int bbh = getBoardBuriedHoles(pbArrTetrisBoardCopy, nHeight, nWidth);
	int bw = getBoardWells(pbArrTetrisBoardCopy, nHeight, nWidth);

	float fScore = m_PDCoff.lh*lh + m_PDCoff.epcm*epcm + m_PDCoff.brt*brt + m_PDCoff.bct*bct + m_PDCoff.bbh * bbh + m_PDCoff.bw * bw;

	return fScore;
}
sPosition CPierreDellacherieTetrisController::pickPositionWithHighestEvalutionScore(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb, float& fHighestEvalutionScoreRet)
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

	float fHighestEvalutionScore = -99999999.0f;
	stringstream ss;
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

		//�������ƶ���(nTargetX, -1)��֮������-1������0��ԭ������Ϊ��
		//��tetris block�ĸ߶���1������Ҫ��ֱ�½���λ����û���κ�����block��ʱ�����ֱ߽�ֵ�ĳ����£���nTargetY��0ʱ 
		//canTetrisBlockMovable�᷵��true�����²��ܽ������evaluationFunction
		for (nTargetY = nTetrisBlockOriginY-1; nTargetY >= -1; nTargetY--)
		{
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
			{
				//ֻ�����²����ƶ���ʱ�򣬲ſ�����TetrisBlockֹͣ�ƶ�
				//����һ���жϣ���������ƶ�ʱ��tetris block�ĸ߶ȴ��ڵ���nTetrisBoardHeight���򲻽���evaluationFunction
				if (stb.nPosY >= nTetrisBoardHeight)
					break;
				nStoppedX = nTetrisBlockPreX;
				nStoppedY = nTetrisBlockPreY;
				stb.nPosX = nStoppedX;
				stb.nPosY = nStoppedY;
				//����evaluationFunction֮ǰ��Ҫ��stb��λ�������ƶ���tetrisBlock���������ƶ���λ��
				float fScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

				ss << "pickPositionWithHighestEvalutionScore stb.nPosX " << stb.nPosX << " stb.nPosY " << stb.nPosY << "fScore " << fScore << endl;
				string debug = ss.str();
				g_fileLogger.Debug(debug);
				ss.clear();
				ss.str("");

				if (fScore >= fHighestEvalutionScore)
				{
					fHighestEvalutionScore = fScore;
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
	//�������ƶ���(nTargetX, -1)��֮������-1������0��ԭ������Ϊ��
	//��tetris block�ĸ߶���1������Ҫ��ֱ�½���λ����û���κ�����block��ʱ�����ֱ߽�ֵ�ĳ����£���nTargetY��0ʱ 
	//canTetrisBlockMovable�᷵��true�����²��ܽ������evaluationFunction
	for (nTargetY = nTetrisBlockOriginY-1; nTargetY >= -1; nTargetY--)
	{
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
		{
			//ֻ�����²����ƶ���ʱ�򣬲ſ�����TetrisBlockֹͣ�ƶ�
			//����һ���жϣ���������ƶ�ʱ��tetris block�ĸ߶ȴ��ڵ���nTetrisBoardHeight���򲻽���evaluationFunction
			if (stb.nPosY >= nTetrisBoardHeight)
				break;
			nStoppedX = nTetrisBlockPreX;
			nStoppedY = nTetrisBlockPreY;
			stb.nPosX = nStoppedX;
			stb.nPosY = nStoppedY;
			//����evaluationFunction֮ǰ��Ҫ��stb��λ�������ƶ���tetrisBlock���������ƶ���λ��
			float fScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

			ss << "pickPositionWithHighestEvalutionScore stb.nPosX " << stb.nPosX << " stb.nPosY " << stb.nPosY << "fScore " << fScore << endl;
			string debug = ss.str();
			g_fileLogger.Debug(debug);
			ss.clear();
			ss.str("");

			if (fScore >= fHighestEvalutionScore)
			{
				fHighestEvalutionScore = fScore;
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
		//�������ƶ���(nTargetX, -1)��֮������-1������0��ԭ������Ϊ��
		//��tetris block�ĸ߶���1������Ҫ��ֱ�½���λ����û���κ�����block��ʱ�����ֱ߽�ֵ�ĳ����£���nTargetY��0ʱ 
		//canTetrisBlockMovable�᷵��true�����²��ܽ������evaluationFunction
		for (nTargetY = nTetrisBlockOriginY-1; nTargetY >= -1; nTargetY--)
		{
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
			{
				//ֻ�����²����ƶ���ʱ�򣬲ſ�����TetrisBlockֹͣ�ƶ�
				//����һ���жϣ���������ƶ�ʱ��tetris block�ĸ߶ȴ��ڵ���nTetrisBoardHeight���򲻽���evaluationFunction
				if (stb.nPosY >= nTetrisBoardHeight)
					break;
				nStoppedX = nTetrisBlockPreX;
				nStoppedY = nTetrisBlockPreY;
				stb.nPosX = nStoppedX;
				stb.nPosY = nStoppedY;
				//����evaluationFunction֮ǰ��Ҫ��stb��λ�������ƶ���tetrisBlock���������ƶ���λ��
				float fScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

				ss << "pickPositionWithHighestEvalutiofScore stb.nPosX " << stb.nPosX << " stb.nPosY " << stb.nPosY << "fScore " << fScore << endl;
				string debug = ss.str();
				g_fileLogger.Debug(debug);
				ss.clear();
				ss.str("");

				if (fScore >= fHighestEvalutionScore)
				{
					fHighestEvalutionScore = fScore;
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
	fHighestEvalutionScoreRet = fHighestEvalutionScore;
	return sp;
}



sPosition CPierreDellacherieTetrisController::generateAICommandListForCurrentTetrisBlock(list<int>& cmdList)
{
	if (!cmdList.empty())
		cmdList.clear();
	//�����Ӧ�� 1 left 2 right 3 down 4 rotate
	bool *pbArrTetrisBoardCopy = new bool[nTetrisBoardHeight*nTetrisBoardWidth];
	int nHeight = nTetrisBoardHeight;
	int nWidth = nTetrisBoardWidth;
	sTetrisBlock stb;
	CTetrisBlock* pTetrisBlock = getCurTetrisBlock();
	int nBlockHeight = pTetrisBlock->getBlockHeight();
	int nBlockWidth = pTetrisBlock->getBlockWidth();
	int nMaxOrientation = pTetrisBlock->getBlockMaxOrientation();
	stb.pbBlock = new bool[nBlockHeight*nBlockWidth];

	getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
	getCurrentTetrisBlockCopy(stb);

	sPosition sp;
	float fHighestScore=-99999999.0f;
	int nBlockRotateTime = 0;
	//��ÿ������˹����Ŀɱ���״����pickPositionWithHighestEvalutionScore���ã�ѡ���evaluationFunction�÷���ߵ�������ת+�����ƶ����
	for (int nRotation = 0; nRotation < nMaxOrientation; nRotation++)
	{	
		//��һ�α���ԭʼ�������״������ת
		if (0 != nRotation)
		{
			RotateTetrisBlock(stb);
			//���ж��Ƿ������ת
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, stb.nPosX, stb.nPosY))
			{
				
				break;
			}
		}
			
		float fHighestEvalutionScoreRet;
		sPosition spTmp;
		spTmp = pickPositionWithHighestEvalutionScore(pbArrTetrisBoardCopy, nHeight, nWidth, stb, fHighestEvalutionScoreRet);
		if (fHighestEvalutionScoreRet >= fHighestScore)
		{
			fHighestScore = fHighestEvalutionScoreRet;
			sp = spTmp;
			nBlockRotateTime = nRotation;
		}
	}
	getCurrentTetrisBlockCopy(stb);
	//��rotate��������뵽cmdList
	while (nBlockRotateTime>0)
	{
		RotateTetrisBlock(stb);
		cmdList.push_back(4);
		nBlockRotateTime--;
	}
	//����������������ƶ�����tetrisBlock�ƶ���sPosition sp���Ϸ�
	if (stb.nPosX > sp.nPosX)
	{
		int nLeftMoveTimes = stb.nPosX - sp.nPosX;
		while (nLeftMoveTimes > 0)
		{
			cmdList.push_back(1);
			nLeftMoveTimes--;
		}
	}
	else if (stb.nPosX < sp.nPosX)
	{
		int nRightMoveTimes = sp.nPosX - stb.nPosX;
		while (nRightMoveTimes > 0)
		{
			cmdList.push_back(2);
			nRightMoveTimes--;
		}
	}

	delete[] stb.pbBlock;
	delete[] pbArrTetrisBoardCopy;

	return sp;
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
	pbBlockCopy = new bool[nPreBlockHeight*nPreBlockWidth];

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

//�õ��з���ռ�ݵ���߲����ţ���Χ�Ǵ�0��nTetrisBoardHeight-1���������0�㶼��û�з���ģ�����ȫ�������ˣ����򷵻�-1
int CPierreDellacherieTetrisController::GetHighestNonEmptyLevel(bool *pbArrTetrisBoardCopy, int nArrayHeight, int nArrayWidth)
{
	int nLevel = -1;
	ArrayIndexRangeCheck arrIndxRngCheck(nArrayHeight, nArrayWidth);
	for (int i = nArrayHeight - 1; i >= 0; i--)
	{
		bool bTemp = false;
		for (int j = 0; j < nArrayWidth; j++)
		{
			arrIndxRngCheck.IndexRangeCheck(i, j);
			bTemp = bTemp | pbArrTetrisBoardCopy[i*nArrayWidth +j];
		}
		if (true == bTemp)
		{
			nLevel = i;
			break;
		}
	}

	return nLevel;
}

int CPierreDellacherieTetrisController::LevelNumErased(bool *pbArrTetrisBoardCopy, int nArrayHeight, int nArrayWidth, int nLevelStart, int nLevelEnd)
{
	//��block�������֮���жϷ���ռ�еĲ����Ƿ�������㣬���ڷ������ռ�Ĳ㣬���ָ�����������4�㣬�򷵻ش���
	if ((nLevelEnd - nLevelStart) > 4)
		return -1;
	if (nLevelEnd < nLevelStart)
		return -2;

	ArrayIndexRangeCheck arrIndxRngCheck(nArrayHeight, nArrayWidth);
	bool bArrayLevelErasable[4];//��¼nLevelStart��nLevelEnd��Ӧ�Ĳ����Ƿ񶼱�ռ������������
	int nHighestNonEmptyLevel = GetHighestNonEmptyLevel(pbArrTetrisBoardCopy, nArrayHeight, nArrayWidth);//��¼�ж��Ƿ�����ǰ����߲���
	for (int i = nLevelStart; i <= nLevelEnd; i++)
	{
		bool bTemp = true;
		for (int j = 0; j < nArrayWidth; j++)
		{
			arrIndxRngCheck.IndexRangeCheck(i, j);
			bTemp = bTemp & pbArrTetrisBoardCopy[i*nArrayWidth + j];
		}
		bArrayLevelErasable[i - nLevelStart] = bTemp;
	}
	int nCurrentLevel = nLevelStart;
	int nTotalLevelErasable = 0;
	while (nCurrentLevel <= nLevelEnd)
	{
		if (true == bArrayLevelErasable[nCurrentLevel - nLevelStart])
		{
			nTotalLevelErasable++;
		}
		else
		{
			//��ǰ�㲻����������Ҫ����nTotalLevelErasable��
			if (0 != nTotalLevelErasable)
			{
				for (int k = 0; k < nArrayWidth; k++)
				{
					arrIndxRngCheck.IndexRangeCheck(nCurrentLevel - nTotalLevelErasable, k);
					arrIndxRngCheck.IndexRangeCheck(nCurrentLevel, k);
					pbArrTetrisBoardCopy[(nCurrentLevel - nTotalLevelErasable)*nArrayWidth + k] = pbArrTetrisBoardCopy[nCurrentLevel*nArrayWidth + k];
				}
			}

		}
		nCurrentLevel++;
	}

	if (0 != nTotalLevelErasable)
	{
		//����ǰԭnLevelEnd֮�ϵĲ�����nTotalLevelErasable��
		for (int m = nLevelEnd + 1; m <= nHighestNonEmptyLevel; m++)
		{
			for (int n = 0; n < nArrayWidth; n++)
			{
				arrIndxRngCheck.IndexRangeCheck(m - nTotalLevelErasable, n);
				arrIndxRngCheck.IndexRangeCheck(m, n);
				pbArrTetrisBoardCopy[(m - nTotalLevelErasable)*nArrayWidth + n] = pbArrTetrisBoardCopy[m*nArrayWidth + n];
			}
		}
		//nHighestNonEmptyLevel�㼰����nTotalLevelErasable����false����
		for (int p = nHighestNonEmptyLevel; p > nHighestNonEmptyLevel - nTotalLevelErasable; p--)
		{
			for (int q = 0; q < nArrayWidth; q++)
			{
				arrIndxRngCheck.IndexRangeCheck(p, q);
				pbArrTetrisBoardCopy[p*nArrayWidth + q] = false;
			}
		}
	}

	return nTotalLevelErasable;
}

void CPierreDellacherieTetrisController::setPDTetrisControllerCoefficient(sPDTetrisControllerCoefficient& coff)
{
	m_PDCoff = coff;
}