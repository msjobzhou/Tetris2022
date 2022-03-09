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

using namespace std;



///////////////////////GLOBALS ////////////////////////////////////

WCHAR*			szApplicationName = TEXT("Tetris2022");
WCHAR*			szWindowClassName = TEXT("TetrisClass");




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

	static CTetrisDraw* pTetrisDraw;
	static CTetrisController* pTetrisController;


	switch (msg)
	{
	case WM_CREATE:
	{
		//seed the random number generator
		srand((unsigned)time(NULL));



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

		SetTimer(hwnd, TIMER_BLOCK_DOWN, 1000, NULL);


		
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

		}

		break;

		case 0x41: //VK_A 向左
		{
			PlaySound(TEXT("left_right.wav"), NULL, SND_FILENAME | SND_ASYNC);
			pTetrisController->executeCommand(1);
		}

		break;

		case 0x53: //VK_S 向下
		{
			//PlaySound(TEXT("left_right.wav"), NULL, SND_FILENAME | SND_ASYNC);
			//这里做下判断，如果不能继续往下移动了，需要释放老的方块，重新随机生成1个新的方块
			if (false == pTetrisController->executeCommand(3))
			{
				//先判断是否因当前方块超过了游戏区域最上端而导致游戏结束
				if(false == pTetrisController->isGameOver())
				{
					pTetrisController->setCurTetrisBlock();
				}
				else
				{
					//停止定时器
					KillTimer(hwnd, TIMER_BLOCK_DOWN);
					MessageBox(hwnd, TEXT("Game Over !"), TEXT("Alert"), MB_OK);
				}
			}
				
		}

		break;

		case 0x44: //VK_D 向右
		{
			PlaySound(TEXT("left_right.wav"), NULL, SND_FILENAME | SND_ASYNC);
			pTetrisController->executeCommand(2);
		}

		break;

		case 0x4A: //VK_J 旋转
		{
			PlaySound(TEXT("rotate.wav"), NULL, SND_FILENAME | SND_ASYNC);
			pTetrisController->executeCommand(4);
		}

		break;


		}//end switch
		//恢复成之前的DC
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
		pTetrisDraw->DrawCurrentTetrisBlock(pTetrisController->getCurTetrisBlock());
		pTetrisDraw->DrawScoreArea(pTetrisController->GetScore());
		pTetrisDraw->DrawNextBlockHintArea();
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

		//if(0 != pTetrisDraw)
		delete pTetrisDraw;

		//if (0 != pTetrisController)
		delete pTetrisController;

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
				if (false == pTetrisController->executeCommand(3))
				{
					//先判断是否因当前方块超过了游戏区域最上端而导致游戏结束
					if (false == pTetrisController->isGameOver())
					{
						pTetrisController->setCurTetrisBlock();
					}
					else
					{
						//停止定时器
						KillTimer(hwnd, TIMER_BLOCK_DOWN);
						MessageBox(hwnd, TEXT("Game Over !"), TEXT("Alert"), MB_OK);
					}
				}
				break;

			default:break;
		}
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

	//unit test 的代码 start
	PierreDellacherieUnittest pdut;
	//pdut.addTestFunc(test_getErodedPieceCellsMetric); 
	//pdut.addTestFunc(test_getBoardRowTransitions); 
	//pdut.addTestFunc(test_getBoardColumnTransitions); 
	//pdut.addTestFunc(test_getBoardBuriedHoles); 
	//pdut.addTestFunc(test_getBoardWells); 
	pdut.addTestFunc(test_pickPositionWithHighestEvalutionScore);
	pdut.runTest();
	//unit test 的代码 end

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



