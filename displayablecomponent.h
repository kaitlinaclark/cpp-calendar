#pragma once
/*
	Declaration of DisplayableComponent. DisplayableComponent is the common base class of all displayable objects.
	It declares and defines the interface that all objects in the composite share.
*/

#include <memory>
#include <vector>
using namespace std;


// forward declaration, needed for friend declaration
class FullCalendarBuilder;
class CalendarInterface;


// abstract base class representing a composite object
class DisplayableComponent {
	friend FullCalendarBuilder;
	friend CalendarInterface;
public:
	// defined by concrete base classes deriving from DisplayableComponent
	virtual void display(unsigned int) = 0;

protected:
	// member variables
	vector<shared_ptr<DisplayableComponent> > children;
	weak_ptr<DisplayableComponent> parent; // weak_ptr to avoid cycles, does not contribute to reference count

	// construction, destruction
	DisplayableComponent(shared_ptr<DisplayableComponent>);
	virtual ~DisplayableComponent();

	// composite and traversal functions
	virtual weak_ptr<DisplayableComponent> getParent();
	virtual shared_ptr<DisplayableComponent> getChild(unsigned int);
	virtual shared_ptr<DisplayableComponent> addComponent(shared_ptr<DisplayableComponent>);
	virtual shared_ptr<DisplayableComponent> removeComponent(unsigned int);
};




