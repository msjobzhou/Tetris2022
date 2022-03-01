#include "PierreDellacherieUnittest.h"


PierreDellacherieUnittest::PierreDellacherieUnittest()
{
}


PierreDellacherieUnittest::~PierreDellacherieUnittest()
{
}



void PierreDellacherieUnittest::addTestFunc(FuncPointer fp)
{
	if(fp!=0)
		m_testFuncList.push_back(fp);
}
void PierreDellacherieUnittest::runTest()
{
	list<FuncPointer>::iterator it = m_testFuncList.begin();
	while (it != m_testFuncList.end())
	{
		FuncPointer fp = *it;
		(*fp)();
	}
}

void test_getErodedPieceCellsMetric()
{
	
}
void test_getBoardRowTransitions()
{

}
void test_getBoardColumnTransitions()
{

}
void test_getBoardBuriedHoles()
{

}
void test_getBoardWells()
{

}