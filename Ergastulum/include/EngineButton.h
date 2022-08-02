#ifndef ENGINEBUTTON_H
#define ENGINEBUTTON_H

#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "Button.h"

class EngineButton : public Button
{
public:
	EngineButton();
	~EngineButton();
	boost::function<void()> onClick;
private:
};




#endif