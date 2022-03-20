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

//���ݶ���˹�������״�����Ӧ�����飬ע��ͬһ����״����ת֮��Ҫ�ͷ��ϵ����飬�����µ�����
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
			ppbBlock[i][j] = false; //false��ʾ����˹�����Ӧ��λ��Ϊ��
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
prepareRotate������ʾԤ��ת������һ����ת��ķ���������Ͻ�����ֵ����ת�󷽿��Ӧ�Ķ�ά����
������int&���͵Ĳ��������ú���ǰ��ʾ������תǰ�����Ͻ�����͸߶ȿ�ȣ����ú���֮���ʾ��ת�����Ӧֵ
����ֵbool**��ʾ��ת�󷽿��Ӧ�Ķ�ά���飬������prepareRotate��Ҫ�����ŵ���prepareRotateResourceClean
���ж�ά�������Դ�ͷ�
*/
bool** CTetrisBlock::prepareRotate(int& nPosX, int& nPosY, bool** ppbBlock, int& nBlockHeight, int& nBlockWidth)
{
	//����������е����òο�Rotate����
	int nPreBlockHeight = nBlockHeight;
	int nPreBlockWidth = nBlockWidth;
	bool **ppbRotateBlock;
	//��ת90��֮�󷽿�ĸ߶ȺͿ�Ȼ���
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
	//����ÿִ��һ�Σ�˳ʱ����ת90��

	int nPreBlockHeight = m_nBlockHeight;
	int nPreBlockWidth = m_nBlockWidth;
	bool **ppbPreBlock = m_ppbBlock;
	//��ת90��֮�󷽿�ĸ߶ȺͿ�Ȼ���
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
	//ɾ����תǰ�����Ӧ������
	deleteBlock2DArr(ppbPreBlock, nPreBlockHeight);

	//���¼�����ת֮��ķ��������Ͻǵ�����,��ת����������ͼ��ʾ����Z��״Ϊ��
	/*
	����   ������תǰ�����Ͻǵ�����Ϊ(x,y)�������½ǵ�����Ϊ(x+2,y-1)������y���Ǵ������µݼ���
	  ���� �ƹ㵽һ�����Σ������½ǵ�����Ϊ(x+nBlockWidth-1,y-nBlockHeight+1)
		   �������ĵ������Ϊ[(x+x+nBlockWidth-1)/2,(y+y-nBlockHeight+1)/2]������ߵ�Z��Ϊ�������ĵ��ڵ�1�е�2����
		   �͵�2�е�1�����м䣨˳��˵���£����Ͻǵ�����x,yָ���Ǳ�ʾ����Ķ�ά����ĵ�һ��Ԫ�ص�λ�ã����λ���ϲ�һ���С���
		   �����ĵ���ת����ζ�����ĵ㲻�䣬���������״����
      ��   �������ĵ㲻�䣬���㷽�������Ͻǵ�����xΪ(x+x+nBlockWidth-1)/2-nBlockHeight/2 + 1= x + (nBlockWidth-nBlockHeight-1)/2 + 1
	����   ���Ͻǵ�����yΪ(y+y-nBlockHeight+1)/2+nBlockWidth/2 -1= y + (nBlockWidth-nBlockHeight+1)/2-1
	��	   ͬ����ZΪ��������ת������ĵ��ڵ�2�еĵ�1�����͵�2�еĵ�2����֮��
		   ���⣬��Ҫע�������ת������Ͻ�������Ҫ����ȡ���������巽��Ϊ+0.5֮�����int���͵�ǿ��ת��
	*/

	m_nPosX = m_nPosX + int(nPreBlockWidth - nPreBlockHeight - 1) / 2 + 1;
	m_nPosY = m_nPosY + int(nPreBlockWidth - nPreBlockHeight + 1) / 2 - 1;
}
void CTetrisBlock::generateBlockShapeRandomOrientation(int nType, int nOrientation)
{
	//nType����˹��������ͣ�����1����Z 2����Z 3��T 4����L 5����L 6��I  7��O
	m_nBlockHeight = 0;
	m_nBlockWidth = 0;
	int nMaxOrientation = 0;
	m_nType = nType;

	int nRandNum = nOrientation;
	if(-1 == nRandNum)
	{
		srand((int)time(NULL));  // �����������
		nRandNum = rand();
	}
	//�����������˹�������ɫ
	srand((int)time(NULL));  // �����������
	int nColorIndex = rand() % nColorTableLen;
	m_BlockColorIndex = nColorIndex;
	
	switch (nType)
	{
	case 1:
		//Z��״�����ַ���Orientation
		nMaxOrientation = 2;
		//�˴�Ϊ����1����һ�еĵ�һ����Ϊtrue���ڶ��еĵڶ�����Ϊtrue
		//����
		//  ����
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
		{//�˴�Ϊ����2����һ�еĵڶ���Ϊtrue���ڶ��еĵ�һ����Ϊtrue�������е�һ��Ϊtrue
		//  ��
		//����
		//��
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
		//��Z��״�����ַ���Orientation
		//  ����
		//����
		nMaxOrientation = 2;
		//�˴�Ϊ����1����һ�еĵڶ�����Ϊtrue���ڶ��еĵ�һ����Ϊtrue
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
		{//�˴�Ϊ����2����һ�еĵ�һ��Ϊtrue���ڶ��еĵ�һ����Ϊtrue�������е�����Ϊtrue
		 //��
		 //����
		 //  ��
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
		//T��״��4�ַ���Orientation
		nMaxOrientation = 4;
		//�˴�Ϊ����1����һ�еĵ�һ������Ϊtrue���ڶ��еĵڶ���Ϊtrue
		//������
		//  ��
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
		{//�˴�Ϊ����2����һ�еĵڶ���Ϊtrue���ڶ��еĵ�һ����Ϊtrue�������е�һ��Ϊtrue
		 //  ��
		 //����
		 //  ��
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[2][1] = true;
		}
		else if (2 == nRandNum % nMaxOrientation)
		{//�˴�Ϊ����3����һ�еĵڶ���Ϊtrue���ڶ��еĵ�һ������Ϊtrue
		 //  ��
		 //������
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[1][2] = true;
		}
		else if (3 == nRandNum % nMaxOrientation)
		{//�˴�Ϊ����4����һ�еĵ�һ��Ϊtrue���ڶ��еĵ�һ����Ϊtrue�������е�һ��Ϊtrue
		 //��
		 //����
		 //��
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
		//��L��״��4�ַ���Orientation
		nMaxOrientation = 4;
		//�˴�Ϊ����1����һ�еĵ�һ��Ϊtrue���ڶ��еĵ�һ��Ϊtrue�������е�һ����Ϊtrue
		//��
		//��
		//����
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
		{//�˴�Ϊ����2����һ�еĵ�һ������Ϊtrue���ڶ��еĵ�һ��Ϊtrue
		 //������
		 //��
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
		{//�˴�Ϊ����3����һ�еĵ�һ����Ϊtrue���ڶ��еĵڶ���Ϊtrue�������еڶ���Ϊtrue
		 //����
		 //  ��
		 //  ��
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[2][1] = true;
		}
		else if (3 == nRandNum % nMaxOrientation)
		{//�˴�Ϊ����4����һ�еĵڶ���Ϊtrue���ڶ��еĵ�һ����Ϊtrue�������е�һ��Ϊtrue
		 //    ��
		 //������
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
		//��L��״��4�ַ���Orientation
		nMaxOrientation = 4;
		//�˴�Ϊ����1����һ�еĵڶ���Ϊtrue���ڶ��еĵڶ���Ϊtrue�������е�һ����Ϊtrue
		//  ��
		//  ��
		//����
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
		{//�˴�Ϊ����2����һ�еĵ�һ��Ϊtrue����һ�еĵ�һ������Ϊtrue
		 //��
		 //������ 
			m_nBlockHeight = 2;
			m_nBlockWidth = 3;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[1][1] = true;
			m_ppbBlock[1][2] = true;
		}
		else if (2 == nRandNum % nMaxOrientation)
		{//�˴�Ϊ����3����һ�еĵ�һ����Ϊtrue���ڶ��еĵ�һ��Ϊtrue�������е�һ��Ϊtrue
		 //����
		 //��
		 //��
			m_nBlockHeight = 3;
			m_nBlockWidth = 2;
			m_ppbBlock = newBlock2DArr(m_nBlockHeight, m_nBlockWidth);
			m_ppbBlock[0][0] = true;
			m_ppbBlock[0][1] = true;
			m_ppbBlock[1][0] = true;
			m_ppbBlock[2][0] = true;
		}
		else if (3 == nRandNum % nMaxOrientation)
		{//�˴�Ϊ����4����һ�еĵ�һ������Ϊtrue���ڶ��еĵ�����Ϊtrue
		 //������
		 //    ��
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
		//I��״�����ַ���Orientation
		nMaxOrientation = 2;
		//�˴�Ϊ����1����һ�еĵ�һ��������Ϊtrue
		//��������
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
		{//�˴�Ϊ����2����һ�������еĵ�һ��Ϊtrue
		 //��
		 //��
		 //��
		 //��
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
		//O��״��1�ַ���Orientation
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
	//����block�ĸ߶Ⱥ͸߶��������ʼλ��
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