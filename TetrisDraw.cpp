#include "TetrisDraw.h"

#include <string>

#include <wchar.h>

using namespace std;

CTetrisDraw::CTetrisDraw(HDC hdcDraw, RECT rectWhole)
{
	m_hdcDraw = hdcDraw;
	SetArea(rectWhole);
	
	for (int y = 0; y < nTetrisBoardHeight; ++y)
	{
		for (int x = 0; x < nTetrisBoardWidth; ++x)
		{
			m_bArrTetris[y][x] = false;
		}
	}
	//下面这段是测试代码，待后期删除
	/*for (int y = 0; y < nTetrisBoardHeight/2; ++y)
	{
		for (int x = 0; x < nTetrisBoardWidth/2; ++x)
		{
			m_bArrTetris[y][x] = true;
		}
	}*/
}


CTetrisDraw::~CTetrisDraw()
{
}

HDC CTetrisDraw::SetHDC(HDC hdcDraw)
{
	HDC hdcOld = m_hdcDraw;
	m_hdcDraw = hdcDraw;
	return hdcOld;
}

void CTetrisDraw::SetArea(RECT rect)
{
	m_rectWhole = rect;
	//这里假定m_rectWhole是left top是（0,0）
	m_rectGameArea.left = m_rectWhole.left;
	m_rectGameArea.top = m_rectWhole.top;
	m_rectGameArea.bottom = m_rectWhole.bottom;
	m_rectGameArea.right = int(m_rectWhole.right * 2 / 3) - 1;

	m_rectScoreArea.left = int(m_rectWhole.right * 2 / 3);
	m_rectScoreArea.top = m_rectWhole.top;
	m_rectScoreArea.bottom = int(m_rectWhole.bottom / 2) - 1;
	m_rectScoreArea.right = m_rectWhole.right;

	m_rectNextBlockHintArea.left = int(m_rectWhole.right * 2 / 3);
	m_rectNextBlockHintArea.top = int(m_rectWhole.bottom / 2);
	m_rectNextBlockHintArea.bottom = m_rectWhole.bottom;
	m_rectNextBlockHintArea.right = m_rectWhole.right;
}

//这里的DrawXXX函数的作图区域默认是x方向从左往右增加，y方向是从上往下增加
void CTetrisDraw::DrawGameArea(int nLow, int nHigh)
{
	const int border = 20;
	int cxClient = m_rectGameArea.right - m_rectGameArea.left + 1;
	int cyClient = m_rectGameArea.bottom - m_rectGameArea.top + 1;

	int BlockSizeX = (cxClient - 2 * border) / nTetrisBoardWidth;
	int BlockSizeY = (cyClient - 2 * border) / nTetrisBoardHeight;

	HBRUSH	BlackBrush, OldBrush, NullBrush;
	HPEN	NullPen, OldPen, BlackPen;
	HBRUSH RedBrush = CreateSolidBrush(RGB(255, 0, 0));


	//grab a null pen so we can see the outlines of the cells
	NullPen = (HPEN)GetStockObject(NULL_PEN);
	BlackPen = (HPEN)GetStockObject(BLACK_PEN);

	//grab a brush to fill our cells with
	BlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	NullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	//select them into the device conext
	OldBrush = (HBRUSH)SelectObject(m_hdcDraw, NullBrush);
	OldPen = (HPEN)SelectObject(m_hdcDraw, BlackPen);

	//作图游戏区域边界

	Rectangle(m_hdcDraw, m_rectGameArea.left + border - 2 , m_rectGameArea.top + border - 2, 
		m_rectGameArea.right - border + 2, m_rectGameArea.bottom - border + 2);
	//restore the original brush
	SelectObject(m_hdcDraw, OldBrush);
	SelectObject(m_hdcDraw, OldPen);

	HBRUSH whiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);

	//select them into the device conext
	OldPen = (HPEN)SelectObject(m_hdcDraw, NullPen);

	for (int y = nLow; y<=nHigh; ++y)
	{
		for (int x = 0; x<nTetrisBoardWidth; ++x)
		{
			//整个游戏区域默认左下角对应的是（0,0），右上角对应的是(nTetrisBoardHeight，nTetrisBoardWidth)
			//因此下面有个从显示器显示区域到tetrisBoard数组的转换关系
			int left = border + (BlockSizeX * x);
			int right = left + BlockSizeX;

			int top = border + (BlockSizeY * (nTetrisBoardHeight-1-y));
			int bottom = top + BlockSizeY;

			
			if (m_bArrTetris[y][x] == 1)
			{
				OldBrush = (HBRUSH)SelectObject(m_hdcDraw, RedBrush);
				//draw the cell
				Rectangle(m_hdcDraw, left, top, right, bottom);

				SelectObject(m_hdcDraw, OldBrush);
			}
			else
			{
				OldBrush = (HBRUSH)SelectObject(m_hdcDraw, whiteBrush);
				
				//clear the cell
				Rectangle(m_hdcDraw, left, top, right, bottom);
				SelectObject(m_hdcDraw, OldBrush);
			}

		}
	}

	
	//restore the original pen
	SelectObject(m_hdcDraw, OldPen);
	DeleteObject(RedBrush);
}

void CTetrisDraw::DrawScoreArea(long score)
{
	wchar_t s[100] = L"";
	swprintf(s, 100, L"游戏得分   %d", score);
	TextOut(m_hdcDraw, m_rectScoreArea.left, m_rectScoreArea.top, s, wcslen(s));
	
}

void CTetrisDraw::DrawNextBlockHintArea()
{
	TCHAR s[20] = TEXT("Next: ");
	TextOut(m_hdcDraw, m_rectNextBlockHintArea.left, m_rectNextBlockHintArea.top, s, wcslen(s));
}

void CTetrisDraw::SetTetrisArrayItem(int nHeight, int nWidth, bool bValue)
{
	m_bArrTetris[nHeight][nWidth] = bValue;
}
bool CTetrisDraw::GetTetrisArrayItem(int nHeight, int nWidth)
{
	return m_bArrTetris[nHeight][nWidth];
}


void CTetrisDraw::DrawCurrentTetrisBlock(CTetrisBlock* pTetrisBlock)
{
	if (0 == pTetrisBlock)
	{
		return;
	}
	const int border = 20;
	int cxClient = m_rectGameArea.right - m_rectGameArea.left + 1;
	int cyClient = m_rectGameArea.bottom - m_rectGameArea.top + 1;

	int BlockSizeX = (cxClient - 2 * border) / nTetrisBoardWidth;
	int BlockSizeY = (cyClient - 2 * border) / nTetrisBoardHeight;
	HBRUSH	BlackBrush, OldBrush;
	HPEN	NullPen, OldPen;

	//grab a null pen so we can see the outlines of the cells
	NullPen = (HPEN)GetStockObject(NULL_PEN);

	//grab a brush to fill our cells with
	BlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//select them into the device conext
	OldBrush = (HBRUSH)SelectObject(m_hdcDraw, BlackBrush);
	OldPen = (HPEN)SelectObject(m_hdcDraw, NullPen);
	int nPosY = pTetrisBlock->getBlockPosY();
	int nPosX = pTetrisBlock->getBlockPosX();
	int nBlockHeight = pTetrisBlock->getBlockHeight();
	int nBlockWidth = pTetrisBlock->getBlockWidth();
	for (int y = nPosY; y>nPosY- nBlockHeight; --y)
	{
		if (y < 0 || y >= nTetrisBoardHeight)
			continue;
		for (int x = nPosX; x<nPosX+ nBlockWidth; ++x)
		{
			if (x < 0 || x >= nTetrisBoardWidth)
				continue;
			//整个游戏区域默认左下角对应的是（0,0），右上角对应的是(nTetrisBoardHeight，nTetrisBoardWidth)
			//因此下面有个从显示器显示区域到tetrisBoard数组的转换关系
			int left = border + (BlockSizeX * x);
			int right = left + BlockSizeX;

			int top = border + (BlockSizeY * (nTetrisBoardHeight - 1 - y));
			int bottom = top + BlockSizeY;

			//这里增加个判断，超出block的二维数组下标范围抛出异常
			if (pTetrisBlock->getBlockValue(nPosY-y, x- nPosX) == true)
			{
				//SelectObject(m_hdcDraw, BlackBrush);

				//draw the cell
				Rectangle(m_hdcDraw, left, top, right, bottom);
			}

		}
	}

	//restore the original brush
	SelectObject(m_hdcDraw, OldBrush);

	//and pen
	SelectObject(m_hdcDraw, OldPen);
}

void CTetrisDraw::ClearCurrentTetrisBlock(CTetrisBlock* pTetrisBlock)
{

	if (0 == pTetrisBlock)
	{
		return;
	}
	const int border = 20;
	int cxClient = m_rectGameArea.right - m_rectGameArea.left + 1;
	int cyClient = m_rectGameArea.bottom - m_rectGameArea.top + 1;

	int BlockSizeX = (cxClient - 2 * border) / nTetrisBoardWidth;
	int BlockSizeY = (cyClient - 2 * border) / nTetrisBoardHeight;
	HBRUSH	WhiteBrush, OldBrush;
	HPEN	NullPen, OldPen;

	//grab a null pen so we can see the outlines of the cells
	NullPen = (HPEN)GetStockObject(NULL_PEN);

	//grab a brush to fill our cells with
	WhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);

	//select them into the device conext
	OldBrush = (HBRUSH)SelectObject(m_hdcDraw, WhiteBrush);
	OldPen = (HPEN)SelectObject(m_hdcDraw, NullPen);
	int nPosY = pTetrisBlock->getBlockPosY();
	int nPosX = pTetrisBlock->getBlockPosX();
	int nBlockHeight = pTetrisBlock->getBlockHeight();
	int nBlockWidth = pTetrisBlock->getBlockWidth();
	for (int y = nPosY; y>nPosY - nBlockHeight; --y)
	{
		if (y < 0 || y >= nTetrisBoardHeight)
			continue;
		for (int x = nPosX; x<nPosX + nBlockWidth; ++x)
		{
			if (x < 0 || x >= nTetrisBoardWidth)
				continue;
			//整个游戏区域默认左下角对应的是（0,0），右上角对应的是(nTetrisBoardHeight，nTetrisBoardWidth)
			//因此下面有个从显示器显示区域到tetrisBoard数组的转换关系
			int left = border + (BlockSizeX * x);
			int right = left + BlockSizeX;

			int top = border + (BlockSizeY * (nTetrisBoardHeight - 1 - y));
			int bottom = top + BlockSizeY;

			//这里增加个判断，超出block的二维数组下标范围抛出异常
			if (pTetrisBlock->getBlockValue(nPosY - y, x - nPosX) == true)
			{
				//SelectObject(m_hdcDraw, WhiteBrush);

				//draw the cell
				Rectangle(m_hdcDraw, left, top, right, bottom);
			}

		}
	}
	//restore the original brush
	SelectObject(m_hdcDraw, OldBrush);

	//and pen
	SelectObject(m_hdcDraw, OldPen);
}
void CTetrisDraw::ClearLevel(int nLevel)
{
	const int border = 20;
	int cxClient = m_rectGameArea.right - m_rectGameArea.left + 1;
	int cyClient = m_rectGameArea.bottom - m_rectGameArea.top + 1;

	int BlockSizeY = (cyClient - 2 * border) / nTetrisBoardHeight;
	HBRUSH	WhiteBrush, OldBrush;
	HPEN	NullPen, OldPen;

	//grab a null pen so we can see the outlines of the cells
	NullPen = (HPEN)GetStockObject(NULL_PEN);

	//grab a brush to fill our cells with
	WhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);

	//select them into the device conext
	OldBrush = (HBRUSH)SelectObject(m_hdcDraw, WhiteBrush);
	OldPen = (HPEN)SelectObject(m_hdcDraw, NullPen);

	int left =  border;
	int right = m_rectGameArea.right - border;

	int top = border + (BlockSizeY * (nTetrisBoardHeight - 1 - nLevel));
	int bottom = top + BlockSizeY;

	Rectangle(m_hdcDraw, left, top, right, bottom);

	SelectObject(m_hdcDraw, OldBrush);
	SelectObject(m_hdcDraw, OldPen);
}
/*
LevelNumErased函数作用：判断m_bArrTetris中是否指定的层数中是否有可以消除的层，并返回具体的层数
如果有可以消除的层，则擦除对应的层，并用上一层的来占住被消除的层
*/
int CTetrisDraw::LevelNumErased(int nLevelStart, int nLevelEnd)
{
	//在block方块落地之后判断方块占有的层数是否可以消层，由于方块最多占四层，因此指定层数差大于4层，则返回错误
	if ((nLevelEnd - nLevelStart) > 4)
		return -1;
	if (nLevelEnd < nLevelStart)
		return -2;
	bool bArrayLevelErasable[4];//记录nLevelStart到nLevelEnd对应的层数是否都被占满，可消除。
	int nHighestNonEmptyLevel = GetHighestNonEmptyLevel();//记录判断是否消层前的最高层数
	for (int i = nLevelStart; i <= nLevelEnd; i++)
	{
		bool bTemp = true;
		for (int j = 0; j < nTetrisBoardWidth; j++)
		{
			bTemp = bTemp & m_bArrTetris[i][j];
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
			//绘画擦除当前层，如果要达成特定的消层动画效果时有用，否则，消层后的游戏窗口重绘，由DrawGameArea统一完成
			//ClearLevel(nCurrentLevel);
		}
		else
		{
			//当前层不可消除，需要下移nTotalLevelErasable层
			if (0 != nTotalLevelErasable)
			{
				for (int k = 0; k < nTetrisBoardWidth; k++)
				{
					m_bArrTetris[nCurrentLevel - nTotalLevelErasable][k] = m_bArrTetris[nCurrentLevel][k];
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
			for (int n = 0; n < nTetrisBoardWidth; n++)
			{
				m_bArrTetris[m - nTotalLevelErasable][n] = m_bArrTetris[m][n];
			}
		}
		//nHighestNonEmptyLevel层及以下nTotalLevelErasable层用false填满
		for (int p = nHighestNonEmptyLevel; p > nHighestNonEmptyLevel - nTotalLevelErasable; p--)
		{
			for (int  q= 0; q < nTetrisBoardWidth; q++)
			{
				m_bArrTetris[p][q] = false;
			}
			//绘画擦除对应的层，如果要达成特定的消层动画效果时有用，否则，消层后的游戏窗口重绘，由DrawGameArea统一完成
			//ClearLevel(p);
		}
		//重绘消层影响到的层数
		if (nLevelStart <= nHighestNonEmptyLevel)
		{
			DrawGameArea(nLevelStart, nHighestNonEmptyLevel);
		}
	}
	
	return nTotalLevelErasable;
}

//得到有方块占据的最高层的序号，范围是从0到nTetrisBoardHeight-1，如果连第0层都是没有方块的（层数全部消除了），则返回-1
int CTetrisDraw::GetHighestNonEmptyLevel()
{
	int nLevel = -1;
	for (int i = nTetrisBoardHeight - 1; i >= 0; i--) 
	{
		bool bTemp = false;
		for (int j = 0; j < nTetrisBoardWidth; j++)
		{
			bTemp = bTemp | m_bArrTetris[i][j];
		}
		if (true == bTemp)
		{
			nLevel = i;
			break;
		}
	}

	return nLevel;
}