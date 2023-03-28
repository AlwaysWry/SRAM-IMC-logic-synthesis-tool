#ifndef TERM_CODE_SEED

#define TERM_CODE_SEED 3//the radix of term value, can be changed if necessary

#endif // !TERM_CODE_SEED

int iteratedCombine
	(long int max_number_of_literals, long int total_number_of_terms, const long int LUT_literals[], long int iter_flag);
int outputIterationMap
	(long int max_number_of_literals, long int total_number_of_terms, long int iter_flag);
int setUncombinedTerms
	(long int max_number_of_literals, long int iter_flag);