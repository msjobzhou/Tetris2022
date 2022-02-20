#include "TetrisController.h"
#include <cstdlib>
#include <ctime>

using namespace std;


CTetrisController::CTetrisController(CTetrisDraw* pTetrisDraw, CTetrisBlock* pTetrisBlock)
{
	m_pTetrisDraw = pTetrisDraw;
	m_pCurTetrisBlock = pTetrisBlock;
	m_lScore = 0;

	if (0 != pTetrisBlock)
		return;
	srand((int)time(NULL));  // 产生随机种子
	int nType = rand() % 7 + 1;
	m_pCurTetrisBlock = new CTetrisBlock(nType);
}


CTetrisController::~CTetrisController()
{
	if (0 != m_pCurTetrisBlock)
	{
		delete m_pCurTetrisBlock;
		m_pCurTetrisBlock = 0;
	}
}

void CTetrisController::setCurTetrisBlock(CTetrisBlock* pCurTetrisBlock)
{
	if (0 != m_pCurTetrisBlock)
	{
		delete m_pCurTetrisBlock;
		m_pCurTetrisBlock = 0;
	}
	if (0 != pCurTetrisBlock)
	{
		m_pCurTetrisBlock = pCurTetrisBlock;
	}
	else
	{
		srand((int)time(NULL));  // 产生随机种子
		int nType = rand() % 7 + 1;
		m_pCurTetrisBlock = new CTetrisBlock(nType);
	}
}
CTetrisBlock* CTetrisController::getCurTetrisBlock()
{
	return m_pCurTetrisBlock;
}
bool CTetrisController::canTetrisBlockMovable(CTetrisBlock* pBlock, int x, int y)
{
	//根据用户指令判断当前方块是否可以旋转或者移动到位置(x,y),x,y从0开始
	//先判断旋转或者移动是否会超出TetrisBoard游戏区之外
	//再使用旋转或者移动之后的方块2维数组和CTetrisDraw.m_bArrTetris中的元素进行逻辑与操作
	bool **ppbBlockArr = pBlock->getBlock2DArr();
	for (int i = 0; i < pBlock->getBlockHeight(); i++)
	{
		for (int j = 0; j < pBlock->getBlockWidth(); j++)
		{
			//移动或者旋转超出左右边界和下边界，这里不判断是否超出上边界，主要是跟block刚出场时的初始位置可能在上边界外有关
			if ((x + j) < 0 || (x + j) >= nTetrisBoardWidth || (y - i) < 0)
				return false;
			//移动或者旋转之后的方块和TetrisArray中任何一个位置都被占用的情况下，返回false
			if (true == ppbBlockArr[i][j] && m_pTetrisDraw->GetTetrisArrayItem(y - i, x + j))
				return false;
		}
	}
	return true;
}

bool CTetrisController::canTetrisBlockMovable(bool **ppbBlockArr, int nHeight, int nWidth, int x, int y)
{
	//函数功能参考canTetrisBlockMovable(CTetrisBlock* pBlock, int x, int y)
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			//移动或者旋转超出左右边界和下边界，这里不判断是否超出上边界，主要是跟block刚出场时的初始位置可能在上边界外有关
			if ((x + j) < 0 || (x + j) >= nTetrisBoardWidth || (y - i) < 0)
				return false;
			//移动或者旋转之后的方块和TetrisArray中任何一个位置都被占用的情况下，返回false
			if (true == ppbBlockArr[i][j] && m_pTetrisDraw->GetTetrisArrayItem(y - i, x + j))
				return false;
		}
	}
	return true;
}
void CTetrisController::setBlockOccupiedTetrisArray(bool bVal)
{
	//设置当前block占据的区域到gameArea，注意posx对应的位置是屏幕的x方向，是tertis二维数组的列或者说是width
	//bVal可以设置成true和false，当为true时，m_pTetrisDraw对应的tetris数组会被设置成占位，当为false时，将tetris数组
	//中对应的位还原，这种用法是因为m_pTetrisDraw的绘图方式是根据tetris数组是否占位进行绘制，如果方块block还能继续下降的话
	//还需要调用setBlockOccupiedTetrisArray(false)将数组还原
	int x = m_pCurTetrisBlock->getBlockPosX();
	int y = m_pCurTetrisBlock->getBlockPosY();
	for (int i = 0; i < m_pCurTetrisBlock->getBlockHeight(); i++)
	{
		for (int j = 0; j < m_pCurTetrisBlock->getBlockWidth(); j++)
		{
			if (true == m_pCurTetrisBlock->getBlockValue(i, j))
			{
				if ((y-i)>=0&& (y - i)<nTetrisBoardHeight&&(x + j)>=0 && (x + j)< nTetrisBoardWidth)
				{
					m_pTetrisDraw->SetTetrisArrayItem(y - i, x + j, bVal);
				}
			}
		}
	}
}
bool CTetrisController::commandLeft()
{	
	if (0 == m_pCurTetrisBlock)
		return false;
	int nCurBlkPosX = m_pCurTetrisBlock->getBlockPosX();
	int nCurBlkPosY = m_pCurTetrisBlock->getBlockPosY();
	bool bRet = canTetrisBlockMovable(m_pCurTetrisBlock, nCurBlkPosX - 1, nCurBlkPosY);
	if (false == bRet)
	{
		return false;
	}
	//执行实际的往左移动指令并让TetrisDraw重绘
	//先清除当前的block
	m_pTetrisDraw->ClearCurrentTetrisBlock(m_pCurTetrisBlock);
	m_pCurTetrisBlock->left();
	//再重绘执行了命令之后的block
	m_pTetrisDraw->DrawCurrentTetrisBlock(m_pCurTetrisBlock);
	return true;
}
bool CTetrisController::commandRight()
{
	if (0 == m_pCurTetrisBlock)
		return false;
	int nCurBlkPosX = m_pCurTetrisBlock->getBlockPosX();
	int nCurBlkPosY = m_pCurTetrisBlock->getBlockPosY();
	bool bRet = canTetrisBlockMovable(m_pCurTetrisBlock, nCurBlkPosX + 1, nCurBlkPosY);
	if (false == bRet)
	{
		return false;
	}
	//执行实际的往右移动指令并让TetrisDraw重绘
	//先清除当前的block
	m_pTetrisDraw->ClearCurrentTetrisBlock(m_pCurTetrisBlock);
	m_pCurTetrisBlock->right();
	//再重绘执行了命令之后的block
	m_pTetrisDraw->DrawCurrentTetrisBlock(m_pCurTetrisBlock);

	return true;
}
bool CTetrisController::commandRotate()
{
	if (0 == m_pCurTetrisBlock)
		return false;
	int nCurBlkPosX = m_pCurTetrisBlock->getBlockPosX();
	int nCurBlkPosY = m_pCurTetrisBlock->getBlockPosY();
	int nBlockHeight = m_pCurTetrisBlock->getBlockHeight();
	int nBlockWidth = m_pCurTetrisBlock->getBlockWidth();
	bool** ppbBlock = m_pCurTetrisBlock->getBlock2DArr();
	bool** ppbRotateBlock = m_pCurTetrisBlock->prepareRotate(nCurBlkPosX, nCurBlkPosY, ppbBlock, nBlockHeight, nBlockWidth);
	bool bRet = canTetrisBlockMovable(ppbRotateBlock, nBlockHeight, nBlockWidth, nCurBlkPosX, nCurBlkPosY);
	//清除资源
	m_pCurTetrisBlock->prepareRotateResourceClean(ppbRotateBlock, nBlockHeight);
	if (false == bRet)
	{
		return false;
	}
	//执行实际的旋转移动指令并让TetrisDraw重绘	
	//先清除当前的block
	m_pTetrisDraw->ClearCurrentTetrisBlock(m_pCurTetrisBlock);
	m_pCurTetrisBlock->Rotate();
	//再重绘执行了命令之后的block
	m_pTetrisDraw->DrawCurrentTetrisBlock(m_pCurTetrisBlock);

	return true;
}
bool CTetrisController::commandDown()
{
	if (0 == m_pCurTetrisBlock)
		return false;
	int nCurBlkPosX = m_pCurTetrisBlock->getBlockPosX();
	int nCurBlkPosY = m_pCurTetrisBlock->getBlockPosY();
	bool bRet1 = canTetrisBlockMovable(m_pCurTetrisBlock, nCurBlkPosX, nCurBlkPosY - 1);
	if (false == bRet1)
	{
		//不能再往下走的场景，block将停止移动，这个时候要设置gameArea对应的二维数组，之后判断是否有可以消层
		setBlockOccupiedTetrisArray(true);
		int nNum = m_pTetrisDraw->LevelNumErased(nCurBlkPosY- m_pCurTetrisBlock->getBlockHeight() + 1, nCurBlkPosY);
		//计算一次消层数目对应的得分
		switch (nNum)
		{
		case 1:
			m_lScore += 100;
			break;
		case 2:
			m_lScore += 300;
			break;
		case 3:
			m_lScore += 700;
			break;
		case 4:
			m_lScore += 1500;
			break;

		default:
			break;
		}
		return false;
	}
	//执行实际的向下移动指令并让TetrisDraw重绘
	//先清除当前的block
	m_pTetrisDraw->ClearCurrentTetrisBlock(m_pCurTetrisBlock);
	m_pCurTetrisBlock->down();
	//再重绘执行了命令之后的block
	m_pTetrisDraw->DrawCurrentTetrisBlock(m_pCurTetrisBlock);

	//更新分数
	m_pTetrisDraw->DrawScoreArea(m_lScore);
	
	return true;
}

bool CTetrisController::executeCommand(int nCmd)
{
	bool bRet = false;
	switch (nCmd)
	{
	case 1:
		bRet = commandLeft();
		break;
	
	case 2:
		bRet = commandRight();
		break;
	
	case 3:
		bRet = commandDown();
		break;
	
	case 4:
		bRet = commandRotate();
		break;

	default:
		break;
	}

	return bRet;
}

bool CTetrisController::isGameOver()
{
	if (m_pCurTetrisBlock != 0) 
	{
		if (m_pCurTetrisBlock->getBlockPosY() >= nTetrisBoardHeight)
			return true;
	}
	return false;
}
long CTetrisController::GetScore()
{
	return m_lScore;
}