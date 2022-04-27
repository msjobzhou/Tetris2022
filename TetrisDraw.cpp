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
			//-1表示无颜色
			m_bArrTetrisColor[y][x] = -1;
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

	m_rectScoreAndNextBlockArea.left = int(m_rectWhole.right * 2 / 3);
	m_rectScoreAndNextBlockArea.top = m_rectWhole.top;
	m_rectScoreAndNextBlockArea.bottom = int(m_rectWhole.bottom / 2) - 1;
	m_rectScoreAndNextBlockArea.right = m_rectWhole.right;

	m_rectGameHintArea.left = int(m_rectWhole.right * 2 / 3);
	m_rectGameHintArea.top = int(m_rectWhole.bottom / 2);
	m_rectGameHintArea.bottom = m_rectWhole.bottom;
	m_rectGameHintArea.right = m_rectWhole.right;
}

//这里的DrawXXX函数的作图区域默认是x方向从左往右增加，y方向是从上往下增加
void CTetrisDraw::DrawGameArea(int nLow, int nHigh)
{
	//如果dcDraw为空，则不作任何绘图动作，给使用遗传算法等计算参数无需绘图的场景使用
	if (0 == m_hdcDraw)
		return;
	const int border = 20;
	int cxClient = m_rectGameArea.right - m_rectGameArea.left + 1;
	int cyClient = m_rectGameArea.bottom - m_rectGameArea.top + 1;

	int BlockSizeX = (cxClient - 2 * border) / nTetrisBoardWidth;
	int BlockSizeY = (cyClient - 2 * border) / nTetrisBoardHeight;

	HBRUSH	BlackBrush, OldBrush, NullBrush;
	HPEN	NullPen, OldPen, BlackPen;
	
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
				COLORREF brushClr = CTetrisBlock::getColorByIndex(m_bArrTetrisColor[y][x]);
				HBRUSH tmpBrush = CreateSolidBrush(brushClr);
				HBRUSH tmpOldBrush = (HBRUSH)SelectObject(m_hdcDraw, tmpBrush);
				//draw the cell
				Rectangle(m_hdcDraw, left, top, right, bottom);

				SelectObject(m_hdcDraw, tmpOldBrush);
				DeleteObject(tmpBrush);
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
	
}

void CTetrisDraw::DrawScoreAndNextBlockArea(long score, CTetrisBlock* pNextTetrisBlock)
{
	//如果dcDraw为空，则不作任何绘图动作，给使用遗传算法等计算参数无需绘图的场景使用
	if (0 == m_hdcDraw)
		return;
	wchar_t sHanzi[100] = L"游戏得分";
	int nMargin = 5;
	int nYouXiDeFenTextHeight = 20;
	SetTextColor(m_hdcDraw, RGB(0, 0, 0));
	TextOut(m_hdcDraw, m_rectScoreAndNextBlockArea.left+ nMargin, m_rectScoreAndNextBlockArea.top+ nMargin, sHanzi, wcslen(sHanzi));
	HFONT hFont;
	HFONT hFontOld;
	int nFontHeight = 35;
	//CreateFont参考
	//https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-createfonta
	hFont = CreateFont(nFontHeight, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
	hFontOld = (HFONT)SelectObject(m_hdcDraw, hFont);
	SetTextColor(m_hdcDraw, RGB(0, 255, 0));
	RECT rectScore;
	rectScore.left = m_rectScoreAndNextBlockArea.left + nMargin;
	rectScore.top = m_rectScoreAndNextBlockArea.top + nMargin + nYouXiDeFenTextHeight;
	rectScore.right = m_rectScoreAndNextBlockArea.right + nMargin;
	rectScore.bottom = m_rectScoreAndNextBlockArea.top + nMargin + 2* nFontHeight;
	wchar_t s[100] = L"";
	swprintf(s, 100, L"%d", score);
	DrawText(m_hdcDraw, s, -1, &rectScore, DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
	SelectObject(m_hdcDraw, hFontOld);
	if (0 == pNextTetrisBlock)
		return;
	RECT rectNextBlock;
	rectNextBlock.left = m_rectScoreAndNextBlockArea.left + nMargin;
	rectNextBlock.top = rectScore.bottom + nMargin;
	rectNextBlock.right = m_rectScoreAndNextBlockArea.right + nMargin;
	rectNextBlock.bottom = m_rectScoreAndNextBlockArea.bottom - nMargin;

	const int border = 20;
	int cxClient = m_rectGameArea.right - m_rectGameArea.left + 1;
	int cyClient = m_rectGameArea.bottom - m_rectGameArea.top + 1;

	int BlockSizeX = (cxClient - 2 * border) / nTetrisBoardWidth;
	int BlockSizeY = (cyClient - 2 * border) / nTetrisBoardHeight;

	int rectNextBlockCenterPointX = (rectNextBlock.left + rectNextBlock.right)/2;
	int rectNextBlockCenterPointY = (rectNextBlock.top + rectNextBlock.bottom) / 2;

	HBRUSH  OldBrush, WhiteBrush;
	HPEN	NullPen, OldPen;

	NullPen = (HPEN)GetStockObject(NULL_PEN);
	COLORREF brushClr = CTetrisBlock::getColorByIndex(pNextTetrisBlock->getBlockColorIndex());
	HBRUSH blockBrush = CreateSolidBrush(brushClr);
	WhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	OldBrush = (HBRUSH)SelectObject(m_hdcDraw, blockBrush);
	OldPen = (HPEN)SelectObject(m_hdcDraw, NullPen);

	int nBlockHeight = pNextTetrisBlock->getBlockHeight();
	int nBlockWidth = pNextTetrisBlock->getBlockWidth();

	//先把rectNextBlock区域清空，再根据下一个Block的实际情况绘画
	FillRect(m_hdcDraw, &rectNextBlock, WhiteBrush);
	int nBlockDrawStartPointX = rectNextBlockCenterPointX - nBlockWidth  * BlockSizeX / 2;
	int nBlockDrawStartPointY = rectNextBlockCenterPointY - nBlockHeight * BlockSizeY / 2;

	for (int y = 0; y<nBlockHeight; y++)
	{
		for (int x = 0; x<nBlockWidth; x++)
		{
			//TetrisBlock x,y的增长方向和显示器的x,y增长方向一致
			int left = nBlockDrawStartPointX + (BlockSizeX * x);
			int right = left + BlockSizeX;

			int top = nBlockDrawStartPointY + (BlockSizeY * y);
			int bottom = top + BlockSizeY;

			//这里增加个判断，超出block的二维数组下标范围抛出异常
			if (pNextTetrisBlock->getBlockValue(y, x) == true)
			{
				//SelectObject(m_hdcDraw, BlackBrush);
				Rectangle(m_hdcDraw, left, top, right, bottom);
			}
		}
	}

	SelectObject(m_hdcDraw, OldBrush);
	SelectObject(m_hdcDraw, OldPen);
	DeleteObject(blockBrush);
	DeleteObject(hFont);
}

void CTetrisDraw::DrawGameHintArea()
{
	//如果dcDraw为空，则不作任何绘图动作，给使用遗传算法等计算参数无需绘图的场景使用
	if (0 == m_hdcDraw)
		return;
	TCHAR s[200] = TEXT("按键说明:\r\n A向左 S向下\r\nD向右 J变形\r\n Space 暂停/运行切换\r\n \r\n切换模式\r\n CTRL+A AI/手工模式切换\r\nR当前模式下重置游戏 ");
	//TextOut(m_hdcDraw, m_rectGameHintArea.left, m_rectGameHintArea.top, s, wcslen(s));
	HFONT hFont;
	HFONT hFontOld;
	//CreateFont参考
	//https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-createfonta
	hFont = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
	hFontOld=(HFONT)SelectObject(m_hdcDraw, hFont);
	SetTextColor(m_hdcDraw, RGB(100, 100, 100));
	//DrawText参考
	//https://docs.microsoft.com/en-us/previous-versions/aa911421(v=msdn.10)?redirectedfrom=MSDN
	DrawText(m_hdcDraw, s, -1, &m_rectGameHintArea, DT_NOCLIP| DT_CENTER);
	SelectObject(m_hdcDraw, hFontOld);
	DeleteObject(hFont);
}

void CTetrisDraw::SetTetrisArrayItem(int nHeight, int nWidth, bool bValue)
{
	m_bArrTetris[nHeight][nWidth] = bValue;
}
void CTetrisDraw::SetTetrisArrayItemColorIndex(int nHeight, int nWidth, int nColorIndex)
{
	m_bArrTetrisColor[nHeight][nWidth] = nColorIndex;
}
bool CTetrisDraw::GetTetrisArrayItem(int nHeight, int nWidth)
{
	return m_bArrTetris[nHeight][nWidth];
}

int CTetrisDraw::GetTetrisArrayItemColorIndex(int nHeight, int nWidth)
{
	return m_bArrTetrisColor[nHeight][nWidth];
}

void CTetrisDraw::DrawCurrentTetrisBlock(CTetrisBlock* pTetrisBlock)
{
	//如果dcDraw为空，则不作任何绘图动作，给使用遗传算法等计算参数无需绘图的场景使用
	if (0 == m_hdcDraw)
		return;
	if (0 == pTetrisBlock)
	{
		return;
	}
	const int border = 20;
	int cxClient = m_rectGameArea.right - m_rectGameArea.left + 1;
	int cyClient = m_rectGameArea.bottom - m_rectGameArea.top + 1;

	int BlockSizeX = (cxClient - 2 * border) / nTetrisBoardWidth;
	int BlockSizeY = (cyClient - 2 * border) / nTetrisBoardHeight;
	HBRUSH	OldBrush;
	HPEN	NullPen, OldPen;

	//grab a null pen so we can see the outlines of the cells
	NullPen = (HPEN)GetStockObject(NULL_PEN);

	//grab a brush to fill our cells with
	COLORREF brushClr = CTetrisBlock::getColorByIndex(pTetrisBlock->getBlockColorIndex());
	HBRUSH blockBrush = CreateSolidBrush(brushClr);

	//select them into the device conext
	OldBrush = (HBRUSH)SelectObject(m_hdcDraw, blockBrush);
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
	DeleteObject(blockBrush);
}

void CTetrisDraw::ClearCurrentTetrisBlock(CTetrisBlock* pTetrisBlock)
{
	//如果dcDraw为空，则不作任何绘图动作，给使用遗传算法等计算参数无需绘图的场景使用
	if (0 == m_hdcDraw)
		return;
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
	//如果dcDraw为空，则不作任何绘图动作，给使用遗传算法等计算参数无需绘图的场景使用
	if (0 == m_hdcDraw)
		return;
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

void CTetrisDraw::ClearTetrisArray()
{
	for (int y = 0; y < nTetrisBoardHeight; ++y)
	{
		for (int x = 0; x < nTetrisBoardWidth; ++x)
		{
			m_bArrTetris[y][x] = false;
		}
	}
}