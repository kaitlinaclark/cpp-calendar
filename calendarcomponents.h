#pragma once
/*
This file contains declarations for various components used in a Calendar, as well as the Calendar itself. 
All components inherit from DisplayableComponent and have a member variable that is a std::tm object, representing
its date and time. 
*/
#include "displayablecomponent.h"
#include <ctime>
#include <string>
#include <vector>
using namespace std;

// here is the layout of the tm struct, it is declared in <ctime>
//struct tm {
//	int tm_sec;   // seconds of minutes from 0 to 61
//	int tm_min;   // minutes of hour from 0 to 59
//	int tm_hour;  // hours of day from 0 to 24
//	int tm_mday;  // day of month from 1 to 31
//	int tm_mon;   // month of year from 0 to 11
//	int tm_year;  // year since 1900
//	int tm_wday;  // days since sunday
//	int tm_yday;  // days since January 1st
//	int tm_isdst; // hours of daylight savings time
//}






// forward declarations needed to avoid circular includes, used to declare friends only
class FullCalendarBuilder;
class CalendarInterface;

/*
-An Event has a 
	name, and a 
	date/time(of type struct tm), as well as 
	anything else it may inherit from base classes.
-Event objects can only be 
	added to Days (DisplayableDay objects). 
-Events should only be added to the day that the event occurs in.
-Recurring and multi day events should be supported(recur every __ days, event lasts for __ days). 
-Each day an event falls on should contain a shared_ptr to a unique Event object. 
-You should not share the same event object between days.
-Events may be created in future years as well (up to the number of years stored in the calendar). 
-Events may not be created in years prior to the current year.
-A day may contain multiple events. Those events should be sorted by their start time.
*/

/// A Task Strut to use for the TODO List
struct task {
	tm date;
	string name;
	bool done;
};

class CalendarComponent : public DisplayableComponent {
	friend FullCalendarBuilder;
	friend CalendarInterface;
public:
	// inherited
	//1 = display w/o events (Calendar, DisplayableYear)
	//2 = display w/ event names (DisplayableMonth)
	//3 = display w/ event names and times (DisplayableDay)
	//4 = display events with edit/delete option (DisplayableEvent)
	virtual void display(unsigned int) = 0;

protected:
	// stores date/time associated with this component
	// see struct tm details above
	tm dateInfo;

	// constructor, protected is ok. Builder class is a friend
	CalendarComponent(std::tm, std::shared_ptr<DisplayableComponent>);

	// some static constants to use in place of hardcoded calendar values
	static const vector<string> daysoftheweek_abbreviated;
	static const vector<string> daysoftheweek_full;
	static const vector<string> months;
	static const vector<int> days;
	static const int HOURSINADAY;
	static const int DAYSINAWEEK;
	static const int MONTHS;
	static const int BASEYEAR;
	static const int DAYSINAYEAR;

};
///Display Event Stuff
class DisplayableEvent : public CalendarComponent {
	//inherits from DisplayableDay
	friend class DisplayableDay;
	friend FullCalendarBuilder;
	friend CalendarInterface;
public:
	std::string name;
	std::string calname;
	unsigned int recurrEvery;
	unsigned int recurrFor;
	
	//constructor is similar to DisplayableDay
		// 1st argument = date/time info 
		// 2nd arg = its parent (DisplayableDay) 
		// 3rd arg = name of event
		// 4th arg = recurrence frequency (e.g. every 7 days, every 2 days, etc)
		// 5th arg = length of event 
	DisplayableEvent(tm d, shared_ptr<DisplayableComponent> p, string name, unsigned int recurrEvery, unsigned int recurrFor, string calname);
	
	virtual void display(unsigned int) override;
	// currently a leaf class, so no need to override addComponent()
};

class DisplayableDay : public CalendarComponent {
	friend class DisplayableMonth;
	friend FullCalendarBuilder;
	friend CalendarInterface;
public:
	// 1st argument = start date/timeof the day, 2nd argument = its parent (month)
	DisplayableDay(tm d, shared_ptr<DisplayableComponent> p);
	virtual void display(unsigned int) override;
	vector<task> TODO;
	void printTODO();
	void CreateTODOList();
	void addToTODOList(string name);
	void markDone(string name);
	///
		/// We need to make changes here to override addComponent() so we can add events
	///
	/// Day contains events, so it is a composite object. override addComponent accordingly
	virtual std::shared_ptr<DisplayableComponent> addComponent(std::shared_ptr<DisplayableComponent>) override;
	};


class DisplayableMonth : public CalendarComponent {
	friend class DisplayableYear;
	friend FullCalendarBuilder;
	friend CalendarInterface;
public: 
	// arguments = date/time info, its parent, name of the month, days in the month
	DisplayableMonth(tm d, shared_ptr<DisplayableComponent> p, string monthname, unsigned int numdays);
	virtual void display(unsigned int) override;
protected:
	//2d array 
	static vector<vector<string>> mon;

	string name;
	unsigned int numberOfDays;

	//print week of tm_mday's
	void printWeek();

	//print a week of event info given an hour
	void printWeekOfEvents(int hr);
	// Month contains days, so it is a composite object. override addComponent accordingly
	virtual shared_ptr<DisplayableComponent> addComponent(shared_ptr<DisplayableComponent>) override;
};

class Calendar;

class DisplayableYear : public CalendarComponent {
	friend Calendar;
	friend FullCalendarBuilder;
	friend CalendarInterface;
public:
	// arguments: date/time info, parent, leap year or no?
	DisplayableYear(tm, shared_ptr<DisplayableComponent>, bool);
	virtual void display(unsigned int) override;
protected:
	bool leap;
	// year contains months - override accordingly
	virtual shared_ptr<DisplayableComponent> addComponent(shared_ptr<DisplayableComponent>) override;
};

class Calendar : public CalendarComponent {
	// friends
	//friend DisplayableEvent;
	friend FullCalendarBuilder;
	friend CalendarInterface;
protected:
	//list of events for each calendar
	vector<shared_ptr<DisplayableEvent> > events;
	
	string name;
	size_t yearsToHold;
	tm currentDate; // current date and time
	// dateInfo is the start date and time


	// Calendar contains years, so override accordingly
	virtual shared_ptr<DisplayableComponent> addComponent(shared_ptr<DisplayableComponent>) override;
public:
	// arguments: name of the calendar, length of the calendar in years
	Calendar(string n, size_t y);
	// inherited methods
	virtual void display(unsigned int) override;

};
