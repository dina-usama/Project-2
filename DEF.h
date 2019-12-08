

#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "component.h"
using namespace std;

struct comp_def
{
	string nikename;
	string lef;
	vector<pin> pn;
	double posx;
	double posy;
	string ori;
	double xwidth;
	double ylength;

	vector<pin> fid(string lf, vector<component> c)
	{
		for (int i = 0; i < c.size(); i++)
		{
			if (c[i].get_name() == lf)
				return c[i].pn;
		}
	}

	void xy(string lf, vector<component> c)
	{

		for (int i = 0; i < c.size(); i++)
		{
			if (c[i].get_name() == lf) {
				this->xwidth = c[i].xwidth;
				this->ylength = c[i].ylength;
				break;
			}
		}
	}
};

struct pins_def
{
	string name;
	string layer;
	string pos_x, pos_y;
	string ori;
};

struct netcomp
{
	comp_def cm;
	string nickname, pin, lef;

};

struct routing
{
	string layer;
	vector<string> pos;
};

struct net
{
	string netname;

	vector<netcomp> ncp;

	vector<routing> r;
};
class DEF
{
private:

	string in_file;
	ifstream input;

public:
	int DEF_Units;
	vector<pins_def> pns;
	vector<comp_def> cd;
	vector<net> nets;
	vector<routing> route;
	DEF(string in_file, vector<component> ct);
};




