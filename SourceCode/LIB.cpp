#include "LIB.h"

LIB::LIB(string filename)
{
	ifstream input;
	int l_num = 0;
	input.open(filename);

	if (input.fail())
	{
		cout << "input file error class terminated" << endl;
		return;
	}
	string temp;
	bool fff = true;
	while (input)
	{
	//new_Cell:

		cell cellx;

		getline(input, temp);
		l_num++;;
		if (temp.find("psst") != -1)
			cout << "there is a problem here!!" << endl;
		if (temp.find("cell (") != -1 && temp.find("osu035_stdcells") == -1)
		{

			int x = temp.find(')');
			cellx.macro = temp.substr(6, x - 6);
			
			getline(input, temp);
			l_num++;;
			// != -1 is the same as aviable 
			while (temp != " * --------------- */" && input && ( (temp.find("-") != -1 )? ( ( temp.find("-0") != -1 )? true:false ) : true ) )
			{
				fff = true;
			
				if (temp.find("pin(") != -1)
				{
					pin y;
					int x = temp.find(')');
					y.pinname = temp.substr(6, x - 6);
					getline(input, temp);
					l_num++;
	
					if (temp.find("direction") != -1)
					{
						int x = temp.find(':');
						int r = temp.find(';');
						y.direction = temp.substr(x + 2, r - x - 2);
						getline(input, temp);
						l_num++;
						int x2 = temp.find(':');
						int r2 = temp.find(';');
						y.capacitance = temp.substr(x2 + 2, r2 - x2 - 2);
						cellx.pins.push_back(y);
					}
					if (y.direction == "output")
					{

						
						getline(input, temp);
						l_num++;
						while (temp != "/* --------------- *" && temp.find("Design") == -1 && input && fff)
						{
						
							timing tx;
							if (temp.find("timing()") != -1)
							{
								getline(input, temp);
								l_num++;
								int a = temp.find('"');
								int b = temp.find(';');
								tx.related_pin = (temp.substr(a + 1, b - a - 2));
								getline(input, temp);
								l_num++;
								int u = temp.find(':');
								int w = temp.find(';');
								tx.timing_sense = temp.substr(u + 2, w - u - 2);
								getline(input, temp);
								l_num++;
								getline(input, temp);
								l_num++;
								string tex = temp, tm;
								int xx = temp.find("\"");
								tex = tex.erase(0, xx + 1);
								while (tex != "")
								{
									int yy = tex.find(",");
									if (yy != -1)
									{
										tm = tex.substr(0, yy);
										tx.output_cap.push_back(tm);
										tex = tex.erase(0, yy + 2);
									}
									else
									{
										yy = tex.find("\"");
										tm = tex.substr(0, yy);
										tx.output_cap.push_back(tm);
										tex = tex.erase(0, yy + 2);
										tex = "";
									}
								}
								getline(input, temp);
								l_num++;
								tex = temp;
								xx = temp.find("\"");
								tex = tex.erase(0, xx + 1);
								while (tex != "")
								{
									int yy = tex.find(",");
									if (yy != -1)
									{
										tm = tex.substr(0, yy);
										tx.input_Trans.push_back(tm);
										tex = tex.erase(0, yy + 2);
									}
									else
									{
										yy = tex.find("\"");
										tm = tex.substr(0, yy);
										tx.input_Trans.push_back(tm);
										tex = tex.erase(0, yy + 2);
										tex = "";
									}
								}
								getline(input, temp);
								l_num++;
								getline(input, temp);
								l_num++;

								while (temp.find("}") == -1)
								{
									tex = temp;
									xx = temp.find("\"");
									tex = tex.erase(0, xx + 1);
									vector<string> ss;
									while (tex != "")
									{

										int yy = tex.find(",");
										if (count(tex.begin(), tex.end(), ',') != 0)
										{
											if (count(tex.begin(), tex.end(), ',') == 1)
											{
												tm = tex.substr(0, yy - 1);

											}
											else
											{
												tm = tex.substr(0, yy);

											}
											ss.push_back(tm);
											tex = tex.erase(0, yy + 2);
										}
										else if (tex.find(";") != -1)
										{
											int mm = tex.find("\"");
											tm = tex.substr(0, mm);
											ss.push_back(tm);
											tex = "";
										}
										else
											tex = "";
									}
									tx.delaysrise.push_back(ss);
									ss.clear();
									getline(input, temp);
									l_num++;
								}
								getline(input, temp);
								l_num++;
								
								getline(input, temp);
								l_num++;
								getline(input, temp);
								l_num++;
								getline(input, temp);
								l_num++;
								while (temp.find("}") == -1)
								{
									tex = temp;
									xx = temp.find("\"");
									tex = tex.erase(0, xx + 1);
									vector<string> ss;
									while (tex != "")
									{

										int yy = tex.find(",");
										if (count(tex.begin(), tex.end(), ',') != 0)
										{
											if (count(tex.begin(), tex.end(), ',') == 1)
											{
												tm = tex.substr(0, yy - 1);

											}
											else
											{
												tm = tex.substr(0, yy);

											}
											ss.push_back(tm);
											tex = tex.erase(0, yy + 2);
										}
										else if (tex.find(";") != -1)
										{
											int mm = tex.find("\"");
											tm = tex.substr(0, mm);
											ss.push_back(tm);
											tex = "";
										}
										else
											tex = "";
									}
									tx.OutTransrise.push_back(ss);
									ss.clear();
									getline(input, temp);
									l_num++;
								}

								getline(input, temp);
								l_num++;

								getline(input, temp);
								l_num++;
								getline(input, temp);
								l_num++;
								getline(input, temp);
								l_num++;
								while (temp.find("}") == -1)
								{
									tex = temp;
									xx = temp.find("\"");
									tex = tex.erase(0, xx + 1);
									vector<string> ss;
									while (tex != "")
									{

										int yy = tex.find(",");
										if (count(tex.begin(), tex.end(), ',') != 0)
										{
											if (count(tex.begin(), tex.end(), ',') == 1)
											{
												tm = tex.substr(0, yy - 1);

											}
											else
											{
												tm = tex.substr(0, yy);

											}
											ss.push_back(tm);
											tex = tex.erase(0, yy + 2);
										}
										else if (tex.find(";") != -1)
										{
											int mm = tex.find("\"");
											tm = tex.substr(0, mm);
											ss.push_back(tm);
											tex = "";
										}
										else
											tex = "";
									}
									tx.delaysfall.push_back(ss);
									ss.clear();
									getline(input, temp);
									l_num++;
								}

							
								getline(input, temp);
								l_num++;;
								getline(input, temp);
								l_num++;;
								getline(input, temp);
								l_num++;;
								getline(input, temp);
								l_num++;;
								while (temp.find("}") == -1)
								{
									tex = temp;
									xx = temp.find("\"");
									tex = tex.erase(0, xx + 1);
									vector<string> ss;
									while (tex != "")
									{

										int yy = tex.find(",");
										if (count(tex.begin(), tex.end(), ',') != 0)
										{
											if (count(tex.begin(), tex.end(), ',') == 1)
											{
												tm = tex.substr(0, yy - 1);

											}
											else
											{
												tm = tex.substr(0, yy);

											}
											ss.push_back(tm);
											tex = tex.erase(0, yy + 2);
										}
										else if (tex.find(";") != -1)
										{
											int mm = tex.find("\"");
											tm = tex.substr(0, mm);
											ss.push_back(tm);
											tex = "";
										}
										else
											tex = "";
									}
									tx.OutTransfall.push_back(ss);
									ss.clear();

									getline(input, temp);
									l_num++;;
								
								}
								
								cellx.lut.push_back(tx);
								fff = false;
								//goto new_Cell;
							}
							
							getline(input, temp);
							l_num++;;
						}


					}

				}
				getline(input, temp);
				l_num++;;

			}
			//getline(input, temp);
		
			cells.push_back(cellx);

		}
	


	}

}
