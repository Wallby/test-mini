// NOTE: compile with gcc -Wl,--wrap=malloc,--wrap=free

#ifndef TEST_MINI_H
#define TEST_MINI_H


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

int tm_test(int a, int(*b)(), int numRepetitions);
// NOTE: will run test_##b() multiple times to assure that it is repeatable
#define TM_TEST(a, numRepetitions) if(tm_test(a, &test_##a, numRepetitions) != 1) { return 1; }

#endif
