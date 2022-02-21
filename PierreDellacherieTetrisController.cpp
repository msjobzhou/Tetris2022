#include "PierreDellacherieTetrisController.h"



CPierreDellacherieTetrisController::CPierreDellacherieTetrisController(CTetrisDraw* pTetrisDraw, CTetrisBlock* pTetrisBlock = 0):
	CTetrisController(pTetrisDraw, pTetrisBlock)
{

}


CPierreDellacherieTetrisController::~CPierreDellacherieTetrisController()
{
	CTetrisController::~CTetrisController();
}

int CPierreDellacherieTetrisController::getLandingHeight()
{

}
int CPierreDellacherieTetrisController::getErodedPieceCellsMetric()
{

}
int CPierreDellacherieTetrisController::getBoardRowTransitions()
{

}
int CPierreDellacherieTetrisController::getBoardColumnTransitions()
{

}
int CPierreDellacherieTetrisController::getBoardBuriedHoles()
{

}
int CPierreDellacherieTetrisController::getBoardWells()
{

}