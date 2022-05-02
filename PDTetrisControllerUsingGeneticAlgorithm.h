#pragma once

#include <vector>

#include "PierreDellacherieTetrisController.h"
#include "GeneticAlgorithm.h"
#include "CParams.h"

using namespace std;

//ʹ���Ŵ��㷨����PierreDellacherie����˹��������㷨������ϵ��(Ȩ��)
class CPDTetrisControllerUsingGeneticAlgorithm
{
private:
	//storage for the population of genomes
	//ÿһ���˿ڵĻ���ÿһ����������Ӧ��ÿһ��PDTetrisControllers������ϵ��(Ȩ��)
	vector<CGenome>	     m_vecPopulation;
	int m_nPopulationSize;
	//m_vecPDTetrisControllers��m_vecPopulationһһ��Ӧ
	vector<CPierreDellacherieTetrisController*> m_vecPDTetrisControllersPointer;
	int m_nNumWeightInPDController;

	//����ѵ���Ķ���˹�������У����еĸ��嶼ʹ����һ�����н���ѵ��
	//���������ȡֵ�ڹ��캯����������ɣ�������m_nTetrisBlockNum���п���
	vector<int> m_vecTetrisBlockType;
	int m_nTetrisBlockNum;
	//��¼�Ŵ��㷨�Ѿ����е��ڶ��ٴ�
	int	m_nCurrentGeneration;

	//ָ���Ŵ��㷨��ָ��
	CGeneticAlgorithm* m_pGA;
	vector<double>		   m_vecAvFitness;
	vector<double>		   m_vecBestFitness;

public:
	CPDTetrisControllerUsingGeneticAlgorithm();
	~CPDTetrisControllerUsingGeneticAlgorithm();
	
	void RunTetrisGameSimulation(CPierreDellacherieTetrisController &pdController, double& dFitness);
	void MainProcess();
};

