#include "logic.hpp"


int IndexCellToRedraw( HWND &hwnd, LPARAM lParam, HWND & _hwnd, FieldMain & _pm )
{
	WORD xPos, yPos;
	// Сохраняем координаты курсора мыши
	xPos = LOWORD( lParam );
	yPos = HIWORD( lParam ); 
	auto it = _pm.m_vec->begin();

	int index{ 0 };
	while (it != _pm.m_vec->end())
	{
		//define index of cell to redraw by going full vector
		if ((xPos > it->getRECT()->left &&
			xPos < it->getRECT()->right)
			&&
			(yPos> it->getRECT()->top &&
				yPos < it->getRECT()->bottom
				))
		{
			return index;
			break;
		}
		++index;
		++it;
	}
	return -1;//if cell haven't found return uexisted value
}

COLORREF colorToRedraw( LPARAM lParam, COLORREF & _Color,Pallete & _pal )
{
	//reminding color to redraw
	COLORREF F = whatColorDuuuude( lParam, _pal );
	//if before have never choosen color in pallete
	if (F == RGB( 200, 200, 200 )
		&& _Color != RGB( 200, 200, 200 ))
		return _Color;
	//if color is initial do not redraw it
	else return F;
}
///////////////////////////////////////////////////////
void DrawIt(REDRAW L, FieldMain & _pm, HDC & _hdc, Pallete & _pal )
{
	//if it cell has similar color - do not redraw
	if (_pm.m_vec->at( L.index ).getKlitColor() == L.color)
		return;
	//if else - establishing color that in structure 
	_pm.m_vec->at( L.index ).SetColor( L.color );
	//create cbrush for redr
	HBRUSH	BrushInside = CreateSolidBrush( L.color);
	SelectObject( _hdc, BrushInside );
	HBRUSH	BrushFrame = CreateSolidBrush( RGB( 220, 220, 220 ) );
	Rectangle( _hdc, _pm.m_vec->at( L.index ).getRECT()->left, _pm.m_vec->at( L.index ).getRECT()->top,
	_pm.m_vec->at( L.index ).getRECT()->right, _pm.m_vec->at( L.index ).getRECT()->bottom );
	//redraw frame
	FrameRect( _hdc, _pm.m_vec->at( L.index ).getRECT(), 
		BrushFrame);
	//cleaning after eatin
	DeleteObject( BrushInside );
	DeleteObject( BrushFrame );
}


const COLORREF & whatColorDuuuude( LPARAM lParam, Pallete & _pall )
{
	//IN FACT IS GETTER
	//correct that color been choosen
	if (HasColorBeenChosen( lParam, _pall ) &&
		whereCatchedCurInPallete( lParam, _pall ) != -1)
	{
		return _pall.m_vec.at( whereCatchedCurInPallete( lParam, _pall ) ).getKlitColor();
	}
	//if it hasnt return initial value
	return RGB( 200, 200, 200 );
}

////////////////////////////////////////////
int whereCatchedCurInPallete( LPARAM lParam, Pallete & _pall )
{
	WORD _xMousePos, _yMousePos;
	// Сохраняем координаты курсора мыши
	_xMousePos = LOWORD( lParam );
	_yMousePos = HIWORD( lParam );
	int counter{ 0 };
	auto it = _pall.m_vec.begin();//begin iterator
		while (it != _pall.m_vec.end())
		{
			//define particular cell
			if ((_xMousePos > it->getRECT()->left &&
				_xMousePos < it->getRECT()->right)
				&&
				(_yMousePos > it->getRECT()->top &&
					_yMousePos < it->getRECT()->bottom
					))
			{
				return counter;
			}
			++counter;
			++it;
		}
		return -1; //if cell wasn't found return -1 for grey
}

bool HasColorBeenChosen( LPARAM lParam, Pallete & _pall )
{
	//this function special for define if AT ALL mouse was in pallete or not
	WORD _xMousePos, _yMousePos;
	// Сохраняем координаты курсора мыши
	_xMousePos = LOWORD( lParam );
	_yMousePos = HIWORD( lParam );
	//checking
	if ((_xMousePos > _pall.getCoordinatesOfWholeRect().left &&
		_xMousePos < _pall.getCoordinatesOfWholeRect().right)
		&&
		(_yMousePos > _pall.getCoordinatesOfWholeRect().top &&
			_yMousePos < _pall.getCoordinatesOfWholeRect().bottom
			)) 
		return true;
	else return false;
}
/////////////////////////
int ChangeColorInPallete( LPARAM lParam, COLORREF & _Color, Pallete & _pal )
{	//define what cell color we need change
	int cur = whereCatchedCurInPallete( lParam, _pal );

	if (HasColorBeenChosen( lParam, _pal ))
	{
		if (cur != -1)
		{
			//and set new color
			_pal.m_vec.at( cur ).SetColor( _Color );
			return 0;
		}
	}
	//if else unexisting value
	return -1;
}

