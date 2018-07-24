#include <windows.h>
#include "pole.hpp"
#include "logic.hpp"
#include "resource.h"

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
REDRAW taknado;

static std::vector<Cell> vec;
HINSTANCE hInst;
static FieldMain p( 23, &vec );
static Pallete pal( 38, p );

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmdLine, int iCmdShow )
{
	static TCHAR szAppName[] = "Mozaik";
	HWND hwnd;
	MSG msg;

	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof( wndclass ); // размер дл€ определени€ типа 
	wndclass.style = CS_HREDRAW | CS_VREDRAW |	CS_DBLCLKS;   // задаем флаги чтобы определить стиль окна 
	wndclass.lpfnWndProc = WndProc; //функци€ дл€ определени€ сообщений в окне 
	wndclass.cbClsExtra = 0;// пам€ть, не надо
	wndclass.cbWndExtra = 0;//// пам€ть, не надо
	wndclass.hInstance = hInstance;//описатель экземпл€ра
	wndclass.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );// иконка 
	wndclass.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1) );// 
	wndclass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );//фон 
	wndclass.lpszMenuName = MAKEINTRESOURCE( IDR_MENU1 );//меню им€ 
	wndclass.lpszClassName = szAppName;//hard!!!
	wndclass.hIconSm = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_ICON1 ) );// иконка 

	RegisterClassEx( &wndclass );

	hwnd = CreateWindow(
		szAppName, // window class name окошко вверху
		"Mozaik", // window caption
		WS_OVERLAPPEDWINDOW , // window style стиль непосредственно окна 
		CW_USEDEFAULT , // initial x position правоориентированна€ система координат 
		CW_USEDEFAULT, // initial y position
		CW_USEDEFAULT, // initial x size
		CW_USEDEFAULT, // initial y size
		NULL, // parent window handle
		NULL, // window menu handle
		hInstance, // program instance handle
		NULL// указатель на какие-то параметры
	); // creation parameters

	ShowWindow( hwnd, iCmdShow );
	UpdateWindow( hwnd );

	while (GetMessage( &msg, NULL, 0, 0 ))
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	HDC hdc;// описатель контекста, дл€ рисовани€ 
	PAINTSTRUCT ps;// структура дл€ рисовани€ 
	RECT rect;// пр€моугольник, структура координат 
	static SIZES s = SIZES::MIDDLE;

	static COLORREF acrCustClr[16];
	HBRUSH hBrush;
	//structure and rgbColor for change and choose color in standart dialog pallete
	static DWORD rgbCurrent;
	CHOOSECOLOR hColor;
	ZeroMemory( &hColor, sizeof( hColor ) );
	hColor.lStructSize = sizeof( hColor );
	hColor.hwndOwner = hwnd;
	hColor.lpCustColors = (LPDWORD)acrCustClr;
	hColor.rgbResult = rgbCurrent;
	hColor.Flags = CC_FULLOPEN;

	switch (iMsg)
	{
	case WM_CREATE:
			return 0;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 930;
		lpMMI->ptMinTrackSize.y = 570;
		lpMMI->ptMaxTrackSize.x = 930;
		lpMMI->ptMaxTrackSize.y = 570;
	}
	break;
	case WM_RBUTTONDBLCLK:
		if (HasColorBeenChosen( lParam, pal ))
		{
			hdc = GetDC( hwnd );
			if (ChooseColor( &hColor ) == TRUE)
			{
				hBrush = CreateSolidBrush( hColor.rgbResult );
				rgbCurrent = hColor.rgbResult;
				ChangeColorInPallete( lParam, rgbCurrent, pal );
				SelectObject( hdc, hBrush );
				Rectangle( hdc,
					pal.m_vec.at( 
						whereCatchedCurInPallete( lParam, pal ) ).getRECT()->left,
					pal.m_vec.at(
						whereCatchedCurInPallete( lParam, pal ) ).getRECT()->top,
					pal.m_vec.at(
						whereCatchedCurInPallete( lParam, pal ) ).getRECT()->right,
					pal.m_vec.at(
						whereCatchedCurInPallete( lParam, pal ) ).getRECT()->bottom );
				hBrush = CreateSolidBrush( RGB( 255, 255, 200 ) );
				FrameRect( hdc, pal.m_vec.at( whereCatchedCurInPallete( lParam, pal )).getRECT(), hBrush);
				DeleteObject( hBrush );
			}
			taknado.color = rgbCurrent;

			ReleaseDC( hwnd, hdc );
		}
		break;
		
	case WM_LBUTTONDOWN:
	{
		taknado.color = colorToRedraw( lParam, taknado.color, pal );
		taknado.index = IndexCellToRedraw( hwnd, lParam, hwnd, p );

		hdc = GetDC( hwnd );
		if (taknado.index != -1)
			DrawIt( taknado, p, hdc, pal );
		ReleaseDC( hwnd, hdc );
		break;
	}
	
	case WM_MOUSEMOVE:
	{
		if (wParam && MK_LBUTTON)
		{
			taknado.index = IndexCellToRedraw(hwnd, lParam, hwnd, p );
			hdc = GetDC( hwnd );

			if (taknado.index != -1)
				DrawIt( taknado, p, hdc, pal );
			
			ReleaseDC( hwnd, hdc );
		}
		return 0;
	}
	//!
	case WM_PAINT:
		hdc = BeginPaint( hwnd, &ps );
		p.PrintPole( hwnd, hdc );
		pal.PrintPole( hwnd, hdc );

		EndPaint( hwnd, &ps );
		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		case ID_CELLSIZE_MINI:
			if (s != SIZES::MINI)
			{
				hdc = GetDC( hwnd );
				s = SIZES::MINI;
				p.ChangePole( s, hwnd, hdc );
				ReleaseDC( hwnd, hdc );
			}
			break;
		case ID_CELLSIZE_MIDI:
			if (s != SIZES::MIDDLE)
			{
				hdc = GetDC( hwnd );
				s = SIZES::MIDDLE;
				p.ChangePole( s, hwnd, hdc );
				ReleaseDC( hwnd, hdc );
			}
			break;
		case ID_CELLSIZE_MAXIMUM:
			if (s != SIZES::MAX)
			{
				hdc = GetDC( hwnd );
				s = SIZES::MAX;
				p.ChangePole( s, hwnd, hdc );
				ReleaseDC( hwnd, hdc );
			}
			break;
		case ID_OPTIONS_CLEARFIELD:
			hdc = GetDC( hwnd );
			p.ChangePole( s, hwnd, hdc );
			ReleaseDC( hwnd, hdc );
			break;
		case ID_FILLFIELDBY_RED:
			hdc = GetDC( hwnd );
			p.m_vec->clear();

			if (s == SIZES::MAX)
			{
				p.makePole( 25, 17, RGB( 255, 255, 255 ) );
				InvalidateRect( hwnd, NULL, true );
			}
			else if (s == SIZES::MIDDLE)
			{
				p.makePole( 31, 20, RGB( 255, 255, 255 ) );
				InvalidateRect( hwnd, NULL, true );
			}
			else
			{
				p.makePole( 38, 25, RGB(255 , 255, 255 ) );
				InvalidateRect( hwnd, NULL, true );
			}
			ReleaseDC( hwnd, hdc );
			break;
		case ID_FILLFIELDBY_BLACK:
			hdc = GetDC( hwnd );
			p.m_vec->clear();

			if (s == SIZES::MAX)
			{
				p.makePole( 25, 17, RGB( 0, 0, 0 ) );
				InvalidateRect( hwnd, NULL, true );
			}
			else if (s == SIZES::MIDDLE)
			{
				p.makePole( 31, 20, RGB( 0, 0, 0 ) );
				InvalidateRect( hwnd, NULL, true );
			}
			else 
			{
				p.makePole( 38, 25, RGB( 0, 0, 0 ) );
				InvalidateRect( hwnd, NULL, true );
			}
			ReleaseDC( hwnd, hdc );
			break;
		case ID_FILLFIELDBY_40008:
			if (ChooseColor( &hColor ) == TRUE)
			{
				hBrush = CreateSolidBrush( hColor.rgbResult );
				rgbCurrent = hColor.rgbResult;
				hdc = GetDC( hwnd );
				p.m_vec->clear();
				if (s == SIZES::MAX)
				{
					p.makePole( 25, 17, rgbCurrent );
					InvalidateRect( hwnd, NULL, true );
				}
				else if (s == SIZES::MIDDLE)
				{
					p.makePole( 31, 20, rgbCurrent );
					InvalidateRect( hwnd, NULL, true );
				}
				else
				{
					p.makePole( 38, 25, rgbCurrent );
					InvalidateRect( hwnd, NULL, true );
				}
				ReleaseDC( hwnd, hdc );
				DeleteObject( hBrush );
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}
	return DefWindowProc( hwnd, iMsg, wParam, lParam );
}