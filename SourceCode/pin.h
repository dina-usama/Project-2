#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//enum Type { inp, out, inout };



struct rect {

	
	float x1;
	float y1;
	float x2;
	float y2;

};


struct posrect
{
	string mlayer;
	vector <rect> rects;

};


class pin
{
private:
	string pinname;
	string comp_name;
	string comp_nickname;
	//string metallayer;
	
	string t;
	



public:
	//static pin connectTo;

	pin(string pinname, string comp_name, string comp_nickname, string type, string metal_layer);
	
	vector <posrect> posrects;
	string getpinname();
	//string getmetal_layer();
	string getcomp_name();
	string getcomp_nickname();
	string getType();

	//void setmetal_layer(string metal);

	void setpinname(string pn);

	void setcomp_name(string cn);
	void setcomp_nickname(string cnn);

	//void connect_To(pin connectTo);



	void setType(string type);





	
	
};