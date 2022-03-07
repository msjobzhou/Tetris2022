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
			if (true == pbArrTetrisBoardCopy[nBlockHeightTmp*nTetrisBoardWidth + j])
			{
				nOccupiedNum++;
			}
		}
		assert(nOccupiedNum < nTetrisBoardWidth);
		//方块填充pbArrTetrisBoardCopy
		for (j = 0; j < stb.nBlockWidth; j++)
		{
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
			//1、pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x]为true，pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x+1]为false
			//2、pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x]为false，pbArrTetrisBoardCopy[y*nTetrisBoardWidth + x+1]为true
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
	//代码逻辑参考getBoardRowTransitions
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
			//移动或者旋转之后的方块和TetrisArray中任何一个位置都被占用的情况下，返回false
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

	int nHighestEvalutionScore = -99999999;
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

		//再向下移动到(nTargetX, 0)
		for (nTargetY = nTetrisBlockOriginY; nTargetY >= 0; nTargetY--)
		{
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
			{
				//只有向下不能移动的时候，才可以算TetrisBlock停止移动
				
				nStoppedX = nTetrisBlockPreX;
				nStoppedY = nTetrisBlockPreY;
				stb.nPosX = nStoppedX;
				stb.nPosY = nStoppedY;
				//调用evaluationFunction之前需要将stb的位置设置移动到tetrisBlock不能向下移动的位置
				int nScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

				if (nScore >= nHighestEvalutionScore)
				{
					nHighestEvalutionScore = nScore;
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
	//再向下移动到(nTargetX, 0)
	for (nTargetY = nTetrisBlockOriginY; nTargetY >= 0; nTargetY--)
	{
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
		{
			//只有向下不能移动的时候，才可以算TetrisBlock停止移动
			nStoppedX = nTetrisBlockPreX;
			nStoppedY = nTetrisBlockPreY;
			stb.nPosX = nStoppedX;
			stb.nPosY = nStoppedY;
			//调用evaluationFunction之前需要将stb的位置设置移动到tetrisBlock不能向下移动的位置
			int nScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

			if (nScore >= nHighestEvalutionScore)
			{
				nHighestEvalutionScore = nScore;
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
		//先向左移动到(nTargetX, nTetrisBlockOriginY)
		if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTetrisBlockOriginY))
		{
			//
			break;
		}
		//对俄罗斯方块开始第一次向下移动的时候，进行处理一下nTetrisBlockPreY
		nTetrisBlockPreY = nTetrisBlockOriginY;
		//再向下移动到(nTargetX, 0)
		for (nTargetY = nTetrisBlockOriginY; nTargetY >= 0; nTargetY--)
		{
			if (!canTetrisBlockMovable(stb, pbArrTetrisBoardCopy, nTargetX, nTargetY))
			{
				//只有向下不能移动的时候，才可以算TetrisBlock停止移动

				nStoppedX = nTetrisBlockPreX;
				nStoppedY = nTetrisBlockPreY;
				stb.nPosX = nStoppedX;
				stb.nPosY = nStoppedY;
				//调用evaluationFunction之前需要将stb的位置设置移动到tetrisBlock不能向下移动的位置
				int nScore = evaluationFunction(pbArrTetrisBoardCopy, nHeight, nWidth, stb);

				if (nScore >= nHighestEvalutionScore)
				{
					nHighestEvalutionScore = nScore;
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

	//对每个俄罗斯的可变形状进行pickPositionWithHighestEvalutionScore调用，选择出evaluationFunction得分最高的最优旋转+最优移动组合

	delete[] stb.pbBlock;
	delete[] pbArrTetrisBoardCopy;

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
	pbBlockCopy = new bool[nPreBlockHeight*nPreBlockHeight];

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