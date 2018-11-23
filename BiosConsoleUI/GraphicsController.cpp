#include "GraphicsController.h"





/*********************WORKING ON*********************/





void GraphicsController::Test() {
	GenerateCrashScreen();
}


/*********************FINISHED*********************/

void GraphicsController::GenerateCrashScreen() {
	ifstream is("BlueScreenOfDeathASCII.txt");
	if (!is.good()) {
		Log("Blue screen of death ASCII.txt  file is missing\n");
		return;
	}

	struct tile {
		int y, x;
		int colBg = COL_BLUE;
		int colFg = COL_BLUE;
		char symb = ' ';
	};
	vector<tile> screen;



	for (int i = 0; i < m_Width && !is.eof(); i++) {
		string line;
		getline(is, line);
		for (int j = 0; j < line.length(); j++) {
			tile temp;
			temp.y = i;
			temp.x = j;
			if (line[j] != ' ') temp.colBg = COL_GREY;
			screen.push_back(temp);
		}
	}
	is.close();


	while (screen.size() != 0) {
		int index = rand() % screen.size();

		DrawSymbol(screen[index].y, screen[index].x, screen[index].colBg, screen[index].colFg, screen[index].symb);
		screen.erase(screen.begin() + index);
		Sleep(0);
	}
}

void GraphicsController::DrawHelpScreen() {
	if (*m_Depth != 4) return;
	//clearing out space
	InsertTextBox(3, 1, "", m_Width - 6, m_Length - 2, COL_GREY, COL_GREY);
	//inserting bigger and smaller boxes
	InsertBox(2, 0, m_Width - 5, m_Length - 1, COL_GREY, COL_BLUE);
	InsertBox(2, 0, 4, m_Length - 1, COL_GREY, COL_BLUE);
	//inserting joints
	InsertText(6, 0, string(1, 195), COL_GREY, COL_BLUE);
	InsertText(6, m_Length - 1, string(1, 180), COL_GREY, COL_BLUE);
	//inserting title text
	string text = "[General Help]";
	int pozX = (m_Length - text.length()) / 2;
	InsertText(4, pozX, text, COL_GREY, COL_BLUE);
	//inserting paragraph text
	vector<string> paragraph;
	paragraph.push_back("The Setup Utility is a ROM-based configuration utility that displays the");
	paragraph.push_back("system's configuration status and provides users with a tool to set their");
	paragraph.push_back("system parameters. Setting incorrect values may cause system boot failure:");
	paragraph.push_back("Load Setup default values to recover.");
	paragraph.push_back("");
	paragraph.push_back("");
	paragraph.push_back("<\x1b/\x1a>: Select Screen");
	paragraph.push_back("<\x18/\x19>: Select Item");
	paragraph.push_back("<Enter>: Select or Enter SubMenu");
	paragraph.push_back("<F9>: Load Setup Default");
	paragraph.push_back("<F10>: Save and Exit");
	paragraph.push_back("<ESC>: Exit Setup");
	paragraph.push_back("<F1>: Displays General Help");
	text.clear();
	for (int i = 0; i < paragraph.size(); i++) {
		InsertText(i + 7, 1, paragraph[i], COL_GREY, COL_BLUE);
	}




}

void GraphicsController::HandleGraphics() {
	if (!m_DataSet) {
		cout << "Error: \"GraphicsController\": data not set.\n";
		return;
	}
	//apply graphical updates
	UpdateTop();
	UpdateL1();
	UpdateL2();
	UpdateBottom();
	if (*m_Depth == 3) UpdateWindow();
	else if (*m_Depth == 4) DrawHelpScreen();
	//redraw screen
	DrawScreen();
	//save current parameters for next frame
	SetLastValues();

}

void GraphicsController::InsertTextBox(int y, int x, string text, int width, int length, int bgCol, int fgCol) {
	vector<string> textBlock;

	//chopping up given text
	while (text.length() >= length) {
		textBlock.push_back(text.substr(0, length));
		text.erase(0, length);
	}
	if (text.length() != 0) {
		textBlock.push_back(text + string(length - text.length(), ' '));
	}

	//displaying given text and  filling remaining box with blank lines
	for (int i = 0; i < width; i++) {
		if (i < textBlock.size())	InsertText(i + y, x, textBlock[i], bgCol, fgCol);
		else						InsertText(i + y, x, string(length, ' '), bgCol, fgCol);
	}
}
void GraphicsController::InsertBox(int y, int x, int width, int length, int bgCol, int fgCol) {
#define tile m_Board[i][j]
	enum {
		lineH = 196,
		lineV = 179,
		cornerTL = 218,
		cornerTR = 191,
		cornerBL = 192,
		cornerBR = 217,
		jointTU = 193,
		jointTR = 195,
		jointTD = 194,
		jointTL = 180,
		jointX = 197
	};

	//drawing top and bottom lines
	InsertText(y, x, string(length, lineH), bgCol, fgCol);
	InsertText(y + width, x, string(length, lineH), bgCol, fgCol);

	//drawing left and right lines
	for (int i = y; i < y + width; i++) {
		InsertText(i, x, string(1, lineV), bgCol, fgCol);
		InsertText(i, x + length, string(1, lineV), bgCol, fgCol);
	}
	//corners
	InsertText(y, x, string(1, cornerTL), bgCol, fgCol);
	InsertText(y, x + length, string(1, cornerTR), bgCol, fgCol);
	InsertText(y + width, x, string(1, cornerBL), bgCol, fgCol);
	InsertText(y + width, x + length, string(1, cornerBR), bgCol, fgCol);

#undef tile
}

void GraphicsController::DrawScreen() {
#define frame m_Board[i][j]
#define frameLast m_BoardLast[i][j]
	for (int i = 0; i < m_Width; i++) {
		for (int j = 0; j < m_Length; j++) {
			/*Redraw tile if:		*/
			//current symbol is space and any parameter don't match except for foreground color 
			if (frame.symb == ' ') {
				if (frame.symb != frameLast.symb ||
					frame.colBg != frameLast.colBg)
					DrawSymbol(i, j, frame.colBg, frame.colFg, frame.symb);
			}
			//current symbol is not space and any of parameters don't match
			else {
				if (frame.symb != frameLast.symb ||
					frame.colBg != frameLast.colBg ||
					frame.colFg != frameLast.colFg)
					DrawSymbol(i, j, frame.colBg, frame.colFg, frame.symb);
			}
		}
	}

}

void GraphicsController::UpdateTop() {
	/*Changing background color*/
	int bgCol = COL_CYAN;
	int textCol = COL_BLACK;
	for (int i = 0; i < m_Length; i++) {
		if (m_Board[0][i].colBg != bgCol) {
			m_Board[0][i].colBg = bgCol;
			m_Board[0][i].needsUpdate = true;
		}
	}

	string text = "InsydeH20 Setup Utility";
	int poz = (m_Length - text.length()) / 2;
	InsertText(0, (m_Length - text.length()) / 2, text, COL_CYAN, COL_BLACK);

	text = "Rev. 5.0";
	InsertText(0, m_Length - text.length() - 2, text, COL_CYAN, COL_BLACK);
}
void GraphicsController::UpdateL1() {
#define cBoard(i) m_Board[1][i]
#define cHier(i) m_Hierarchy->at(i)
	//highlighted option color values
	int cTextCol = COL_BLUE;
	int cBgCol = COL_GREY;
	//non-highlighted option color values
	int textCol = COL_GREY;
	int bgCol = COL_BLUE;

	//painting row blue
	for (int j = 0; j < m_Length; j++) {
		if (cBoard(j).colBg != bgCol) {
			cBoard(j).colBg = bgCol;
			cBoard(j).needsUpdate = true;
		}
	}

	//applying graphical update where neccessary
	for (int i = 0; i < m_Hierarchy->size(); i++) {
		//calculating i-th text element pozition
		int x = 2;
		for (int j = 0; j < i; j++)
			x += m_Hierarchy->at(j).title.length() + 1;
		//checking if i-th element needs redrawing and then do it
		if (i == *m_L1) {

			InsertText(1, x, cHier(i).title, cBgCol, cTextCol);
		}
		else {

			InsertText(1, x, cHier(i).title, bgCol, textCol);
		}
	}
#undef cBoard
#undef cHier
}
void GraphicsController::UpdateL2() {
#define cAtt m_Hierarchy->at(*m_L1).attribute
	//highlighted option color values
	int cTextCol = COL_WHITE;
	int cBgCol = COL_GREY;
	//non-highlighted option color values
	int textCol = COL_BLUE;
	int bgCol = COL_GREY;
	//total length
	double ratio = 0.663;
	int lineLength = m_Length * ratio;

	/*
		Filling background space and inserting text on left side
	*/
	for (int i = 3; i < 5; i++)
		InsertText(i, 1, string(lineLength, ' '), bgCol, bgCol);
	for (int i = 0; i < cAtt.size(); i++) {

		InsertText(i + 5, 1, string(lineLength, ' '), bgCol, bgCol);
		if (i == *m_L2) {
			if (cAtt[i].type == "L2Static") {
				InsertText(i + 5, 1, cAtt[i].name, cBgCol, COL_BLACK);
				InsertText(i + 5, 40, cAtt[i].value, cBgCol, COL_BLACK);
			}
			else if (cAtt[i].type == "L2Editable") {
				InsertText(i + 5, 1, cAtt[i].name, cBgCol, cTextCol);
				InsertText(i + 5, 40, "[" + cAtt[i].window[cAtt[i].chosenValue].option + "]", cBgCol, cTextCol);
			}
			else if (cAtt[i].type == "L2Command") {
				InsertText(i + 5, 1, cAtt[i].value, cBgCol, cTextCol);
			}
		}
		else {
			if (cAtt[i].type == "L2Static") {
				InsertText(i + 5, 1, cAtt[i].name, bgCol, COL_BLACK);
				InsertText(i + 5, 40, cAtt[i].value, bgCol, COL_BLACK);
			}
			else if (cAtt[i].type == "L2Editable") {
				InsertText(i + 5, 1, cAtt[i].name, bgCol, textCol);
				InsertText(i + 5, 40, "[" + cAtt[i].window[cAtt[i].chosenValue].option + "]", bgCol, textCol);
			}
			else if (cAtt[i].type == "L2Command") {
				InsertText(i + 5, 1, cAtt[i].value, bgCol, textCol);
			}
		}
	}
	for (int i = cAtt.size() + 5; i < m_Width - 3; i++) {
		InsertText(i, 1, string(lineLength, ' '), bgCol, bgCol);
	}

	/*
		Drawing box and adding description
	*/
	//basic box shape
	InsertBox(2, 0, m_Width - 5, m_Length - 1, COL_GREY, COL_BLUE);

	/*description section if needed*/
	if (m_Hierarchy->at(*m_L1).attribute[*m_L2].type != "L2Static") {
		//vertical line
		for (int i = 3; i < m_Width - 2; i++) {
			InsertText(i, lineLength + 1, string(1, 179), COL_GREY, COL_BLUE);
		}

		//┬ and ┴ symbols
		InsertText(2, lineLength + 1, string(1, 194), COL_GREY, COL_BLUE);
		InsertText(m_Width - 3, lineLength + 1, string(1, 193), COL_GREY, COL_BLUE);

		//horizontal line
		InsertText(6, lineLength + 2, string(47, 196), COL_GREY, COL_BLUE);

		//├ and ┤ symbols
		InsertText(6, lineLength + 1, string(1, 195), COL_GREY, COL_BLUE);
		InsertText(6, m_Length - 1, string(1, 180), COL_GREY, COL_BLUE);

		//grey filling and static text
		InsertTextBox(3, lineLength + 2, "", 3, m_Length - lineLength - 3, COL_GREY, COL_BLUE);
		string helpText = "Item Specific Help";
		int poz = lineLength + (m_Length - lineLength - helpText.length()) / 2;
		InsertText(4, poz, helpText, COL_GREY, COL_BLUE);

		//description text
		string text = m_Hierarchy->at(*m_L1).attribute[*m_L2].description;
		InsertTextBox(7, lineLength + 2, text, m_Width - 10, m_Length - lineLength - 3, COL_GREY, COL_BLUE);
	}
	//if current option doesn't have description, then erase last description by filling with black space
	else {
		InsertTextBox(3, lineLength + 1, "", m_Width - 6, m_Length - lineLength - 2, COL_GREY, COL_GREY);
	}
}
void GraphicsController::UpdateWindow() {
	if (*m_Depth != 3) return;

#define cWind m_Hierarchy->at(*m_L1).attribute[*m_L2].window
	int longestOption = 0;
	int optionCount = cWind.size();
	for (int i = 0; i < optionCount; i++) {
		longestOption = max(longestOption, cWind[i].option.length());
	}

	int y = (m_Width / 2) - ((optionCount) / 2);
	int x = (m_Length / 2) - ((longestOption) / 2);
	int offsetY = 1;
	int offsetX = 5;
	//box sides
	InsertBox(y, x, optionCount + offsetY, longestOption + offsetX, COL_BLUE, COL_GREY);

	//fill
	InsertTextBox(y + 1, x + 1, "", optionCount, longestOption + offsetX - 1, COL_BLUE, COL_BLUE);

	//text
	for (int i = 0; i < optionCount; i++) {
		if (i == *m_L3) {
			InsertText(y + i + 1, x + 2, cWind[i].option, COL_GREY, COL_BLUE);
		}
		else {
			InsertText(y + i + 1, x + 2, cWind[i].option, COL_BLUE, COL_GREY);

		}
	}
}
void GraphicsController::UpdateBottom() {
	/*Changing background color*/
	for (int i = 0; i < m_Length; i++) {
		//second to last row
		m_Board[m_Width - 2][i].colBg = COL_CYAN;
		m_Board[m_Width - 2][i].needsUpdate = true;
		//last row
		m_Board[m_Width - 1][i].colBg = COL_CYAN;
		m_Board[m_Width - 1][i].needsUpdate = true;
	}

	/*Adding text*/
	double pozRatio[] = { 0.01, 0.19, 0.44, 0.76 };
	string firstKeys[] = { "F1", "\x18\x19", "F5/F6", "F9" };
	string secondKeys[] = { "Esc", "\x1b\x1a", "Enter", "F10" };
	string firstCommand[] = { "Help", "Select Item", "Change Values", "Setup Defaults" };
	string secondCommand[] = { "Exit", "Select Menu", "Select \x10 SubMenus", "Save and Exit" };

	for (int i = 0; i < 4; i++) {
		int pozValue = m_Length * pozRatio[i];
		int pozOffset = max(firstKeys[i].length(), secondKeys[i].length()) + 1;
		//second to last row
		InsertText(m_Width - 2, pozValue, firstKeys[i], COL_CYAN, COL_WHITE);
		InsertText(m_Width - 2, pozValue + pozOffset, firstCommand[i], COL_CYAN, COL_BLACK);
		//last row
		InsertText(m_Width - 1, pozValue, secondKeys[i], COL_CYAN, COL_WHITE);
		InsertText(m_Width - 1, pozValue + pozOffset, secondCommand[i], COL_CYAN, COL_BLACK);
	}
}

void GraphicsController::InsertText(int y, int x, string text, int colBg, int colFg) {
	//not exceeding vertical bounds
	if (y >= m_Width || y < 0) return;

	for (int i = x; i < x + text.length() && i < m_Length; i++) {
		//update tile only if it needs update, aka one of the parameters don't match
		if (text[i - x] == ' ') {
			if (text[i - x] != m_Board[y][i].symb ||
				m_Board[y][i].colBg != colBg)
				m_Board[y][i].needsUpdate = true;
		}
		else {
			if (text[i - x] != m_Board[y][i].symb ||
				m_Board[y][i].colBg != colBg ||
				m_Board[y][i].colFg != colFg)
				m_Board[y][i].needsUpdate = true;
		}

		m_Board[y][i].symb = text[i - x];
		m_Board[y][i].colBg = colBg;
		m_Board[y][i].colFg = colFg;
	}
}

void GraphicsController::SetLastValues() {
	m_DepthLast = *m_Depth;
	m_L1Last = *m_L1;
	m_L2Last = *m_L2;
	m_L3Last = *m_L3;

	for (int i = 0; i < m_Width; i++) {
		for (int j = 0; j < m_Length; j++) {
			m_BoardLast[i][j] = m_Board[i][j];
		}
	}
}

void GraphicsController::Goto(int y, int x) {
	COORD coord;
	coord.Y = y;
	coord.X = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void GraphicsController::SetColor(int background, int foreground) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), background * 16 + foreground);
}
void GraphicsController::DrawSymbol(int y, int x, int background, int foreground, char symbol) {
	Goto(y, x);
	SetColor(background, foreground);
	cout << symbol;
	SetColor(COL_BLACK, COL_BLACK);
}

GraphicsController::GraphicsController() {
	os.open("LogGraphicsController.txt");
}
GraphicsController::~GraphicsController() {

}
void GraphicsController::SetGraphicsController(
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
