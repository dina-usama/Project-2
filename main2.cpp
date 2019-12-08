#include <iostream>
#include <fstream>
#include <string>
#include "circuit.h"
#include "DEF.h"
#include "LIB.h"
#include <vector>
#include <algorithm>
using namespace std;


int main()
{

	string s1;
	cout << "please enter name of LEF file " << endl;
	cin >> s1;
	circuit c(s1);

	return 0;
}


