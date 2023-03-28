#ifndef TEST_NUMBER_OF_TERMS
#define TEST_NUMBER_OF_TERMS 100000
#endif // !TEST_NUMBER_OF_TERMS

#ifndef TEST_NUMBER_OF_LITERALS
/*at the beginning, max_number_of_literals is unknown. TEST_NUMBER_OF_LITERALS is a test capability value,
which can be changed in Global.h*/
#define TEST_NUMBER_OF_LITERALS 1024
#endif // !TEST_NUMBER_OF_LITERALS
#define NUMBER_OF_STATS 3
#define NUMBER_OF_LUT_LITERALS 2
#define ITERATION_FINISH 1
#define MONTE_SEED 37
//if digit is less than 40, the ternary number is smaller than 2^64-1, or the code has to be stored in a string
#define TERNARY_UPPERDIGIT 40

//extern char func_info[TEST_NUMBER_OF_TERMS][TEST_NUMBER_OF_LITERALS];
//extern long int variables_info[TEST_NUMBER_OF_LITERALS][TEST_NUMBER_OF_TERMS];
//extern long int ternary_code[TEST_NUMBER_OF_LITERALS][TEST_NUMBER_OF_TERMS];

typedef struct TI
{
    long int number_of_literals;
	char* code;
	long long int value_of_term;
	long int order_of_term;
}term_info;

typedef struct LC
{
	long int number_of_iterations;
	long int number_of_literals;
	long int order_of_combs;
	//long long int value_of_cluster;//difference between TI and LC: value_of_term in LC is a __int64 variable.
	char* cluster_code;
	/*a pointer for finding the term order information in result_map, with coordinates above(iteration&literals&combs).
	for independent cluster, coordinates are(literals&combs_order), in this case, the pointer should be NULL*/
	long int* order_of_term;
	long int red_upperbound;
}logical_cluster;

typedef struct SI
{	
	long int valid_flag;//a flag for judging if this cluster group is valid(valid group will be used in spatial combination)
	long int redundancy[4];
	long int cluster_group[2];
}spatial_info;

typedef struct _Ternary_encode
{
    char* code;
    long long int digital_value;
} encode_info;

extern struct PI//a global struct contains information of the logic function needs to be accessed publicly
{
	char** func_info;//the original info matrix of logic function
	long int** variables_info;//info matrix of logic function after division
	encode_info** ternary_code;//info matrix of ternary codes of each term
	term_info** independent_info;//info matrix of terms cannot be combined in a certain iteration of combination
}process_info;

extern struct LI
{
	long int LUT_literals[2];//the optimal LUT literal set
	long int number_of_combinations;//number of combinations under this set
	long int opt_column;
	long int* comb_info;
	/*result_map is a 3-D array, 2-D array contains the order of terms queue according to the division of combinations,
	the highest dimension is for all groups of terms(different number of literals)*/
	long int*** result_map;
	term_info** iteration_info;
}logical_info;

extern struct LI* iteration_array;//an struct array, store result of each iteration in logical combination
long int** invalid_LUT_set;//a matrix contains the number of combination implemented under each LUT literal set, as ending condition of iteration

logical_cluster* adversed_array;
logical_cluster* original_array;

spatial_info* spatial_array;
spatial_info** spatial_triangle;
spatial_info** spatial_matrix;
long int original_area;