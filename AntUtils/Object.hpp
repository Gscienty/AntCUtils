#pragma once
#include <string>
class Object{
public:
	Object(){}
	virtual std::string ToString(){ return "Object"; }
};