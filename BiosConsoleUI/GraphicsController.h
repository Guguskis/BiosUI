#pragma once
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "DataStructures.h"

using namespace std;

class GraphicsController{
	/**********Data**********/
private:
	static const int m_Length = 147, m_Width = 66;
	Board m_Board[m_Width][m_Length];
	Board m_BoardLast[m_Width][m_Length];

	int *m_Depth;
	int *m_L1, *m_L2, *m_L3;
	int m_DepthLast, m_L1Last, m_L2Last, m_L3Last;

	bool m_DataSet = false;
	vector<L1> *m_Hierarchy;


	ofstream os;
	//color macros
	enum {
		COL_BLACK = 0,
		COL_BLUE = 1,
		COL_GREY = 7,
		COL_CYAN = 11,
		COL_WHITE = 15
	};
	/**********Methods**********/
protected:
	void HandleGraphics();
	void SetGraphicsController(int*, int*, int*, int*, vector<L1> *);
	void GenerateCrashScreen();
private:
	void UpdateL1();
	void UpdateL2();
	void UpdateTop();
	void UpdateBottom();
	void UpdateWindow();
	void DrawHelpScreen();

	void InsertTextBox(int, int, string, int, int, int, int);
	void InsertBox(int, int, int, int, int, int);

	void InsertText(int, int, string, int, int);

	void SetLastValues();

	void DrawScreen();
	void DrawSymbol(int, int, int, int, char);
	void SetColor(int, int);
	void Goto(int, int);
	template<typename T> void Log(T output) {os << output;}
public:
	void Test();
	GraphicsController();
	~GraphicsController();
};

