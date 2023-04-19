/*
Author: Jon Shidal
Purpose: Define Calendar component classes.
*/

#include "stdafx.h"
#include "calendarcomponents.h"
#include "calendarbuilder.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// from <ctime>
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

// static class variables
const std::vector<string> CalendarComponent::daysoftheweek_abbreviated = { "S", "M", "T", "W", "T", "F", "S" };
const std::vector<string> CalendarComponent::daysoftheweek_full = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
const std::vector<string> CalendarComponent::months = { "January", "February", "March", "April", "May", "June", "July", "August","September", "October", "November","December" };
const std::vector<int> CalendarComponent::days = { 31,28,31,30,31,30,31,31,30,31,30,31 };

const int CalendarComponent::HOURSINADAY = 24;

const int CalendarComponent::DAYSINAWEEK = 7;
const int CalendarComponent::MONTHS = 12;
const int CalendarComponent::BASEYEAR = 1900;
const int CalendarComponent::DAYSINAYEAR = 365;


//DesignStrategyPattern
///Event->display()
void DisplayableEvent::display(unsigned int b) {
	//display event names ONLY
	if (b == 2) {
		cout << " " << setw(11);
		cout << name << setw(11);

		return;
	}
	//display events with time info
	else if (b == 3) {
		cout << "\t";
		cout << calname << " : " << dateInfo.tm_hour << " " << name << endl;
		return;
	}
	//display edit/delete option with Event info
	else if (b == 4) { //b == 4
		cout << name << " happens on " << months[dateInfo.tm_mon] << " " << dateInfo.tm_mday << ", " << dateInfo.tm_year + CalendarComponent::BASEYEAR;
		cout << " at " << dateInfo.tm_hour << endl;;
		cout << "To edit event info: edit" << endl;
		cout << "To delete event: delete" << endl;
	}


}

//print TODO list
void DisplayableDay::printTODO() {
	//print todo list
	if (!TODO.empty()) {
		cout << "\t\t\t\t";
		cout << "TODO List:" << endl;
		for (int j = 0; j < TODO.size(); j++) {
			if (TODO[j].name != "temp") {
				if (TODO[j].done == true) {
					cout << "\t\t\t\t";
					cout << TODO[j].name << " is COMPLETE" << endl;
				}
				else {
					cout << "\t\t\t\t";
					cout << TODO[j].name << " is TODO" << endl;
				}
			}
		}
		//ask user what they want to do
		cout << "add TODO list item: addtodo" << endl;
		cout << "mark TODO list: mark" << endl;
	}
	else {
		cout << "create TODO item: todo" << endl;
	}
}

///Day->display()
void DisplayableDay::display(unsigned int b) {
	//display ONLY DAYS
	if (b == 1) {
		if (dateInfo.tm_mday == 1) {
			cout << "\t\t";
			for (int i = 0; i < dateInfo.tm_wday; i++) {
				cout << " " << setw(5);
			}
			cout << dateInfo.tm_mday << setw(5);
			if (dateInfo.tm_wday == 6) {
				cout << endl;
				cout << "\t\t";
			}
			return;
		}
		else if (dateInfo.tm_wday == 6) {
			cout << dateInfo.tm_mday << setw(5);
			cout << endl;
			cout << "\t\t";
			return;
		}
		else {
			cout << dateInfo.tm_mday << setw(5);
			return;
		}
	}
	//display days with event names
	else if (b == 2) {
		for (size_t g = 0; g < children.size(); ++g) {
			if (children[g] != nullptr) {
				children[g]->display(b);
			}
		}
	}
	//display ONE DAY
	else { //b==3
		cout << CalendarComponent::daysoftheweek_full[dateInfo.tm_wday] << " "
			<< dateInfo.tm_mon + 1 << "/" << dateInfo.tm_mday << "/" << dateInfo.tm_year + CalendarComponent::BASEYEAR << endl;
		for (size_t h = 0; h < children.size(); ++h) {
			if (children[h] != nullptr) {
				children[h]->display(3);
			}
		}
		printTODO();
		string in;
		cin >> in; 
		if (in == "todo") {
			CreateTODOList();
		}
		else if (in == "addtodo") {
			cout << "Enter name of todo list item" << endl;
			string n;
			cin >> n;
			addToTODOList(n);
		}
		else if (in == "mark") {
			cout << "Enter name of todo list item" << endl;
			string n2;
			cin >> n2;
			markDone(n2);
		}
		//if none of the options were displayed return to call
		return;
	}
}
///Month->display()
void DisplayableMonth::display(unsigned int b) {
	cout << endl;
	//print month name
	cout << name << endl;
	//display days ONLY
	if (b == 1) {
		//print days of the week
		cout << "\t\t";
		for (size_t j = 0; j < daysoftheweek_abbreviated.size(); ++j) cout << daysoftheweek_abbreviated[j] << setw(5);
		cout << endl;

		//display days of the month
		for (size_t i = 0; i < children.size(); ++i) {
			if (children[i] != nullptr) { // forward request to all children
				children[i]->display(1);
			}
		}
		cout << endl;
	}
	//displays with event names
	else if (b == 2) {
		//print days of the week
		cout << "\t";
		for (size_t j = 0; j < daysoftheweek_full.size(); ++j) cout << daysoftheweek_full[j] << setw(11);
		cout << endl;
		//////////////////////
		/////W/O SPACING//////
		/////////////////////
		//PRINT OUT DATES BY WEEK
		/*int index = dateInfo.tm_mday - 1;
		//display days of the month
		if (dateInfo.tm_mday == 1) {
		cout << "\t\t";
		for (int i = 0; i < dateInfo.tm_wday; i++) {
		cout << " " << setw(5);
		}
		cout << dateInfo.tm_mday << setw(5);
		if (dateInfo.tm_wday == 6) {
		cout << endl;
		cout << "\t\t";
		}
		return;
		}
		else if (dateInfo.tm_wday == 6) {
		cout << dateInfo.tm_mday << setw(5);
		cout << endl;
		cout << "\t\t";
		return;
		}
		else {
		cout << dateInfo.tm_mday << setw(5);
		return;
		}
		*/
		//print out first week of dates
		for (size_t i = 0; i < children.size(); ++i) {
			if (children[i] != nullptr) { // forward request to all children
				DisplayableDay* d = dynamic_cast<DisplayableDay *>(children[i].get());
				//check if the first of the month
				if (d->dateInfo.tm_mday == 1) {
					//change weekday and monthday
					dateInfo.tm_wday = d->dateInfo.tm_wday;
					dateInfo.tm_mday = d->dateInfo.tm_mday;

					//print week
					printWeek();
					for (unsigned int g = 0; g < CalendarComponent::HOURSINADAY; ++g) {
						//update weekday and monthday
						dateInfo.tm_wday = d->dateInfo.tm_wday;
						dateInfo.tm_mday = d->dateInfo.tm_mday;
						//print week of events for given hr
						printWeekOfEvents(g);
					}
				}
				//check to see if it is the beginning of the week
				else if (d->dateInfo.tm_wday == 0) {
					//change weekday and monthday
					dateInfo.tm_wday = d->dateInfo.tm_wday;
					dateInfo.tm_mday = d->dateInfo.tm_mday;

					//print week
					printWeek();
					for (unsigned int h = 0; h < CalendarComponent::HOURSINADAY; ++h) {
						//update weekday and monthday
						dateInfo.tm_wday = d->dateInfo.tm_wday;
						dateInfo.tm_mday = d->dateInfo.tm_mday;
						//print week of events for given hr
						printWeekOfEvents(h);
					}
				}

			}
		}
	}
}
	//print week
void DisplayableMonth::printWeek() {
	//if it's the first of the month/first week
	if (dateInfo.tm_mday == 1) {
		cout << "\t";
		//add spacing
		for (int i = 0; i < dateInfo.tm_wday; i++) cout << " " << setw(11);
		//print dates for 
		unsigned int stop = (CalendarComponent::DAYSINAWEEK - dateInfo.tm_wday) + 1;
		//cout << "stop: "  << stop << endl;
		for (unsigned int k = dateInfo.tm_mday; k < stop; ++k) {
			//print out day
			cout << k << setw(11);
			//check for end of week
			if (dateInfo.tm_wday == 6) {
				cout << endl;
				return;
			}
			
		}
		return;
		
	}

	//print out week last week
	else if ((children.size() - dateInfo.tm_mday) < (unsigned int ) 7) {
		cout << "\t";
		//print dates for 
		unsigned int stop = children.size() - dateInfo.tm_mday;
		for (unsigned int k = 0; k <= stop; ++k) {
			//print out day
			cout << dateInfo.tm_mday << setw(11);

			//update monthday
			dateInfo.tm_mday++;
		}
		cout << endl;
		return;
	}
	else {
		cout << "\t";
		for (size_t i = dateInfo.tm_mday; i < dateInfo.tm_mday + CalendarComponent::DAYSINAWEEK; ++i) {
			cout << i << setw(11);
		}
		cout << endl;
		return;
	}
	
}
void DisplayableMonth::printWeekOfEvents(int hr) {
		//if it's the first of the month/first week
		if (dateInfo.tm_mday == 1) {
			cout << "\t";
			//add spacing
			for (int i = 0; i < dateInfo.tm_wday; i++) cout << " " << setw(11);
			//print events for 
			unsigned int stop = (CalendarComponent::DAYSINAWEEK - dateInfo.tm_wday) + 1;
			for (unsigned int k = dateInfo.tm_mday; k < stop; ++k) {
				DisplayableDay* d = dynamic_cast<DisplayableDay*>(children[k].get());

				//get the event
				shared_ptr<DisplayableComponent> e = d->getChild(hr);
				if (e != nullptr) {
					//print out event name
					e->display(2);
					//check for end of week
					if (dateInfo.tm_wday == 6) {
						cout << endl;
						return;
					}
				}
				cout << " " << setw(11);

			}
			return;

		}

		//print out week last week
		else if ((children.size() - dateInfo.tm_mday) < (unsigned int)7) {
			cout << "\t";
			//print dates for 
			unsigned int stop = children.size() - dateInfo.tm_mday;
			for (unsigned int k = 0; k <= stop; ++k) {
				//get day and event with given hr
				DisplayableDay* d = dynamic_cast<DisplayableDay*>(children[dateInfo.tm_mday].get());
				shared_ptr<DisplayableComponent> e = d->getChild(hr);
				if (e != nullptr) {
					//display event name
					e->display(2);
					//update monthday
					dateInfo.tm_mday++;
				}
				cout << " " << setw(11);
				
			}
			cout << endl;
			return;
		}
		else {
			cout << "\t";
			for (size_t i = dateInfo.tm_mday; i < dateInfo.tm_mday + CalendarComponent::DAYSINAWEEK; ++i) {
				DisplayableDay* d = dynamic_cast<DisplayableDay*>(children[i].get());
				shared_ptr<DisplayableComponent> e = d->getChild(hr);
				if (e != nullptr) {
					e->display(2);
				}
				cout << " " << setw(11);
			}
			cout << endl;
			return;
		}

	
}

///Year->display()
void DisplayableYear::display(unsigned int b) {
	cout << "\tYear " << dateInfo.tm_year + CalendarComponent::BASEYEAR << ":" << endl;
	for (size_t i = 0; i < children.size(); ++i) { // forward request to all children
		if (children[i] != nullptr) {
			children[i]->display(1);
		}
	}
}
///Calendar->display()
void Calendar::display(unsigned int b) {
	cout << "Calendar: " << name << endl;
	for (size_t i = 0; i < children.size(); ++i) { // forward request to all children
		if (children[i] != nullptr) {
			children[i]->display(1);
		}
	}
}



// CalendarComponent
CalendarComponent::CalendarComponent(std::tm d, std::shared_ptr<DisplayableComponent> p)
	: DisplayableComponent(p), dateInfo(d) {}


//Event (no need for addComponent())
///Event->constructor()
DisplayableEvent::DisplayableEvent(std::tm d, std::shared_ptr<DisplayableComponent> p, std::string eventname, unsigned int recurrEvery, unsigned int recurrFor, string calname)
	: CalendarComponent(d, p), name(eventname), recurrEvery(recurrEvery), recurrFor(recurrFor), calname(calname) { }


// Day
///Day->constructor()
DisplayableDay::DisplayableDay(std::tm d, std::shared_ptr<DisplayableComponent> p)
	: CalendarComponent(d, p), TODO({}) {
	for (size_t i = 0; i < HOURSINADAY; ++i) {
		children.push_back(nullptr);
	}

}

///Day->addComponent()
shared_ptr<DisplayableComponent> DisplayableDay::addComponent(shared_ptr<DisplayableComponent> comp) {
	// try to dynamically cast comp to a pointer to a DisplayableDay, will fail if the DisplayableComponent is not a day
	DisplayableEvent* e = dynamic_cast<DisplayableEvent *>(comp.get()); // can only cast regular pointers, not shared_ptrs
	if (e == nullptr) { // the cast failed
		return nullptr;
	}
	// otherwise, add the day to the correct location
	int hourOfDay = e->dateInfo.tm_hour - 1;
	if (children[hourOfDay] == nullptr) { // event does not already exist
		children[hourOfDay] = comp;
		return comp;
	}
	else {  // event already exist, return existing event
		return children[hourOfDay];
	}
}


// Month
///Month->constructor()
DisplayableMonth::DisplayableMonth(std::tm d, std::shared_ptr<DisplayableComponent> p, string monthname, unsigned int numdays)
	: CalendarComponent(d, p), name(monthname) {
	numberOfDays = days[d.tm_mon];
	// initialize children vector, one spot for each day
	for (size_t i = 0; i < numberOfDays; ++i) {
		children.push_back(nullptr);
	}
}

///Month->addComponent()
shared_ptr<DisplayableComponent> DisplayableMonth::addComponent(shared_ptr<DisplayableComponent> comp) {
	// try to dynamically cast comp to a pointer to a DisplayableDay, will fail if the DisplayableComponent is not a day
	DisplayableDay* d = dynamic_cast<DisplayableDay *>(comp.get()); // can only cast regular pointers, not shared_ptrs
	if (d == nullptr) { // the cast failed
		return nullptr;
	}
	// otherwise, add the day to the correct location
	int dayOfMonth = d->dateInfo.tm_mday - 1;
	if (children[dayOfMonth] == nullptr) { // day does not already exist
		children[dayOfMonth] = comp;
		return comp;
	}
	else {  // day already exist, return existing day
		return children[dayOfMonth];
	}
}


// Year
///Year->constructor()
DisplayableYear::DisplayableYear(std::tm d, std::shared_ptr<DisplayableComponent> p, bool l)
	: CalendarComponent(d, p), leap(l) {
	for (size_t i = 0; i < CalendarComponent::MONTHS; ++i) {
		children.push_back(nullptr);
	}
}

///Year->addComponent()
shared_ptr<DisplayableComponent> DisplayableYear::addComponent(shared_ptr<DisplayableComponent> comp) {
	// try to dynamically cast comp to a pointer to a DisplayableMonth
	DisplayableMonth * m = dynamic_cast<DisplayableMonth *>(comp.get());
	if (m == nullptr) { // if the cast fails, return nullptr
		return nullptr;
	}
	// otherwise, add the month to the correct location
	int monthOfYear = m->dateInfo.tm_mon;
	if (children[monthOfYear] == nullptr) { // month does not already exist
		children[monthOfYear] = comp;
		return comp;
	}
	else {  // month already exist, return existing month
		return children[monthOfYear];
	}
}

// Calendar
///Calendar->constructor()
Calendar::Calendar(string n, size_t y)
	: CalendarComponent(tm(), nullptr), name(n), yearsToHold(y), events({}) { // just initialize with a default tm for now.
	time_t now = time(0); // get the current time
	tm now_tm;
	gmtime_s(&now_tm, &now); // create a struct tm(now_tm) from the current time
	currentDate = now_tm;    // set Calendar's date and time to now
	dateInfo = now_tm; // setup dateInfo to represent January 1st of the current year, start time of the calendar
	dateInfo.tm_sec = 0;
	dateInfo.tm_min = 0;
	dateInfo.tm_hour = 0;
	dateInfo.tm_mday = 1;
	dateInfo.tm_mon = 0;
	// calculate and set day of the week to that of January 1st, 2018. Very sloppy, I know
	dateInfo.tm_wday = (now_tm.tm_wday + CalendarComponent::DAYSINAWEEK - (now_tm.tm_yday % CalendarComponent::DAYSINAWEEK)) % CalendarComponent::DAYSINAWEEK;
	dateInfo.tm_yday = 0;
	dateInfo.tm_isdst = 0;
	// intialize calendar to hold __ years
	for (size_t i = 0; i < yearsToHold; ++i) {
		children.push_back(nullptr);
	}
}

///Calendar->addComponent()
shared_ptr<DisplayableComponent> Calendar::addComponent(std::shared_ptr<DisplayableComponent> comp) {
	DisplayableYear* y = dynamic_cast<DisplayableYear *>(comp.get());
	if (y == nullptr) { // if the cast fails, return nullptr
		return nullptr;
	}
	int calendarYear = CalendarComponent::BASEYEAR + dateInfo.tm_year;
	int yearAdding = CalendarComponent::BASEYEAR + y->dateInfo.tm_year;
	int index = yearAdding - calendarYear; // which child?
	if ((index >= 0)
		&& ((size_t)index < children.size())
		&& (children[index] == nullptr)) {
		children[index] = comp;
		return comp;
	}
	else {
		return nullptr;
	}
}



///Create a TODOLIST only if there is no TODO List already
void DisplayableDay::CreateTODOList() {
	task temp;
	temp.date = dateInfo;
	temp.done = true;
	temp.name = "temp";
	TODO.push_back(temp);
}
///Add to the TODO list
void DisplayableDay::addToTODOList(string name) {
	task newTask;
	newTask.date = dateInfo;
	newTask.name = name;
	newTask.done = false;
	TODO.push_back(newTask);
}

///Mark an item done on the 
void DisplayableDay::markDone(string name) {
	bool check = false;
	for (unsigned int i = 0; i < TODO.size();i++) {
		if (TODO[i].name == name) {
			TODO[i].done = true;
			check = true;
		}
	}
	if (check == false) {
		cout << "Unable to find " << name << " on TODO list" << endl;
	}
}



/*
///Day->display()
void DisplayableDay::display() {
cout << "\t\t\t" << daysoftheweek[dateInfo.tm_wday] << " ";
cout << dateInfo.tm_mon + 1 << "/" << dateInfo.tm_mday << "/" << dateInfo.tm_year + CalendarComponent::BASEYEAR << endl;

///Displaying the Events
for (size_t i = 0; i < children.size(); ++i) {
DisplayableEvent* e = dynamic_cast<DisplayableEvent *>(children[i].get());
if (e != nullptr) { // forward request to all children
cout << "\t\t\t\t";
cout << e->calname << ": " << e->dateInfo.tm_hour << " " << e->name << endl;
}
}


}
}




*/