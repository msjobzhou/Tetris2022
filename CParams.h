#ifndef CPARAMS_H
#define CPARAMS_H
//------------------------------------------------------------------------
//
//	Name: CParams.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Class to hold all the parameters used in this project. The values
//        are loaded in from an ini file when an instance of the class is
//        created.
//       
//
//------------------------------------------------------------------------
#include <fstream>
#include <windows.h>

using namespace std;	



class CParams
{

public:

  //---------------------------------------GA parameters
  static double dCrossoverRate;
  static double dMutationRate;

  //the maximum amount the ga may mutate each weight by
  static double dMaxPerturbation;

  //used for elitism
  static int    iNumElite;
  static int    iNumCopiesElite;

  //ctor
  CParams()
  {
    if(!LoadInParameters("params.ini"))
    {
      MessageBox(NULL, TEXT("Cannot find ini file!"), TEXT("Error"), 0);
    }

  }

  bool LoadInParameters(char* szFileName);
};






#endif