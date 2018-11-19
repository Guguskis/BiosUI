#include "InputHandler.h"




void InputHandler::Test(){
	
}


/***************************FINISHED***************************/

void InputHandler::IncreaseL1() {
	if (*m_L1 + 1 < m_Hierarchy->size()) {
		*m_L1 += 1;
		*m_L2 = 0;
		*m_L3 = 0;
		//finding first non static option in L2 and setting it to m_L2
		for (int i = 0; i < m_Hierarchy->at(*m_L1).attribute.size(); i++) {
			if (m_Hierarchy->at(*m_L1).attribute[i].type != "L2Static") {
				*m_L2 = i;
				break;
			}
		}
	}
}
void InputHandler::IncreaseLX() {
	/*
		This method is used to increment value of active menu
		Does not allow to exceed maximum active menu option count
	*/
	if (*m_Depth == 2) {
		//finding the first option that is not static
		for (int i = *m_L2+1; i < m_Hierarchy->at(*m_L1).attribute.size(); i++) {
			if (m_Hierarchy->at(*m_L1).attribute.at(i).type != "L2Static") {
				*m_L2 = i;
				*m_L3 = 0;
				break;
			}
		}
	}
	else if (*m_Depth == 3) {
		if (*m_L3 + 1 < m_Hierarchy->at(*m_L1).attribute.at(*m_L2).window.size()) {
			*m_L3 += 1;
		}
	}
	else {
		cout << "\"InputHandler::IncreaseLX()\": value should be 2 or 3, but is: " << *m_Depth << endl;
	}
}
void InputHandler::DecreaseL1() {
	if (*m_L1 > 0) {
		*m_L1 -= 1;
		*m_L2 = 0;
		*m_L3 = 0;
		//finding first non static option in L2 and setting it to m_L2

		for (int i = 0; i < m_Hierarchy->at(*m_L1).attribute.size(); i++) {
			if (m_Hierarchy->at(*m_L1).attribute[i].type != "L2Static") {
				*m_L2 = i;
				break;
			}
		}
	}
}
void InputHandler::DecreaseLX() {
	/*
		This method is used to decrement value of active menu
		Does not allow values to go negative
	*/
	if (*m_Depth == 2) {
		for (int i = *m_L2 - 1; i >= 0; i--) {
			if (m_Hierarchy->at(*m_L1).attribute.at(i).type != "L2Static") {
				*m_L2 = i;
				*m_L3 = 0;
				break;
			}
		}
	}
	else if (*m_Depth == 3) {
		if (*m_L3 > 0) {
			*m_L3-=1;
		}
	}
	else {
		cout << "\"InputHandler::DecreaseLX()\": value should be 2 or 3, but is: " << *m_Depth << endl;
	}
}
void InputHandler::IncreaseDepth() {
	/*
		This method increments active menu depth.
		Does not allow to increase depth if active menu doesn't have submenus.
	*/

	if (*m_Depth == 2 && m_Hierarchy->at(*m_L1).attribute.at(*m_L2).window.size() != 0) {
		*m_Depth+=1;
		*m_L3 = m_Hierarchy->at(*m_L1).attribute.at(*m_L2).chosenValue;
	}
}
void InputHandler::DecreaseDepth() {
	/*
		This method decrements active menu depth.
	*/
	if (*m_Depth > 2) {
		*m_Depth-=1;
		*m_L3 = 0;
	}

}

void InputHandler::GetInput() {
	enum KeyCodes {
		//Function keys
		KEY_FUNCTION = 0,
		KEY_F1 = 59,
		KEY_F5 = 63,
		KEY_F6 = 64,
		KEY_F9 = 67,
		KEY_F10 = 68,
		//Arrow keys
		KEY_ARROW = 224,
		KEY_UP = 72,
		KEY_LEFT = 75,
		KEY_RIGHT = 77,
		KEY_DOWN = 80,
		//Command keys
		KEY_ENTER = 13,
		KEY_ESC = 27
	};

	int firstValue = _getch();
	int secondValue = _getch();

	/*
		Q: Why I sometimes call this method recursively?
		A: To iterate this method until I get specified input without continuing any
		outside loops. For example, if I get unspecified input, then I won't change
		m_PressedKey value, but will exit this method. Thus, the last command will be
		executed again in the next program loop. This is unwanted behaviour.
	*/

	//Reading function keys
	if (firstValue == KEY_FUNCTION) {

		switch (secondValue) {
		case KEY_F1: m_PressedKey = "F1"; break;
		case KEY_F5: m_PressedKey = "F5"; break;
		case KEY_F6: m_PressedKey = "F6"; break;
		case KEY_F9: m_PressedKey = "F9"; break;
		case KEY_F10: m_PressedKey = "F10"; break;
		default: this->GetInput(); break;
		}

	}
	//Reading arrow keys
	else if (firstValue == KEY_ARROW) {

		switch (secondValue) {
		case KEY_UP: m_PressedKey = "Up"; break;
		case KEY_LEFT: m_PressedKey = "Left"; break;
		case KEY_RIGHT: m_PressedKey = "Right"; break;
		case KEY_DOWN: m_PressedKey = "Down"; break;
		default: this->GetInput(); break;
		}

	}
	//Reading command keys
	else if (firstValue == KEY_ENTER) { m_PressedKey = "Enter"; }
	else if (firstValue == KEY_ESC) { m_PressedKey = "Esc"; }
	else { this->GetInput(); }
}
bool InputHandler::KeyPressed(std::string key) {
	return key == m_PressedKey ? true : false;
}

InputHandler::InputHandler(){
}
InputHandler::~InputHandler(){
}
void InputHandler::SetInputHandler(
	int *depth, int *l1, int *l2, int *l3, vector<L1> *hierarchy) {
	/*
		linking InputHandler and BIOS data
	*/

	m_Depth = depth;
	m_L1 = l1;
	m_L2 = l2;
	m_L3 = l3;
	m_DataSet = true;
	m_Hierarchy = hierarchy;
}