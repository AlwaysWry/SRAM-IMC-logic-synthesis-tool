#define _CRT_SECURE_NO_WARNINGS
#define INDEPENDENT_VALUE 0
#ifndef ITERATION_FINISH
#define ITERATION_FINISH 1
#endif // !ITERATION_FINISH


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../header files/logical_Combination.h"
#include "../header files/logical_iteration.h"
#include "../header files/Global.h"

struct LI* iteration_array;
struct LI logical_info;

/*basically identical with the original iteration, including encoding, sorting, and conditional writing.*/
int iteratedCombine
	(long int max_number_of_literals, long int total_number_of_terms, const long int LUT_literals[NUMBER_OF_LUT_LITERALS], long int iter_flag)
{
	int free_i, free_j;//counter for freeing memory space
	long int row_order, group_order, count, iter_count = 0;
	long int iter_term, term_order;
	long int number_of_literals, number_of_terms;//quantity information of groups
	long int number_in_comb = 1, number_of_combs = 0, opt_column = 0;//counter for combinations
	term_info** sorted_array;
	term_info** unsorted_array;
	char* term;
	int* term_code;
	double bit;

	struct LTI
	{
		long int LUT_literals[2];
		long int total_number_of_combs;
		long int opt_column;
		long int* comb_info;
		long int*** result_map;
		term_info** iteration_info;
	}logical_temp;//a local struct, get searching result under each set of LUT literals, compare with the global struct

	if (iter_flag == 1)
	{
		iteration_array[0] = logical_info;
	}

	/*allocation of arrays*/
	term = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
	memset(term, 0, (max_number_of_literals + 1) * sizeof(char));

	term_code = (int*)malloc((max_number_of_literals + 1) * sizeof(int));
	memset(term_code, 0, (max_number_of_literals + 1) * sizeof(int));

	/*allocate memory space*/
	sorted_array = (term_info**)malloc(max_number_of_literals * sizeof(term_info*));
	unsorted_array = (term_info**)malloc(max_number_of_literals * sizeof(term_info*));

	logical_temp.comb_info = (long int*)calloc(max_number_of_literals, sizeof(long int));
	logical_temp.result_map = (long int***)malloc(max_number_of_literals * sizeof(long int**));
	logical_temp.iteration_info = (term_info**)malloc(max_number_of_literals * sizeof(term_info*));
	logical_temp.total_number_of_combs = 0;
	logical_temp.opt_column = 0;

	for (row_order = 0; row_order < max_number_of_literals; row_order++)
	{
		iter_term = iteration_array[iter_flag - 1].iteration_info[row_order][0].order_of_term;
		number_of_literals = process_info.variables_info[row_order][0];
		number_of_terms = process_info.variables_info[row_order][1];

		//if (number_of_literals < 2 || number_of_terms < 2 || iter_term == 0)
        if (number_of_terms < 2 || iter_term == 0)
		{
			continue;
		}

        //row_order for logical_ is just used as a counter for result_map memory allocation, no meanings here
        logical_temp.result_map[row_order] = (long int**)malloc(iter_term * sizeof(long int*));
        //iteration_info, same as above
        logical_temp.iteration_info[row_order] = (term_info*)malloc((iter_term + 1) * sizeof(term_info));

        unsorted_array[row_order] = (term_info*)malloc(iter_term * sizeof(term_info));
        sorted_array[row_order] = (term_info*)malloc(iter_term * sizeof(term_info));

        memset(unsorted_array[row_order], 0, iter_term * sizeof(term_info));
        memset(sorted_array[row_order], 0, iter_term * sizeof(term_info));

		for (group_order = 1; group_order <= iter_term; group_order++)
		{
			term_order = iteration_array[iter_flag - 1].iteration_info[row_order][group_order].order_of_term;

			logical_temp.result_map[row_order][group_order - 1] = (long int*)malloc((iter_term + 1) * sizeof(long int));
			memset(logical_temp.result_map[row_order][group_order - 1], -1, (iter_term + 1) * sizeof(long int));

			unsorted_array[row_order][group_order - 1].number_of_literals = number_of_literals;
			unsorted_array[row_order][group_order - 1].order_of_term = term_order;

            if (max_number_of_literals > TERNARY_UPPERDIGIT)
            {
                unsorted_array[row_order][group_order - 1].code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
                sorted_array[row_order][group_order - 1].code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
            }

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
                        unsorted_array[row_order][group_order - 1].value_of_term += (long long int)pow(TERM_CODE_SEED, bit) * term_code[count];
                    } else
                        unsorted_array[row_order][group_order - 1].code[count] = (char)(term_code[count] + 48);
				}
				else
				{
                    if (max_number_of_literals <= TERNARY_UPPERDIGIT)
                    {
                        continue;
                    } else
                        unsorted_array[row_order][group_order - 1].code[count] = '0';
				}
			}
            sorted_array[row_order][group_order - 1].order_of_term = unsorted_array[row_order][group_order - 1].order_of_term;
            sorted_array[row_order][group_order - 1].value_of_term = unsorted_array[row_order][group_order - 1].value_of_term;
            sorted_array[row_order][group_order - 1].number_of_literals = unsorted_array[row_order][group_order - 1].number_of_literals;
            if (max_number_of_literals > TERNARY_UPPERDIGIT)
            {
                strcpy(sorted_array[row_order][group_order - 1].code, unsorted_array[row_order][group_order - 1].code);
            }
		}

		//memmove(sorted_array[row_order], unsorted_array[row_order], iter_term * sizeof(term_info));
		/*quick sort to the value element of these structs, make it easy to be searched*/
        if (max_number_of_literals <= TERNARY_UPPERDIGIT)
        {
            qsort(sorted_array[row_order], iter_term, sizeof(term_info), compareValueofTerms);
        } else
            qsort(sorted_array[row_order], iter_term, sizeof(term_info), compareStringofTerms);
		//qsort(sorted_array[row_order], iter_term, sizeof(term_info), compareValueofTerms);
		//memcpy(logical_info.iteration_info[row_order], sorted_array[row_order], iter_count * sizeof(term_info));
		//printf("Number of literals\tOrder of term\tValue of term\n");
		//printf("-----------------------------------------------------\n");
		//for (count = 0; count < iter_term; count++)
		//{
		//	printf("%18ld\t%13ld\t%13ld\n", number_of_literals, sorted_array[row_order][count].order_of_term, sorted_array[row_order][count].value_of_term);
		//}
		//printf("-----------------------------------------------------\n");
        int equal_flag;
		for (count = 1; count < iter_term; count++)
		{
            if (max_number_of_literals <= TERNARY_UPPERDIGIT)
            {
                equal_flag =
                        sorted_array[row_order][count].value_of_term == (sorted_array[row_order][count - 1].value_of_term) ? 1 : 0;
            } else
                equal_flag =
                        (strcmp(sorted_array[row_order][count].code, sorted_array[row_order][count - 1].code)) == 0 ? 1 : 0;

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
				if (count < iter_term - 1)
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
		logical_temp.comb_info[row_order] = number_of_combs;
		logical_temp.opt_column += opt_column;
		/*when there is only 1 term in uncombined queue, the process controls iter_count will not be triggered,
		so the original value iter_term should be copied.*/
		if (iter_term == 1)
		{
			logical_temp.iteration_info[row_order][0].order_of_term = iter_term;
			logical_temp.iteration_info[row_order][1] = sorted_array[row_order][0];
		}
		else
		{
			logical_temp.iteration_info[row_order][0].order_of_term = iter_count;
		}
		number_of_combs = 0;
		number_in_comb = 1;
		opt_column = 0;
		iter_count = 0;
	}

	/*copy result from local struct to global struct*/
	memmove(logical_temp.LUT_literals, LUT_literals, NUMBER_OF_LUT_LITERALS * sizeof(long int));

	invalid_LUT_set[logical_temp.LUT_literals[0]][logical_temp.LUT_literals[1]] = logical_temp.total_number_of_combs;
	
	if (logical_temp.opt_column > iteration_array[iter_flag].opt_column)
	{
		iteration_array[iter_flag].number_of_combinations = logical_temp.total_number_of_combs;
		iteration_array[iter_flag].opt_column = logical_temp.opt_column;
		memmove(iteration_array[iter_flag].LUT_literals, logical_temp.LUT_literals, sizeof(logical_temp.LUT_literals));
		memmove(iteration_array[iter_flag].comb_info, logical_temp.comb_info, max_number_of_literals * sizeof(long int));

		for (row_order = 0; row_order < max_number_of_literals; row_order++)
		{
			number_of_literals = process_info.variables_info[row_order][0];
			number_of_terms = process_info.variables_info[row_order][1];
            iter_term = iteration_array[iter_flag - 1].iteration_info[row_order][0].order_of_term;
			//if (number_of_literals < 2 || number_of_terms < 2 || iter_term == 0)
            if (number_of_terms < 2 || iter_term == 0)
            {
				continue;
			}
			else
			{
                iter_count = logical_temp.iteration_info[row_order][0].order_of_term;
				for (count = 0; count < logical_temp.comb_info[row_order]; count++)
				{
					memmove(iteration_array[iter_flag].result_map[row_order][count], logical_temp.result_map[row_order][count], (iter_term + 1) * sizeof(long int));
				}

                for (count = 0; count <= iter_count; count++)
                {
                    iteration_array[iter_flag].iteration_info[row_order][count].order_of_term = logical_temp.iteration_info[row_order][count].order_of_term;
                    iteration_array[iter_flag].iteration_info[row_order][count].number_of_literals = logical_temp.iteration_info[row_order][count].number_of_literals;
                    if (max_number_of_literals > TERNARY_UPPERDIGIT && count > 0)
                    {
                        strcpy(iteration_array[iter_flag].iteration_info[row_order][count].code, logical_temp.iteration_info[row_order][count].code);
                    } else
                        iteration_array[iter_flag].iteration_info[row_order][count].value_of_term = logical_temp.iteration_info[row_order][count].value_of_term;
                }
				//memmove(iteration_array[iter_flag].iteration_info[row_order], logical_temp.iteration_info[row_order], (iter_count + 1) * sizeof(term_info));
			}
		}
	}

	//printf("number of combinations: %d\n", logical_temp.total_number_of_combs);

	/*free memory space used in this function*/
    free(term);
    free(term_code);
	for (free_i = 0; free_i < max_number_of_literals; free_i++)
	{
		number_of_literals = process_info.variables_info[free_i][0];
		number_of_terms = process_info.variables_info[free_i][1];
        iter_term = iteration_array[iter_flag - 1].iteration_info[free_i][0].order_of_term;

		//if (number_of_literals < 2 || number_of_terms < 2 || iter_term == 0)
        if (number_of_terms < 2 || iter_term == 0)
		{
			continue;
		}
		for (free_j = 0; free_j < iter_term; free_j++)
		{
            free(logical_temp.result_map[free_i][free_j]);
            if (max_number_of_literals > TERNARY_UPPERDIGIT)
            {
                free(unsorted_array[free_i][free_j].code);
                free(sorted_array[free_i][free_j].code);
            }
		}
        free(unsorted_array[free_i]);
        free(sorted_array[free_i]);
		free(logical_temp.result_map[free_i]);
		free(logical_temp.iteration_info[free_i]);
	}
    free(unsorted_array);
    free(sorted_array);
    free(logical_temp.result_map);
    free(logical_temp.iteration_info);
	free(logical_temp.comb_info);

	return 1;
}

/*output results of each iteration*/
int outputIterationMap
	(long int max_number_of_literals, long int total_number_of_terms, long int iter_flag)
{
	int row_order, group_order, count = 0, flag = 0;
	long int number_of_literals, number_of_terms;
	long int iter_count;

	printf("------------------ITERATION RESULT-------------------\n");

	for (row_order = 0; row_order < max_number_of_literals; row_order++)
	{
		number_of_literals = process_info.variables_info[row_order][0];
		number_of_terms = process_info.variables_info[row_order][1];
        iter_count = iteration_array[iter_flag].iteration_info[row_order][0].order_of_term;
		//if (number_of_literals < 2 || number_of_terms < 2)
        if (number_of_terms < 2)
		{
			continue;
		}
		else
		{
			if (iteration_array[iter_flag].comb_info[row_order] >= 1)
			{
				printf("\nNumber of literals\tOrder of Combs\tOrder of term\n");
				printf("-----------------------------------------------------\n");
				for (group_order = 0; group_order < iteration_array[iter_flag].comb_info[row_order]; group_order++)
				{
					printf("%18ld\t%14ld\t{", number_of_literals, group_order);
					while (count < number_of_terms && iteration_array[iter_flag].result_map[row_order][group_order][count] != -1)
					{//if next element is -1, it means all valid elements have been printed
						printf("%ld", iteration_array[iter_flag].result_map[row_order][group_order][count]);
						count++;
						if (iteration_array[iter_flag].result_map[row_order][group_order][count] != -1)
						{
							printf(",");
						}
					}
					printf("}\n");
					count = 0;
				}
				printf("-----------------------------------------------------\n");
			}
		}

		if (iter_count != 0)
		{
			printf("unmerged terms with %ld literals:\n{", number_of_literals);
			for (count = 1; count <= iter_count; count++)
			{
				printf("%ld", iteration_array[iter_flag].iteration_info[row_order][count].order_of_term);
				if (count != iter_count)
				{
					printf(",");
				}
			}
			printf("}\n");
			flag = 1;
		}
		count = 0;
	}
	if (flag == 0)
	{
		printf("All terms have been logic merged!\n");
		return ITERATION_FINISH;
	}
	else
	{
		return 0;
	}
}

int setUncombinedTerms
	(long int max_number_of_literals, long int iter_flag)
{
	int iter_count, independent_count;
	long int row_order, group_order;
	long int number_of_literals, number_of_terms;

	/*write those terms cannot be combined in the last iteration into independent group, 
	meanwhile output those independent terms*/
	for (row_order = 0; row_order < max_number_of_literals; row_order++)
	{
		number_of_literals = process_info.variables_info[row_order][0];
		number_of_terms = process_info.variables_info[row_order][1];

		iter_count = iteration_array[iter_flag - 1].iteration_info[row_order][0].order_of_term;
		independent_count = process_info.independent_info[row_order][0].order_of_term;

		if (number_of_terms == 0)
		{
			continue;
		}
		else
		{
			printf("Independent terms with %ld literals:\n", number_of_literals);

			if (iter_count == 0 && independent_count == 0)
			{
				printf("NONE\n");
				continue;
			}
			printf("{");

			if (independent_count != 0)
			{
				for (group_order = 1; group_order <= independent_count; group_order++)
				{
					printf("%d", process_info.independent_info[row_order][group_order].order_of_term);
					if (group_order != independent_count)
					{
						printf(",");
					}
				}
			}
			if (iter_count != 0)
			{
				for (group_order = 1; group_order <= iter_count; group_order++)
				{
					process_info.independent_info[row_order][independent_count + group_order] = iteration_array[iter_flag - 1].iteration_info[row_order][group_order];
					printf("%d", process_info.independent_info[row_order][independent_count + group_order].order_of_term);
					if (group_order != iter_count)
					{
						printf(",");
					}
				}
			}

			printf("}\n");
			process_info.independent_info[row_order][0].order_of_term = independent_count + iter_count;
		}
	}
	return 0;
}
