#include "test_mini.h"

#include <stdio.h>


int numTimesTest1WasCalled = 0;
int test_1()
{
	++numTimesTest1WasCalled;
	
	return 1;
}

int main(int argc, char** argv)
{
	TM_TEST(1, 9); //< 9 repetitions means ran 10 times
	
	if(numTimesTest1WasCalled != 10)
	{
		fprintf(stderr, "error: numTimesTest1WasCalled != 10 (numTimesTest1WasCalled == %i)\n", numTimesTest1WasCalled);
	}
	
	return 0;
}
