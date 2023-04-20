#include "CalendarInterface.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;
unsigned int displayOption = 0;

CalendarInterface::CalendarInterface(string builderType, size_t years) 
	: builder(nullptr), cal(nullptr), currentDisplay(nullptr) { // initialize member variables
	if (builderType == "full") {
		builder = make_shared<FullCalendarBuilder>();
		string in;
		cout << "Name This Calender: " << endl;
		cin >> in;
		cal = builder->buildCalendar(in, years);
		currentDisplay = cal;
	}
}

void CalendarInterface::display() {
	currentDisplay->display(displayOption);
	//Change the Calendar’s current view
	cout << "zoom out: out, zoom in: in, quit: q" << endl;
	// Prompt for adding an event
	cout << "add event: +event" << endl;
	//Search for an Event by name
	cout << "search event: search" << endl;
	//Jump to a specific date
	cout << "jump to date: jump" << endl;
	//Save the Calendar to a file
	cout << "save calendar to file: save" << endl;
	//Restore a Calendar from a file
	cout << "import calendar from a file: restore" << endl;
	//Merge this calendar with an saved Calendar
	cout << "Merge this calendar with a saved calendar: merge" << endl;
		string in;
	cin >> in;
	if (in == "in") {
		cout << "index? (0-" << currentDisplay->children.size() - 1 << ")" << endl;
		int index = 0;
		cin >> index;
		zoomIn(index);
	}
	else if (in == "out") {
		zoomOut();
	}
	else if (in=="+event") {
		///call function for adding an event
		addEvent();
	}else if(in == "search"){
		search();
	}
	else if(in == "jump"){
		jump();
	}
	else if(in == "save"){
		save();
	}
	else if(in == "restore"){
		restore();
	}
	else if (in == "merge") {
		merge();
	}
	else if (in == "edit") {
		edit();
	}
	else if (in == "q") {
		return;
	}
	return;
}

void CalendarInterface::zoomIn(unsigned int index) {
	shared_ptr<DisplayableComponent> temp = currentDisplay->getChild(index);
	if (temp != nullptr) {
		currentDisplay = temp;
		displayOption++;
	}
	display();
}

void CalendarInterface::zoomOut() {
	if (currentDisplay->getParent().lock() != nullptr) {
		currentDisplay = currentDisplay->getParent().lock();
		displayOption--;
	}
	display();
}

///Adding the Event function
void CalendarInterface::addEvent() {
	std::string name;
	std::string in;
	int time;
	int year;
	int month;
	int dayOfMonth;
	int recurrEvery;
	int recurrFor;
	std::tm when;
	stringstream ss;
	/// First Get all the information we need from the user
	cout << "Year Of Event (integer): " << endl;
	cin >> in;
	ss << in;
	ss >> year;
	ss.clear();


	cout << "Month Of Event (integer): " << endl;
	cin >> in;
	ss << in;
	ss >> month;
	ss.clear();

	///We should probably check that the day exists in that month, but we can add that functionality later
	cout << "Day Of Event (integer): " << endl;
	cin >> in;
	ss << in;
	ss >> dayOfMonth;
	ss.clear();
	 
    
	cout << "Time of day (integer): " << endl;
	cin >> in;
	ss << in;
	ss >> time;
	ss.clear();

	cout << "Name of Event: " << endl;
	cin >> name;
	ss.clear();
	
	while (in != "y" && in != "n") {
		cout << "Would you like this event to reoccur? (y/n)" << endl;
		cin >> in;
		if (in == "y") {
			cout << "Would you like reccurEvery, or reccurFor? (re/rf)" << endl;
			cin >> in;
			//while (in!="re" && in != "rf") {
				if (in == "re") {
					cout << "Number of days after which to repeat (integer): " << endl;
					cin >> in;
					ss << in;
					ss >> recurrEvery;
					recurrFor = 0;
					break;
				}
				else if (in == "rf") {
					cout << "Number of days to repeat for (integer): " << endl;
					cin >> in;
					ss << in;
					ss >> recurrFor;
					recurrEvery = 0;
					break;
				}
		}
		else {
			recurrFor = 0;
			recurrEvery = 0;
		}
	}
	//set date/time info for Event
	when.tm_hour = time;
	when.tm_mon = month - 1;
	when.tm_mday = dayOfMonth;
	when.tm_year = year-1900;
	
	//getday
	int calendarYear = CalendarComponent::BASEYEAR + cal->dateInfo.tm_year;
	int yearAdding = CalendarComponent::BASEYEAR + when.tm_year;
	int index = calendarYear - yearAdding; // which child?
	shared_ptr<DisplayableComponent> tempY = currentDisplay->getChild(index);
	shared_ptr<DisplayableComponent> tempM = tempY->getChild(when.tm_mon);
	shared_ptr<DisplayableComponent> day = tempM->getChild(when.tm_mday-1);

		
	cout << recurrEvery << endl;
	cout << recurrFor << endl;
	//add event to day
	if (recurrEvery > 0 || recurrFor > 0) {
		if (recurrEvery > 0) {
			while (true){
				cal->events.push_back(builder->buildEvent(when, day, name, recurrEvery, recurrFor, cal->name));

				///Go to the next day
				when.tm_mday = when.tm_mday + recurrEvery;
				///Get the number of days in the current month
				int daysInMonth = cal->days[when.tm_mon];
				index = calendarYear - yearAdding; // which child?

				///Change the year if needed
				if (when.tm_mon == 11 && when.tm_mday>31) {
					when.tm_year = when.tm_year + 1;
					calendarYear = CalendarComponent::BASEYEAR + cal->dateInfo.tm_year;
					yearAdding = CalendarComponent::BASEYEAR + when.tm_year;
					index = calendarYear - yearAdding; // which child?
					if (index + 1>cal->yearsToHold) {
						break;
					}
				}
				///Change the month if needed
				if (when.tm_mday>daysInMonth) {
					when.tm_mday = 1;
					if (when.tm_mon<11) {
						when.tm_mon = when.tm_mon + 1;
					}
					else {
						when.tm_mon = 0;
					}
				}
				if (index < cal->yearsToHold) {
					tempY = currentDisplay->getChild(index);
					tempM = tempY->getChild(when.tm_mon);
					day = tempM->getChild(when.tm_mday-1);
				}
				else {
					break;
				}
				
			}
		}
		else if (recurrFor > 0) {
			for (int i = 0; i <= recurrFor;i++) {

				cal->events.push_back(builder->buildEvent(when, day, name, recurrEvery, recurrFor,cal->name));

				///Go to the next day
				when.tm_mday = when.tm_mday + 1;
				///Get the number of days in the current month
				int daysInMonth = cal->days[when.tm_mon];

				///Change the year if needed
				if (when.tm_mon==11 && when.tm_mday>31) {

					when.tm_year = when.tm_year + 1;
					calendarYear = CalendarComponent::BASEYEAR + cal->dateInfo.tm_year;
					yearAdding = CalendarComponent::BASEYEAR + when.tm_year;
					index = calendarYear - yearAdding; // which child?
					if (index+1>cal->yearsToHold) {
						break;
					}
				}
				///Change the month if needed
				if (when.tm_mday>daysInMonth) {

					when.tm_mday = 1;
					if (when.tm_mon<11) {

						when.tm_mon = when.tm_mon + 1;
					}
					else {
						when.tm_mon = 0;
					}
				}
				tempY = currentDisplay->getChild(index);
				tempM = tempY->getChild(when.tm_mon);
				day = tempM->getChild(when.tm_mday-1);				
			}
		}
	}
	else {
		cal->events.push_back(builder->buildEvent(when, day, name, recurrEvery, recurrFor, cal->name));
	}
	//display calendar again
	display();
}

//search
void CalendarInterface::search() {
	/*Should display details of the event if found by
	1. updating the current view of the Calendar to display the event
	2. If multiple Events have the same name, a list of all Events with the matching name should be displayed.
	-The user can then choose one to display.
	3. If no event matches the name, a message should be printed to alert the user.*/
	cout << "Enter the event name you wish to search: " << endl;
	string name_to_search;
	cin >> name_to_search;

	//vector to save search results
	vector<DisplayableEvent> events_found = {};
	for (unsigned int i = 0; i < cal->events.size(); ++i) {
		if (name_to_search == (cal->events[i])->name) {
			//push event to vector once event with matching name is found
			events_found.push_back(*(cal->events[i]));
		}
	}

	//if there are multiple events with the same name
	if (events_found.size() > 1) {
		//display all events with same name
		for (unsigned int j = 0; j < events_found.size(); ++j) {
			cout << (j + 1) << ". " << events_found[j].name;
			cout << " happens on " << cal->months[events_found[j].dateInfo.tm_mon] << " " << events_found[j].dateInfo.tm_mday << ", " << events_found[j].dateInfo.tm_year + CalendarComponent::BASEYEAR;
			cout << " at " << events_found[j].dateInfo.tm_hour << endl;
		}

		//have user choose one
		cout << "Please select one of the above events. Enter a number from 1-" << events_found.size() << endl;
		string in;
		cin >> in;

		istringstream is(in);
		unsigned int e_loc;
		is >> e_loc;

		//display event
		--e_loc;
		displayOption = 4;
		events_found[e_loc].display(displayOption);
	}
	else if (events_found.size() == 0) {

		cout << "No events found with that name." << endl;
	}
	else {
		//display single event
		displayOption = 4;
		events_found[0].display(displayOption);
	}
}

//jump to date
void CalendarInterface::jump() {
	/*Jump to date and select the granularity to view in the calendar
	Ex: a user may specify August 10th, 2018 and “day” as the granularity,
	the display should then update its current view to the DisplayableDay object
	representing August 10th, 2018.
	If the user specifies “month” as the granularity,
	the display should update its current view to the DisplayableMonth object
	containing the DisplayableDay object for August 10th in its component subtree.
	If the date does not exist in the calendar,
	a useful message should be printed to notify the user and the current view should be displayed again.*/
	cout << "Enter Date in form MM/DD/YYYY with no trailing zeros (e.g. 12/8/2018): " << endl;
	string input;
	cin >> input;

	//get month, day and year from input
	istringstream ss(input);
	string token;
	getline(ss, token, '/');
	int mon = stoi(token);

	getline(ss, token, '/');
	int day = stoi(token);

	getline(ss, token, '/');
	int year = stoi(token);
	ss.clear();

	//set time/date info
	tm date;
	date.tm_mon = mon - 1;
	date.tm_mday = day;
	date.tm_year = year - CalendarComponent::BASEYEAR;


	cout << "Please select one of following view options: \n Year \n Month \n Day" << endl;
	string gran;
	cin >> gran;

	//put input string to all lowercase
	for (unsigned int i = 0; i < gran.size(); i++) putchar(tolower(gran[i]));

	//change current display
	currentDisplay = builder->getComponentByDate(date, gran);
	if (gran == "year") {
		displayOption = 1;
	}
	else if (gran == "month") {
		displayOption = 2;
	}
	else if (gran == "day") {
		displayOption = 3;
	}

	//re-display calendar
	display();
}

//save calendar in file
void CalendarInterface::save() {
	/*save calendar so that it may be restored later,
	make sure to save/restore the calendar’s name as well*/
	cout << "Enter file name to save calendar under (saved as TXT file): " << endl;
	string filename;
	cin >> filename;
	string purename = filename;
	filename = filename + ".txt";
	ofstream saveFile;
	saveFile.open(filename);
	/// List all the Calander Info
	saveFile << "CalenderInfo " 
			<< cal->name << " " 
			<< cal->yearsToHold << " " 
			<< cal->dateInfo.tm_sec << " " 
			<< cal->dateInfo.tm_min << " " 
			<< cal->dateInfo.tm_hour << " " 
			<< cal->dateInfo.tm_mday << " " 
			<< cal->dateInfo.tm_mon << " " 
			<< cal->dateInfo.tm_wday << " " 
			<< cal->dateInfo.tm_yday << " "
			<< cal->dateInfo.tm_isdst 
		<< endl;
	
	for (unsigned int i = 0; i < cal->events.size(); i++) {
		DisplayableEvent e = *(cal->events[i]);
		//cout << "saving event" << endl;
		if (filename == "test" || e.calname == "test") {
			saveFile << e.name << " " << e.recurrEvery << " " << e.recurrFor << " " << e.dateInfo.tm_year << " " << e.dateInfo.tm_mon << " " << e.dateInfo.tm_mday << " " << e.dateInfo.tm_hour << " " << purename << endl;
		}
		else {
			saveFile << e.name << " " << e.recurrEvery << " " << e.recurrFor << " " << e.dateInfo.tm_year << " " << e.dateInfo.tm_mon << " " << e.dateInfo.tm_mday << " " << e.dateInfo.tm_hour << " " << e.calname << endl;
		}
		
	}
	saveFile.close();
	//cout << filename << ".txt has been saved" << endl;
	display();
}

//restore calendar from file
void CalendarInterface::restore() {
	/*This should overwrite the contents of the current Calendar
	Perhaps a new concrete Builder would work well here*/
	cout << "Enter filename (must be TXT file): " << endl;
	string filename;
	cin >> filename;
	string purename = filename;
	filename = filename + ".txt";
	ifstream saveFile;

	saveFile.open(filename);
	///Check that file was opened
	if (!saveFile) {
		cout << "Error!! Could not open requested file! Please check file name and location!" << endl;
		exit(1);
	}
	std::string line;


	std::getline(saveFile, line);
	std::istringstream iss(line);
	///What Save() puts as the first line

	std::string throwaway;
	std::string calname;
	size_t yearsToHold;
	int sec;
	int mins;
	int hour;
	int mday;
	int mon;
	int wday;
	int yday;
	int isdst;
	///Get in all the info from the first line that is realated to calendar
	iss >> throwaway;
	//cout << throwaway << endl;
	iss >> calname;
	//cout << calname << endl;
	iss >> yearsToHold;
	//cout << yearsToHold << endl;
	iss >> sec;
	//cout << sec << endl;
	iss >> mins;
	//cout << mins << endl;
	iss >> hour;
	//cout << hour << endl;
	iss >> mday;
	//cout << mday << endl;
	iss >> mon;
	//cout << mon << endl;
	iss >> wday;
	//cout << wday << endl;
	iss >> yday;
	//cout << yday << endl;
	iss >> isdst;
	//cout << isdst << endl;

	///Get all the date related info together

	std::tm currentDate;
	currentDate.tm_sec = sec;
	currentDate.tm_min = mins;
	currentDate.tm_hour = hour;
	currentDate.tm_mday = mday;
	currentDate.tm_mon = mon;
	currentDate.tm_wday = wday;
	currentDate.tm_yday = yday;
	currentDate.tm_isdst = isdst;

	/// Create the new Calender
	restoreBuilder = make_shared<FullCalendarBuilder>();
	restoreCal = restoreBuilder->buildCalendar(calname, yearsToHold);
	restoreCal->currentDate = currentDate;
	currentDisplay = restoreCal;


	///Go through all the other lines in the text file which contain all the events;
	while (std::getline(saveFile, line)) {

		std::istringstream iss(line);
		///This is how we save events
		std::string name;
		std::string calname;
		int recurrEvery;
		int recurrFor;
		int year;
		int mon;
		int mday;
		int hour;
		///Extract the info we want
		iss >> name;
		iss >> recurrEvery;
		iss >> recurrFor;
		iss >> year;
		iss >> mon;
		//cout << "month:" << mon << endl;

		iss >> mday;
		//cout << "day:" << mday << endl;
		iss >> hour;
		iss >> calname;
		std::tm eventDate;
		eventDate.tm_year = year;
		eventDate.tm_mon = mon;
		eventDate.tm_mday = mday;
		eventDate.tm_hour = hour;
		//			saveFile << e.name << " " << e.recurrEvery << " " << e.recurrFor << " " << e.dateInfo.tm_year << " " << e.dateInfo.tm_mon << " " << e.dateInfo.tm_mday << " " << e.dateInfo.tm_hour << " " << e.calname << endl;

		///Create an event and add it to the calender
		int calendarYear = CalendarComponent::BASEYEAR + cal->dateInfo.tm_year;
		int yearAdding = CalendarComponent::BASEYEAR + eventDate.tm_year;
		int index = calendarYear - yearAdding; // which child?
		//cout << "index: " << index << endl;
		shared_ptr<DisplayableComponent> tempY = currentDisplay->getChild(index);
		//cout << "eventDate.tm_mon: " << eventDate.tm_mon << endl;
		shared_ptr<DisplayableComponent> tempM = tempY->getChild(mon);
		//cout << "eventDate.tm_mday: " << eventDate.tm_mon << endl;

		shared_ptr<DisplayableComponent> day = tempM->getChild(mday-1);

		
		//add event to day
		cal->events.push_back(builder->buildEvent(eventDate, day, name, recurrEvery, recurrFor, calname));



	}
	saveFile.close();
	//display the new calender
	display();
	//cout << filename << ".txt Has been restored" << endl;
}

//Edit or delete file
void CalendarInterface::edit() {
	std::string name;
	std::string in;
	std::string option;
	int time;
	int year;
	int month;
	int dayOfMonth;

	std::tm when;
	stringstream ss;
	/// First Get all the information we need from the user
	cout << "Year Of Event (integer): " << endl;
	cin >> in;
	ss << in;
	ss >> year;
	ss.clear();


	cout << "Month Of Event (integer): " << endl;
	cin >> in;
	ss << in;
	ss >> month;
	ss.clear();

	///We should probably check that the day exists in that month, but we can add that functionality later
	cout << "Day Of Event (integer): " << endl;
	cin >> in;
	ss << in;
	ss >> dayOfMonth;
	ss.clear();


	cout << "Time of day (integer): " << endl;
	cin >> in;
	ss << in;
	ss >> time;
	ss.clear();

	cout << "Name of Event: " << endl;
	cin >> name;
	ss.clear();

	///Construct the when for the event
	when.tm_hour = time;
	when.tm_mon = month - 1;
	when.tm_mday = dayOfMonth;
	when.tm_year = year - 1900;

	cout << "Edit or Delete this Event? (delete/edit): ";
	cin >> option;
	ss.clear();

	int calendarYear = CalendarComponent::BASEYEAR + cal->dateInfo.tm_year;
	int yearAdding = CalendarComponent::BASEYEAR + when.tm_year;
	int index = calendarYear - yearAdding; // which child?
	shared_ptr<DisplayableComponent> tempY = currentDisplay->getChild(index);
	shared_ptr<DisplayableComponent> tempM = tempY->getChild(when.tm_mon);
	shared_ptr<DisplayableComponent> day = tempM->getChild(when.tm_mday);

	bool check = false;
	if (option == "delete") {
		cout << "here1" << endl;
		for (int i = 0; i < cal->events.size();i++) {
			std::string currentName = cal->events[i]->name;
			std::tm currentWhen = cal->events[i]->dateInfo;
			int currentTime = currentWhen.tm_hour;
			int currentYear = currentWhen.tm_year;
			int currentMonth = currentWhen.tm_mon;
			int currentDayOfMonth = currentWhen.tm_mday;
			//cout << day->children[currentTime] << endl;
			cout << "here2" << endl;
			if (currentName == name && currentTime == when.tm_hour && currentYear == year - 1900 && currentMonth == month - 1 && currentDayOfMonth == when.tm_mday) {
				cout << "here3" << endl;
				cal->events[i] = nullptr;
				DisplayableEvent* e = dynamic_cast<DisplayableEvent *>(day->children[currentTime].get());
				e = nullptr;
				day->children[currentTime] = nullptr;
			

				//day->children[currentTime]->display();
				//day->removeComponent(currentTime);
				currentDisplay = cal;

				check = true;

				cout << name << " has been deleted" << endl;
				//cout << day->children[currentTime] << endl;

				display();
				break;
			}

		}
		if (check == false) {
			display();
			cout << "Error could not find the event to delete it." << endl;
		}
	}
	else if (option == "edit") {
		for (int i = 0; i < cal->events.size();i++) {
			std::string currentName = cal->events[i]->name;
			std::tm currentWhen = cal->events[i]->dateInfo;
			int currentTime = currentWhen.tm_hour;
			int currentYear = currentWhen.tm_year;
			int currentMonth = currentWhen.tm_mon;
			int currentDayOfMonth = currentWhen.tm_mday;

			if (currentName == name && currentTime == time && currentYear == year && currentMonth == month && currentDayOfMonth == dayOfMonth) {
				std::string newName;
				int newTime;
				int newYear;
				int newMonth;
				int newDayOfMonth;
				std::tm newWhen;


				/// First Get all the information we need from the user
				cout << "New Year Of Event (integer): " << endl;
				cin >> in;
				ss << in;
				ss >> newYear;
				ss.clear();


				cout << "New Month Of Event (integer): " << endl;
				cin >> in;
				ss << in;
				ss >> newMonth;
				ss.clear();

				///We should probably check that the day exists in that month, but we can add that functionality later
				cout << "New Day Of Event (integer): " << endl;
				cin >> in;
				ss << in;
				ss >> newDayOfMonth;
				ss.clear();


				cout << "New Time of day (integer): " << endl;
				cin >> in;
				ss << in;
				ss >> newTime;
				ss.clear();

				cout << "New Name of Event: " << endl;
				cin >> newName;
				ss.clear();

				///Construct the when for the event
				newWhen.tm_hour = newTime;
				newWhen.tm_mon = newMonth - 1;
				newWhen.tm_mday = newDayOfMonth;
				newWhen.tm_year = newYear - 1900;

				///Edit the actual event
				currentName = newName;
				currentTime = newWhen.tm_hour;
				currentYear = newWhen.tm_year;
				currentMonth = newWhen.tm_mon;
				currentDayOfMonth = newWhen.tm_mday;

				check = true;
				display();
				cout << "Your event has been edited" << endl;
				break;
			}

		}
		if (check == false) {
			display();
			cout << "Error could not find the event to edit it." << endl;
		}
	}

}

//Merge Two Calenders
void CalendarInterface::merge() {
	///Get the file we need to load
	cout << "Enter the name of the file you want to merge the current calender with (saved as TXT file): " << endl;
	string filename;
	cin >> filename;
	filename = filename + ".txt";

	
	ifstream LoadFile;

	LoadFile.open(filename);
	///Check that file was opened
	if (!LoadFile) {
		cout << "Error!! Could not open requested file! Please check file name and location!" << endl;
		exit(1);
	}
	std::string line;

	///Load File we want to merge with
	std::getline(LoadFile, line);
	///Get the firstline and toss it as it contains only info for creating a new calendar
	std::istringstream iss(line);


	///Go through all the other lines in the text file which contain all the events;
	while (std::getline(LoadFile, line)) {

		std::istringstream iss(line);
		///This is how we save events
		std::string name;
		std::string calname;
		int recurrEvery;
		int recurrFor;
		int year;
		int mon;
		int mday;
		int hour;

		///Extract the info we want
		iss >> name;
		iss >> recurrEvery;
		iss >> recurrFor;
		iss >> year;
		iss >> mon;
		iss >> mday;
		iss >> hour;
		iss >> calname;
		std::tm eventDate;
		eventDate.tm_year = year;
		eventDate.tm_mon = mon;
		eventDate.tm_mday = mday;
		eventDate.tm_hour = hour;



		///Create an event and add it to the calender
		int calendarYear = CalendarComponent::BASEYEAR + cal->dateInfo.tm_year;
		int yearAdding = CalendarComponent::BASEYEAR + eventDate.tm_year;
		int index = calendarYear - yearAdding; // which child?
		shared_ptr<DisplayableComponent> tempY = currentDisplay->getChild(index);
		shared_ptr<DisplayableComponent> tempM = tempY->getChild(eventDate.tm_mon);
		shared_ptr<DisplayableComponent> day = tempM->getChild(eventDate.tm_mday-1);


		//add event to day
		cal->events.push_back(builder->buildEvent(eventDate, day, name, recurrEvery, recurrFor, calname));



	}
	LoadFile.close();
	//display the new calender

		///Get the file we want to save to
	std::cout << "Enter the name of the new merged calendar (saved as TXT file): " << endl;
	string savename;
	cin >> savename;
	savename = savename + ".txt";
	ofstream saveFile;
	saveFile.open(savename);
	/// List all the Calander Info
	saveFile << "CalenderInfo "
		<< cal->name << " "
		<< cal->yearsToHold << " "
		<< cal->dateInfo.tm_sec << " "
		<< cal->dateInfo.tm_min << " "
		<< cal->dateInfo.tm_hour << " "
		<< cal->dateInfo.tm_mday << " "
		<< cal->dateInfo.tm_mon << " "
		<< cal->dateInfo.tm_wday << " "
		<< cal->dateInfo.tm_yday << " "
		<< cal->dateInfo.tm_isdst
		<< endl;

	for (unsigned int i = 0; i < cal->events.size(); i++) {
		DisplayableEvent e = *(cal->events[i]);
		//cout << "saving event" << endl;
		if (filename == "test" || e.calname=="test") {
			saveFile << e.name << " " << e.recurrEvery << " " << e.recurrFor << " " << e.dateInfo.tm_year << " " << e.dateInfo.tm_mon << " " << e.dateInfo.tm_mday << " " << e.dateInfo.tm_hour << " " << filename << endl;
		}
		else {
			saveFile << e.name << " " << e.recurrEvery << " " << e.recurrFor << " " << e.dateInfo.tm_year << " " << e.dateInfo.tm_mon << " " << e.dateInfo.tm_mday << " " << e.dateInfo.tm_hour << " " << e.calname << endl;
		}

	}
	saveFile.close();
	//cout << savename << ".txt has been saved" << endl;


	display();
}
