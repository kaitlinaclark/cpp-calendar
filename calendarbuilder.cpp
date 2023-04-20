/*
Author: Jon Shidal
Purpose: Define builder classes, responsible for building and managing the structure of the calendar
*/
#include <iostream>
#include <sstream>
#include "calendarbuilder.h"
#include "calendarcomponents.h"

using namespace std;

shared_ptr<Calendar> FullCalendarBuilder::buildCalendar(string name, size_t years) {
	currentCalendar = make_shared<Calendar>(name, years);

	// construct each year in a recursive way, add each year as a child of the calendar
	for (size_t i = 0; i < years; ++i) {
		tm y = currentCalendar->dateInfo;
		y.tm_year += i;
		y.tm_wday = (y.tm_wday + CalendarComponent::DAYSINAYEAR * i) % CalendarComponent::DAYSINAWEEK; // calculate day of the week for first day of the year
		currentCalendar->addComponent(buildYear(y, currentCalendar));
	}
	return currentCalendar;
}
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// you may decide to define this.
shared_ptr<DisplayableEvent> FullCalendarBuilder::buildEvent(std::tm when, shared_ptr<DisplayableComponent> parent, string name, int recurrEvery, int recurrFor, std::string calname) {
	shared_ptr<DisplayableEvent> event = make_shared<DisplayableEvent>(when, parent, name, recurrEvery, recurrFor, calname);

	parent->addComponent(event);
	return event;
}

/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// you may decide to define this.
/*
1st arg = parent of specified granularity
2nd arg = date/time info
3rd arg = type of component you want returned*/
//no option for event dislay
shared_ptr<DisplayableComponent> FullCalendarBuilder::getComponentByDate(tm d, string granularity) {
	//string granularity = type of component you want to return (e.g. Year, Month, Day, Event)
	int calendarYear = CalendarComponent::BASEYEAR + currentCalendar->dateInfo.tm_year;
	int yearAdding = CalendarComponent::BASEYEAR + d.tm_year;
	int index = calendarYear - yearAdding; // which child?
	shared_ptr<DisplayableComponent> year = currentCalendar->getChild(index);
	shared_ptr<DisplayableComponent> month = year->getChild(d.tm_mon);
	shared_ptr<DisplayableComponent> day = month->getChild(d.tm_mday - 1);


	if (granularity == "day") {
		return day;
	}
	else if (granularity == "month") {
		return month;
	}
	else if (granularity == "year") {
		return year;
	}
	return currentCalendar;


}
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

shared_ptr<DisplayableComponent> FullCalendarBuilder::buildDay(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	shared_ptr<DisplayableComponent> day = make_shared<DisplayableDay>(d, p);

	return day;
}

shared_ptr<DisplayableComponent> FullCalendarBuilder::buildMonth(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	int index = d.tm_mon;
	shared_ptr<DisplayableComponent> m = make_shared<DisplayableMonth>(d, p, CalendarComponent::months[index], CalendarComponent::days[index]);

	for (int i = 0; i < CalendarComponent::days[index]; ++i) { // for each day in the month
		m->addComponent(buildDay(d, m)); // construct day and add as a child of the month
		++(d.tm_mday); // increment day of the month
		d.tm_wday = (d.tm_wday + 1) % CalendarComponent::DAYSINAWEEK; // increment weekday, reset to 0 if needed
	}
	return m;
}
shared_ptr<DisplayableComponent> FullCalendarBuilder::buildYear(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	shared_ptr<DisplayableComponent> y = make_shared<DisplayableYear>(d, p, false);


	// construct each month and add it as a child of the year
	for (int i = 0; i < CalendarComponent::MONTHS; ++i) {
		d.tm_mon = i;
		y->addComponent(buildMonth(d, y));
		// set week day of first day of the next month
		d.tm_wday = (d.tm_wday + CalendarComponent::days[i]) % CalendarComponent::DAYSINAWEEK;
	}
	return y;
}






