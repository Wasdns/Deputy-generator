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
	infoGenerator_instance.generateStudentInfo();
	infoGenerator_instance.generateDepartmentInfo();

	inputJsonGenerator generator_instance;
	generator_instance.generateJson("test/test1.json");
	
	return 0;
}
