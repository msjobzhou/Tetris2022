#pragma once

#include <stdexcept> 

using namespace std;


class ArrayIndexRangeCheck
{

private:
	int m_nRowIndexRange;
	int m_nColIndexRange;

public:
	//���캯����������������еĳ��ȷ�Χ
	ArrayIndexRangeCheck(int nRowBound, int nColBound);
	~ArrayIndexRangeCheck();

	void IndexRangeCheck(int nRowIndex, int nColIndex);
};

