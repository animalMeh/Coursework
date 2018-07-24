#ifndef _POLE_
#define _POLE_

#include <functional>
#include <string>
#include <tchar.h>
#include <vector>
#include <Windows.h>

enum class SIZES
{
	MINI, MIDDLE, MAX
};

class Cell
{
public:

	Cell( int _coordX, int _coordY,COLORREF _DRRR, int size = 5 ); /*конструктор заполняющий приватные поля*/
	~Cell() = default; //деструктор по умолчанию
	int getCoordX(); //inline getter X coord
	int getCoordY();//inline getter Y coord
	int getCellSize(); // getter CellSize
	const RECT * getRECT(); // getter cell's Rectangle 
	void SetColor( COLORREF _r );// setter cell's color
	COLORREF const & getKlitColor();// getter cell's color

private:
	int CellSize; // in pole
	COLORREF m_nKlitColor;
	int CoordX;
	int CoordY;
	RECT KLIT;
};

class Pole
{
protected:
	virtual	void PrintPole( HWND & _hwnd, HDC& _hdc  ) = 0; // funct for printn field
	int m_perX;// whole field coord X
	int m_perY; // whole field coord Y
	RECT m_WholeRectOfField;
public:
	RECT const & getCoordinatesOfWholeRect(); // getter whole rect
};

class FieldMain
	:public Pole
{
public: 
	/*const takes static vector of cells*/
	FieldMain( int sizeCell,std::vector<Cell> * _vec);
	~FieldMain() = default;
	/*pattern метод фабрики  */
	void makePole( int _perX , int  _perY, COLORREF _init = RGB( 200, 200, 200 ) );
	void setWholeFieldCoordinates(); 
	int getWeightOfField() ;
	void PrintPole( HWND & _hwnd, HDC& _hdc ) override;
	void ChangePole(SIZES & _c, HWND & _hwnd, HDC& _hdc );// change field by sizes

	std::vector<Cell> * m_vec;
private:
	int m_CellSize;
};

class Pallete : public Pole
{
public:
	Pallete( int sizeCell, FieldMain & _pm  ); // constructor
	void PrintPole( HWND & _hwnd, HDC& _hdc ) override; // print func
	void setWholeFieldCoordinates();// setter whole coordinates
	std::vector<Cell> m_vec; // local vector
private:
	int m_CellSize;
};

inline int Cell::getCoordX()
{
	return CoordX;
}

inline int Cell::getCoordY()
{
	return CoordY;
}

inline int Cell::getCellSize()
{
	return CellSize;
}

#endif