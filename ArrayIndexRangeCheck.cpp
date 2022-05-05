#include "ArrayIndexRangeCheck.h"

#include <sstream>
#include <cstdio>

#include <assert.h>

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
	
	//使用string的性能消耗太大，因此去掉string的使用，过程中尝试使用sprintf_s的消耗也挺大
	//string strInfo = string("IndexRangeCheck:");

	//第二种写法sprintf_s的相关代码
	//char strInfo[1024] = { '\0' };
	
	//stringstream ss;
	
	if ((nRowIndex >= m_nRowIndexRange) || (nRowIndex < 0))
	{
		//ss << "Row index " << nRowIndex << " out of limit " << m_nRowIndexRange;
		//string expectionMsg = ss.str();
		//strInfo = strInfo + expectionMsg;
		//ss.clear();
		//ss.str("");

		//第二种写法sprintf_s的相关代码
		//sprintf_s(strInfo, sizeof(strInfo), "IndexRangeCheck: Row index %d out of limit %d", nRowIndex, m_nRowIndexRange);

		//throw out_of_range(strInfo);
		assert(nRowIndex < m_nRowIndexRange);
	}

	if ((nColIndex >= m_nColIndexRange) || (nColIndex < 0))
	{
		//ss << "Column index " << nColIndex << " out of limit " << m_nColIndexRange;
		//string expectionMsg = ss.str();
		//strInfo = strInfo + expectionMsg;
		//ss.clear();
		//ss.str("");


		//第二种写法sprintf_s的相关代码
		//sprintf_s(strInfo, sizeof(strInfo), "IndexRangeCheck: Column index %d out of limit %d", nColIndex, m_nColIndexRange);


		//throw out_of_range(strInfo);
		assert(nColIndex < m_nColIndexRange);
	}
}
