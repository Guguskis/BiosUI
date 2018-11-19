#pragma once
#pragma warning(disable:4996)
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <iomanip>
#include <thread>

#include "DataStructures.h"
#include "InputHandler.h"
#include "GraphicsController.h"

using namespace std;

class BIOS :
	private InputHandler,
	private GraphicsController{
	/**********Data**********/
private:
	/*stores main option tree*/
	vector<L1> m_Hierarchy;

	string m_FileName = "Settings.txt";
	bool m_DataLoaded;
	/*these variables are explained more in depth in "InputHandler.h"*/
	int m_Depth = 2;
	int m_L1 = 0, m_L2 = 0, m_L3 = 0;



	/**********Methods**********/
private:
	void System32Mode();

	void SetConsole();

	void HandleGraphics();
	void HandleInput();
	bool KeyPressed(string);

	void SaveData(string);
	void LoadData(string);

	ofstream osTemp;
	template<typename T> void LogBios(T output) { osTemp << output; }
public:
	void Test();
	void Run();
	BIOS();
	~BIOS();
};

