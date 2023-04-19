// Calendar.cpp : Defines the entry point for the console application.
//

// You should only edit this file to change the builder type if you complete that functionality,
// or the number of years
#include "stdafx.h"
#include "CalendarInterface.h"
#include<iostream>
using namespace std;

int main()
{
	CalendarInterface c("full", 1);
	c.display();
	return 0;
}

