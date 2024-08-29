#include "test_mini.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef __linux__ //< chromeos
struct info_about_memory_allocation_t
{
	void* a;
	int numBytesAllocated;
};
struct info_about_memory_allocation_t* infoPerMemoryAllocation = NULL;
int numMemoryAllocations = 0; //< really # tracked memory allocations (excluding memory allocations for infoPerMemoryAllocation)
size_t numBytesAllocated;

// NOTE: thanks to https://stackoverflow.com/questions/1208644/how-can-i-get-the-size-of-a-memory-block-allocated-using-malloc
void* __real_malloc(size_t a);
void __real_free(void* a);

void* __wrap_malloc(size_t a)
{
	void* b = __real_malloc(a);

	struct info_about_memory_allocation_t* c = infoPerMemoryAllocation;
	//infoPerMemoryAllocation = new info_about_memory_allocation_t[numMemoryAllocations + 1];
	infoPerMemoryAllocation = (struct info_about_memory_allocation_t*)__real_malloc(sizeof(struct info_about_memory_allocation_t) * (numMemoryAllocations + 1));

	if(c != NULL)
	{
		memcpy(infoPerMemoryAllocation, c, sizeof(struct info_about_memory_allocation_t) * numMemoryAllocations);
		__real_free(c);
	}

	if((b == NULL) | (infoPerMemoryAllocation == NULL))
	{
		fputs("error: out of memory\n", stdout); //< not sure if that is the only case __real_malloc would return NULL..?
		if(b == NULL)
		{
			__real_free(b);
		}
		if(infoPerMemoryAllocation == NULL)
		{
			__real_free(infoPerMemoryAllocation);
		}
		exit(1); //< i.e. continuing "like this" would not make a whole lot of sense..?
	}

	++numMemoryAllocations;
	infoPerMemoryAllocation[numMemoryAllocations - 1].a = b;
	infoPerMemoryAllocation[numMemoryAllocations - 1].numBytesAllocated = a;
	numBytesAllocated += a;

	return b;
}

void __wrap_free(void* a)
{
	int i;
	for(i = 0; i < numMemoryAllocations; ++i)
	{
		if(infoPerMemoryAllocation[i].a == a)
		{
			break;
		}
	}
	if(i == numMemoryAllocations)
	{
		fputs("warning: attempted to free memory that was never allocated\n", stdout);
		exit(1); //< i.e. continuing "like this" would not make a whole lot of sense..?
	}

	numBytesAllocated -= infoPerMemoryAllocation[i].numBytesAllocated;
	if((numMemoryAllocations - 1) > 0)
	{
		struct info_about_memory_allocation_t* b = infoPerMemoryAllocation;
		//infoPerMemoryAllocation = new info_about_memory_allocation_t[numMemoryAllocations - 1];
		infoPerMemoryAllocation = (struct info_about_memory_allocation_t*)__real_malloc(sizeof(struct info_about_memory_allocation_t) * (numMemoryAllocations - 1));

		int numElementsBefore = i;
		int numElementsAfter = numMemoryAllocations - numElementsBefore - 1;

		if(numElementsBefore > 0)
		{
			memcpy(infoPerMemoryAllocation, b, sizeof(struct info_about_memory_allocation_t) * numElementsBefore);
		}
		if(numElementsAfter > 0)
		{
			memcpy(infoPerMemoryAllocation + numElementsBefore, b + numElementsBefore + 1, sizeof(struct info_about_memory_allocation_t) * numElementsAfter);
		}

		__real_free(b);
	}
	else
	{
		__real_free(infoPerMemoryAllocation);
		infoPerMemoryAllocation = NULL;
	}
	--numMemoryAllocations;

	__real_free(a);
}
#endif

//*****************************************************************************

int tm_test(int a, int(*b)(), int numRepetitions)
{
	printf("test %i started\n", a);
	
	int c;
	int i;
	for(i = 0; i < 1 + numRepetitions; ++i)
	{
#ifdef __linux__ //< chromeos
		size_t numBytesAllocatedBeforeTest = numBytesAllocated;
#endif
		c = b();
		if(c != 1)
		{
			break;
		}
#ifdef __linux__ //< chromeos
		size_t numBytesAllocatedAfterTest = numBytesAllocated;
		size_t numBytesLeakedByTest = numBytesAllocatedAfterTest - numBytesAllocatedBeforeTest;
		if(numBytesAllocatedAfterTest != numBytesAllocatedBeforeTest)
		{
			fprintf(stderr, "error: test %i leaked %zi bytes of memory %i%s time\n", a, numBytesLeakedByTest, i + 1, TM_ORDINAL_NUMBER_SUFFIX(i+1));
			return 0;
		}
#endif
	}
	if(c != 1)
	{
		fprintf(stderr, "error: test %i failed %i%s time\n", a, i + 1, TM_ORDINAL_NUMBER_SUFFIX(i+1));
		return 0;
	}
	
	printf("test %i succeeded\n", a);
	
	return 1;
}

#ifdef __linux__ //< chromeos
int __real_main(int argc, char** argv);
//extern int tm_main();
int __wrap_main(int argc, char** argv)
{
	size_t numBytesAllocatedBefore = numBytesAllocated;
	
	int a = __real_main(argc, argv);
	if(a != 0)
	{
		return a;
	}
	
	size_t numBytesAllocatedAfter = numBytesAllocated;
	size_t numBytesLeaked = numBytesAllocatedAfter - numBytesAllocatedBefore;
	if(numBytesAllocatedAfter != numBytesAllocatedBefore)
	{
		printf("error: %zi bytes of memory leaked\n", numBytesLeaked);
	}

	fputs("all tests succeeded\n", stdout);

	return 0;
}
#endif
