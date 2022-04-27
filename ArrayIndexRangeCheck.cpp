#include "ArrayIndexRangeCheck.h"

#include <sstream>
using namespace std;

//nRowBound��ʾʵ�ʵĳ��ȣ��������Ŷ�1
ArrayIndexRangeCheck::ArrayIndexRangeCheck(int nRowBound, int nColBound)
{
	m_nRowIndexRange = nRowBound;
	m_nColIndexRange = nColBound;
}


ArrayIndexRangeCheck::~ArrayIndexRangeCheck()
{
}

//nRowIndex��0��ʼ
void ArrayIndexRangeCheck::IndexRangeCheck(int nRowIndex, int nColIndex)
{
	string strInfo = string("IndexRangeCheck:");
	stringstream ss;
	
	if ((nRowIndex >= m_nRowIndexRange) || (nRowIndex < 0))
	{
		ss << "Row index " << nRowIndex << " out of limit " << m_nRowIndexRange;
		string expectionMsg = ss.str();
		strInfo = strInfo + expectionMsg;
		ss.clear();
		ss.str("");

		throw out_of_range(strInfo);
	}

	if ((nColIndex >= m_nColIndexRange) || (nColIndex < 0))
	{
		ss << "Column index " << nColIndex << " out of limit " << m_nColIndexRange;
		string expectionMsg = ss.str();
		strInfo = strInfo + expectionMsg;
		ss.clear();
		ss.str("");

		throw out_of_range(strInfo);
	}
}
