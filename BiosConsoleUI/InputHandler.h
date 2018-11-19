#pragma once
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <string>
#include <vector>

#include "DataStructures.h"

using namespace std;

class InputHandler{

	/**********Data**********/
private: 
	/*
		stores last pressed key
	*/
	std::string m_PressedKey;
	/*
		m_Depth tells which menu is active: 2 - lowest, 3 - highest
		m_LX tells the current value of 3 levels of menu
		level 1 - in the upper screen, 2nd row in blue
		level 2 - in the middle, grey area
		level 3 - in new window
	*/
	int *m_Depth;
	int *m_L1, *m_L2, *m_L3;
	
	vector<L1> *m_Hierarchy;

	/*
		tells if data pointers were successfully set
	*/
	bool m_DataSet = false;
	/**********Methods**********/

protected:
	void IncreaseL1();
	void IncreaseLX();
	void IncreaseDepth();
	void DecreaseL1();
	void DecreaseLX();
	void DecreaseDepth();

	void GetInput();
	bool KeyPressed(std::string);
	void SetInputHandler(int*, int*, int*, int*, vector<L1> *);
public:
	InputHandler();
	~InputHandler();
	void Test();
};

