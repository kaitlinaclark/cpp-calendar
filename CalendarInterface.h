#pragma once
/*
	Declare the user interface for our calendar
*/

#include "calendarbuilder.h"
#include "calendarcomponents.h"
#include <memory>

using namespace std;




class CalendarInterface {
	shared_ptr<Calendar> cal; // the calendar
	shared_ptr<Calendar> restoreCal; // the calendar
	shared_ptr<CalendarBuilder> builder; // builder for managing construction of the calendar
	shared_ptr<CalendarBuilder> restoreBuilder; // builder for managing construction of the calendar we are loading in
	shared_ptr<DisplayableComponent> currentDisplay; // which component the user is currently viewing
public:
	// constructor
	// arguments: 1 - what type of builder? 2 - how many years to hold? 
	CalendarInterface(string builderType, size_t years);
	
	// calendar traversal functions
	void zoomIn(unsigned int index); // zoom in on a child of the current_display
	void zoomOut(); // zoom out to the parent of the current_display
	void display(); // display the current view to the user
	void addEvent(); /// add an event to the calendar
	void jump(); ///jump to a specified date
	void search(); ///search for event by name
	void save(); ///save calendar into outfile
	void restore(); ///overwrite calendar with input file
	void merge();/// merge current calculator with saved calendar
	void edit(); // edit event
};
