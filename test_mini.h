#ifndef TEST_MINI_H
#define TEST_MINI_H

#define TEST_MINI_VERSION 0.1.1


// NOTE: link with "gcc -Wl,--wrap=malloc,--wrap=free,--wrap=main"

// NOTE: for 11/12/13.. th (exceptions)
//       for 1,21,31,etc... st
//       for 2,22,32,etc... nd
//       for 3,23,33,etc... rd
//       otherwise.. th
#define TM_ORDINAL_NUMBER_SUFFIX(a) \
	((a == 11) | (a == 12) | (a == 13) ? "th" : \
	(a%10) == 1 ? "st" : \
	(a%10) == 2 ? "nd" : \
	(a%10) == 3 ? "rd" : \
	"th")

// NOTE: will run b, which is labelled "test <a>", 1+numReptitions times
int tm_test(int a, int(*b)(), int numRepetitions);
#define tm_test2(a, b) tm_test(a, b, 0)
// NOTE: will run test_##a() 1+numRepetitions times
#define TM_TEST(a, numRepetitions) if(tm_test(a, &test_##a, numRepetitions) != 1) { return 1; }
#define TM_TEST2(a) TM_TEST(a, 0)

#endif
