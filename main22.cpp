#include <iostream>
#include <fstream>
#include <string>
#include "circuit.h"
#include "DEF.h"
#include "LIB.h"
#include <vector>
#include <algorithm>
using namespace std;

void getOrigin(comp_def  c, double& x0, double& y0);
void getCorners(comp_def c, rect r, double& c1x, double& c1y, double& c2x, double& c2y, double unit);
bool pin_intersect(netcomp nc, double routex, double routey, circuit c);


int main()
{
	string s1, s2, s3;

	cout << "please enter name of LEF file " << endl;
	cin >> s1;

	cout << "please enter name of DEF file " << endl;
	cin >> s2;

	/*
	cout << "please enter name of library file " << endl;
	cin >> s3;
	*/
	circuit c(s1);
	DEF d(s2, c.comp);
	//LIB b(s3);


}


void getOrigin( comp_def c, double& x0, double& y0)
{
	
	
	string ori = c.ori;
	if (ori == "N")
	{
		x0 = c.posx;
		y0 = c.posy;
	}

	else if (ori == "S")
	{
		x0 = c.posx + c.xwidth;
		y0 = c.posy - c.ylength;
	}

	else if (ori == "FS")
	{
		x0 = c.posx;
		y0 = c.posy - c.ylength;
	}



	else if (ori == "E")
	{
		x0 = c.posx;
		y0 = c.posy - c.xwidth;
	}

	else if (ori == "FE")
	{
		x0 = c.posx+c.ylength;
		y0 = c.posy - c.xwidth;
	}

	else if (ori == "W")
	{
		x0 = c.posx + c.ylength;
		y0 = c.posy ;
	}


	else if (ori == "FW")
	{
		x0 = c.posx;
		y0 = c.posy;
	}


	else if (ori == "FN")
	{
		x0 = c.posx+c.xwidth;
		y0 = c.posy;
	}


}


void getCorners( comp_def c, rect r , double& c1x, double& c1y, double& c2x, double& c2y, double unit)

{
	double x0;
	double y0;
	getOrigin(c, x0, y0);


	
	string ori = c.ori;

	if (ori == "N")
	{
		c1x = x0 + unit* r.x1;
		c1y = y0 - unit * r.y1;


		c2x = x0 + unit * r.x2;
		c2y = y0 - unit * r.y2;

		
	}

	else if (ori == "S")
	{
		c1x = x0 - unit * r.x1;
		c1y = y0 + unit * r.y1;


		c2x = x0 - unit * r.x2;
		c2y = y0 + unit * r.y2;
	
	}

	else if (ori == "FS")
	{

		c1x = x0 + unit * r.x1;
		c1y = y0 + unit * r.y1;

		c2x = x0 + unit * r.x2;
		c2y = y0 + unit * r.y2;

		
	}



	else if (ori == "E")
	{
	
		c1x = x0 + unit * r.y1;
		c1y = y0 + unit * r.x1;

		c2x = x0 + unit * r.y2;
		c2y = y0 + unit * r.x2;


		
	}

	else if (ori == "FE")
	{

		c1x = x0 - unit * r.y1;
		c1y = y0 + unit * r.x1;

		c2x = x0 - unit * r.y2;
		c2y = y0 + unit * r.x2;


	}

	else if (ori == "W")
	{
		c1x = x0 - unit * r.y1;
		c1y = y0 - unit * r.x1;

		c2x = x0 - unit * r.y2;
		c2y = y0 - unit * r.x2;




	}


	else if (ori == "FW")
	{
		c1x = x0 + unit * r.y1;
		c1y = y0 - unit * r.x1;

		c2x = x0 + unit * r.y2;
		c2y = y0 - unit * r.x2;




	}


	else if (ori == "FN")
	{
		
		c1x = x0 - unit * r.x1;
		c1y = y0 - unit * r.y1;

		c2x = x0 - unit * r.x2;
		c2y = y0 - unit * r.y2;

	}



}


bool pin_intersect(netcomp nc,  double routex, double routey, circuit c)
{
	
	int x = 0;
	bool flag = false;

	while (nc.cm.pn[x].getpinname != nc.pin)
	{
		x++;

	}

	double c1x, c1y, c2x, c2y;

	for (int i = 0; i < nc.cm.pn[x].posrects.size(); i++)
	{
		for (int j = 0; j < nc.cm.pn[x].posrects[i].rects.size(); j++)
		{
			int s;
			int t;
				
			getCorners(nc.cm, nc.cm.pn[x].posrects[i].rects[j], c1x, c1y, c2x, c2y, c.unit);

			if (c1x > c2x) s = 0;
			else s = 1;


			if (c1y > c2y) t = 0;
			else t= 1;

			if (s == 0 && t == 0)
			{
				if (routex >= c2x && routex <= c1x && routey >= c2y && routey <= c1y) flag = true;
				else flag = false;

			}

			else if (s == 0 && t == 1)
			{
				if (routex >= c2x && routex <= c1x && routey >= c1y && routey <= c2y) flag = true;
				else flag = false;

			}

			else if (s == 1 && t == 0)
			{
				if (routex <= c2x && routex >= c1x && routey >= c2y && routey <= c1y) flag = true;
				else flag = false;
			}

			else if (s == 1 && t == 1)
			{
				if (routex <= c2x && routex >= c1x && routey <= c2y && routey >= c1y) flag = true;
				else flag = false;
			}
			if (flag == true) break;

		}
		if (flag == true) break;

	}

	return flag;


}













