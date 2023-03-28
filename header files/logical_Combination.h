#define TERM_CODE_SEED 3//the radix of term value, can be changed if necessary

int getLUTliterals
	(long int max_number_of_literals, long int LUT_literals[]);
int getValueofTerms
	(long int max_number_of_literals, long int total_number_of_terms, const long int LUT_literals[]);
int getExternalSpace
	(long int max_number_of_literals, long int total_number_of_terms, long int iter_flag);
int sortValueofTerms
	(long int total_number_of_terms, long int max_number_of_literals, const long int LUT_literals[], long int iter_flag);
int compareValueofTerms
	(const void* sa, const void* sb);
int compareStringofTerms
    (const void* sa, const void* sb);
int outputResultMap
	(long int max_number_of_literals, long int total_number_of_terms, long int iter_flag);