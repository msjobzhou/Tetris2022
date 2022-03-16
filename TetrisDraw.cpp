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
	//��������ǲ��Դ��룬������ɾ��
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
	//����ٶ�m_rectWhole��left top�ǣ�0,0��
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

//�����DrawXXX��������ͼ����Ĭ����x��������������ӣ�y�����Ǵ�����������
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

	//��ͼ��Ϸ����߽�

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
			//������Ϸ����Ĭ�����½Ƕ�Ӧ���ǣ�0,0�������ϽǶ�Ӧ����(nTetrisBoardHeight��nTetrisBoardWidth)
			//��������и�����ʾ����ʾ����tetrisBoard�����ת����ϵ
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
	swprintf(s, 100, L"��Ϸ�÷�   %d", score);
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
			//������Ϸ����Ĭ�����½Ƕ�Ӧ���ǣ�0,0�������ϽǶ�Ӧ����(nTetrisBoardHeight��nTetrisBoardWidth)
			//��������и�����ʾ����ʾ����tetrisBoard�����ת����ϵ
			int left = border + (BlockSizeX * x);
			int right = left + BlockSizeX;

			int top = border + (BlockSizeY * (nTetrisBoardHeight - 1 - y));
			int bottom = top + BlockSizeY;

			//�������Ӹ��жϣ�����block�Ķ�ά�����±귶Χ�׳��쳣
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
			//������Ϸ����Ĭ�����½Ƕ�Ӧ���ǣ�0,0�������ϽǶ�Ӧ����(nTetrisBoardHeight��nTetrisBoardWidth)
			//��������и�����ʾ����ʾ����tetrisBoard�����ת����ϵ
			int left = border + (BlockSizeX * x);
			int right = left + BlockSizeX;

			int top = border + (BlockSizeY * (nTetrisBoardHeight - 1 - y));
			int bottom = top + BlockSizeY;

			//�������Ӹ��жϣ�����block�Ķ�ά�����±귶Χ�׳��쳣
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
LevelNumErased�������ã��ж�m_bArrTetris���Ƿ�ָ���Ĳ������Ƿ��п��������Ĳ㣬�����ؾ���Ĳ���
����п��������Ĳ㣬�������Ӧ�Ĳ㣬������һ�����ռס�������Ĳ�
*/
int CTetrisDraw::LevelNumErased(int nLevelStart, int nLevelEnd)
{
	//��block�������֮���жϷ���ռ�еĲ����Ƿ�������㣬���ڷ������ռ�Ĳ㣬���ָ�����������4�㣬�򷵻ش���
	if ((nLevelEnd - nLevelStart) > 4)
		return -1;
	if (nLevelEnd < nLevelStart)
		return -2;
	bool bArrayLevelErasable[4];//��¼nLevelStart��nLevelEnd��Ӧ�Ĳ����Ƿ񶼱�ռ������������
	int nHighestNonEmptyLevel = GetHighestNonEmptyLevel();//��¼�ж��Ƿ�����ǰ����߲���
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
			//�滭������ǰ�㣬���Ҫ����ض������㶯��Ч��ʱ���ã�������������Ϸ�����ػ棬��DrawGameAreaͳһ���
			//ClearLevel(nCurrentLevel);
		}
		else
		{
			//��ǰ�㲻����������Ҫ����nTotalLevelErasable��
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
		//����ǰԭnLevelEnd֮�ϵĲ�����nTotalLevelErasable��
		for (int m = nLevelEnd + 1; m <= nHighestNonEmptyLevel; m++)
		{
			for (int n = 0; n < nTetrisBoardWidth; n++)
			{
				m_bArrTetris[m - nTotalLevelErasable][n] = m_bArrTetris[m][n];
			}
		}
		//nHighestNonEmptyLevel�㼰����nTotalLevelErasable����false����
		for (int p = nHighestNonEmptyLevel; p > nHighestNonEmptyLevel - nTotalLevelErasable; p--)
		{
			for (int  q= 0; q < nTetrisBoardWidth; q++)
			{
				m_bArrTetris[p][q] = false;
			}
			//�滭������Ӧ�Ĳ㣬���Ҫ����ض������㶯��Ч��ʱ���ã�������������Ϸ�����ػ棬��DrawGameAreaͳһ���
			//ClearLevel(p);
		}
		//�ػ�����Ӱ�쵽�Ĳ���
		if (nLevelStart <= nHighestNonEmptyLevel)
		{
			DrawGameArea(nLevelStart, nHighestNonEmptyLevel);
		}
	}
	
	return nTotalLevelErasable;
}

//�õ��з���ռ�ݵ���߲����ţ���Χ�Ǵ�0��nTetrisBoardHeight-1���������0�㶼��û�з���ģ�����ȫ�������ˣ����򷵻�-1
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