#include "component.h"

using namespace std;
component::component(string name, string nickname)
{
	this->name = name;
	this->nickname = nickname;
}

component::~component()
{
	name = nickname = "";
}

void component::set_name(string name)
{
	this->name = name;
}

void component::set_nickname(string nickname)
{
	this->nickname = nickname;
}

void component::set_pin(pin p)
{
	pn.push_back(p);
}

string component::get_name()
{
	return name;
}

string component::get_nickname()
{
	return nickname;
}

