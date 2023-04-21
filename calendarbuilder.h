#pragma once
/*
Purpose: declare abstract base class to represent a builder for the calendar. Declare FullCalendarBuilder
which builds all components of the calendar at once.
*/
#include "displayablecomponent.h"
#include "calendarcomponents.h"
#include <ctime>
#include <memory>

using namespace std;

class Calendar;


// Concrete builder class
// builds a full calendar, all at once
class FullCalendarBuilder {
public:
	shared_ptr<DisplayableEvent> buildEvent(tm when, shared_ptr<DisplayableComponent>,string name,  int recurrEvery, int recurrFor, string calname);
	shared_ptr<DisplayableComponent> getComponentByDate(tm d, string granularity);
	shared_ptr<Calendar> buildCalendar(string name, size_t years);
protected:
	// the Calendar object the builder is currently constructing and managing. Each of the above functions 
	// takes a shared_ptr to the calendar as an argument, but let's go ahead and store it anyway
	shared_ptr<Calendar> currentCalendar;

	// functions to build components of the calendar
	// these will be used internally only
	shared_ptr<DisplayableComponent> buildYear(tm d, shared_ptr<DisplayableComponent> p);
	shared_ptr<DisplayableComponent> buildDay(tm d, shared_ptr<DisplayableComponent> p);
	shared_ptr<DisplayableComponent> buildMonth(tm d, shared_ptr<DisplayableComponent> p);
};
