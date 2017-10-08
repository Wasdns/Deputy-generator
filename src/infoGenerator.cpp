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

int seedBase = 0;

/* interest tags */
string interest_tags[20] = {"study", "film", "English", "music", "reading", "chess", "football", "dance", "programming", "basketball"};

static int generateWeekday() {
	srand((unsigned)(time(NULL))+seedBase);
	seedBase++;
	return rand()%7;
}

static int generateStartTime() {
	srand((unsigned)(time(NULL))+seedBase);
	seedBase++;
	
	int rawTime = rand()%1440;
	if (rawTime <= 480) { 
		rawTime += 480;
	} else if (rawTime >= 1260) { 
		rawTime = 1260-(rawTime-1260);
	}

	return rawTime;
}

static int generateEndTime(int base) {
	srand((unsigned)(time(NULL))+seedBase);
	seedBase++;

	int idx = rand()%1440;
	if (base+idx >= 1440) {
		idx = base+idx-1440;
	}
	
	// 30min <= idx <= 180min 
	idx %= 150;
	idx += 30;

	return base+idx;
}

static string generateTimeString(int weekday, int startTime, int EndTime) {
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

static int generateTagHandle(int randomBase) {
	srand((unsigned)(time(NULL))+randomBase+seedBase);
	seedBase++;
	return rand()%10;
}

static int generateLimitation(int randomBase) {
	srand((unsigned)(time(NULL)+randomBase)+seedBase);
	seedBase++;
	return 10+rand()%6;
}

/*
 *  Function Name: generateStudentInfo
 *  generate student information
 */
void infoGenerator::generateStudentInfo() {
	// generate basic student information
	for (int i = 0; i < 300; i++) {
		// set srand
		srand((unsigned)(time(NULL)+i+seedBase));

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

		// Considering the real world situation, we determind to 
		// reduce the free_time_number based on the interview
		// of some students.
		// 
		// The number of universal schedules is about 2-8.
		// And the schedules often be holded on weekends.

		student[i].free_time_number = student[i].free_time_number%7+2;

		// Avoid free time collision in a day
		// Each day has no more than 2 schedule timezones.

		// timeZones[i][0]: the startTime of weekday i;
		// timeZones[i][1]: the endTime of weekday i.
		int timeZones[10][5];
		memset(timeZones, 0, sizeof(timeZones));

		// isWeekdayUsed[weekday] = true: the weekday has been used
		bool isWeekdayUsed[10];
		for (int j = 0; j < 7; j++) {
			isWeekdayUsed[j] = false;
		}

		// disable more than 2 schedule timezones in a day
		bool isWeekdayDisabled[10];
		for (int j = 0; j < 7; j++) {
			isWeekdayDisabled[j] = false;
		}

		// generate "free_time_number" free time strings
		for (int j = 0; j < student[i].free_time_number; j++) {
			// acquire free time string components
			int weekday, startTime, endTime;
			weekday = generateWeekday(); 

			// if the weekday already has two time zones, search for the next day
			if (isWeekdayDisabled[weekday]) {
				while (isWeekdayDisabled[weekday]) {
					weekday = (weekday+1)%7;
				}
			}

			// the day has been scheduled once
			if (isWeekdayUsed[weekday]) {
				int lastTimeStart = timeZones[weekday][0];
				int lastTimeEnd = timeZones[weekday][1];

				// if lastTimeStart-480(8:00) > 180(3h), set it as upperBond
				// to generate free time zone
				if (lastTimeStart-480 > 180) {
					// 480 <= startTime <= lastTimeStart-180
					// startTime = rand()%(n-m+1)+m, n=lastTimeStart-180, m=480
					startTime = rand()%(lastTimeStart-180-480+1)+480;
					endTime = generateEndTime(startTime);
				} else { // otherwise, choose another part of the day
					// lastTimeEnd <= startTime <= 1260
					// startTime = rand()%(n-m+1)+m, n=1260, m=lastTimeEnd
					startTime = rand()%(1260-lastTimeEnd+1)+lastTimeEnd;
					endTime = generateEndTime(startTime);
				}

				// mark the day as "disabled"
				isWeekdayDisabled[weekday] = true;

			} else { // the day has not been scheduled
				startTime = generateStartTime();
				endTime = generateEndTime(startTime);

				// mark the day as been scheduled
				isWeekdayUsed[weekday] = true;
				
				// record this time schedule in timeZones
				timeZones[weekday][0] = startTime;
				timeZones[weekday][1] = endTime;
			}

			student[i].free_time[j] = generateTimeString(weekday, startTime, endTime);
		}

		// We are not trying to limit the number of student interests.

		// random number of tags
		student[i].tag_number = rand()%9+2;

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

		// for current student, randomly generate his applications_department_number: 1-5
		student[i].applications_department_number = random()%5+1;

		seedBase++;
	}

	// generate applications_departments for each student

	// the number of popular departments: 2-4
	// for each of popular departments: ~100(80-120) people 
	int popularDepartmentNumber = 0;
	popularDepartmentNumber = random()%3+2;
	// baseValue for calculating desired departments
	int baseValue = 10*popularDepartmentNumber+4*(20-popularDepartmentNumber);

	// for each of the students, generate his desired departments
	for (int i = 0; i < 300; i++) { 
		// avoid the same department demands of one student, eg. 1st dep1, 2nd dep1...
		// applications_department_handler[i] = 1: the department i has been choosed.
		int applications_department_handler[20];
		memset(applications_department_handler, 0, sizeof(applications_department_handler));

		// current student have chosen N(currentChooseDepNumber) departments 
		int currentChooseDepNumber = student[i].applications_department_number;

		// based on the randomly rate, choose the desired department
		for (int j = 0; j < currentChooseDepNumber; j++) {
			srand((unsigned)(time(NULL)+i+seedBase));
			seedBase++;
			
			int randomNumber = random()%baseValue;
			int currentDepHandle = 0;

			// if the randomNumber is in the range (0, popularDepartmentNumber*10),
			// that means the student lucky choose popular department
			if (randomNumber < popularDepartmentNumber*10) {
				currentDepHandle = randomNumber/10;
			} else { // otherwise, choose the colder department
				currentDepHandle = (randomNumber-popularDepartmentNumber*10)/4+popularDepartmentNumber+1;
			}

			// if the current department handle has been chosen by this student
			// search for the nearest department.
			if (applications_department_handler[currentDepHandle] == 1) {
				while (applications_department_handler[currentDepHandle] == 1) {
					currentDepHandle = (currentDepHandle+1)%20;
				}
			} 
			
			// choose the department
			student[i].applications_department[j] = department[currentDepHandle].department_number;
			// mark current department has been chosen by this student
			applications_department_handler[currentDepHandle] = 1;
		}
	}

}

/*
 *  Function Name: generateDepartmentInfo
 *  generate department information
 */
void infoGenerator::generateDepartmentInfo() {
		for (int i = 0; i < 20; i++) {
		// set srand
		srand((unsigned)(time(NULL)+i+seedBase));

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

		// Considering the real world situation, we determind to 
		// reduce the event_schedules_number based on the interview
		// of a manager of one department.
		// 
		// The number of universal schedules is about 2-5.
		// And the schedules often be holded on weekends/Wed.

		department[i].event_schedules_number = department[i].event_schedules_number%4+2;

		// Avoid free time collision in a day
		// Each day has no more than 2 schedule timezones.

		// timeZones[i][0]: the startTime of weekday i;
		// timeZones[i][1]: the endTime of weekday i.
		int timeZones[10][5];
		memset(timeZones, 0, sizeof(timeZones));

		// isWeekdayUsed[weekday] = true: the weekday has been used
		bool isWeekdayUsed[10];
		for (int j = 0; j < 7; j++) {
			isWeekdayUsed[j] = false;
		}

		// disable more than 2 schedule timezones in a day
		bool isWeekdayDisabled[10];
		for (int j = 0; j < 7; j++) {
			isWeekdayDisabled[j] = false;
		}

		// generate "event_schedules_number" event schedule strings
		for (int j = 0; j < department[i].event_schedules_number; j++) {
			// acquire free time string components
			int weekday, startTime, endTime;
			weekday = generateWeekday(); 

			// if the weekday already has two time zones, search for the next day
			if (isWeekdayDisabled[weekday]) {
				while (isWeekdayDisabled[weekday]) {
					weekday = (weekday+1)%7;
				}
			}

			// the day has been scheduled once
			if (isWeekdayUsed[weekday]) {
				int lastTimeStart = timeZones[weekday][0];
				int lastTimeEnd = timeZones[weekday][1];

				// if lastTimeStart-480(8:00) > 180(3h), set it as upperBond
				// to generate free time zone
				if (lastTimeStart-480 > 180) {
					// 480 <= startTime <= lastTimeStart-180
					// startTime = rand()%(n-m+1)+m, n=lastTimeStart-180, m=480
					startTime = rand()%(lastTimeStart-180-480+1)+480;
					endTime = generateEndTime(startTime);
				} else { // otherwise, choose another part of the day
					// lastTimeEnd <= startTime <= 1260
					// startTime = rand()%(n-m+1)+m, n=1260, m=lastTimeEnd
					startTime = rand()%(1260-lastTimeEnd+1)+lastTimeEnd;
					endTime = generateEndTime(startTime);
				}

				// mark the day as "disabled"
				isWeekdayDisabled[weekday] = true;
				
			} else { // the day has not been scheduled
				startTime = generateStartTime();
				endTime = generateEndTime(startTime);

				// mark the day as been scheduled
				isWeekdayUsed[weekday] = true;
				
				// record this time schedule in timeZones
				timeZones[weekday][0] = startTime;
				timeZones[weekday][1] = endTime;
			}

			department[i].event_schedules[j] = generateTimeString(weekday, startTime, endTime);
		}

		// The number limitation of department tags

		// random number of tags
		department[i].tag_number = rand()%4+2;

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

		seedBase++;
	}
}
