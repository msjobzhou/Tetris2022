//#define WIN32_LEAN_AND_MEAN

#include <windows.h>   
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

#include "defines.h"
#include "TetrisDraw.h"
#include "TetrisBlock.h"
#include "TetrisController.h"
#include "PierreDellacherieTetrisController.h"

#include "PierreDellacherieUnittest.h"

#include <list>

#include "FileLogger.h"
#include <sstream>

#include <stdlib.h>  
#include <crtdbg.h>  

#include "PDTetrisControllerUsingGeneticAlgorithm.h"

#ifdef _DEBUG  
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)  
#endif  

void EnableMemLeakCheck()
{
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);
}

FileLogger g_fileLoggerMain("tetrisBlockMainLog.txt", debug);
//��ʼ���Ŵ��㷨ʹ�õĲ����������в���ʽ���ñ���g_Params
CParams   g_Params;

using namespace std;



///////////////////////GLOBALS ////////////////////////////////////

WCHAR*			szApplicationName = TEXT("Tetris2022");
WCHAR*			szWindowClassName = TEXT("TetrisClass");

int timer_speedup = 1;

stringstream ss;
//-----------------------------------WinProc------------------------------------------
//
//------------------------------------------------------------------------------------

LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
	//device context for our window
	HDC				hdc;
	PAINTSTRUCT     ps;

	//these hold the dimensions of the client window area
	static int cxClient, cyClient;

	//used to create the back buffer
	static HDC		hdcBackBuffer;
	static HBITMAP	hBitmap;
	static HBITMAP	hOldBitmap;

	static CTetrisDraw* pTetrisDraw=0;
	//TetrisController������CurrentTetrisBlock
	static CTetrisController* pTetrisController=0;

	static CTetrisBlock* pNextTetrisBlock=0;

	static CPierreDellacherieTetrisController* pPDTetrisController=0;

	enum game_mode {ManualMode, AIMode, ReservedMode};

	//AIģʽ���Ƿ����two piece���㷨
	static bool AIMode_two_piece;

	static game_mode mode;
	static list<int> AICmdList;

	static bool bPaused;

	switch (msg)
	{
	case WM_CREATE:
	{

		//seed the random number generator
		srand((unsigned int )clock());



		//get the size of the client window
		RECT rect;
		GetClientRect(hwnd, &rect);

		cxClient = rect.right;
		cyClient = rect.bottom;

		//create a surface for us to render to(backbuffer)
		hdcBackBuffer = CreateCompatibleDC(NULL);

		HDC hdc = GetDC(hwnd);

		hBitmap = CreateCompatibleBitmap(hdc,
			cxClient,
			cyClient);

		ReleaseDC(hwnd, hdc);

		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);

		//pTetrisDraw = new CTetrisDraw(hdcBackBuffer, rect);
		pTetrisDraw = new CTetrisDraw(hdc, rect);
		pTetrisController = new CTetrisController(pTetrisDraw);
		pPDTetrisController = new CPierreDellacherieTetrisController(pTetrisDraw);
		
		srand((unsigned int)clock());  // �����������
		int nType = rand() % 7 + 1;
		pNextTetrisBlock = new CTetrisBlock(nType);
		
		SetTimer(hwnd, TIMER_BLOCK_DOWN, int(1000/timer_speedup), NULL);
		//һ���ӿ���ģ��5�ΰ���
		SetTimer(hwnd, TIMER_AICommandExecution, int(1000/(5* timer_speedup)), NULL);
		mode = ManualMode;
		AIMode_two_piece = true;
		
		if (mode == AIMode)
		{
			//AIģʽ�£�20���ٽ���
			timer_speedup = 5;
			if (AIMode_two_piece)
				pPDTetrisController->generateAICommandListForCurrentTetrisBlockWithTheKnowledgeOfNextTetrisBlock(AICmdList, pNextTetrisBlock);
			else
				pPDTetrisController->generateAICommandListForCurrentTetrisBlock(AICmdList);
		}

		bPaused = false;
	}

	break;
	//check key press messages
	case WM_KEYDOWN:
	{
		HDC hdc = GetDC(hwnd);
		HDC hdcOld = pTetrisDraw->SetHDC(hdc);
		switch (wparam)
		{
		case VK_RETURN:
		{


		}

		break;

		case VK_ESCAPE:
		{
			PostQuitMessage(0);
		}

		break;

		case VK_SPACE:
		{
			//�ո�����£���ͣ������֮���л�
			bPaused = !bPaused;
		}

		break;

		case 0x41: //VK_A ����
		{
			bool bCtrlDown = GetAsyncKeyState(VK_CONTROL) & 0x8000;
			if ((mode == ManualMode)&&!bPaused&&!bCtrlDown)
			{
				PlaySound(TEXT("left_right.wav"), NULL, SND_FILENAME | SND_ASYNC);
				pTetrisController->executeCommand(1);
			}
			//��סCTRL+A������AIģʽ���ֶ�ģʽ�л�
			if (bCtrlDown)
			{
				if (ManualMode == mode)
				{
					mode = AIMode;
					//AIģʽ�£�20���ٽ���
					timer_speedup = 5;
					SendMessage(hwnd, WM_KEYDOWN, 0x52, 0);
					break;
				}
				if (AIMode == mode)
				{
					mode = ManualMode;
					//�ֹ�ģʽ�£�1���ٽ���
					timer_speedup = 1;
					SendMessage(hwnd, WM_KEYDOWN, 0x52, 0);
					break;
				}
			}
		}

		break;

		case 0x52: //VK_R 
		{
			bPaused = true;
			MessageBox(hwnd, TEXT("Game RESET !"), TEXT("Alert"), MB_OK);
			
			KillTimer(hwnd, TIMER_BLOCK_DOWN);
			KillTimer(hwnd, TIMER_AICommandExecution);
			
			pTetrisDraw->ClearTetrisArray();
			
			if (0 != pNextTetrisBlock)
				delete pNextTetrisBlock;

			srand((unsigned int)clock());  // �����������
			int nType = rand() % 7 + 1;
			CTetrisBlock* pTB = new CTetrisBlock(nType);

			srand((unsigned int)clock());  // �����������
			nType = rand() % 7 + 1;
			pNextTetrisBlock = new CTetrisBlock(nType);

			SetTimer(hwnd, TIMER_BLOCK_DOWN, int(1000 / timer_speedup), NULL);
			SetTimer(hwnd, TIMER_AICommandExecution, int(1000 / (5 * timer_speedup)), NULL);

			if (mode == AIMode)
			{
				pPDTetrisController->setCurTetrisBlock(pTB);
				if (AIMode_two_piece)
					pPDTetrisController->generateAICommandListForCurrentTetrisBlockWithTheKnowledgeOfNextTetrisBlock(AICmdList, pNextTetrisBlock);
				else
					pPDTetrisController->generateAICommandListForCurrentTetrisBlock(AICmdList);
			}

			if (mode == ManualMode)
			{
				pTetrisController->setCurTetrisBlock(pTB);
			}

			bPaused = false;
			InvalidateRect(hwnd, NULL, true);
			UpdateWindow(hwnd);
		}
		break;

		case 0x53: //VK_S ����
		{
			if ((mode == ManualMode) && !bPaused)
			{
				//PlaySound(TEXT("left_right.wav"), NULL, SND_FILENAME | SND_ASYNC);
				//���������жϣ�������ܼ��������ƶ��ˣ���Ҫ�ͷ��ϵķ��飬�����������1���µķ���
				if (false == pTetrisController->executeCommand(3))
				{
					//���ж��Ƿ���ǰ���鳬������Ϸ�������϶˶�������Ϸ����
					if (false == pTetrisController->isGameOver())
					{
						pTetrisController->setCurTetrisBlock(pNextTetrisBlock);

						srand((unsigned int)clock());  // �����������
						int nType = rand() % 7 + 1;
						pNextTetrisBlock = new CTetrisBlock(nType);

						pTetrisDraw->DrawScoreAndNextBlockArea(pTetrisController->GetScore(), pTetrisController->getTetrisBlockNumUsed(), pNextTetrisBlock);
					}
					else
					{
						//ֹͣ��ʱ��
						KillTimer(hwnd, TIMER_BLOCK_DOWN);
						MessageBox(hwnd, TEXT("Game Over !"), TEXT("Alert"), MB_OK);
					}
				}
			}
				
		}

		break;

		case 0x44: //VK_D ����
		{
			if ((mode == ManualMode) && !bPaused)
			{
				PlaySound(TEXT("left_right.wav"), NULL, SND_FILENAME | SND_ASYNC);
				pTetrisController->executeCommand(2);
			}
		}

		break;

		case 0x4A: //VK_J ��ת
		{
			if ((mode == ManualMode) && !bPaused)
			{
				PlaySound(TEXT("rotate.wav"), NULL, SND_FILENAME | SND_ASYNC);
				pTetrisController->executeCommand(4);
			}
		}

		break;


		}//end switch
		//�ָ���֮ǰ��DC
		hdc = pTetrisDraw->SetHDC(hdcOld);
		ReleaseDC(hwnd, hdc);
	}

	break;

	//has the user resized the client area?
	case WM_SIZE:
	{
		cxClient = LOWORD(lparam);
		cyClient = HIWORD(lparam);

		//resize the backbuffer accordingly
		SelectObject(hdcBackBuffer, hOldBitmap);

		HDC hdc = GetDC(hwnd);
		HDC hdcOld = pTetrisDraw->SetHDC(hdc);
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = cxClient;
		rect.bottom = cyClient;

		hBitmap = CreateCompatibleBitmap(hdc,
			cxClient,
			cyClient);



		hOldBitmap = (HBITMAP)SelectObject(hdcBackBuffer, hBitmap);
		pTetrisDraw->SetArea(rect);

		//�ָ���֮ǰ��DC
		hdc = pTetrisDraw->SetHDC(hdcOld);
		ReleaseDC(hwnd, hdc);
	}

	break;

	case WM_PAINT:
	{

		BeginPaint(hwnd, &ps);

		//fill our backbuffer with white
		BitBlt(hdcBackBuffer, 0, 0, cxClient, cyClient, NULL, NULL, NULL, WHITENESS);

		pTetrisDraw->SetHDC(hdcBackBuffer);
		//pTetrisDraw->SetHDC(ps.hdc);
		pTetrisDraw->DrawGameArea();
		if (ManualMode == mode)
		{
			pTetrisDraw->DrawCurrentTetrisBlock(pTetrisController->getCurTetrisBlock());
			pTetrisDraw->DrawScoreAndNextBlockArea(pTetrisController->GetScore(), pTetrisController->getTetrisBlockNumUsed(), pNextTetrisBlock);
		}
		else if (AIMode == mode)
		{
			pTetrisDraw->DrawCurrentTetrisBlock(pPDTetrisController->getCurTetrisBlock());
			pTetrisDraw->DrawScoreAndNextBlockArea(pPDTetrisController->GetScore(), pPDTetrisController->getTetrisBlockNumUsed(), pNextTetrisBlock);
		}
		
		pTetrisDraw->DrawGameHintArea();
		//now blit backbuffer to front
		BitBlt(ps.hdc, 0, 0, cxClient, cyClient, hdcBackBuffer, 0, 0, SRCCOPY);


		EndPaint(hwnd, &ps);
	}

	break;

	case WM_DESTROY:
	{

		SelectObject(hdcBackBuffer, hOldBitmap);

		//clean up our backbuffer objects
		DeleteDC(hdcBackBuffer);
		DeleteObject(hBitmap);

		if(0 != pTetrisDraw)
			delete pTetrisDraw;

		if (0 != pTetrisController)
			delete pTetrisController;

		if (0 != pPDTetrisController)
			delete pPDTetrisController;

		if (0 != pNextTetrisBlock)
			delete pNextTetrisBlock;
		// kill the application, this sends a WM_QUIT message 
		PostQuitMessage(0);

	}

	break;

	case WM_TIMER:
	{
		HDC hdc = GetDC(hwnd);
		HDC hdcOld = pTetrisDraw->SetHDC(hdc);
		switch (wparam)
		{
			case TIMER_BLOCK_DOWN:
				if ((mode == ManualMode) && !bPaused)
				{
					if (false == pTetrisController->executeCommand(3))
					{
						//���ж��Ƿ���ǰ���鳬������Ϸ�������϶˶�������Ϸ����
						if (false == pTetrisController->isGameOver())
						{
							pTetrisController->setCurTetrisBlock(pNextTetrisBlock);

							srand((unsigned int)clock());  // �����������
							int nType = rand() % 7 + 1;
							pNextTetrisBlock = new CTetrisBlock(nType);

							pTetrisDraw->DrawScoreAndNextBlockArea(pTetrisController->GetScore(), pTetrisController->getTetrisBlockNumUsed(), pNextTetrisBlock);
						}
						else
						{
							//ֹͣ��ʱ��
							KillTimer(hwnd, TIMER_BLOCK_DOWN);
							MessageBox(hwnd, TEXT("Game Over !"), TEXT("Alert"), MB_OK);
						}
					}
				}
				

				break;
			case TIMER_AICommandExecution:
				if ((mode == AIMode) && !bPaused)
				{
					if (!AICmdList.empty())
					{
						CTetrisBlock* pTB=pPDTetrisController->getCurTetrisBlock();
						//AIģʽ�£�ֻ���ڷ�����ȫ���䵽nTetrisBoardHeight-1֮���ٷ���Ӧ�Ķ���ָ�����ִ�������ƶ�һ�������
						if(pTB->getBlockPosY()<=nTetrisBoardHeight-1)
						{
							int cmd = AICmdList.front();
							pPDTetrisController->executeCommand(cmd);
							AICmdList.pop_front();
						}
						else {
							bool bMoveDown = pPDTetrisController->executeCommand(3);
							if (false == bMoveDown)
							{
								if (true == pPDTetrisController->isGameOver())
								{
									KillTimer(hwnd, TIMER_AICommandExecution);
									MessageBox(hwnd, TEXT("Game Over !"), TEXT("Alert"), MB_OK);
								}
							}
						}
					}
					else
					{
						//AICmdListΪ��ʱ
						//ֱ���ƶ�����ǰ���鴹ֱ����������λ��
						bool bMoveDown = true;
						while (bMoveDown)
						{
							bMoveDown = pPDTetrisController->executeCommand(3);
						}
						if (false == bMoveDown)
						{
							//���ж��Ƿ���ǰ���鳬������Ϸ�������϶˶�������Ϸ����
							if (false == pPDTetrisController->isGameOver())
							{
								pPDTetrisController->setCurTetrisBlock(pNextTetrisBlock);

								srand((unsigned int)clock());  // �����������
								int nNum = rand();
								int nType = nNum % 7 + 1;
								pNextTetrisBlock = new CTetrisBlock(nType);

								pTetrisDraw->DrawScoreAndNextBlockArea(pPDTetrisController->GetScore(), pPDTetrisController->getTetrisBlockNumUsed(), pNextTetrisBlock);

								AICmdList.clear();
								if (AIMode_two_piece)
									pPDTetrisController->generateAICommandListForCurrentTetrisBlockWithTheKnowledgeOfNextTetrisBlock(AICmdList, pNextTetrisBlock);
								else
									pPDTetrisController->generateAICommandListForCurrentTetrisBlock(AICmdList);
								//ss << "random number" << nNum << " "<< nType << endl;
								// (int i = 0; i < 10; i++)
								//{
								//	ss << rand() << " ";
								//}
								//string debug = ss.str();
								//g_fileLoggerMain.Debug(debug);
								//ss.clear();
								//ss.str("");
							}
							else
							{
								
								//ֹͣ��ʱ��
								KillTimer(hwnd, TIMER_BLOCK_DOWN);
								MessageBox(hwnd, TEXT("Game Over !"), TEXT("Alert"), MB_OK);
								
							}
						}


					}
				}

				break;
			default:break;
		}
		//�ָ���֮ǰ��DC
		hdc = pTetrisDraw->SetHDC(hdcOld);
		ReleaseDC(hwnd, hdc);
	}
	break;

	default:break;

	}//end switch

	 // default msg handler 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

}//end WinProc


 //-----------------------------------WinMain-----------------------------------------
 //	Entry point for our windows application
 //-----------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hprevinstance,
	LPSTR lpcmdline,
	int ncmdshow)
{
	EnableMemLeakCheck();
	//_CrtSetBreakAlloc(1333);


	WNDCLASSEX winclass;
	HWND	   hwnd;
	MSG		   msg;

	// first fill in the window class stucture
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = NULL;
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = szWindowClassName;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	

	// register the window class
	if (!RegisterClassEx(&winclass))
		return 0;

	// create the window
	if (!(hwnd = CreateWindowEx(NULL,
		szWindowClassName,
		szApplicationName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		hinstance,
		NULL)))
		return 0;

	ShowWindow(hwnd, ncmdshow);
	UpdateWindow(hwnd);

	//unit test �Ĵ��� start
	PierreDellacherieUnittest pdut;
	pdut.addTestFunc(test_getErodedPieceCellsMetric); 
	
	////pdut.addTestFunc(test_getBoardRowTransitions); 
	pdut.addTestFunc(test_getBoardColumnTransitions); 
	pdut.addTestFunc(test_getBoardBuriedHoles); 
	pdut.addTestFunc(test_getBoardWells); 
	pdut.addTestFunc(test_pickPositionWithHighestEvalutionScore);
	pdut.addTestFunc(test_RotateTetrisBlock); 
	pdut.addTestFunc(test_generateAICommandListForCurrentTetrisBlock);
	pdut.addTestFunc(test_evaluationFunction);
	
	pdut.addTestFunc(test_findRectangularPath);
	//pdut.addTestFunc(test_generateAICommandListForCurrentTetrisBlockWithTheKnowledgeOfNextTetrisBlock);
	pdut.addTestFunc(test2_generateAICommandListForCurrentTetrisBlockWithTheKnowledgeOfNextTetrisBlock);
	
	pdut.runTest();
	//unit test �Ĵ��� end


	//���Ժ������ܴ���start
	/*
	CPDTetrisControllerUsingGeneticAlgorithm pdcuga_test = CPDTetrisControllerUsingGeneticAlgorithm();
	double dFitness = 0;
	RECT rect_test;
	rect_test.top = 0;
	rect_test.left = 0;
	rect_test.right = 100;
	rect_test.bottom = 200;
	//rectû�����壬�Ŵ��㷨���沢����ʹ�ã�CTetrisDraw��һ������HDC���ó�0����ʾ�Ŵ��㷨������̲������ʵ�ʻ�ͼ
	CTetrisDraw* pTetrisDraw_test = new CTetrisDraw(0, rect_test);

	//0.369141 0.142426 0.330902 0.351837 0.253052 0.450378
	CPierreDellacherieTetrisController pdtc_test = CPierreDellacherieTetrisController(pTetrisDraw_test);
	sPDTetrisControllerCoefficient pdcCoeff_test;
	pdcCoeff_test.lh = 0.369141;
	pdcCoeff_test.epcm = 0.142426;
	pdcCoeff_test.brt = 0.330902;
	pdcCoeff_test.bct = 0.351837;
	pdcCoeff_test.bbh = 0.253052;
	pdcCoeff_test.bw = 0.450378;
	pdtc_test.setPDTetrisControllerCoefficient(pdcCoeff_test);
	stringstream ss_test;
	for (int iTemp = 0; iTemp < 10; iTemp++)
	{
		clock_t start_time, end_time;
		pdtc_test.resetGame();
		start_time = clock();
		//���Է��֣�ArrayIndexRangeCheck::IndexRangeCheck������ʹ��string������������̫��
		pdcuga_test.RunTetrisGameSimulation(pdtc_test, dFitness);
		end_time = clock();
		double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
		ss_test << "elapsed_time: " << elapsed_time << endl;
		string debug = ss_test.str();
		g_fileLoggerMain.Debug(debug);
		ss_test.clear();
		ss_test.str("");
	}

	delete pTetrisDraw_test;
	*/
	//���Ժ������ܴ��� end

	CPDTetrisControllerUsingGeneticAlgorithm pdcuga = CPDTetrisControllerUsingGeneticAlgorithm();
	//pdcuga.MainProcess();


	//enter the message loop
	bool bDone = false;
	

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_QUIT)
		{
			// Stop loop if it's a quit message
			bDone = true;
			break;
		}

		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(szWindowClassName, winclass.hInstance);


	return 0;


} // end WinMain



