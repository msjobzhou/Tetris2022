#include "ArrayIndexRangeCheck.h"

#include <sstream>
#include <cstdio>

#include <assert.h>

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
	
	//ʹ��string����������̫�����ȥ��string��ʹ�ã������г���ʹ��sprintf_s������Ҳͦ��
	//string strInfo = string("IndexRangeCheck:");

	//�ڶ���д��sprintf_s����ش���
	//char strInfo[1024] = { '\0' };
	
	//stringstream ss;
	
	if ((nRowIndex >= m_nRowIndexRange) || (nRowIndex < 0))
	{
		//ss << "Row index " << nRowIndex << " out of limit " << m_nRowIndexRange;
		//string expectionMsg = ss.str();
		//strInfo = strInfo + expectionMsg;
		//ss.clear();
		//ss.str("");

		//�ڶ���д��sprintf_s����ش���
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


		//�ڶ���д��sprintf_s����ش���
		//sprintf_s(strInfo, sizeof(strInfo), "IndexRangeCheck: Column index %d out of limit %d", nColIndex, m_nColIndexRange);


		//throw out_of_range(strInfo);
		assert(nColIndex < m_nColIndexRange);
	}
}
