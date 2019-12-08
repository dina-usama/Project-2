#include "pin.h"

pin::pin(string pinname, string comp_name, string comp_nickname, string type, string metal_layer)
{

	this->pinname = pinname;
	this->comp_name = comp_name;
	this->comp_nickname = comp_nickname;
	if (type == "INPUT") this->t = "I";
	else if (type == "OUTPUT")this->t = "O";
	else if (type == "INOUT") this->t = "B";
	//this->metallayer = metal_layer;

}

string pin::getpinname()
{
	return pinname;

}

string pin::getcomp_name()
{
	return comp_name;
}

void pin::setpinname(string pn)
{
	this->pinname = pn;
}

void pin::setcomp_name(string cn)
{

	this->comp_name = cn;
}

void pin::setcomp_nickname(string cnn)
{
	this->comp_nickname = cnn;
}

string pin::getcomp_nickname()
{
	return comp_nickname;
}

string pin::getType()
{
	return t;
}



void pin::setType( string type)
{
	if (type == "INPUT") 
		this->t = "I";
	else if (type == "OUTPUT")
		this->t = "O";
	else if (type == "INOUT") 
		this->t = "B";
}


