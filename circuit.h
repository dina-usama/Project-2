#pragma once
#include <fstream>
#include <iostream>
#include "component.h"
using namespace std;

struct metalprop
{
	string mlayer;
	double cpsqr;
	double rpsqr;
	double width;

};

class circuit
{
private:
	string in_filename;
	ifstream input;
	
	
public:
	vector<component> comp;
	vector<metalprop> mprops;
	int unit;

	circuit(string in);
};

