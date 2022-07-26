#pragma once

#include<list>
#include<assert.h>
#include"PierreDellacherieTetrisController.h"

using std::list;

typedef void(*FuncPointer)();

class PierreDellacherieUnittest
{
private:
	list<FuncPointer> m_testFuncList;

public:

public:
	PierreDellacherieUnittest();
	~PierreDellacherieUnittest();
	void addTestFunc(FuncPointer fp);
	void runTest();
};

void test_getErodedPieceCellsMetric();
void test_getBoardRowTransitions();
void test_getBoardColumnTransitions();
void test_getBoardBuriedHoles();
void test_getBoardWells();
void test_pickPositionWithHighestEvalutionScore();

void test_RotateTetrisBlock();
void test_generateAICommandListForCurrentTetrisBlock();
void test_evaluationFunction();

void test_findRectangularPath();

void test_generateAICommandListForCurrentTetrisBlockWithTheKnowledgeOfNextTetrisBlock();

void test2_generateAICommandListForCurrentTetrisBlockWithTheKnowledgeOfNextTetrisBlock();