#include "CParams.h"


//because we will always be loading in the settings from an ini file
//we can just initialize everything to zero

double CParams::dCrossoverRate      = 0;
double CParams::dMutationRate       = 0;
double CParams::dMaxPerturbation    = 0;
int CParams::iNumElite              = 0;
int CParams::iNumCopiesElite        = 0;

//this function loads in the parameters from a given file name. Returns
//false if there is a problem opening the file.
bool CParams::LoadInParameters(char* szFileName)
{
  ifstream grab(szFileName);

  //check file exists
  if (!grab)
  {
    return false;
  }

  //load in from the file
  char ParamDescription[40];

  grab >> ParamDescription;
  grab >> dCrossoverRate;
  grab >> ParamDescription;
  grab >> dMutationRate;
  grab >> ParamDescription;
  grab >> dMaxPerturbation;
  grab >> ParamDescription;
  grab >> iNumElite;
  grab >> ParamDescription;
  grab >> iNumCopiesElite;


  return true;
}
 




  
  
