#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>
#include <vector>
#include <wchar.h>
using namespace std;

struct L3 {
	/*
		L3 is used for options in new window
	*/
	string type;
	string option;
};
struct L2 {
	/*	Possible types:
			static
			editable
			command
	*/
	string type;
	/*
		Static data:
			name, value
		Editable data:
			name, chosenValue, window, description
		Command data:
			value, description
	*/
	string name, value, description;
	int chosenValue = 0;
	vector<L3> window;
};
struct L1 {
	string title;
	vector<L2> attribute;
};
struct Board {
	int colFg=7, colBg=0;
	bool needsUpdate = true;
	char symb = ' ';
	bool windowVisible = false;
};

#endif