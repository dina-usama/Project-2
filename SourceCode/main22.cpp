#include <iostream>
#include <fstream>
#include <string>
#include "circuit.h"
#include "DEF.h"
#include "LIB.h"
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

void getOrigin(comp_def  c, double& x0, double& y0, double unit);
void getCorners(comp_def c, rect r, double& c1x, double& c1y, double& c2x, double& c2y, double unit);

bool pin_intersect(netcomp nc, double routex, double routey, double unit);


struct C_res {
	string endNode;
	string endPin;
	double res = 0.0;
};

struct wire {
	string startComp;
	string startPin;
	double cap = 0.0;
	C_res res;
	string layer;
	double length = 0.0;
};

struct IN {
	string name;
	double posx = 0.0;
	double posy = 0.0;
};

vector <wire> wires;
vector<IN> internalNodes;

int main()
{
	string s1, s2;

	std::cout << "please enter name of LEF file " << endl;
	cin >> s1;

	std::cout << "please enter name of DEF file " << endl;
	cin >> s2;

	circuit c(s1);


	
	DEF d(s2, c.comp);
	
	wire wire;
	int counter = 0;
	ofstream output("spef.txt");
	for (int x = 0; x < d.nets.size(); x++)
	{
		for (int y = 0; y < d.nets[x].r.size(); y++)
		{
			bool flag = false;
			wire.layer = d.nets[x].r[y].layer;
			int w_l = 0;
			for (int yy = 0; yy < d.nets[x].r[y].pos.size(); yy++)
			{
				double xpos = stod(d.nets[x].r[y].pos[yy].substr(0, d.nets[x].r[y].pos[yy].find(" ")));
				double ypos = stod(d.nets[x].r[y].pos[yy].substr(d.nets[x].r[y].pos[yy].find(" "), string::npos));

				//searching in the internal nodes first

				for (int zz = 0; zz < internalNodes.size() && !flag; zz++)
				{
					if (xpos == internalNodes[zz].posx && ypos == internalNodes[zz].posy)
					{
						if (yy == 0)
						{
							wire.startComp = internalNodes[zz].name;
							wire.startPin = "INN";
						}
						else if (yy == d.nets[x].r[y].pos.size() - 1)
						{
							wire.res.endNode = internalNodes[zz].name;
							wire.res.endPin = "INN";
						}
						flag = true;

					}

				}

				for (int z = 0; z < d.nets[x].ncp.size() && !flag; z++)
				{

					if (pin_intersect(d.nets[x].ncp[z], xpos / d.unitd, ypos / d.unitd, d.unitd))
					{
						flag = true;
						if (yy == 0)
						{
							wire.startComp = d.nets[x].ncp[z].nickname;
							wire.startPin = d.nets[x].ncp[z].pin;
						}
						else if (yy == d.nets[x].r[y].pos.size() - 1)
						{
							wire.res.endNode = d.nets[x].ncp[z].nickname;
							wire.res.endPin = d.nets[x].ncp[z].pin;
						}
					}
				}
				if (!flag)
				{
					IN in;
					in.name = to_string(counter++);
					if (yy == 0)
						wire.startComp = in.name;
					else if (yy == d.nets[x].r[y].pos.size() - 1)
					{
						wire.res.endNode = (in.name);
						wire.res.endPin = "INN";
					}
					in.posx = xpos;
					in.posy = ypos;
					internalNodes.push_back(in);
				}



				//next position in the same route. //calculate total length
				flag = false;

				if (yy != d.nets[x].r[y].pos.size() - 1)
					w_l += abs(xpos - stod(d.nets[x].r[y].pos[yy + 1].substr(0, d.nets[x].r[y].pos[yy + 1].find(" ")))) + abs(ypos - stod(d.nets[x].r[y].pos[yy + 1].substr(d.nets[x].r[y].pos[yy + 1].find(" "), string::npos)));

			}
			wire.length = w_l / d.unitd;
			for (int k = 0; k < c.mprops.size(); k++)
			{
				if (c.mprops[k].mlayer.find(wire.layer) != -1)
				{
					wire.cap = c.mprops[k].cpsqr * wire.length * c.mprops[k].width;
					wire.res.res = c.mprops[k].rpsqr * (wire.length / c.mprops[k].width);
					break;
				}
			}
			wires.push_back(wire);
			//next route in the net

		}
		//next net
		output << "*D_NET " << d.nets[x].netname << " ";
		double tempo = 0;
		for (int aa = 0; aa < wires.size(); aa++)
		{
			tempo += wires[aa].cap;
		}
		output << tempo << endl;
		output << "*CONN " << endl;
		//connection part of SPEF	
		for (int a = 0; a < d.nets[x].ncp.size(); a++)
		{
			if (d.nets[x].ncp[a].nickname == "PIN")
				output << "*P " << d.nets[x].ncp[a].pin << " I" << endl;
			else
			{
				output << "*I " << d.nets[x].ncp[a].nickname << ":" << d.nets[x].ncp[a].pin << " ";
				bool flag1 = false;
				for (int b = 0; b < d.nets[x].ncp[a].cm.pn.size() && !flag1; b++)
				{
					if (d.nets[x].ncp[a].pin == d.nets[x].ncp[a].cm.pn[b].getpinname())
					{
						flag1 = true;
						output << d.nets[x].ncp[a].cm.pn[b].getType() << endl;
					}
				}
			}
		}
		//capacitance part of SPEF
		output << "*CAP" << endl;
		for (int c = 0; c < wires.size(); c++)
		{
			output << c + 1 << (wires[c].startPin == "INN" ? (" "+d.nets[x].netname + wires[c].startComp) : (" " + wires[c].startComp + ":" + wires[c].startPin)) <<" "<< wires[c].cap << endl;
		}
		//resistance part of SPEF
		output << "*RES" << endl;
		for (int dd = 0; dd < wires.size()-1; dd++)
		{
			output << dd + 1 << (wires[dd].startPin == "INN" ? (" "+d.nets[x].netname + wires[dd].startComp) : (" " + wires[dd].startComp + ":" + wires[dd].startPin)) << " " << (wires[dd].res.endPin == "INN" ? (d.nets[x].netname + wires[dd].res.endNode) : (wires[dd].res.endNode + ":" + wires[dd].res.endPin)) << " " << wires[dd].res.res << endl;
		}
		output << "*END" << endl << endl;
		wires.clear();
		internalNodes.clear();
	}

	

	//// Testing First Net
	//bool flag = false;
	//int i;
	//cout << "CHECKK FIRST COORDINATE" << endl;
	//for ( i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(25280.0/100.0), (double)(13200.0/100.0), d.unitd);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;

	//cout << "CHECK SECOND COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(25120.0 / 100.0), (double)(13200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;


	//cout << "CHECK THIRD COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(22880.0 / 100.0), (double)(9800.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;


	//cout << "CHECK FORUTH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(25120.0 / 100.0), (double)(9800.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;


	//cout << "CHECK FIFTH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(25120.0 / 100.0), (double)(13200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;


	//cout << "CHECK SIXTH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(17440.0 / 100.0), (double)(9200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;

	//cout << "CHECK SEVENTH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(17440.0 / 100.0), (double)(11200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;

	//cout << "CHECK EIGTH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(22880.0 / 100.0), (double)(11200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;


	//cout << "CHECK NINTH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(22880.0 / 100.0), (double)(9800.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;


	//cout << "CHECK TENTH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(14560.0 / 100.0), (double)(9200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;

	//cout << "CHECK ELEVENTH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(14560.0 / 100.0), (double)(11200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;


	//cout << "CHECK 12TH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(17440.0 / 100.0), (double)(11200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;

	//cout << "CHECK 13TH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(14400.0 / 100.0), (double)(13200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;

	//cout << "CHECK 14TH COORDINATE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(14560.0 / 100.0), (double)(13200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;


	//cout << "CHECK 15TH COORDINTE" << endl;

	//for (i = 0; i < d.nets[0].ncp.size(); i++)
	//{

	//	flag = pin_intersect(d.nets[0].ncp[i], (double)(14560.0 / 100.0), (double)(11200.0 / 100.0), 100);
	//	if (flag == true) break;
	//}


	//if (flag == true)
	//{
	//	cout << "found" << endl;
	//	cout << i << endl;
	//}

	//else cout << "not found" << endl;




	system("pause");
	return 0;

}


void getOrigin( comp_def c, double& x0, double& y0, double unit)
{
	
	
	string ori = c.ori;
	if (ori == "N")
	{
		x0 = c.posx/unit;
		y0 = c.posy / unit;
		//cout << " origin is " << x0  << " " << y0 << endl;


	}

	else if (ori == "S")
	{
		x0 = c.posx / unit + c.xwidth;
		y0 = c.posy / unit + c.ylength;
		//cout << " origin is " << x0 << " " << y0 << endl;
	}

	else if (ori == "FS")
	{
		x0 = c.posx/unit;
		y0 = c.posy/unit + c.ylength;
		//cout << " origin is " << x0 << " " << y0 << endl;
	}



	else if (ori == "E")
	{
		x0 = c.posx/unit;
		y0 = c.posy/unit +  c.xwidth;
		//cout << " origin is " << x0 << " " << y0 << endl;
	}

	else if (ori == "FE")
	{
		x0 = c.posx/unit+c.ylength;
		y0 = c.posy/unit + c.xwidth;
		//cout << " origin is " << x0 << " " << y0 << endl;

	}

	else if (ori == "W")
	{
		x0 = c.posx/unit + c.ylength;
		y0 = c.posy/unit ;
		//cout << " origin is " << x0 << " " << y0 << endl;
	}


	else if (ori == "FW")
	{
		x0 = c.posx/unit;
		y0 = c.posy/unit;
		//cout << " origin is " << x0 << " " << y0 << endl;
	}


	else if (ori == "FN")
	{
		x0 = c.posx/unit+c.xwidth;
		y0 = c.posy/unit;
		//cout << " origin is " << x0 << " " << y0 << endl;
	}


}


void getCorners( comp_def c, rect r , double& c1x, double& c1y, double& c2x, double& c2y, double unit)

{
	double x0;
	double y0;
	getOrigin(c, x0, y0,unit);


	
	string ori = c.ori;

	if (ori == "N")
	{
		c1x = x0 +  r.x1;
		c1y = y0 +  r.y1;


		c2x = x0 +  r.x2;
		c2y = y0 + r.y2;

		//cout << "Corner 1 is " << c1x << " " << c1y << endl;
		//cout << "Corner 2 is " << c2x << " " << c2y << endl;


		
	}

	else if (ori == "S")
	{
		c1x = x0 -  r.x1;
		c1y = y0 - r.y1;


		c2x = x0 -  r.x2;
		c2y = y0 - r.y2;

		//cout << "Corner 1 is " << c1x << " " << c1y << endl;
		//cout << "Corner 2 is " << c2x << " " << c2y << endl;



	}

	else if (ori == "FS")
	{

		c1x = x0 + r.x1;
		c1y = y0 -  r.y1;

		c2x = x0 +  r.x2;
		c2y = y0 - r.y2;

		//cout << "Corner 1 is " << c1x << " " << c1y << endl;
		//cout << "Corner 2 is " << c2x << " " << c2y << endl;

		

		
	}



	else if (ori == "E")
	{
	
		c1x = x0 +  r.y1;
		c1y = y0 -  r.x1;

		c2x = x0 +  r.y2;
		c2y = y0 -  r.x2;

		//cout << "Corner 1 is " << c1x << " " << c1y << endl;
		//cout << "Corner 2 is " << c2x << " " << c2y << endl;




		
	}

	else if (ori == "FE")
	{

		c1x = x0 - r.y1;
		c1y = y0 -  r.x1;

		c2x = x0 -  r.y2;
		c2y = y0 -  r.x2;

		//cout << "Corner 1 is " << c1x << " " << c1y << endl;
		//cout << "Corner 2 is " << c2x << " " << c2y << endl;



	}

	else if (ori == "W")
	{
		c1x = x0 - r.y1;
		c1y = y0 +  r.x1;

		c2x = x0 -  r.y2;
		c2y = y0 +  r.x2;

		//cout << "Corner 1 is " << c1x << " " << c1y << endl;
		//cout << "Corner 2 is " << c2x << " " << c2y << endl;






	}


	else if (ori == "FW")
	{
		c1x = x0 +  r.y1;
		c1y = y0 +  r.x1;

		c2x = x0 +  r.y2;
		c2y = y0 +  r.x2;

		//cout << "Corner 1 is " << c1x << " " << c1y << endl;
		//cout << "Corner 2 is " << c2x << " " << c2y << endl;






	}


	else if (ori == "FN")
	{
		
		c1x = x0 -  r.x1;
		c1y = y0 +  r.y1;

		c2x = x0 -  r.x2;
		c2y = y0 +  r.y2;

		//cout << "Corner 1 is " << c1x << " " << c1y << endl;
		//cout << "Corner 2 is " << c2x << " " << c2y << endl;



	}



}


bool pin_intersect(netcomp nc,  double routex, double routey, double unit)
{
	
	int x = 0;
	bool flag = false;

	if (nc.nickname != "PIN")
	{
		while (nc.cm.pn[x].getpinname() != nc.pin)
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

				//cout << " Rect " << i << endl;

				getCorners(nc.cm, nc.cm.pn[x].posrects[i].rects[j], c1x, c1y, c2x, c2y, unit);

				if (c1x > c2x) s = 0;
				else s = 1;
				//cout << " s is " << s << endl;

				if (c1y > c2y) t = 0;
				else t = 1;

				if (s == 0 && t == 0)
				{
					if (routex >= c2x && routex <= c1x && routey >= c2y && routey <= c1y) flag = true;
					else flag = false;

					//cout << " Here are values used in finding flag" << c1x << " " << c1y << " " << c2x << " " <<  c2y << "against the coordiantes" << routex << " " <<routey << endl;
					//cout << "The flag is " << flag << endl;


				}

				else if (s == 0 && t == 1)
				{
					if (routex >= c2x && routex <= c1x && routey >= c1y && routey <= c2y) flag = true;
					else flag = false;
					

					//cout << " Here are values used in finding flag" << c1x << " " << c1y << " " << c2x << " " << c2y << "against the coordiantes" << routex << " " << routey << endl;
					//cout << "The flag is " << flag << endl;

				}

				else if (s == 1 && t == 0)
				{
					if (routex <= c2x && routex >= c1x && routey >= c2y && routey <= c1y) flag = true;
					else flag = false;
					//cout << " Here are values used in finding flag" << c1x << " " << c1y << " " << c2x << " " << c2y << "against the coordiantes" << routex << " " << routey << endl;
					//cout << "The flag is " << flag << endl;

				}

				else if (s == 1 && t == 1)
				{
					if (routex <= c2x && routex >= c1x && routey <= c2y && routey >= c1y) flag = true;
					else flag = false;
					//cout << " Here are values used in finding flag" << c1x << " " << c1y << " " << c2x << " " << c2y << "against the coordiantes" << routex << " " << routey << endl;
					//cout << "The flag is " << flag << endl;

				}
				if (flag == true) break;

			}
			if (flag == true) break;

		}
	}

	else {

		if (nc.cm.posx == routex && nc.cm.posy == routey) flag = true;

	}

	return flag;


}














