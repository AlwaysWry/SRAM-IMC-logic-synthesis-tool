#ifndef ADVERSE_CODE_SEED

#define ADVERSE_CODE_SEED 3

#endif // !ADVERSE_CODE_SEED
#ifndef INDEPENDENT_VALUE

#define INDEPENDENT_VALUE -1

#endif // !INDEPENDENT_VALUE

int outputLogicalResult
	(void);
long int getAdversedValue
	(long int max_number_of_literals, int iter_flag, int code_seed[], logical_cluster*);
long int getOriginalValue
	(long int max_number_of_literals, int iter_flag, int code_seed[], logical_cluster*);
long int getSlaveRedundancy
	(long int max_number_of_literals, long int red_upperbound);
long int getSpatialParameter
	(long int red_upperbound, long int* redundancy_lim, long int combination);
int removeBadMatching
	(long int* para_lim);
int getMasterRedundancy
	(long int red_upperbound);
int getRedundancy
	(long int max_number_of_literals, long int red_upperbound);
int expandMapCases
	();

