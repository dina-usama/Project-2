#include "circuit.h"

circuit::circuit(string in)
{
	string temp;
	int x;
	in_filename = in;
	input.open(in_filename);

	if (input.fail())
	{
		cout << "input file error class terminated" << endl;
		return;
	}
	string comp_name, pin_name, pin_type, pin_metal;
	int count = -1;
	bool flag = false;
	pin pp("", "", "", "", "");
	posrect pr;
	rect r;
	int fl;
	while (input)
	{
		
		component cp("", "");
			pr.rects.clear();

		getline(input, temp);
		metalprop mp;

		if (temp.find("MICRONS") != -1)
		{
			string u = temp.substr(19, temp.find(" ",19)-19);
			cout << u << endl;

			unit = stoi(u);
		}

		string ls = temp.substr(0, temp.length()-1);
		if (ls == "LAYER metal")
		{
			mp.mlayer = temp.substr(5, string::npos);

			getline(input, temp);
			while (temp.find("WIDTH") == -1)
			{
				getline(input, temp);

			}

			mp.width = stod (temp.substr(9, temp.find(';') - 1 - 9) );

			getline(input,temp);
			getline(input, temp);

			mp.rpsqr = stod(temp.substr(20, temp.find(';') - 1 - 20));
			getline(input, temp);
			mp.cpsqr = stod(temp.substr(25, temp.find(';') - 1 - 25));

			mprops.push_back(mp);


		}
		
		
		
		
		
		if (x = temp.find("MACRO") != -1)
		{
			count++;
			comp.push_back(cp);
			comp_name = temp.substr(x + 5, string::npos);
			comp[count].set_name(comp_name);
			//cp.set_name(comp_name);
			flag = true;
		}
		if (flag)
		{
			
			int lrect;

			if (x = temp.find("SIZE") != -1)
			{
				string xs = temp.substr(7, temp.find("BY") - 7-1);
				string ys = temp.substr(temp.find("BY") + 3, temp.find(';') - (temp.find("BY") + 3));
				cout << xs << endl;
				cout << ys << endl;
				comp[count].xwidth = stod(xs);
				comp[count].ylength = stod(ys);

			}

			if (x = temp.find("PIN") != -1)
			{
				pin_name = temp.substr(x + 5, string::npos);
				pp.setcomp_name(comp_name);
				pp.setpinname(pin_name);
				pp.posrects.clear();
				 fl = 0;	
			}
			
			if (x = temp.find("DIRECTION") != -1)
			{
				int m = temp.find(";");
				pin_type = temp.substr(x + 13);
				pin_type = pin_type.erase(pin_type.find(" ;"),pin_type.size());
				pp.setType(pin_type);
				//comp[count].set_pin(pp);
			}
			
			if (x = temp.find("LAYER") != -1 )
			{
				pr.mlayer = temp.substr(12,temp.find(';') - 12-1);	
				pp.posrects.push_back(pr);
				fl++;
				

				//pin_metal = temp.substr(x + 11, temp.find(';') - 2);
				//pp.setmetal_layer(pin_metal);
				
			}

			if (x = temp.find("RECT ") != -1)
			{
				int s = temp.find(' ', 13);
				int q = temp.find(' ', s+1);
				int t = temp.find(' ', q+1);
				int w = temp.find(' ', t+1);

				r.x1 = stof ( temp.substr(13, s - 13 ) );
				string shjd = temp.substr(13, s - 13 );

				r.y1 = stof (temp.substr(s + 1, q - s -1) );
				string sh = temp.substr(s + 1, q - s -1);

				r.x2 = stof (temp.substr(q + 1, t - q -1) );
				string as = temp.substr(q + 1, t - q -1);

				
				r.y2 = stof (temp.substr(t + 1, w- t -1) );
				string jd = temp.substr(t + 1, w - t -1);

				pp.posrects[fl-1].rects.push_back(r);
				

			}

			if (temp == "    END")
			{

				comp[count].set_pin(pp);

			}

			if (temp == "  OBS")
			{


				getline(input, temp);
				while (temp != "  END")
				{
					getline(input, temp);
				}

			}


			
		}


	}
}
