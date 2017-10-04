#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <jsoncpp/json/json.h>
#include "definition.h"
#include "inputJsonGenerator.h"
using namespace std; 

extern students student[310];
extern departments department[25];

/*
 *  Function Name: generateJson
 *	Used to output JSON to the assigned file
 */
bool inputJsonGenerator::generateJson(const char* jsonFile) {
	Json::Value root;

	// for each of the students
	for (int i = 0; i < 300; i++) {
		Json::Value student_instance;

		// link free time to student instance
		for (int j = 0; j < student[i].free_time_number; j++) {
			student_instance["free_time"].append(Json::Value(student[i].free_time[j]));
		}

		// student number
		student_instance["student_no"] = Json::Value(student[i].student_number);

		// link applications department to student instance
		for (int j = 0; j < student[i].applications_department_number; j++) {
			student_instance["applications_department"].append(Json::Value(student[i].applications_department[j]));
		}

		// link tags to student instance
		for (int j = 0; j < student[i].tag_number; j++) {
			student_instance["tags"].append(Json::Value(student[i].tags[j]));
		}

		// link the student instance to students
		root["students"].append(Json::Value(student_instance));
	}

	// for each of the departments
	for (int i = 0; i < 20; i++) {
		Json::Value department_instance;

		// link event schedules to department instance
		for (int j = 0; j < department[i].event_schedules_number; j++) {
			department_instance["event_schedules"].append(Json::Value(department[i].event_schedules[j]));
		}

		// department number
		department_instance["department_no"] = Json::Value(department[i].department_number);

		// member limitation
		department_instance["member_limit"] = Json::Value(department[i].member_limit);

		// link tags to department instance
		for (int j = 0; j < department[i].tag_number; j++) {
			department_instance["tags"].append(Json::Value(department[i].tags[j]));
		}

		// link the department instance to students
		root["departments"].append(Json::Value(department_instance));
	}

	// output to .json file
	Json::StyledWriter jsonWriter;

	fstream outputStream(jsonFile);
	outputStream << jsonWriter.write(root);
	outputStream.close();

	return true;
}
