// calendar.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "CalendarInterface.h"
#include <iostream>
using namespace std;

int main()
{
	CalendarInterface c("full", 1);
	c.display();
	return 0;
}

