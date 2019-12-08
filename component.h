#pragma once
#include <string>
#include <vector>
#include "pin.h"

using namespace std;
class component
{
private:
	string name;
	string nickname;
	

public:
	vector<pin> pn;
	double xwidth;
	double ylength;
	component(string name, string nickname);
	~component();
	void set_name(string name);
	void set_nickname(string nickname);
	void set_pin(pin p);
	string get_name();
	string get_nickname();

};

