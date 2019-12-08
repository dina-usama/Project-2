#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "pin.h"

class LIB
{
private:
	struct timing {

		string related_pin;
		string timing_sense;
		vector<string> input_Trans;
		vector<string> output_cap;
		vector<vector<string>> delaysrise;
		vector<vector<string>> delaysfall;
		vector<vector<string>> OutTransrise;
		vector<vector<string>> OutTransfall;
	};

	

	struct pin {
		string pinname;
		string direction;
		string capacitance;

	};

	struct cell {

		string macro;
		vector<pin> pins;
		vector<timing> lut;

	};


	

public:

	
	vector <cell> cells;
	LIB(string);
};

