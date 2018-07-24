#ifndef _LOGIC_
#define _LOGIC_

#include "pole.hpp"
#include <Windows.h>

struct REDRAW
{
	int index{ -1 };
	COLORREF color{ RGB( 200, 200, 200 ) };
};
//define cell by mouse coord to redraw
int IndexCellToRedraw(HWND &hwnd, LPARAM lParam, HWND & _hwnd, FieldMain & _pm );
//define what color was chosen or now  is choosing 
COLORREF colorToRedraw( LPARAM lParam, COLORREF & _Color,  Pallete & _pal );
//redrawing cell
void DrawIt(REDRAW L, FieldMain & _pm , HDC &_hdc, Pallete & _pal  );

// help func 4 func color to redraw
//IN FACT IS GETTER
const COLORREF&  whatColorDuuuude( LPARAM lParam,Pallete & _pall);
//location
int whereCatchedCurInPallete( LPARAM lParam, Pallete & _pall );
//help func
bool HasColorBeenChosen( LPARAM lParam, Pallete & _pall);

//IN FACT IS SETTER
int ChangeColorInPallete(LPARAM lParam , COLORREF & _Color, Pallete & _pal);

#endif

