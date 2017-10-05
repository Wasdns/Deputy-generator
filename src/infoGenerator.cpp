#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <time.h>
#include "definition.h"
#include "infoGenerator.h"
using namespace std;

extern students student[310];
extern departments department[25];

/* interest tags */
string interest_tags[20] = {"study", "film", "English", "music", "reading", "chess", "football", "dance", "programming", "basketball"};

int generateWeekday() {
	srand((unsigned)(time(NULL)));
	return rand()%7;
}

int generateStartTime() {
	srand((unsigned)(time(NULL)));
	return rand()%1440;
}

int generateEndTime(int base) {
	srand((unsigned)(time(NULL)));

	int idx = rand()%1440;
	if (base+idx >= 1440) {
		idx = base+idx-1440;
	}
	
	return base+idx;
}

string generateTimeString(int weekday, int startTime, int EndTime) {
	string outputString = "";

	// weekday
	if (weekday == 0) {
		outputString += "Sun.";
	} else if (weekday == 1) {
		outputString += "Mon.";
	} else if (weekday == 2) {
		outputString += "Tues.";
	} else if (weekday == 3) {
		outputString += "Wed.";
	} else if (weekday == 4) {
		outputString += "Thur.";
	} else if (weekday == 5) {
		outputString += "Fri.";
	} else if (weekday == 6) {
		outputString += "Sat.";
	}

	// StartTime
	int startTimeHour = startTime/60;
	int startTimeMinute = startTime%60;
	string startTimeHour_str = "", startTimeMinute_str = "";
	string startTimeHour_str_suffix = "", startTimeMinute_str_suffix = "";

	stringstream startss1, startss2;
	startss1 << startTimeHour;
	startss1 >> startTimeHour_str_suffix;
	startss2 << startTimeMinute;
	startss2 >> startTimeMinute_str_suffix;

	if (startTimeHour_str_suffix.length() < 2) {
		startTimeHour_str += "0";
	}
	if (startTimeMinute_str_suffix.length() < 2) {
		startTimeMinute_str += "0";
	}

	startTimeHour_str += startTimeHour_str_suffix;
	startTimeMinute_str += startTimeMinute_str_suffix;

	outputString = outputString+startTimeHour_str+":"+startTimeMinute_str;

	// partition flag
	outputString += "~";

	// EndTime
	int endTimeHour = EndTime/60;
	int endTimeMinute = EndTime%60;
	string endTimeHour_str = "", endTimeMinute_str = "";
	string endTimeHour_str_suffix = "", endTimeMinute_str_suffix = "";

	stringstream endss1, endss2;
	endss1 << endTimeHour;
	endss1 >> endTimeHour_str_suffix;
	endss2 << endTimeMinute;
	endss2 >> endTimeMinute_str_suffix;

	if (endTimeHour_str_suffix.length() < 2) {
		endTimeHour_str += "0";
	}
	if (endTimeMinute_str_suffix.length() < 2) {
		endTimeMinute_str += "0";
	}

	endTimeHour_str += endTimeHour_str_suffix;
	endTimeMinute_str += endTimeMinute_str_suffix;

	outputString = outputString+endTimeHour_str+":"+endTimeMinute_str;

	return outputString;
}

int generateTagHandle(int randomBase) {
	srand((unsigned)(time(NULL))+randomBase);
	return rand()%10;
}

int generateLimitation(int randomBase) {
	srand((unsigned)(time(NULL)+randomBase));
	return 10+rand()%6;
}

/*
 *  Function Name: generateStudentInfo
 *  generate student information
 */
void infoGenerator::generateStudentInfo() {
	for (int i = 0; i < 300; i++) {
		// set srand
		srand((unsigned)(time(NULL)+i));

		// generate student_no
		string index = "";
		string index_suffix = "";
		stringstream ss;
		ss << i+1;
		ss >> index_suffix;

		if (index_suffix.length() == 1) {
			index = index+"00"+index_suffix;
		} else if (index_suffix.length() == 2) {
			index = index+"0"+index_suffix;
		} else {
			index = index_suffix;
		}
		
		student[i].student_number = "031502"+index;

		// random number of free time
		// 140 = 10*2/day*7days
		student[i].free_time_number = rand()%140; 

		// generate "free_time_number" free time strings
		for (int j = 0; j < student[i].free_time_number; j++) {
			// acquire free time string components
			int weekday, startTime, endTime;
			weekday = generateWeekday();
			startTime = generateStartTime();
			endTime = generateEndTime(startTime);

			student[i].free_time[j] = generateTimeString(weekday, startTime, endTime);
		}

		// random number of tags
		student[i].tag_number = rand()%10;

		// searchKey[handle] == 1: the key has been used
		int searchKey[10];
		memset(searchKey, 0, sizeof(searchKey));
		// current tag number
		int currentTagNumber = 0, base = 0;

		for (currentTagNumber = 0; currentTagNumber < student[i].tag_number; currentTagNumber++) {
			int handle = generateTagHandle(base);
			// if the tag has been added
			if (searchKey[handle] == 1) {
				// find the nearest one
				while (searchKey[handle] == 1) {
					handle++;
					handle = handle%10;
					if (searchKey[handle] == 0) break;
				}
			}
			// add the tag
			student[i].tags[currentTagNumber] = interest_tags[handle];
			searchKey[handle] = 1;
		}

		// TODO: applications_department
	}
}

/*
 *  Function Name: generateDepartmentInfo
 *  generate department information
 */
void infoGenerator::generateDepartmentInfo() {
		for (int i = 0; i < 20; i++) {
		// set srand
		srand((unsigned)(time(NULL)+i));

		// generate student_no
		string index = "";
		string index_suffix = "";
		stringstream ss;
		ss << i+1;
		ss >> index_suffix;

		if (index_suffix.length() == 1) {
			index = index+"00"+index_suffix;
		} else if (index_suffix.length() == 2) {
			index = index+"0"+index_suffix;
		} else {
			index = index_suffix;
		}
		
		department[i].department_number = "D"+index;

		// random number of event_schedules
		// 140 = 10*2/day*7days
		department[i].event_schedules_number = rand()%140; 

		// generate "event_schedules_number" event schedule strings
		for (int j = 0; j < department[i].event_schedules_number; j++) {
			// acquire free time string components
			int weekday, startTime, endTime;
			weekday = generateWeekday();
			startTime = generateStartTime();
			endTime = generateEndTime(startTime);

			department[i].event_schedules[j] = generateTimeString(weekday, startTime, endTime);
		}

		// random number of tags
		department[i].tag_number = rand()%10;

		// searchKey[handle] == 1: the key has been used
		int searchKey[10];
		memset(searchKey, 0, sizeof(searchKey));
		// current tag number
		int currentTagNumber = 0, base = 0;

		for (currentTagNumber = 0; currentTagNumber < department[i].tag_number; currentTagNumber++) {
			int handle = generateTagHandle(base);
			// if the tag has been added
			if (searchKey[handle] == 1) {
				// find the nearest one
				while (searchKey[handle] == 1) {
					handle++;
					handle = handle%10;
					if (searchKey[handle] == 0) break;
				}
			}
			// add the tag
			department[i].tags[currentTagNumber] = interest_tags[handle]; 
			searchKey[handle] = 1;
		}

		// random limitation
		department[i].member_limit = generateLimitation(i);
	}
}
