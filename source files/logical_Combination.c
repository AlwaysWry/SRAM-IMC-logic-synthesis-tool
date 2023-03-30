#define _CRT_SECURE_NO_WARNINGS
#define INDEPENDENT_VALUE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../header files/logical_Combination.h"
#include "../header files/Global.h"

int getLUTliterals
	(long int max_number_of_literals, long int LUT_literals[])
{
	if (LUT_literals[1] >= max_number_of_literals - 1)
		{
			LUT_literals[1] = 0;
			LUT_literals[0]++;
			do
			{
				LUT_literals[1]++;
			} while (LUT_literals[1] <= LUT_literals[0]);
			if (LUT_literals[1] > max_number_of_literals - 1)
			{
				return ITERATION_FINISH;
			}
		}
	else
	{
		LUT_literals[1]++;
	}
	return 0;
}

int getValueofTerms
	(long int max_number_of_literals, long int total_number_of_terms, const long int LUT_literals[])
{
	long int group_order = 1,term_order;
	int count,number_of_literals = 1;
	double bit;

	char* term;
	int* term_code;

	/*allocation of arrays*/
	term = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
	memset(term, 0, (max_number_of_literals + 1) * sizeof(char));

	term_code = (int*)malloc((max_number_of_literals + 1) * sizeof(int));
	memset(term_code, 0, (max_number_of_literals + 1) * sizeof(int));

    for (count = 0; count < max_number_of_literals; count++)
    {
        for (int code_count = 0; code_count < total_number_of_terms; ++code_count)
        {
            process_info.ternary_code[count][code_count].digital_value = 0;
        }
    }

    while (number_of_literals <= max_number_of_literals)//traverse all rows of variables_info
	{
		if (process_info.variables_info[number_of_literals - 1][0] == 0)
		{
			//if there's no terms in this group, skip it
			number_of_literals++;
			continue;
		}
		/*else if (process_info.variables_info[number_of_literals - 1][0] < 2)
		{
			number_of_literals++;
			continue;
		}*/
		else if (process_info.variables_info[number_of_literals - 1][1] < 2)
		{
			number_of_literals++;
			continue;
		}
		else
		{
			/*terms with different number of literals cannot be logically combined. because the max number 
			of LUT literals is 2, the absorptive theorem can be used in simplification, making them become 2
			terms with the same amount of literals, if there are identical remaining literals between these terms*/

			while (group_order <= process_info.variables_info[number_of_literals - 1][1])
			{
				term_order = process_info.variables_info[number_of_literals - 1][group_order + 1];
				strcpy(term, process_info.func_info[term_order]);

				for (count = 0; count < max_number_of_literals; count++)
				{ 
					if (count != LUT_literals[0] && count != LUT_literals[1])
					{
						switch (term[count])
						{
						case '0':
							term_code[count] = 1;
							break;
						case '1':
							term_code[count] = 2;
							break;
						case '-':
							term_code[count] = 0;
							break;
						default:
							break;
						}
						bit = count;
						/*calculate the value of each term. using ternary encoding system*/
                        if (max_number_of_literals <= TERNARY_UPPERDIGIT)
                        {
                            process_info.ternary_code[number_of_literals - 1][group_order - 1].digital_value += (long long int)pow(TERM_CODE_SEED, bit) * term_code[count];
                        } else
                            process_info.ternary_code[number_of_literals - 1][group_order - 1].code[count] = (char)(term_code[count] + 48);
					}
					else
					{
                        if (max_number_of_literals <= TERNARY_UPPERDIGIT)
                        {
                            continue;
                        } else
                            process_info.ternary_code[number_of_literals - 1][group_order - 1].code[count] = '0';
					}
				}
				group_order++;//to next term
			}
		}
		number_of_literals++;//to next row
		group_order = 1;//reinitialize when a certain row's encoding is finished
	}

	/*free the memory space only used in this function*/
	free(term);
	free(term_code);
	return 0;
}

int getExternalSpace(long int max_number_of_literals, long int total_number_of_terms, long int iter_flag)
{
	int row_order, combs_order;

	if (iter_flag >= 0)
	{
		iteration_array[iter_flag].comb_info = (long int*)calloc(max_number_of_literals, sizeof(long int));
		iteration_array[iter_flag].result_map = (long int***)malloc(max_number_of_literals * sizeof(long int**));
		iteration_array[iter_flag].iteration_info = (term_info**)malloc(max_number_of_literals * sizeof(term_info*));
		for (row_order = 0; row_order < max_number_of_literals; row_order++)
		{
            iteration_array[iter_flag].iteration_info[row_order] = (term_info*)malloc((total_number_of_terms + 1) * sizeof(term_info));
			iteration_array[iter_flag].result_map[row_order] = (long int**)malloc(total_number_of_terms * sizeof(long int*));
            memset(iteration_array[iter_flag].iteration_info[row_order], 0, total_number_of_terms * sizeof(term_info));
			for (combs_order = 0; combs_order <= total_number_of_terms; combs_order++)
			{
                //use combs_order as a counter here. iteration_info is not relative to combs_order.
                if (max_number_of_literals > TERNARY_UPPERDIGIT)
                {
                    iteration_array[iter_flag].iteration_info[row_order][combs_order].code = (char*) malloc((max_number_of_literals + 1) * sizeof (char));
                }
                if (combs_order < total_number_of_terms)
                {
                    iteration_array[iter_flag].result_map[row_order][combs_order] = (long int*)malloc((total_number_of_terms + 1) * sizeof(long int));
                    memset(iteration_array[iter_flag].result_map[row_order][combs_order], -1, (total_number_of_terms + 1) * sizeof(long int));
                }
			}
		}
		return 0;
	}

	if (iter_flag == -1)
	{
		logical_info.comb_info = (long int*)calloc(max_number_of_literals, sizeof(long int));
		logical_info.result_map = (long int***)malloc(max_number_of_literals * sizeof(long int**));
		logical_info.iteration_info = (term_info**)malloc(max_number_of_literals * sizeof(term_info*));
		invalid_LUT_set = (long int**)malloc(max_number_of_literals * sizeof(long int*));
		for (row_order = 0; row_order < max_number_of_literals; row_order++)
		{
			logical_info.result_map[row_order] = (long int**)malloc(total_number_of_terms * sizeof(long int*));
			logical_info.iteration_info[row_order] = (term_info*)malloc((total_number_of_terms + 1) * sizeof(term_info));
			memset(logical_info.iteration_info[row_order], 0, total_number_of_terms * sizeof(term_info));
			invalid_LUT_set[row_order] = (long int*)malloc(max_number_of_literals * sizeof(long int));
			memset(invalid_LUT_set[row_order], 0, max_number_of_literals * sizeof(long int));
			for (combs_order = 0; combs_order <= total_number_of_terms; combs_order++)
			{
                logical_info.iteration_info[row_order][combs_order].code = (char*) malloc((max_number_of_literals + 1) * sizeof(char));
                if (combs_order < total_number_of_terms)
                {
                    logical_info.result_map[row_order][combs_order] = (long int*)malloc(total_number_of_terms * sizeof(long int));
                    /*make -1 as the end sign of array*/
                    memset(logical_info.result_map[row_order][combs_order], -1, total_number_of_terms * sizeof(long int));
                }

			}
		}
		return 0;
	} else
        return -1;
}

int sortValueofTerms
	(long int max_number_of_literals, long int total_number_of_terms, 
	const long int LUT_literals[NUMBER_OF_LUT_LITERALS], long int iter_flag)
{
	long int count, iter_count = 0;//counters
	int free_i, free_j;//counter for freeing memory space
	long int number_of_literals, number_of_terms;//quantity information of groups
	long int number_in_comb = 1, number_of_combs = 0, opt_column = 0;//counter for combinations
	long int row_order, group_order;
	/*term_info is a struct contains literal quantity, order of term and the value calculated by getValueofTerms().*/
	term_info** unsorted_array;//original struct array
	term_info** sorted_array;//sorted struct array

	long int* comb_info;

	struct LLI
	{
		long int LUT_literals[2];
		long int total_number_of_combs;
		long int opt_column;
		/*a 3-D array, 2-D array contains the order of terms queue according to the division of combinations,
		the highest dimension is for all groups of terms(different number of literals)*/
		long int*** result_map;
		term_info** iteration_info;
	}logical_temp;//a local struct, get searching result under each set of LUT literals, compare with the global struct

	/*allocate memory space*/
	unsorted_array = (term_info**)malloc(max_number_of_literals * sizeof(term_info*));
	sorted_array = (term_info**)malloc(max_number_of_literals * sizeof(term_info*));

	logical_temp.result_map = (long int***)malloc(max_number_of_literals * sizeof(long int**));
	logical_temp.iteration_info = (term_info**)malloc(max_number_of_literals * sizeof(term_info*));
	logical_temp.total_number_of_combs = 0;
	logical_temp.opt_column = 0;

	comb_info = (long int*)calloc(max_number_of_literals, sizeof(long int));

	//printf("\nSorting Values...\nSingle term or terms have less than 3 literals will be ignored in sorting.\n");
	//printf("------------------RESULT OF SORTING------------------\n");

	for ( row_order = 0; row_order < max_number_of_literals; row_order++ )
	{
		number_of_literals = process_info.variables_info[row_order][0];
		number_of_terms = process_info.variables_info[row_order][1];
		//row_order for logical_ is just used as a counter for result_map memory allocation, no meanings here
		logical_temp.result_map[row_order] = (long int**)malloc(number_of_terms * sizeof(long int*));
		//iteration_info, same as above
		logical_temp.iteration_info[row_order] = (term_info*)malloc((number_of_terms + 1) * sizeof(term_info));

		//if (number_of_literals < 2 || number_of_terms < 2)
        if (number_of_terms < 2)
		{
			/*these terms have been decided as independent terms in getValueofTerms()*/
			continue;
		}
		else
		{
			/*dynamic memory allocation, according to the number of terms in this group*/
			unsorted_array[row_order] = (term_info*)malloc(number_of_terms * sizeof(term_info));
			sorted_array[row_order] = (term_info*)malloc(number_of_terms * sizeof(term_info));

			memset(unsorted_array[row_order], 0, number_of_terms * sizeof(term_info));
			memset(sorted_array[row_order], 0, number_of_terms * sizeof(term_info));

			for (group_order = 1; group_order <= number_of_terms; group_order++)
			{
				/*same as before, just using group_order as a counter, for allocating memory space. the memory space allocated for result_map have to be 1
				more than demanded number_of_terms, for an extra space will be needed when judging the end of the terms in combination(by recognizing the 
				-1 behind the last term in this combination)*/
				logical_temp.result_map[row_order][group_order - 1] = (long int*)malloc((number_of_terms + 1) * sizeof(long int));
				memset(logical_temp.result_map[row_order][group_order - 1], -1, (number_of_terms + 1) * sizeof(long int));

                /*write literal quantity into struct array*/
                unsorted_array[row_order][group_order - 1].number_of_literals = number_of_literals;
                unsorted_array[row_order][group_order - 1].order_of_term = process_info.variables_info[number_of_literals - 1][group_order + 1];

                if (max_number_of_literals > TERNARY_UPPERDIGIT)
                {
                    unsorted_array[row_order][group_order - 1].code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
                    sorted_array[row_order][group_order - 1].code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
                    strcpy(unsorted_array[row_order][group_order - 1].code, process_info.ternary_code[number_of_literals - 1][group_order - 1].code);
                } else
                    unsorted_array[row_order][group_order - 1].value_of_term = process_info.ternary_code[number_of_literals - 1][group_order - 1].digital_value;

                //sorted_array[row_order][count - 1] = unsorted_array[row_order][count - 1];
                sorted_array[row_order][group_order - 1].order_of_term = unsorted_array[row_order][group_order - 1].order_of_term;
                sorted_array[row_order][group_order - 1].value_of_term = unsorted_array[row_order][group_order - 1].value_of_term;
                sorted_array[row_order][group_order - 1].number_of_literals = unsorted_array[row_order][group_order - 1].number_of_literals;
                if (max_number_of_literals > TERNARY_UPPERDIGIT)
                {
                    strcpy(sorted_array[row_order][group_order - 1].code, unsorted_array[row_order][group_order - 1].code);
                }

				//if (process_info.ternary_code[number_of_literals - 1][group_order - 1] != 0)
				//{
					/*if the value in ternary_code not equals to 0, then the corresponding term is not an independent term temperarily.
					we can write order and value of this term into original struct array*/
					//unsorted_array[row_order][count - 1].order_of_term = process_info.variables_info[number_of_literals - 1][group_order + 1];
					//unsorted_array[row_order][count - 1].value_of_term = process_info.ternary_code[number_of_literals - 1][group_order - 1];
					/*copy from original array to need-to-be-sorted array*/
					//sorted_array[row_order][count - 1] = unsorted_array[row_order][count - 1];
				//}
				//else
				//{
					//continue;
				//}
				//count++;
			}

			/*quick sort to the value element of these structs, make it easy to be searched*/
            if (max_number_of_literals <= TERNARY_UPPERDIGIT)
            {
                qsort(sorted_array[row_order], number_of_terms, sizeof(term_info), compareValueofTerms);
            } else
                qsort(sorted_array[row_order], number_of_terms, sizeof(term_info), compareStringofTerms);

			//printf("Terms have %d literals:\n", number_of_literals);
			//printf("Number of literals\tOrder of term\tValue of term\n");
			//printf("-----------------------------------------------------\n");
			//for (count = 0; count < number_of_terms; count++)
			//{
			//    printf("%18ld\t%13ld\t%13lld\n", number_of_literals, sorted_array[row_order][count].order_of_term, sorted_array[row_order][count].value_of_term);
			//}
			//printf("-----------------------------------------------------\n");

            int equal_flag;
			for (count = 1; count < number_of_terms; count++)
			{
                if (max_number_of_literals <= TERNARY_UPPERDIGIT)
                {
                    equal_flag =
                            sorted_array[row_order][count].value_of_term == sorted_array[row_order][count - 1].value_of_term ? 1 : 0;
                } else {
                    equal_flag =
                            strcmp(sorted_array[row_order][count].code, sorted_array[row_order][count - 1].code) == 0 ? 1 : 0;
                }
				if (equal_flag)
				{
					if (number_in_comb == 1)
					{
						number_of_combs++;
						logical_temp.result_map[row_order][number_of_combs - 1][number_in_comb - 1] = sorted_array[row_order][count - 1].order_of_term;
					}
					/*compare with the previous value, if equal, the combination is judged as valid*/
					number_in_comb++;//the number of terms in this combination increase by 1 in this condition
					opt_column += number_in_comb - 1;
					/*write the order into the result map, same row of its previous term, means in same combination*/
					logical_temp.result_map[row_order][number_of_combs - 1][number_in_comb - 1] = sorted_array[row_order][count].order_of_term;
				}
				else
				{
					/*cases that the value is different from previous one, the term will be allocated in iteration group, means it cannot
					be combined under this LUT literal set*/
					number_in_comb = 1;
					if (count == 1)
					{
						iter_count++;
						logical_temp.iteration_info[row_order][iter_count] = sorted_array[row_order][count - 1];
					}
					if (count < number_of_terms - 1)
					{
                        if (max_number_of_literals <= TERNARY_UPPERDIGIT)
                        {
                            if (sorted_array[row_order][count].value_of_term != sorted_array[row_order][count + 1].value_of_term)
                            {
                                iter_count++;
                                logical_temp.iteration_info[row_order][iter_count] = sorted_array[row_order][count];
                            }
                        } else {
                            if (strcmp(sorted_array[row_order][count].code, sorted_array[row_order][count + 1].code) != 0)
                            {
                                iter_count++;
                                logical_temp.iteration_info[row_order][iter_count] = sorted_array[row_order][count];
                            }
                        }
					}
					else
					{
						iter_count++;
						logical_temp.iteration_info[row_order][iter_count] = sorted_array[row_order][count];
					}
				}
			}

			logical_temp.total_number_of_combs += number_of_combs;//calculate the total number of valid combinations
			logical_temp.opt_column += opt_column;
			comb_info[row_order] = number_of_combs;
			number_of_combs = 0;
			number_in_comb = 1;
			opt_column = 0;
		}
		logical_temp.iteration_info[row_order][0].order_of_term = iter_count;
		iter_count = 0;
	}

	/*copy result from local struct to global struct*/
	memmove(logical_temp.LUT_literals, LUT_literals, NUMBER_OF_LUT_LITERALS * sizeof(long int));

	/*store the number of combinations of this set*/
	invalid_LUT_set[logical_temp.LUT_literals[0]][logical_temp.LUT_literals[1]] = logical_temp.total_number_of_combs;

	if (logical_temp.opt_column > logical_info.opt_column)
	{
		logical_info.number_of_combinations = logical_temp.total_number_of_combs;
		logical_info.opt_column = logical_temp.opt_column;
		memmove(logical_info.LUT_literals, logical_temp.LUT_literals, sizeof(logical_temp.LUT_literals));
		memmove(logical_info.comb_info, comb_info, max_number_of_literals * sizeof(long int));

		for (row_order = 0; row_order < max_number_of_literals; row_order++)
		{
			number_of_literals = process_info.variables_info[row_order][0];
			number_of_terms = process_info.variables_info[row_order][1];
			iter_count = logical_temp.iteration_info[row_order][0].order_of_term;

			//if (number_of_literals < 2 || number_of_terms < 2)
            if (number_of_terms < 2)
			{
				continue;
			}
			else
			{
				for ( count = 0; count < comb_info[row_order]; count++)
				{
					memmove(logical_info.result_map[row_order][count], logical_temp.result_map[row_order][count], (number_of_terms + 1) * sizeof(long int));
				}
                //logical_info.iteration_info[row_order][0] = logical_temp.iteration_info[row_order][0];
                for (count = 0; count <= iter_count; count++)
                {
                    logical_info.iteration_info[row_order][count].order_of_term = logical_temp.iteration_info[row_order][count].order_of_term;
                    logical_info.iteration_info[row_order][count].number_of_literals = logical_temp.iteration_info[row_order][count].number_of_literals;
                    if (max_number_of_literals > TERNARY_UPPERDIGIT && count > 0)
                    {
                        strcpy(logical_info.iteration_info[row_order][count].code, logical_temp.iteration_info[row_order][count].code);
                    } else
                        logical_info.iteration_info[row_order][count].value_of_term = logical_temp.iteration_info[row_order][count].value_of_term;
                }
				//memmove(logical_info.iteration_info[row_order], logical_temp.iteration_info[row_order], (iter_count + 1) * sizeof(term_info));
			}
		}
	}

	//printf("number of combinations: %d\n", logical_temp.total_number_of_combs);

	/*free memory space used in this function*/
	free(comb_info);
	for (free_i = 0; free_i < max_number_of_literals; free_i++)
	{

			if (process_info.variables_info[free_i][0] > 2 && process_info.variables_info[free_i][1] >= 2)
			{
                for (free_j = 0; free_j < process_info.variables_info[free_i][1]; free_j++) {
                    free(logical_temp.result_map[free_i][free_j]);
                    if (max_number_of_literals > TERNARY_UPPERDIGIT)
                    {
                        free(unsorted_array[free_i][free_j].code);
                        free(sorted_array[free_i][free_j].code);
                    }
                }
                free(unsorted_array[free_i]);
                free(sorted_array[free_i]);
			}
		free(logical_temp.result_map[free_i]);
		free(logical_temp.iteration_info[free_i]);
	}
    free(unsorted_array);
    free(sorted_array);
    free(logical_temp.result_map);
    free(logical_temp.iteration_info);
	return 0;
}

/*the comparison function for qsort()*/
int compareValueofTerms
	(const void* sa, const void* sb)
{
	term_info* SA = (term_info*)sa;
	term_info* SB = (term_info*)sb;
    if (SA->value_of_term != SB->value_of_term)
    {
        return ((SA->value_of_term) > (SB->value_of_term)) ? 1 : -1;
    }
    else
    {
        return 0;
    }

}

int compareStringofTerms
        (const void* sa, const void* sb)
{
    term_info* SA = (term_info*)sa;
    term_info* SB = (term_info*)sb;
    return strcmp(SA->code, SB->code);
}

/*output results of first iteration*/
int outputResultMap(long int max_number_of_literals, long int total_number_of_terms, long int iter_flag)

{
	int row_order, count = 0, flag = 0;
	long int number_of_literals, number_of_terms;
	long int group_order, iter_count;

	printf("\nSEARCH RESULT:\n");
	fprintf(ofp, "\nSEARCH RESULT:\n");

	if (logical_info.number_of_combinations <= 0)
	{
		/*if there's no combination in the original iteration, or all combinations have been found in the
		original iteration, then it's unnecessary to continue*/
		printf("No terms can be merged.\n");
		fprintf(ofp, "No terms can be merged.\n");
		return ITERATION_FINISH;
	}

	printf("The optELS is: literal %ld and literal %ld\nMakes %ld combinations, reduced by %ld columns.\n", logical_info.LUT_literals[0], logical_info.LUT_literals[1], logical_info.number_of_combinations, logical_info.opt_column);
	fprintf(ofp, "The optELS is: literal %ld and literal %ld\nMakes %ld combinations, reduced by %ld columns.\n", logical_info.LUT_literals[0], logical_info.LUT_literals[1], logical_info.number_of_combinations, logical_info.opt_column);
	
	printf("------------------ITERATION RESULT-------------------\n");
	fprintf(ofp, "------------------ITERATION RESULT-------------------\n");

	for (row_order = 0; row_order < max_number_of_literals; row_order++)
	{
		number_of_literals = process_info.variables_info[row_order][0];
		number_of_terms = process_info.variables_info[row_order][1];

		//if (number_of_literals < 2 || number_of_terms < 2)
        if (number_of_terms < 2)
		{
			continue;
		}
		else
		{
			if (logical_info.comb_info[row_order] >= 1)
			{
				printf("\nNumber of literals\tOrder of Combs\tOrder of term\n");
				fprintf(ofp, "\nNumber of literals\tOrder of Combs\tOrder of term\n");
				printf("-----------------------------------------------------\n");
				fprintf(ofp, "-----------------------------------------------------\n");
				for (group_order = 0; group_order < logical_info.comb_info[row_order]; group_order++)
				{
					printf("%18ld\t%14ld\t{", number_of_literals, group_order);
					fprintf(ofp, "%18ld\t%14ld\t{", number_of_literals, group_order);
					while (count < number_of_terms && logical_info.result_map[row_order][group_order][count] != -1)
					{
						printf("%ld", logical_info.result_map[row_order][group_order][count]);
						fprintf(ofp, "%ld", logical_info.result_map[row_order][group_order][count]);
						count++;
						if (logical_info.result_map[row_order][group_order][count] != -1)
						{
							printf(",");
							fprintf(ofp, ",");
						}
					}
					printf("}\n");
					fprintf(ofp, "}\n");
					count = 0;
				}
				printf("-----------------------------------------------------\n");
				fprintf(ofp, "-----------------------------------------------------\n");
			}
		}
		iter_count = logical_info.iteration_info[row_order][0].order_of_term;
		if (iter_count != 0)
		{
			printf("unmerged terms with %ld literals:\n{", number_of_literals);
			fprintf(ofp, "unmerged terms with %ld literals:\n{", number_of_literals);
			for (count = 1; count <= iter_count; count++)
			{
				printf("%ld", logical_info.iteration_info[row_order][count].order_of_term);
				fprintf(ofp, "%ld", logical_info.iteration_info[row_order][count].order_of_term);
				if (count != iter_count)
				{
					printf(",");
					fprintf(ofp, ",");
				}
			}
			printf("}\n");
			fprintf(ofp, "}\n");
			flag = 1;
		}
		count = 0;
	}
	if (flag == 0)
	{
		/*it means there's no terms(except independent terms) left in iteration group*/
		printf("All terms have been logic merged!\n");
		fprintf(ofp, "All terms have been logic merged!\n");
		return ITERATION_FINISH;
	}
	else
	{
		return 0;
	}
}