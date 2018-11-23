#include <iostream>
#include "BIOS.h"
#include <vector>
#include <conio.h>
using namespace std;

void SetConsole() {
	//setting text color to normal
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

	//Setting up font
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = 12;                   // Width of each character in the font
	cfi.dwFontSize.Y = 16;                   // Height
	cfi.FontWeight = 400;
	std::wcscpy(cfi.FaceName, L"Consolas");  // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	//setting dimensions
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 1300, 600, TRUE); // 1300 width, 600 height

}

int main() {
	bool displayWarning = true;

	//display warning messages and setup guide
	if (displayWarning) {
		SetConsole();

		cout << "Warning, this program will probably change your console graphical settings, so you will need to restore them manually.\n";
		cout << "This will only work correctly in FHD (1920x1080) resolution or higher.\n\nDo you want to proceed? (y/n)\n";
		//handling input
		while (true) {
			int firstKey = _getch();
			int secondKey = _getch();
			//if pressed n
			if (firstKey == 78 || firstKey == 110) exit(0);
			//if pressed y
			else if (firstKey == 121 || firstKey == 89) break;
		}

		cout << "In order to correctly display this program, you will need to manually change some console graphical settings.\n\n";
		cout << "1. Press Alt+Spacebar.\n";
		cout << "2. In menu press \"Properties\"\n";
		cout << "3. Press \"Layout\" tab.\n";
		cout << "4. At screen buffer set the values:\n";
		cout << "	Uncheck \"Wrap text output on resize\" box\n";
		cout << "	Set Width to 147\n";
		cout << "	Set Height to 97\n";
		cout << "5. Press okay.\n";
		cout << "\nAfter you have finished, press keys to proceed or exit (y/n):\n";

		//handling input
		while (true) {
			int firstKey = _getch();
			int secondKey = _getch();
			//if pressed n
			if (firstKey == 78 || firstKey == 110) exit(0);
			//if pressed y
			else if (firstKey == 121 || firstKey == 89) break;
		}
	}

	BIOS Bios;
	Bios.Run();


}