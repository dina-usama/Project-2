#include <iostream>
#include <fstream>
#include <string>
#include "circuit.h"
#include "DEF.h"
#include "LIB.h"
#include <vector>
#include <algorithm>
using namespace std;



struct pathcomp
{
	string name;
	string lef;
	string pin;
	string input_trans;
	string output_trans;
	string load_cap;
	string outpin;
	double delayfall = 0;
	double delayrise = 0;

};

vector<pathcomp>path;
void readinput(circuit& c, DEF& d, LIB& b, string filename);
void delaycalc(circuit& c, DEF& d, LIB& b, vector <pathcomp> path, string rise_fall, double& pathdelay_rise, double& pathdelay_fall);
int min_index(vector<string>v, string x);

//validating if the path exists 
bool valide(circuit& c, DEF& d, LIB& b);




int main()
{
	string s1, s2, s3;

	cout << "please enter name of LEF file " << endl;
	cin >> s1;

	cout << "please enter name of DEF file " << endl;
	cin >> s2;

	cout << "please enter name of library file " << endl;
	cin >> s3;

	circuit c(s1);
	DEF d(s2, c.comp);
	LIB b(s3);
	
	string in_name;
	cout << "please enter the input file name: " << endl;
	cin >> in_name;
	



	readinput(c,d,b,in_name);
	double d_rise = 0, d_fall = 0;
	if (valide(c,d,b))
	{
		delaycalc(c,d,b,path, "rise", d_rise, d_fall);
		cout << "The delay of the circuit: " << max(d_rise, d_fall) << endl;
	}
	else
		cout << "Path Not Valide!!!" << endl;
		
	system("pause");
}


//validating if the path exists 
bool valide(circuit &c, DEF& d, LIB& b)
{

	for (int i = 0; i < path.size(); i++)
	{
		
		//getting output pin
		int j = 0;
		bool flag = false;
		while (j < d.cd.size() && flag == false)
		{
			if (d.cd[j].nikename == path[i].name)
			{


				path[i].lef = d.cd[j].lef;
				bool f4 = false;
				int u = 0;


				while (u < b.cells.size() && f4 == false)
				{
					if (path[i].lef == b.cells[u].macro)
					{
						for (int w = 0; w < b.cells[u].pins.size(); w++)
						{
							if (b.cells[u].pins[w].direction == "output")
							{
								path[i].outpin = b.cells[u].pins[w].pinname;
								break;
							}
						}
						f4 = true;
					}
					else u++;
				}


				flag = true;
			}
			else j++;

		}

		// forming vector of components in same net ( getting input of connected gates)
		vector<netcomp> nf;

		bool f2 = false;
		int y = 0;
		int v = 0;
		while (y < d.nets.size() && f2 == false)
		{

			// v will show th eindex of the outpin pin
			for (int t = 0; t < d.nets[y].ncp.size(); t++)
			{
				if (d.nets[y].ncp[t].nickname == path[i].name && d.nets[y].ncp[t].pin == path[i].outpin)
				{
					nf = d.nets[y].ncp;
					f2 = true;
					v = t;
					break;
				}

			}
			y++;
		}

		//removing the item from the same net
		nf.erase(nf.begin() + v);

		//checking if the following gate is connected
		if (i != path.size() - 1)
		{
			bool f1 = true;
			for (int m = 0; f1 & m < nf.size(); m++)
			{
				if (nf[m].nickname == path[i + 1].name)
					f1 = false;
			}
			if (f1 == true)
				return false;
		}


	}
	return true;
}

void readinput(circuit& c, DEF& d, LIB& b,string filename)
{

	ifstream input;

	input.open(filename);

	if (input.fail())
	{
		cout << "input file error class terminated" << endl;
		return;
	}
	string temp;

	pathcomp x;
	getline(input, temp);
	int y = temp.find('/');
	x.name = temp.substr(0, y - 0);
	int a = temp.find('-');
	x.pin = temp.substr(y + 1, a - y - 1);
	x.input_trans = temp.substr(a + 1, string::npos);
	path.push_back(x);
	while (input)
	{
		pathcomp x;

		getline(input, temp);
		int y = temp.find('/');
		x.name = temp.substr(0, y - 0);
		x.pin = temp.substr(y + 1, string::npos);
		if(input)
		path.push_back(x);

	}
}

int min_index(vector<string>v, string x)
{

	double mindiff = 100;
	int minindex = 0; 

	for (int i = 0; i < v.size(); i++)
	{
		double diff = abs (stold(v[i]) - stold(x));

		if (diff < mindiff)
		{
			mindiff = diff;
			minindex = i;
		}

	}

	return minindex;
}

void delaycalc(circuit& c, DEF& d, LIB& b,vector <pathcomp> path, string rise_fall, double & pathdelay_rise, double & pathdelay_fall)
{
	
	

	for (int i = 0; i < path.size(); i++)
	{
		int indexcell = 0; // index of cell at which coomponen tis found
		int indexlut = 0; // index of lut related to pin we are interested in

		int inpco = 0; // index of input trnasition
		int outco = 0; // index of output capaictance

		// getting output pin

		int j = 0;
		bool flag = false;
		while (j < d.cd.size() && flag == false)
		{
			if (d.cd[j].nikename == path[i].name)
			{
				
				
				path[i].lef = d.cd[j].lef;
				bool f4 = false;
				int u = 0;


				while (u < b.cells.size() && f4 == false)
				{
					if (path[i].lef == b.cells[u].macro)
					{
						for (int w = 0; w < b.cells[u].pins.size(); w++)
						{
							if (b.cells[u].pins[w].direction == "output")
							{
								path[i].outpin = b.cells[u].pins[w].pinname;
								break;
							}
						}
						f4 = true;
					}
					else u++;
				}


				flag = true;
			}
			else j++;

		}



		// forming vector of components in same net ( getting input of connected gates)
		vector<netcomp> nf;

		bool f2 = false;
		int y = 0;
		int v = 0;
		while (y < d.nets.size() && f2 == false)
		{

			// v will show th eindex of the outpin pin
			for (int t = 0; t < d.nets[y].ncp.size(); t++)
			{
				if (d.nets[y].ncp[t].nickname == path[i].name && d.nets[y].ncp[t].pin == path[i].outpin)
				{
					nf = d.nets[y].ncp;
					f2 = true;
					v = t;
					break;
				}

			}
			y++;
		}

		//correct 

		nf.erase(nf.begin() + v);

		// caluclating  load capacitance of outpin pin of current component

		double loadcap = 0;

		for (int i = 0; i < nf.size(); i++)
		{
			bool f3 = false;
			int p = 0;
			while (p < d.cd.size() && f3 == false)
			{
				if (nf[i].nickname == d.cd[p].nikename)
				{
					nf[i].lef = d.cd[p].lef;
					f3 = true;

				}
				p++;

			}
			//correct 
			// access lib to get input pin capacitance

			bool f4 = false;
			int u = 0;


			while (u < b.cells.size() && f4 == false)
			{
				if (nf[i].lef == b.cells[u].macro)
				{
					int x=0;
					bool pinf = false;
					while (pinf == false && x < b.cells[u].pins.size())
					{
						if (nf[i].pin == b.cells[u].pins[x].pinname)
						{
							if (b.cells[u].pins[x].direction == "input")
							{
								string h = b.cells[u].pins[x].capacitance;
								loadcap += stold(h);
								pinf = true;
							}
							else
							pinf = true;

						}
						else x++;
					}
					f4 = true;
				}
				else u++;

			}

		}

		// assign load capacitance to current component in path
		path[i].load_cap = to_string(loadcap);



		bool f6 = false;
		int o = 0;

		// assigning input transition in case not first component

		
		if (i != 0)
		{
			path[i].input_trans = path[i - 1].output_trans;
		}

		// get the output transition

		// get indices of closest input transition and output capacitance and find output transition

		while (f6 == false && o < b.cells.size())
		{
			if (path[i].lef == b.cells[o].macro)
			{
					
				for (int i = 0; i < b.cells[o].lut.size(); i++)

				{
					if (b.cells[o].lut[i].related_pin == path[i].pin)
					{

						inpco = min_index(b.cells[o].lut[i].input_Trans, path[i].input_trans);
						outco = min_index(b.cells[o].lut[i].output_cap, path[i].load_cap);
						if (b.cells[o].lut[i].timing_sense == "positive_unate")
						{
							path[i].output_trans = b.cells[o].lut[i].OutTransrise[outco+1][inpco];

						}
						else
						{
							path[i].output_trans = b.cells[o].lut[i].OutTransfall[outco+1][inpco];
						}
						indexlut = i;
						break;
					}

				}
				indexcell = o;
				f6 = true;
			}
			else o++;


		}

		
		




		// calculate the delay based on input transition and load capacitance

		
		double delayfall = stold(b.cells[indexcell].lut[indexlut].delaysfall[outco+1][inpco]);
		//if(b.cells[indexcell].macro == "DFFNEGX1"
		double delayrise;
		if(b.cells[indexcell].macro == "DFFNEGX1" || b.cells[indexcell].macro == "DFFPOSX1" || b.cells[indexcell].macro == "DFFSR" || b.cells[indexcell].macro == "LATCH")
			delayrise =stold(b.cells[indexcell].lut[indexlut].delaysrise[outco+1][inpco]);
		else 
			delayrise = stold(b.cells[indexcell].lut[indexlut].delaysrise[outco][inpco]);
		

		path[i].delayrise = delayrise;
		path[i].delayfall = delayfall;

		pathdelay_rise += path[i].delayrise;
		pathdelay_fall += path[i].delayfall;
	}
	return;

}

// the delay of the path will be caluclated twice oncefor rise and the other for fall and the path of the delay will be the longest


	