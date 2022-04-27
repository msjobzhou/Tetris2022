#pragma once

#include <stdexcept> 

using namespace std;


class ArrayIndexRangeCheck
{

private:
	int m_nRowIndexRange;
	int m_nColIndexRange;

public:
	//构造函数设置数组的行与列的长度范围
	ArrayIndexRangeCheck(int nRowBound, int nColBound);
	~ArrayIndexRangeCheck();

	void IndexRangeCheck(int nRowIndex, int nColIndex);
};

