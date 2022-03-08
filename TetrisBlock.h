#pragma once
class CTetrisBlock
{
private:
	int m_nPosX;
	int m_nPosY;
	int m_nType;
	int m_nBlockHeight, m_nBlockWidth;
	bool **m_ppbBlock;
	int m_nMaxOrientation;

protected:
	bool** newBlock2DArr(int nRow, int nCol);
	void deleteBlock2DArr(bool **ppbBlock, int nRow);
	void generateBlockShapeRandomOrientation(int nType);

public:
	CTetrisBlock(int nType);
	~CTetrisBlock();
	bool** prepareRotate(int& nPosX, int& nPosY, bool** ppbBlock, int& nBlockHeight, int& nBlockWidth);
	void prepareRotateResourceClean(bool **ppbBlock, int nRow);
	void Rotate();
	int getBlockHeight();
	int getBlockWidth();
	int getBlockPosX();
	int getBlockPosY();
	int getBlockMaxOrientation();
	bool** getBlock2DArr();
	void setBlockHeight(int nHeight);
	void setBlockWidth(int nWidth);
	void left();
	void right();
	void down();
	bool getBlockValue(int nRow, int nCol);
};

