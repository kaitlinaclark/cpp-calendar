/*
Author: Jon Shidal
Purpose: declare the user interface for our calendar
*/
#pragma once
#include "calendarbuilder.h"
#include "calendarcomponents.h"
#include<memory>




class CalendarInterface {
	std::shared_ptr<Calendar> cal; // the calendar
	std::shared_ptr<Calendar> restoreCal; // the calendar
	std::shared_ptr<CalendarBuilder> builder; // builder for managing construction of the calendar
	std::shared_ptr<CalendarBuilder> restoreBuilder; // builder for managing construction of the calendar we are loading in
	std::shared_ptr<DisplayableComponent> currentDisplay; // which component the user is currently viewing
public:
	// constructor
	// arguments: 1 - what type of builder? 2 - how many years to hold? 
	CalendarInterface(std::string builderType, size_t years);
	
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
