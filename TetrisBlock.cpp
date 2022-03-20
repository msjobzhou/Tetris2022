#include "TetrisBlock.h"
#include "defines.h"
#include <cstdlib>
#include <ctime>

using namespace std;

COLORREF CTetrisBlock::ColorTable[8] = { RGB(255,0,0),RGB(255,255,0),RGB(255,0,255),RGB(0,255,255),RGB(255,100,0),RGB(100,255,0),RGB(125,255,120),RGB(220,120,255) };

CTetrisBlock::CTetrisBlock(int nType, int nOrientation)
{
	generateBlockShapeRandomOrientation(nType, nOrientation);
}


CTetrisBlock::~CTetrisBlock()
{
	deleteBlock2DArr(m_ppbBlock, m_nBlockHeight);
}

//根据俄罗斯方块的形状申请对应的数组，注意同一个形状，旋转之后要释放老的数组，申请新的数组
bool**  CTetrisBlock::newBlock2DArr(int nRow, int nCol)
{
	bool **ppbBlock = new bool *[nRow];
	for (int i = 0; i < nRow; i++)
	{
		ppbBlock[i] = new bool[nCol];
	}

	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			ppbBlock[i][j] = false; //false表示俄罗斯方块对应的位置为空
		}	
	}
	return ppbBlock;
}

void CTetrisBlock::deleteBlock2DArr(bool **ppbBlock, int nRow)
{
	//delete 2d array
	for (int i = 0; i < nRow; i++)
	{
		delete[] ppbBlock[i];
	}
	delete[] ppbBlock;
}

bool CTetrisBlock::getBlockValue(int nRow, int nCol)
{
	if (nRow<0 || nRow >= m_nBlockHeight)
		return false;
	if (nCol<0 || nCol >= m_nBlockWidth)
		return false;
	return m_ppbBlock[nRow][nCol];
}

int CTetrisBlock::getBlockHeight()
{
	return m_nBlockHeight;
}

int CTetrisBlock::getBlockWidth()
{
	return m_nBlockWidth;
}

int CTetrisBlock::getBlockPosX()
{
	return m_nPosX;
}

int CTetrisBlock::getBlockPosY()
{
	return m_nPosY;
}

bool** CTetrisBlock::getBlock2DArr()
{
	return m_ppbBlock;
}

void CTetrisBlock::setBlockHeight(int nHeight)
{
	m_nBlockHeight = nHeight;
}

void CTetrisBlock::setBlockWidth(int nWidth)
{
	m_nBlockWidth = nWidth;
}

/*
prepareRotate函数表示预旋转，返回一个旋转后的方块的最左上角坐标值和旋转后方块对应的二维数组
函数中int&类型的参数，调用函数前表示方块旋转前的左上角坐标和高度宽度，调用函数之后表示旋转后的相应值
返回值bool**表示旋转后方块对应的二维数组，调用了prepareRotate，要紧接着调用prepareRotateResourceClean
进行二维数组的资源释放
*/
bool** CTetrisBlock::prepareRotate(int& nPosX, int& nPosY, bool** ppbBlock, int& nBlockHeight, int& nBlockWidth)
{
	//各代码语句中的作用参考Rotate函数
	int nPreBlockHeight = nBlockHeight;
	int nPreBlockWidth = nBlockWidth;
	bool **ppbRotateBlock;
	//旋转90度之后方块的高度和宽度互换
	nBlockHeight = nPreBlockWidth;
	nBlockWidth = nPreBlockHeight;

	ppbRotateBlock = newBlock2DArr(nBlockHeight, nBlockWidth);

	for (int i = 0; i < nPreBlockHeight; i++)
	{
		for (int j = 0; j < nPreBlockWidth; j++)
		{
			ppbRotateBlock[j][nPreBlockHeight - i - 1] = ppbBlock[i][j];
		}
	}

	nPosX = nPosX + int(nPreBlockWidth - nPreBlockHeight - 1) / 2 + 1;
	nPosY = nPosY + int(nPreBlockWidth - nPreBlockHeight + 1) / 2 - 1;

	return ppbRotateBlock;
}

void CTetrisBlock::prepareRotateResourceClean(bool **ppbBlock, int nRow)
{
	deleteBlock2DArr(ppbBlock, nRow);
}

void CTetrisBlock::Rotate()
{
	//函数每执行一次，顺时针旋转90度

	int nPreBlockHeight = m_nBlockHeight;
	int nPreBlockWidth = m_nBlockWidth;
	bool **ppbPreBlock = m_ppbBlock;
	//旋转90度之后方块的高度和宽度互换
	m_nBlockHeight = nPreBlockWidth;
	m_nBlockWidth = nPreBlockHeight;

	m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);

	for (int i = 0; i < nPreBlockHeight; i++)
	{
		for (int j = 0; j < nPreBlockWidth; j++)
		{
			m_ppbBlock[j][nPreBlockHeight-i-1] = ppbPreBlock[i][j];
		}
	}
	//删除旋转前方块对应的数组
	deleteBlock2DArr(ppbPreBlock, nPreBlockHeight);

	//重新计算旋转之后的方块最左上角的坐标,旋转的中心如下图所示，以Z形状为例
	/*
	■■   假设旋转前最左上角的坐标为(x,y)，最右下角的坐标为(x+2,y-1)，其中y轴是从上往下递减的
	  ■■ 推广到一般情形，最右下角的坐标为(x+nBlockWidth-1,y-nBlockHeight+1)
		   则其中心点的坐标为[(x+x+nBlockWidth-1)/2,(y+y-nBlockHeight+1)/2]，以左边的Z行为例，中心点在第1行第2个■
		   和第2行第1个■中间（顺便说明下，左上角的坐标x,y指的是表示方块的二维数组的第一个元素的位置，这个位置上不一定有■）
		   绕中心点旋转的意味着中心点不变，则旋后的形状如下
      ■   根据中心点不变，计算方块最左上角的坐标x为(x+x+nBlockWidth-1)/2-nBlockHeight/2 + 1= x + (nBlockWidth-nBlockHeight-1)/2 + 1
	■■   左上角的坐标y为(y+y-nBlockHeight+1)/2+nBlockWidth/2 -1= y + (nBlockWidth-nBlockHeight+1)/2-1
	■	   同样以Z为例，其旋转后的中心点在第2行的第1个■和第2行的第2个■之间
		   另外，需要注意的是旋转后的左上角坐标需要经过取整处理，具体方法为+0.5之后进行int类型的强制转换
	*/

	m_nPosX = m_nPosX + int(nPreBlockWidth - nPreBlockHeight - 1) / 2 + 1;
	m_nPosY = m_nPosY + int(nPreBlockWidth - nPreBlockHeight + 1) / 2 - 1;
}
void CTetrisBlock::generateBlockShapeRandomOrientation(int nType, int nOrientation)
{
	//nType俄罗斯方块的类型，共有1：左Z 2：右Z 3：T 4：左L 5：右L 6：I  7：O
	m_nBlockHeight = 0;
	m_nBlockWidth = 0;
	int nMaxOrientation = 0;
	m_nType = nType;

	int nRandNum = nOrientation;
	if(-1 == nRandNum)
	{
		srand((int)time(NULL));  // 产生随机种子
		nRandNum = rand();
	}
	//随机产生俄罗斯方块的颜色
	srand((int)time(NULL));  // 产生随机种子
	int nColorIndex = rand() % nColorTableLen;
	m_BlockColorIndex = nColorIndex;
	
	switch (nType)
	{
	case 1:
		//Z形状，两种方向Orientation
		nMaxOrientation = 2;
		//此处为方向1，第一行的第一二列为true，第二行的第二三列为true
		//■■
		//  ■■
		if (0 == nRandNum % nMaxOrientation)
		{
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[1][2] = true;
			
		}
		else if (1 == nRandNum % nMaxOrientation)
		{//此处为方向2，第一行的第二列为true，第二行的第一二列为true，第三行第一列为true
		//  ■
		//■■
		//■
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[2][0] = true;
		}
		break;
	case 2:
		//右Z形状，两种方向Orientation
		//  ■■
		//■■
		nMaxOrientation = 2;
		//此处为方向1，第一行的第二三列为true，第二行的第一二列为true
		if (0 == nRandNum % nMaxOrientation)
		{
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][1] = true;
			m_ppbBlock[0][2] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;

		}
		else if (1 == nRandNum % nMaxOrientation)
		{//此处为方向2，第一行的第一列为true，第二行的第一二列为true，第三行第三列为true
		 //■
		 //■■
		 //  ■
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[2][1] = true;
		}
		break;
	case 3:
		//T形状，4种方向Orientation
		nMaxOrientation = 4;
		//此处为方向1，第一行的第一二三列为true，第二行的第二列为true
		//■■■
		//  ■
		if (0 == nRandNum % nMaxOrientation)
		{
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[0][2] = true;
			m_ppbBlock[1][1] = true;

		}
		else if (1 == nRandNum % nMaxOrientation)
		{//此处为方向2，第一行的第二列为true，第二行的第一二列为true，第三行第一列为true
		 //  ■
		 //■■
		 //  ■
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[2][1] = true;
		}
		else if (2 == nRandNum % nMaxOrientation)
		{//此处为方向3，第一行的第二列为true，第二行的第一二三列为true
		 //  ■
		 //■■■
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[1][2] = true;
		}
		else if (3 == nRandNum % nMaxOrientation)
		{//此处为方向4，第一行的第一列为true，第二行的第一二列为true，第三行第一列为true
		 //■
		 //■■
		 //■
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[2][0] = true;
		}
		break;
	case 4:
		//左L形状，4种方向Orientation
		nMaxOrientation = 4;
		//此处为方向1，第一行的第一列为true，第二行的第一列为true，第三行的一二列为true
		//■
		//■
		//■■
		if (0 == nRandNum % nMaxOrientation)
		{
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[2][0] = true;
			m_ppbBlock[2][1] = true;

		}
		else if (1 == nRandNum % nMaxOrientation)
		{//此处为方向2，第一行的第一二三列为true，第二行的第一列为true
		 //■■■
		 //■
		 //  
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[0][2] = true;
			m_ppbBlock[1][0] = true;
		}
		else if (2 == nRandNum % nMaxOrientation)
		{//此处为方向3，第一行的第一二列为true，第二行的第二列为true，第三行第二列为true
		 //■■
		 //  ■
		 //  ■
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[2][1] = true;
		}
		else if (3 == nRandNum % nMaxOrientation)
		{//此处为方向4，第一行的第二列为true，第二行的第一二列为true，第三行第一列为true
		 //    ■
		 //■■■
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][2] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[1][2] = true;
		}
		break;
	case 5:
		//右L形状，4种方向Orientation
		nMaxOrientation = 4;
		//此处为方向1，第一行的第二列为true，第二行的第二列为true，第三行的一二列为true
		//  ■
		//  ■
		//■■
		if (0 == nRandNum % nMaxOrientation)
		{
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[2][0] = true;
			m_ppbBlock[2][1] = true;

		}
		else if (1 == nRandNum % nMaxOrientation)
		{//此处为方向2，第一行的第一列为true，第一行的第一二三列为true
		 //■
		 //■■■ 
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[1][2] = true;
		}
		else if (2 == nRandNum % nMaxOrientation)
		{//此处为方向3，第一行的第一二列为true，第二行的第一列为true，第三行第一列为true
		 //■■
		 //■
		 //■
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[2][0] = true;
		}
		else if (3 == nRandNum % nMaxOrientation)
		{//此处为方向4，第一行的第一二三列为true，第二行的第三列为true
		 //■■■
		 //    ■
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[0][2] = true;
			m_ppbBlock[1][2] = true;
		}
		break;
	case 6:
		//I形状，两种方向Orientation
		nMaxOrientation = 2;
		//此处为方向1，第一行的第一二三四列为true
		//■■■■
		//  
		if (0 == nRandNum % nMaxOrientation)
		{
			m_nBlockHeight = 1;
			m_nBlockWidth = 4;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[0][2] = true;
			m_ppbBlock[0][3] = true;

		}
		else if (1 == nRandNum % nMaxOrientation)
		{//此处为方向2，第一二三四行的第一列为true
		 //■
		 //■
		 //■
		 //■
			m_nBlockHeight = 4;
			m_nBlockWidth = 1;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[2][0] = true;
			m_ppbBlock[3][0] = true;
		}
		break;
	case 7:
		//O形状，1种方向Orientation
		nMaxOrientation = 1;
		m_nBlockHeight = 2;
		m_nBlockWidth = 2;
		m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
		m_ppbBlock[0][0] = true;
		m_ppbBlock[0][1] = true;
		m_ppbBlock[1][0] = true;
		m_ppbBlock[1][1] = true;
		break;
	default:
		break;
	}
	m_nMaxOrientation = nMaxOrientation;
	//根据block的高度和高度设置其初始位置
	m_nPosX = int(nTetrisBoardWidth / 2 - 1 - m_nBlockWidth / 2);
	m_nPosY = nTetrisBoardHeight - 1 + m_nBlockHeight - 1;
	//m_nPosY = 19;
}

void CTetrisBlock::left()
{
	m_nPosX--;
}

void CTetrisBlock::right()
{
	m_nPosX++;
}

void CTetrisBlock::down()
{
	m_nPosY--;
}

int CTetrisBlock::getBlockMaxOrientation()
{
	return m_nMaxOrientation;
}

COLORREF CTetrisBlock::getColorByIndex(int nIndex)
{
	nIndex = nIndex%nColorTableLen;
	return ColorTable[nIndex];
}

int CTetrisBlock::getBlockColorIndex()
{
	return m_BlockColorIndex;
}