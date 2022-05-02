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
	//对stb的数组进行赋值
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			stb.pbBlock[i*stb.nBlockWidth+j]= pTetrisBlock->getBlockValue(i,j);
		}
	}
}
//以下几个函数都需要在当前TetrisBlock落定不能移动之后，且执行setBlockOccupiedTetrisArray(true)之后才能调用
//另外
int CPierreDellacherieTetrisController::getLandingHeight(sTetrisBlock& stb)
{
	return stb.nPosY;
}
//函数getErodedPieceCellsMetric会使用stb占用的位置来填充pbArrTetrisBoardCopy
int CPierreDellacherieTetrisController::getErodedPieceCellsMetric(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth, sTetrisBlock& stb)
{
	//stringstream ss;
	//ss << "getErodedPieceCellsMetric para stb.nPosX " << stb.nPosX << " stb.nPosY" << stb.nPosY << endl;

	//string debug = ss.str();
	//g_fileLogger.Debug(debug);

	ArrayIndexRangeCheck arrIndxRngCheck(nHeight, nWidth);

	int nLevelErasedAfterTetrisBlockDownTillItCannotMove = 0;
	int nBlockElementContributeToLevelErased = 0;
	//用当前俄罗斯方块所在的位置填充pbArrTetrisBoardCopy
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		int j;
		//注意方块的坐标左上是(0,0)，x轴往右增加，y轴往下增加， Tetrisboard左下是(0,0)，y轴往上增加
		//nBlockHeightTmp表示当前方块的第i行对应的tetrisBoard的高度（也就是tetris board的行）
		int nBlockHeightTmp = stb.nPosY -i;
		//计算填充前方块所在tetris board的行中，有元素占据的位置个数
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
		//方块填充pbArrTetrisBoardCopy
		for (j = 0; j < stb.nBlockWidth; j++)
		{
			arrIndxRngCheck.IndexRangeCheck(nBlockHeightTmp, stb.nPosX + j);
			if(true == stb.pbBlock[i*stb.nBlockWidth + j])
			{
				pbArrTetrisBoardCopy[nBlockHeightTmp*nTetrisBoardWidth + stb.nPosX+j]
					= stb.pbBlock[i*stb.nBlockWidth + j];
			}
		}

		//计算填充后方块所在tetris board的行中，有元素占据的位置个数
		int nOccupiedNumAfterTetrisBlockDownTillItCannotMove = 0;
		for (j = 0; j < nTetrisBoardWidth; j++)
		{
			arrIndxRngCheck.IndexRangeCheck(nBlockHeightTmp, j);
			if (true == pbArrTetrisBoardCopy[nBlockHeightTmp*nTetrisBoardWidth + j])
			{
				nOccupiedNumAfterTetrisBlockDownTillItCannotMove++;
			}
		}
		//如果当前行中每个位置都被占满的话，则需要计算可以消除的行数，以及对消行做出贡献的方块element数
		//所谓的element指的是组成tetris block的最小元素，如横直线型方块■■■■中的每个■都是一个element
		if (nTetrisBoardWidth == nOccupiedNumAfterTetrisBlockDownTillItCannotMove)
		{
			nLevelErasedAfterTetrisBlockDownTillItCannotMove++;
			nBlockElementContributeToLevelErased+=(nOccupiedNumAfterTetrisBlockDownTillItCannotMove- nOccupiedNum);
		}
		//这里可以优化一下，如果一行都被占满的话，当前没有对pbArrTetrisBoardCopy进行消层操作，因为后面的getBoardRowTransitions
		//等函数都需要使用被方块占据后的pbArrTetrisBoardCopy进行计算，如果方块的放置方式会在下面形成“空洞”，在会给getBoardBuriedHoles
		//等函数造成比较大的负值，而实际上被占满的一层会被消层掉，因此“空洞”在消层后并不存在了。
		LevelNumErased(pbArrTetrisBoardCopy, nHeight, nWidth, stb.nPosY - stb.nBlockHeight + 1, stb.nPosY);
	}
	//按照PierreDellacherie构造此特征的数值为nLevelErasedAfterTetrisBlockDownTillItCannotMove*nBlockElementContributeToLevelErased
	return nLevelErasedAfterTetrisBlockDownTillItCannotMove*nBlockElementContributeToLevelErased;
}
int CPierreDellacherieTetrisController::getBoardRowTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth)
{
	int nTransitions = 0;
	for (int y = 0; y < nHeight; ++y)
	{
		for (int x = 0; x < nWidth - 1; ++x)
		{
			//发生transition的两种情况是如下：（可用异或运算来判断）
			//1、pbArrTetrisBoardCopy[y*nWidth + x]为true，pbArrTetrisBoardCopy[y*nWidth + x+1]为false
			//2、pbArrTetrisBoardCopy[y*nWidth + x]为false，pbArrTetrisBoardCopy[y*nWidth + x+1]为true
			if ((pbArrTetrisBoardCopy[y*nWidth + x] ^ pbArrTetrisBoardCopy[y*nWidth + x + 1]) == true)
			{
				nTransitions++;
			}
		}
		//tetris board的边界算是有石块被占据
		//先看左边界和本行第一个石块的情况
		//if (false == pbArrTetrisBoardCopy[y*nWidth + 0])
		//	nTransitions++;
		//再看本行最后一个石块和右边界的情况
		//if (false == pbArrTetrisBoardCopy[y*nWidth + nWidth - 1])
		//	nTransitions++;
	}

	return nTransitions;
}
int CPierreDellacherieTetrisController::getBoardColumnTransitions(bool *pbArrTetrisBoardCopy, int nHeight, int nWidth)
{
	//代码逻辑参考getBoardRowTransitions
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
	//对于每一列，从上往下检测，碰到第一个为true的element之后，在这个方块之下的每个为false的element都计数为1个Buried hole
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
	//对于每一列，从上往下检测，如果当前的element为false，则检测左右两侧element是否为true，或者是board边界
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
	//判断当前方块是否可以旋转或者移动到位置(x,y),x,y从0开始
	//先判断旋转或者移动是否会超出TetrisBoard游戏区之外
	//再使用旋转或者移动之后的方块2维数组和CTetrisDraw.m_bArrTetris中的元素进行逻辑与操作
	bool *pbBlockArr = stb.pbBlock;
	for (int i = 0; i < stb.nBlockHeight; i++)
	{
		for (int j = 0; j < stb.nBlockWidth; j++)
		{
			//移动或者旋转超出左右边界和下边界，这里不判断是否超出上边界，主要是跟block刚出场时的初始位置可能在上边界外有关
			if ((x + j) < 0 || (x + j) >= nTetrisBoardWidth || (y - i) < 0)
				return false;
			
			//if ((y - i)*nTetrisBoardWidth + x + j >= nTetrisBoardHeight*nTetrisBoardWidth)
			//{
			//	g_fileLogger.Error("pbArrTetrisBoardCopy out of range in function canTetrisBlockMovable");
			//}
			//移动或者旋转之后的方块和TetrisArray中任何一个位置都被占用的情况下，返回false
			//AI模式下，可能对于刚出场的block进行旋转或者移动操作，此时y的值是超过nTetrisBoardHeight-1的
			//这里再访问数组前加个判断，防止超过pbArrTetrisBoardCopy数组的边界，而引发delete []异常
			//同时对于从屏幕上方刚出来的tetris block中在nTetrisBoardHeight行及以上的，肯定不会有冲突，不用做额外判断了
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
	//这里只搜索直角形路径或者垂直下降路径(如下图↓和→←指示的方向1,2,3)，不考虑移动过程中变形，钻洞等复杂场景
	//   1←■  →→3
	//   1↓■■   ↓3
	//   1↓  ■   ↓3
	//   1↓  ↓2  ↓3
	//
	//■■   ■■■  ■■
	//■■■ ■■■■■■
	sPosition sp;
	sp.nPosX = -1;
	sp.nPosY = -1;

	int nTargetX, nTargetY;
	int nTetrisBlockOriginX = stb.nPosX;
	int nTetrisBlockOriginY = stb.nPosY;

	//俄罗斯方块在朝目标位置移动过程中因被阻挡而停留在的位置
	int nStoppedX, nStoppedY;

	int nTetrisBlockPreX = nTetrisBlockOriginX;
	int nTetrisBlockPreY = nTetrisBlockOriginY;

	float fHighestEvalutionScore = -99999999.0f;
	stringstream ss;
	//按照路径1的路线进行搜索
	for (nTargetX = nTetrisBlockOriginX-1; nTargetX >= 0; nTargetX--)
	{
		getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
		//先向左移动到(nTargetX, nTetrisBlockOriginY)
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTetrisBlockOriginY))
		{
			//
			break;
		}
		//对俄罗斯方块开始第一次向下移动的时候，进行处理一下nTetrisBlockPreY
		nTetrisBlockPreY = nTetrisBlockOriginY;

		//再向下移动到(nTargetX, -1)，之所以是-1而不是0的原因，是因为，
		//当tetris block的高度是1，且其要垂直下降的位置上没有任何其他block的时候，这种边界值的场景下，在nTargetY是0时 
		//canTetrisBlockMovable会返回true，导致不能进入计算evaluationFunction
		for (nTargetY = nTetrisBlockOriginY-1; nTargetY >= -1; nTargetY--)
		{
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
			{
				//只有向下不能移动的时候，才可以算TetrisBlock停止移动
				//增加一个判断，如果不能移动时，tetris block的高度大于等于nTetrisBoardHeight，则不进入evaluationFunction
				if (stb.nPosY >= nTetrisBoardHeight)
					break;
				nStoppedX = nTetrisBlockPreX;
				nStoppedY = nTetrisBlockPreY;
				stb.nPosX = nStoppedX;
				stb.nPosY = nStoppedY;
				//调用evaluationFunction之前需要将stb的位置设置移动到tetrisBlock不能向下移动的位置
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

				//将stb方块的坐标还原成原始位置，以供下次使用
				stb.nPosX = nTetrisBlockOriginX;
				stb.nPosY = nTetrisBlockOriginY;
				break;
			}
				
			nTetrisBlockPreX = nTargetX;
			nTetrisBlockPreY = nTargetY;
		}
	}

	//按照路径2的路线进行搜索
	nTargetX = nTetrisBlockOriginX;
	nTetrisBlockPreX = nTetrisBlockOriginX;
	nTetrisBlockPreY = nTetrisBlockOriginY;

	getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
	//再向下移动到(nTargetX, -1)，之所以是-1而不是0的原因，是因为，
	//当tetris block的高度是1，且其要垂直下降的位置上没有任何其他block的时候，这种边界值的场景下，在nTargetY是0时 
	//canTetrisBlockMovable会返回true，导致不能进入计算evaluationFunction
	for (nTargetY = nTetrisBlockOriginY-1; nTargetY >= -1; nTargetY--)
	{
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
		{
			//只有向下不能移动的时候，才可以算TetrisBlock停止移动
			//增加一个判断，如果不能移动时，tetris block的高度大于等于nTetrisBoardHeight，则不进入evaluationFunction
			if (stb.nPosY >= nTetrisBoardHeight)
				break;
			nStoppedX = nTetrisBlockPreX;
			nStoppedY = nTetrisBlockPreY;
			stb.nPosX = nStoppedX;
			stb.nPosY = nStoppedY;
			//调用evaluationFunction之前需要将stb的位置设置移动到tetrisBlock不能向下移动的位置
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

			//将stb方块的坐标还原成原始位置，以供下次使用
			stb.nPosX = nTetrisBlockOriginX;
			stb.nPosY = nTetrisBlockOriginY;
			break;
		}

		nTetrisBlockPreX = nTargetX;
		nTetrisBlockPreY = nTargetY;
	}
	
	//按照路径3的路线进行搜索
	nTetrisBlockPreX = nTetrisBlockOriginX;
	nTetrisBlockPreY = nTetrisBlockOriginY;
	for (nTargetX = nTetrisBlockOriginX + 1; nTargetX < nTetrisBoardWidth; nTargetX++)
	{
		getArrTetrisBoardCopyFromCTetrisDraw(pbArrTetrisBoardCopy);
		//先向右移动到(nTargetX, nTetrisBlockOriginY)
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTetrisBlockOriginY))
		{
			//
			break;
		}
		//对俄罗斯方块开始第一次向下移动的时候，进行处理一下nTetrisBlockPreY
		nTetrisBlockPreY = nTetrisBlockOriginY;
		//再向下移动到(nTargetX, -1)，之所以是-1而不是0的原因，是因为，
		//当tetris block的高度是1，且其要垂直下降的位置上没有任何其他block的时候，这种边界值的场景下，在nTargetY是0时 
		//canTetrisBlockMovable会返回true，导致不能进入计算evaluationFunction
		for (nTargetY = nTetrisBlockOriginY-1; nTargetY >= -1; nTargetY--)
		{
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
			{
				//只有向下不能移动的时候，才可以算TetrisBlock停止移动
				//增加一个判断，如果不能移动时，tetris block的高度大于等于nTetrisBoardHeight，则不进入evaluationFunction
				if (stb.nPosY >= nTetrisBoardHeight)
					break;
				nStoppedX = nTetrisBlockPreX;
				nStoppedY = nTetrisBlockPreY;
				stb.nPosX = nStoppedX;
				stb.nPosY = nStoppedY;
				//调用evaluationFunction之前需要将stb的位置设置移动到tetrisBlock不能向下移动的位置
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

				//将stb方块的坐标还原成原始位置，以供下次使用
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
	//命令对应表 1 left 2 right 3 down 4 rotate
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
	//对每个俄罗斯方块的可变形状进行pickPositionWithHighestEvalutionScore调用，选择出evaluationFunction得分最高的最优旋转+最优移动组合
	for (int nRotation = 0; nRotation < nMaxOrientation; nRotation++)
	{	
		//第一次保持原始方块的形状，不旋转
		if (0 != nRotation)
		{
			RotateTetrisBlock(stb);
			//后判断是否可以旋转
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
	//将rotate的命令加入到cmdList
	while (nBlockRotateTime>0)
	{
		RotateTetrisBlock(stb);
		cmdList.push_back(4);
		nBlockRotateTime--;
	}
	//按照向左或者向右移动，将tetrisBlock移动到sPosition sp的上方
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

//仿照CTetrisBlock的prepareRotate实现的功能
void CPierreDellacherieTetrisController::RotateTetrisBlock(sTetrisBlock& stb)
{
	//各代码语句中的作用参考CTetrisBlock的prepareRotate函数
	int nPreBlockHeight = stb.nBlockHeight;
	int nPreBlockWidth = stb.nBlockWidth;
	bool *pbBlockCopy;
	//旋转90度之后方块的高度和宽度互换
	stb.nBlockHeight = nPreBlockWidth;
	stb.nBlockWidth = nPreBlockHeight;
	
	//对stb.pbBlock构造一个副本
	pbBlockCopy = new bool[nPreBlockHeight*nPreBlockWidth];

	for (int m = 0; m < nPreBlockHeight; m++)
	{
		for (int n = 0; n < nPreBlockWidth; n++)
		{
			pbBlockCopy[m*nPreBlockWidth + n] = stb.pbBlock[m*nPreBlockWidth + n];
		}
	}
	
	//对数组进行向右旋转90度
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

//得到有方块占据的最高层的序号，范围是从0到nTetrisBoardHeight-1，如果连第0层都是没有方块的（层数全部消除了），则返回-1
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
	//在block方块落地之后判断方块占有的层数是否可以消层，由于方块最多占四层，因此指定层数差大于4层，则返回错误
	if ((nLevelEnd - nLevelStart) > 4)
		return -1;
	if (nLevelEnd < nLevelStart)
		return -2;

	ArrayIndexRangeCheck arrIndxRngCheck(nArrayHeight, nArrayWidth);
	bool bArrayLevelErasable[4];//记录nLevelStart到nLevelEnd对应的层数是否都被占满，可消除。
	int nHighestNonEmptyLevel = GetHighestNonEmptyLevel(pbArrTetrisBoardCopy, nArrayHeight, nArrayWidth);//记录判断是否消层前的最高层数
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
			//当前层不可消除，需要下移nTotalLevelErasable层
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
		//消层前原nLevelEnd之上的层下移nTotalLevelErasable层
		for (int m = nLevelEnd + 1; m <= nHighestNonEmptyLevel; m++)
		{
			for (int n = 0; n < nArrayWidth; n++)
			{
				arrIndxRngCheck.IndexRangeCheck(m - nTotalLevelErasable, n);
				arrIndxRngCheck.IndexRangeCheck(m, n);
				pbArrTetrisBoardCopy[(m - nTotalLevelErasable)*nArrayWidth + n] = pbArrTetrisBoardCopy[m*nArrayWidth + n];
			}
		}
		//nHighestNonEmptyLevel层及以下nTotalLevelErasable层用false填满
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