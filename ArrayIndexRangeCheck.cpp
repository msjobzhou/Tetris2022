#include "ArrayIndexRangeCheck.h"

#include <sstream>
using namespace std;

//nRowBound表示实际的长度，比最大序号多1
ArrayIndexRangeCheck::ArrayIndexRangeCheck(int nRowBound, int nColBound)
{
	m_nRowIndexRange = nRowBound;
	m_nColIndexRange = nColBound;
}


ArrayIndexRangeCheck::~ArrayIndexRangeCheck()
{
}

//nRowIndex从0开始
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
