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
	srand((int)time(NULL));  // �����������
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
		srand((int)time(NULL));  // �����������
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
	//�����û�ָ���жϵ�ǰ�����Ƿ������ת�����ƶ���λ��(x,y),x,y��0��ʼ
	//���ж���ת�����ƶ��Ƿ�ᳬ��TetrisBoard��Ϸ��֮��
	//��ʹ����ת�����ƶ�֮��ķ���2ά�����CTetrisDraw.m_bArrTetris�е�Ԫ�ؽ����߼������
	bool **ppbBlockArr = pBlock->getBlock2DArr();
	for (int i = 0; i < pBlock->getBlockHeight(); i++)
	{
		for (int j = 0; j < pBlock->getBlockWidth(); j++)
		{
			//�ƶ�������ת�������ұ߽���±߽磬���ﲻ�ж��Ƿ񳬳��ϱ߽磬��Ҫ�Ǹ�block�ճ���ʱ�ĳ�ʼλ�ÿ������ϱ߽����й�
			if ((x + j) < 0 || (x + j) >= nTetrisBoardWidth || (y - i) < 0)
				return false;
			//�ƶ�������ת֮��ķ����TetrisArray���κ�һ��λ�ö���ռ�õ�����£�����false
			if (true == ppbBlockArr[i][j] && m_pTetrisDraw->GetTetrisArrayItem(y - i, x + j))
				return false;
		}
	}
	return true;
}

bool CTetrisController::canTetrisBlockMovable(bool **ppbBlockArr, int nHeight, int nWidth, int x, int y)
{
	//�������ܲο�canTetrisBlockMovable(CTetrisBlock* pBlock, int x, int y)
	for (int i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			//�ƶ�������ת�������ұ߽���±߽磬���ﲻ�ж��Ƿ񳬳��ϱ߽磬��Ҫ�Ǹ�block�ճ���ʱ�ĳ�ʼλ�ÿ������ϱ߽����й�
			if ((x + j) < 0 || (x + j) >= nTetrisBoardWidth || (y - i) < 0)
				return false;
			//�ƶ�������ת֮��ķ����TetrisArray���κ�һ��λ�ö���ռ�õ�����£�����false
			if (true == ppbBlockArr[i][j] && m_pTetrisDraw->GetTetrisArrayItem(y - i, x + j))
				return false;
		}
	}
	return true;
}
void CTetrisController::setBlockOccupiedTetrisArray(bool bVal)
{
	//���õ�ǰblockռ�ݵ�����gameArea��ע��posx��Ӧ��λ������Ļ��x������tertis��ά������л���˵��width
	//bVal�������ó�true��false����Ϊtrueʱ��m_pTetrisDraw��Ӧ��tetris����ᱻ���ó�ռλ����Ϊfalseʱ����tetris����
	//�ж�Ӧ��λ��ԭ�������÷�����Ϊm_pTetrisDraw�Ļ�ͼ��ʽ�Ǹ���tetris�����Ƿ�ռλ���л��ƣ��������block���ܼ����½��Ļ�
	//����Ҫ����setBlockOccupiedTetrisArray(false)�����黹ԭ
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
	//ִ��ʵ�ʵ������ƶ�ָ���TetrisDraw�ػ�
	//�������ǰ��block
	m_pTetrisDraw->ClearCurrentTetrisBlock(m_pCurTetrisBlock);
	m_pCurTetrisBlock->left();
	//���ػ�ִ��������֮���block
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
	//ִ��ʵ�ʵ������ƶ�ָ���TetrisDraw�ػ�
	//�������ǰ��block
	m_pTetrisDraw->ClearCurrentTetrisBlock(m_pCurTetrisBlock);
	m_pCurTetrisBlock->right();
	//���ػ�ִ��������֮���block
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
	//�����Դ
	m_pCurTetrisBlock->prepareRotateResourceClean(ppbRotateBlock, nBlockHeight);
	if (false == bRet)
	{
		return false;
	}
	//ִ��ʵ�ʵ���ת�ƶ�ָ���TetrisDraw�ػ�	
	//�������ǰ��block
	m_pTetrisDraw->ClearCurrentTetrisBlock(m_pCurTetrisBlock);
	m_pCurTetrisBlock->Rotate();
	//���ػ�ִ��������֮���block
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
		//�����������ߵĳ�����block��ֹͣ�ƶ������ʱ��Ҫ����gameArea��Ӧ�Ķ�ά���飬֮���ж��Ƿ��п�������
		setBlockOccupiedTetrisArray(true);
		int nNum = m_pTetrisDraw->LevelNumErased(nCurBlkPosY- m_pCurTetrisBlock->getBlockHeight() + 1, nCurBlkPosY);
		//����һ��������Ŀ��Ӧ�ĵ÷�
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
	//ִ��ʵ�ʵ������ƶ�ָ���TetrisDraw�ػ�
	//�������ǰ��block
	m_pTetrisDraw->ClearCurrentTetrisBlock(m_pCurTetrisBlock);
	m_pCurTetrisBlock->down();
	//���ػ�ִ��������֮���block
	m_pTetrisDraw->DrawCurrentTetrisBlock(m_pCurTetrisBlock);

	//���·���
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