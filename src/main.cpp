#include <stdlib.h>
#include <iostream>
#include "definition.h"
#include "infoGenerator.h"
#include "inputJsonGenerator.h"
using namespace std;

extern students student[310];
extern departments department[25];

int main() {
	infoGenerator infoGenerator_instance;
	infoGenerator_instance.generateDepartmentInfo();
	infoGenerator_instance.generateStudentInfo();

	inputJsonGenerator generator_instance;
	generator_instance.generateJson("input_data.txt");
	
	return 0;
}
