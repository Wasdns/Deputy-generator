#include <stdlib.h>
#include <iostream>
using namespace std;

struct students {
	// student number
	string student_number;

	// chosen departments
	int applications_department_number;
	string applications_department[20];

	// interest tag
	int tag_number;
	string tags[100];

	// free time
	int free_time_number;
	string free_time[200];
	
	// isChosenByDepartment?
	bool isChosenByDepartment;
};

struct departments {
	// department number
	string department_number;

	// member limitation
	int member_limit;

	// interest tag
	int tag_number;
	string tags[100];

	// event schedules
	int event_schedules_number;
	string event_schedules[200];
};