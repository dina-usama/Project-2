
#include "DEF.h"
#include <algorithm>

DEF::DEF(string in_filename, vector<component> ct)
{
	int coo = 0;
	string temp;
	int x;
	in_file = in_filename;
	input.open(in_file);

	if (input.fail())
	{
		cout << "input file error class terminated" << endl;
		return;
	}
	int count = 0;
	while (input)
	{
		getline(input, temp);
		//cout << temp << endl;
		//parsing the components
		/*
		if (x = temp.find("DISTANCE MICRONS") != -1)
		{
			DEF_Units = stoi()
		}

		*/

		if (temp.find("MICRONS") != -1)
		{
			string u = temp.substr(23, temp.find(" ", 23) - 23);
			cout << u << endl;

			unitd = stoi(u);
		}
		if (x = temp.find("COMPONENTS") != -1)
		{
			string st;
			do {
				if (temp.find("END COMPONENTS") == -1)
				{
					comp_def s;
					getline(input, temp);
					//cout << temp << endl;
					if (temp == "")
					{

					}
					else {
						int xx = temp.find(" ", 3);
						st = temp.substr(2, xx - 2);
						s.nikename = st;
						int yy = temp.find(" ", xx + 1);
						st = temp.substr(xx + 1, yy - xx - 1);
						vector<pin> ppp = s.fid(st, ct);
						s.pn = ppp;
						s.lef = st;
						s.xy(s.lef, ct);
						xx = temp.find("(");
						yy = temp.find(")");
						st = temp.substr(xx + 2, yy - xx - 3);
						string posx;
						string posy;
						posx = st.substr(0, st.find(" "));
						posy = st.substr(st.find(" ") + 1, string::npos);
						if (posx.find("ND") == -1) {
						s.posx = stod(posx);
						s.posy = stod(posy);
					}
						xx = temp.find(";");
						st = temp.substr(yy + 2, xx- (yy+2+1));
						s.ori = st;
						cd.push_back(s);
					}
				}
			} while (temp.find("END COMPONENTS") == -1);
			cd.erase(cd.end() - 1);
		} //end of componentes parsing to get the nickname, real name, pins, position, and orientation

		//parsing pins 

		if (x = temp.find("PINS") != -1)
		{
			pins_def p;
			getline(input, temp);
			//cout << temp << endl;
			while (temp.find("END PINS") == -1)
			{
				int m = temp.find(" ", 2);
				p.name = temp.substr(2, m - 2);
				getline(input, temp);
				//	cout << temp << endl;
				p.layer = temp.substr(10, 6);
				getline(input, temp);
				//	cout << temp << endl;
				string nn = temp.substr(temp.find('('), temp.find(')') - 10);
				p.pos_x = stod (nn.substr(2, nn.find(" ", 2) - 2) );
				p.pos_y =stod ( nn.substr(nn.find(" ", 2) + 1, nn.find(')') - nn.find(" ", 2) - 2) );
				p.ori = temp.substr(temp.find(')') + 2, temp.find(';') - temp.find(')') - 3);
				pns.push_back(p);
				getline(input, temp);
				//	cout << temp << endl;

			}

		} //pins parsed including names, layer, position, and orientation 


		//parsing nets
		if (x = temp.find("NETS") != -1 && temp.find("SPECIALNETS") == -1)
		{
			netcomp nn;
			net n;
			bool fls = true;
			while (temp.find("END NETS") == -1 && (temp.size() > 2 || temp == ";"))
			{
				if (temp == ";")
					getline(input, temp);
				if (temp == "END NETS")
					cout << endl;
				if (temp.find("ROUTED ") == -1 && temp[0] != '-' && temp != "END NETS")
				{
					getline(input, temp);
					//	cout << temp << endl;
				}
				string st;
				if (temp[0] == '-')
				{
					st = temp.substr(2, string::npos);
					n.netname = st;
					n.ncp.clear();
					getline(input, temp);
				}
				if (temp[2] == '(' && temp.size() >= 3)
				{
					while (temp[2] == '(')
					{
						int xx = temp.find("(");
						int yy = temp.find(" ", xx + 2);
						st = temp.substr(xx + 2, yy - x - 3);
						nn.nickname = st;
						xx = temp.find(" ", yy + 1);
						st = temp.substr(yy + 1, xx - yy - 1);
						nn.pin = st;
						if(nn.nickname != "PIN")
							for (int i = 0; i < cd.size(); i++)
							{
								if (cd[i].nikename == nn.nickname)
								{
									nn.cm = cd[i];
									break;
								}
							}
							
						else
						{
							for (int i = 0; i < this->pns.size(); i++)
							{
								if (nn.pin == pns[i].name)
								{
									nn.cm.posx = pns[i].pos_x;
									nn.cm.posy = pns[i].pos_y;
								}

							}

						}
						
						n.ncp.push_back(nn);
						getline(input, temp);
						//	cout << temp << endl;
						if (temp.size() < 3)
							break;
					}

				}
				else if (temp.find("ROUTED ") != -1)
				{
					while (temp[0] != '-')
					{
						size_t nnn = std::count(temp.begin(), temp.end(), '(');
						routing rr;
						string tec;
						int a;
						if (int(nnn) > 1)
						{
							rr.layer = temp.substr(temp.find("metal"), 6);
							tec = temp;
							tec.erase(0, temp.find('('));
							string pr;
							while (tec.length() != 0 && tec != " " && tec.find('(') != -1)
							{
								string pre = tec.substr(2, tec.find(')') - 3);
								a = pre.find('*');
								if (a == -1) {
									a--;

									rr.pos.push_back(pre);
								}
								else {
									a--;
									pre = pre.erase(0, 1);
									if (a == 0)
										pre = pre.replace(a, 1, pr.substr(0, pr.find(" ")) + " ");
									else
										pre = pre.replace(a, 1, pr.substr(pr.find(" ") + 1, string::npos));
									rr.pos.push_back(pre);
								}
								tec.erase(tec.find('('), tec.find(')') + 1);
								pr = pre;
							}
							route.push_back(rr);

							getline(input, temp);

						}
						else
							getline(input, temp);
					}
					n.r = route;
					nets.push_back(n);
					route.clear();
					n.r.clear();
				}
			} // ending of parsing the nets I guess


		}




	}
}


