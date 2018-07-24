#include "pole.hpp"
#include "palleteColors.hpp"

Cell::Cell( int _coordX, int _coordY, COLORREF _DRRR, int size ) :CoordX( _coordX ),
CoordY( _coordY ),m_nKlitColor(_DRRR), CellSize( size )
{
	KLIT.left = CoordX;
	KLIT.top = CoordY;
	KLIT.right = CoordX + CellSize;
	KLIT.bottom = CoordY + CellSize;
}


const RECT * Cell::getRECT()
{
	return  &KLIT;
}

void Cell::SetColor( COLORREF _r )
{
	m_nKlitColor = _r;
}

COLORREF const & Cell::getKlitColor()
{
	return m_nKlitColor;
}


FieldMain::FieldMain( int sizeCell, std::vector<Cell>  * _vec )
	:m_CellSize(sizeCell)
{
	m_vec = _vec;
	makePole( 31, 20 );
}

void FieldMain::makePole( int _perX, int _perY, COLORREF _init  )
{
	m_perX = _perX, m_perY = _perY;
	int SizeWholeField = _perX * _perY; //helper var
	int CoordX = 10, CoordY = 15; // initial position of 1 cell field
	for(int i = 0 ; i< _perX ; i++)
		for (int j = 0; j < _perY; j++)
		{
			// pic should look like matrix or spreadsheet
			m_vec->push_back( Cell( CoordX + (i * m_CellSize), 
				CoordY + (j * m_CellSize), _init , m_CellSize ) );
		}
	setWholeFieldCoordinates();
}

void FieldMain::PrintPole( HWND & _hwnd, HDC & _hdc ) 
{
	HBRUSH BrushFrame = CreateSolidBrush( RGB( 220, 220, 220 ) ); // init frame color white-grey
	HBRUSH BrushInside;// color inside cell 
	for (int i = 0; i < m_vec->size(); i++)
	{
		BrushInside = CreateSolidBrush( m_vec->at( i ).getKlitColor() ); // создаем кисть
		SelectObject( _hdc, BrushInside ); // выбираем кисть
		Rectangle( _hdc, m_vec->at( i).getRECT()->left, m_vec->at( i ).getRECT()->top, m_vec->at( i ).getRECT()->right, m_vec->at( i ).getRECT()->bottom );
		FrameRect( _hdc, m_vec->at( i ).getRECT(), BrushFrame);
		DeleteObject( BrushInside );
	}
	DeleteObject( BrushFrame );
}

void FieldMain::ChangePole( SIZES & _c, HWND & _hwnd, HDC & _hdc )
{
	//decided size
	switch (_c)
	{
	case SIZES::MINI :
		{
			m_CellSize = 19;
			//clear previous vector
			m_vec->clear();
				// new fabric
			makePole( 38, 25 );
			InvalidateRect( _hwnd, NULL, true );
		}
		break;
	case SIZES::MIDDLE:
		 {
		m_CellSize = 23;
		m_vec->clear();
		makePole( 31, 20 );
		InvalidateRect( _hwnd, NULL, true );
	}
		break;
	case SIZES::MAX:
		 {
		m_CellSize = 28;
		m_vec->clear();
		makePole(25, 17 );
		InvalidateRect( _hwnd, NULL, true );
	}
	default:break;
	}
}

int FieldMain::getWeightOfField()
{
	return m_perX * m_vec->at( 0 ).getCellSize();
}

void FieldMain::setWholeFieldCoordinates()
{
	m_WholeRectOfField.left = m_vec->at( 0 ).getRECT()->left;
	m_WholeRectOfField.right = m_vec->at( m_vec->size() - 1 ).getRECT()->right;
	m_WholeRectOfField.top = m_vec->at( m_perY ).getRECT()->top;
	m_WholeRectOfField.bottom = m_vec->at( m_vec->size() - 1 ).getRECT()->bottom;
}
Pallete::Pallete( int sizeCell, FieldMain & _pm )
	:m_CellSize(sizeCell)
{
	
	const int CoordX = _pm.getWeightOfField() + 70,
		CoordY = (_pm.getCoordinatesOfWholeRect().bottom - _pm.getCoordinatesOfWholeRect().top)/6 ;
	for (int i = 0; i < 16; i++)
	{
		if(i >=8)
			m_vec.push_back( Cell( CoordX + m_CellSize + 10, CoordY + ((i-8) * m_CellSize), m_colors16[i], m_CellSize ) );
		else
			m_vec.push_back( Cell( CoordX, CoordY + (i * m_CellSize), m_colors16[i], m_CellSize ) );
	}
	setWholeFieldCoordinates();
}

void Pallete::PrintPole( HWND & _hwnd, HDC & _hdc )
{
	HBRUSH BrushFrame = CreateSolidBrush( RGB( 255, 255, 200 ) );
	HBRUSH BrushInside;
	for (int i = 0; i < 16; i++)
	{
		BrushInside = CreateSolidBrush( m_vec.at( i ).getKlitColor() );
		SelectObject( _hdc, BrushInside ); // выбираем кисть
		Rectangle( _hdc, m_vec.at( i ).getRECT()->left, m_vec.at( i ).getRECT()->top, m_vec.at( i ).getRECT()->right, m_vec.at( i ).getRECT()->bottom );
		FrameRect( _hdc, m_vec.at( i ).getRECT(), BrushFrame );
		DeleteObject( BrushInside);
	}
	DeleteObject( BrushFrame );
}

RECT const & Pole::getCoordinatesOfWholeRect()
{
	return m_WholeRectOfField;
}

void Pallete::setWholeFieldCoordinates()
{
	m_WholeRectOfField.left = m_vec.at( 0 ).getRECT()->left;
	m_WholeRectOfField.right = m_vec.at( m_vec.size() - 1 ).getRECT()->right;
	m_WholeRectOfField.top = m_vec.at( m_perY ).getRECT()->top;
	m_WholeRectOfField.bottom = m_vec.at( m_vec.size() - 1 ).getRECT()->bottom;
}
