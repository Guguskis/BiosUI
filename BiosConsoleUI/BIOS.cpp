#include "BIOS.h"






void BIOS::Test() {
	while (1) {
		HandleGraphics();
		HandleInput();
	}
}


/***************************FINISHED***************************/
void BIOS::System32Mode() {
	double timer = 1000;
	double ratio = 0.85;
	int height = 1080;
	int length = 1920;

	thread mouseMover([&timer, ratio]() {
		while (timer > 0.1) {
			SetCursorPos(rand() % 1920, rand() % 1080);
			timer *= ratio;
			Sleep((int)timer);
		}
	});
	thread soundMaker([&timer]() {
		while (timer > 0.1) {
			cout << (char)7;
			Sleep(max((int)timer, 160));
		}
	});


	mouseMover.join();
	soundMaker.join();
	//system("c:\\windows\\system32\\shutdown /s");
	exit(0);

}


void BIOS::Run() {
	while (1) {
		HandleGraphics();
		HandleInput();
	}
}

BIOS::BIOS(){
	SetConsole();
	InputHandler::SetInputHandler(&m_Depth, &m_L1, &m_L2, &m_L3, &m_Hierarchy);
	GraphicsController::SetGraphicsController(&m_Depth, &m_L1, &m_L2, &m_L3, &m_Hierarchy);
	LoadData(m_FileName);

	osTemp.open("LogBIOS.txt");
}
BIOS::~BIOS(){
	InputHandler::~InputHandler();
	//reset console settings to default
	system("reset");
}

void BIOS::SetConsole() {
	//Enabling fullscreen
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	
	//ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	
	//this mess up special character display, like left arrow
	//system("MODE CON COLS=147 LINES=67");

	//this somehow makes cursor visible
	/*
	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	*/

	//Removing scrollbar
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	COORD new_size =
	{
		info.srWindow.Right - info.srWindow.Left + 1,
		info.srWindow.Bottom - info.srWindow.Top + 1
	};
	SetConsoleScreenBufferSize(handle, new_size);

	//Setting up font
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 12;                   // Width of each character in the font
	cfi.dwFontSize.Y = 16;                   // Height
	cfi.FontWeight = 600;
	std::wcscpy(cfi.FaceName, L"Terminal");  // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	//Hiding cursor
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
	
	//Moving mouse pointer to corner to hide it
	SetCursorPos(1920, 1080);
}

void BIOS::HandleInput() {
	InputHandler::GetInput();	 //reads user input

#define Att m_Hierarchy.at(m_L1).attribute.at(m_L2)
		if (m_Depth == 2) {
			if (KeyPressed("Down")) IncreaseLX();
			else if (KeyPressed("Up")) DecreaseLX();
			else if (KeyPressed("Right")) IncreaseL1();
			else if (KeyPressed("Left")) DecreaseL1();
			else if (KeyPressed("Esc")) exit(0);
			else if (KeyPressed("F1")) m_Depth = 4;
			else if (KeyPressed("Enter") && Att.type == "L2Editable") IncreaseDepth();
			else if (KeyPressed("Enter") && Att.type == "L2Command") {
				if (Att.value == "Exit Saving Changes") {
					SaveData(m_FileName);
					exit(0);
				}
				else if (Att.value == "Exit Discarding Changes") {
					exit(0);
				}
				else if (Att.value == "Discard Changes") {
					LoadData(m_FileName);
				}
				else if (Att.value == "Save Changes") {
					SaveData(m_FileName);
				}
				else if (Att.value == "Load Default Settings") {
					LoadData("DefaultSettings.txt");
				}
				else if (Att.value == "Delete System32") {
					System32Mode();
				}
			}
		}
		else if (m_Depth == 3) {
			if (KeyPressed("Down")) IncreaseLX();
			else if (KeyPressed("Up")) DecreaseLX();
			else if (KeyPressed("Enter")) {
				Att.chosenValue = m_L3;
				DecreaseDepth();
			}
			else if (KeyPressed("Esc")) DecreaseDepth();
		}
		else if (m_Depth == 4) {
			if (KeyPressed("F1")) m_Depth = 2;
		}
}
void BIOS::HandleGraphics() {
	GraphicsController::HandleGraphics();
}
bool BIOS::KeyPressed(string key) {
	return InputHandler::KeyPressed(key);
}

void BIOS::SaveData(string fileName) {
	vector<string> outputData;

	/*
		dumping data into outputData vector as a protection
		to disallow unwanted Data file editing if generated output is corrupted
	*/
	for (int i = 0; i < m_Hierarchy.size(); i++) {
		outputData.push_back("L1");
		outputData.push_back(m_Hierarchy[i].title);

		for (int j = 0; j < m_Hierarchy[i].attribute.size(); j++) {
			//defining current attribute to make code look cleanier
#define cAttr m_Hierarchy[i].attribute[j] 
#define cout osTemp
			outputData.push_back(cAttr.type);

			if (cAttr.type == "L2Static") {
				outputData.push_back(cAttr.name);
				outputData.push_back(cAttr.value);
			}
			else if (cAttr.type == "L2Editable") {
				outputData.push_back(cAttr.name);
				outputData.push_back(to_string(cAttr.chosenValue));
				outputData.push_back(cAttr.description);
				//L3Window
				for (int k = 0; k < cAttr.window.size(); k++) {
					outputData.push_back(cAttr.window[k].type);
					outputData.push_back(cAttr.window[k].option);
				}
			}
			else if (cAttr.type == "L2Command") {
				outputData.push_back(cAttr.value);
				outputData.push_back(cAttr.description);
			}
			else if (cAttr.type == "") {
				//skipping empty attribute
			}
			else {
				cout << "Unspecified instruction at hierachy index " << i << ", attribute index: " << j << endl;
				cout << m_Hierarchy[i].title << endl;
				cout << "Instruction: \"" << cAttr.type << "\"\n";
				cout << "Canceling data save.\n";
				return;
			}
		}
	}

	/*
		Writing to data file after "kind of making sure" output is correct
	*/
	ofstream os(fileName);

	for (int i = 0; i < outputData.size(); i++) {
		os << outputData[i];
		if (i != outputData.size() - 1) os << endl;
	}

	os.close();
}
void BIOS::LoadData(string fileName) {
	ifstream is(fileName);
	if (!is.good()) {
		m_DataLoaded = false;
		cout << "Given data file does not exist: \"" << fileName << "\"" << endl;
		is.close();
		return;
	}
	m_Hierarchy.clear();
	while (!is.eof()) {
		string level;
		getline(is, level);

		/*
			this is protection from unnecessary space characters and blank lines
		*/
		if (level.find("L1") + 1 != 0) level = "L1";
		else if (level.find("L2Static") + 1 != 0) level = "L2Static";
		else if (level.find("L2Editable") + 1 != 0) level = "L2Editable";
		else if (level.find("L2Command") + 1 != 0) level = "L2Command";
		else if (level.find("L3Window") + 1 != 0) level = "L3Window";
		else if (level == "")						continue;

		/*
			These variables makes code look cleanier
			lastL1Index - stores the value of last L1 index
			lastL2Index - stores the value of last L1's lastL2 index
		*/
		int lastL1Index = m_Hierarchy.size() - 1;
		int lastL2Index = 0;
		if (lastL1Index >= 0)
			lastL2Index = m_Hierarchy[lastL1Index].attribute.size() - 1;
		/*
			Todo:
				protection
					maybe I should check if m_Hierarchy vector has atleast 1 element
					before push_backing L2 data type?
		*/
		if (level == "L1") {
			L1 tempStr;
			getline(is, tempStr.title);
			m_Hierarchy.push_back(tempStr);
		}
		else if (level == "L2Static") {
			L2 tempStr;
			tempStr.type = level;
			getline(is, tempStr.name);
			getline(is, tempStr.value);
			m_Hierarchy[lastL1Index].attribute.push_back(tempStr);
		}
		else if (level == "L2Editable") {
			L2 tempStr;
			tempStr.type = level;
			getline(is, tempStr.name);
			string tempChosenValue;
			getline(is, tempChosenValue);

			//if number is invalid, set it to 0
			try { tempStr.chosenValue = stoi(tempChosenValue); }
			catch (...) { tempStr.chosenValue = 0; }

			getline(is, tempStr.description);

			m_Hierarchy[lastL1Index].attribute.push_back(tempStr);
		}
		else if (level == "L2Command") {
			L2 tempStr;
			tempStr.type = level;
			getline(is, tempStr.value);
			getline(is, tempStr.description);

			m_Hierarchy[lastL1Index].attribute.push_back(tempStr);
		}
		else if (level == "L3Window") {
			L3 tempStr;
			tempStr.type = level;
			getline(is, tempStr.option);
			m_Hierarchy[lastL1Index].attribute[lastL2Index].window.push_back(tempStr);
		}
		else {
			cout << "Error reading data file, unknown command: \"" << level << "\"" << endl;
			m_DataLoaded = false;
			is.close();
			return;
		}
	}

	//crash protection against empty file
	if (m_Hierarchy.size() == 0){
		L1 temp;
		temp.attribute.push_back(L2());
		m_Hierarchy.push_back(temp);
	}
	else {
		for (int i = 0; i < m_Hierarchy.size(); i++) {
			if (m_Hierarchy[i].attribute.size() == 0) {
				m_Hierarchy[i].attribute.push_back(L2());
			}
		}
	}

	m_DataLoaded = true;
	is.close();
}
