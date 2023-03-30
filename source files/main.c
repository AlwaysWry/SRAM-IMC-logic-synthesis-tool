#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#ifndef ITERATION_FINISH
#define ITERATION_FINISH 1
#endif // !ITERATION_FINISH
//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DBG_NEW
//#endif // !DBG_NEW
//#endif // _DEBUG


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <time.h>
//#include <crtdbg.h>
#include "../header files/Global.h"
#include "../header files/get_Terms_of_Function.h"
#include "../header files/divide_Terms_of_Function.h"
#include "../header files/logical_Combination.h"
#include "../header files/logical_iteration.h"
#include "../header files/spatial_Combination.h"
#include "../header files/optimize_Methods.h"

struct PI process_info;//definition of a global struct. see declarations in Global.h
struct LI logical_info;

/*inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}*/

int freeAllMemory
	(const long int stats_of_function[], long int iter_flag);
long int rootprocess
	(int mode_select);

int main
	(void)
{
    setbuf(stdout, NULL);
	int mode_select;

	//puts("*************************************");
	//puts("*                                   *");
	//puts("* MISS: A SRAM LOGIC SYNTHESIS TOOL *");
	//puts("*                                   *");
	//puts("*************************************");
	puts("Choose an item:");
	puts("0: Exit");
	puts("1: Synthesis process");

	input:
		scanf("%d", &mode_select);

	switch (mode_select)
	{
	case 0:
        break;
	case 1:
		rootprocess(1);
		break;
	default:
        puts("Please input the correct operation index!");
		break;
	}
	system("exit");
	exit(0);
	return 0;
}

long int rootprocess
	(int mode_select)
{
	//EnableMemLeakCheck();
	//file name
	char FILE_NAME[FILE_NAME_LENGTH];
	char OUTPUT_FILE[FILE_NAME_LENGTH];
	/*basic info of logic function. includes the max number of literals and number of terms*/
	long int stats_of_function[NUMBER_OF_STATS];
	//the order number of LUT literals in the literal table
    long int LUT_literals[NUMBER_OF_LUT_LITERALS];
	//a flag to check whether iterationCombine() was called. The function may not be called when all LUT sets are proved invalid or being used.
	int itcflag = 0;
	//mode select for spatial mergence
	int syn_mode = 0;
	//iteration flag for logical combination
    int iter_flag = -1, invalid_number = 0;
	long int red_upperbound = 0, final_area = 0;

	int original_seed[3] = { 1,2,0 };
	int adversed_seed[3] = { 2,1,0 };
	
	switch (mode_select)
	{
	case 1:
		//long int term_value[TEST_NUMBER_OF_TERMS];
		/*enter name of the file*/
		puts("Enter the function file name (with suffix) to be processed:");
		scanf("%1000s", FILE_NAME);
		puts("Enter the output file name:");
		scanf("%1000s", OUTPUT_FILE);
		//open output file
		if ((ofp = fopen(OUTPUT_FILE, "w")) == NULL)
		{
			printf("\nIllegal output file!\n");
			system("exit");
			exit(0);
			return 0;
		}
        //strcat(FILE_NAME, ".txt");
		break;
	default:
		break;
	}

	/*initialize the stats of function. stats of a function contain 2 elements: the maximum number of literals of all terms, and 
	the number of terms of the original form(read from file) of this function*/
	stats_of_function[0] = 0;//number of literals
	stats_of_function[1] = 0;//number of terms

	LUT_literals[0] = 0;
	LUT_literals[1] = 1;

	/*get the code of terms in the function*/
	getTermsofFunction(FILE_NAME, stats_of_function, FILE_NAME_LENGTH, NUMBER_OF_STATS);
	long int total_number_of_LUT_sets = (stats_of_function[0] * (stats_of_function[0] - 1)) / 2;
	/*get the matrix of the information of number of variables that terms contain. each row refers to a certain number of variables, 
	the elements refers to the order of terms in the function*/
	getNumberofVariables(stats_of_function[0], stats_of_function[1], stats_of_function);

	//system("pause");

	/*allocate memory space for logic_info*/
	getExternalSpace(stats_of_function[0], stats_of_function[1], iter_flag);
	logical_info.number_of_combinations = -1;
	logical_info.opt_column = -1;
	iter_flag = 0;

	printf("\n*****************************************************\n");
	fprintf(ofp, "\n*****************************************************\n");
	printf("Logic mergence...\n");
	fprintf(ofp, "Logic mergence...\n");

	//get the array of literals chosen to be LUT literals
    separateTerms(stats_of_function[0],stats_of_function[1]);
	do
	{
		//printf("\nLUT literals:%d and %d\n", LUT_literals[0], LUT_literals[1]);

		getValueofTerms(stats_of_function[0], stats_of_function[1], LUT_literals);

		sortValueofTerms(stats_of_function[0], stats_of_function[1], LUT_literals, iter_flag);

	} while (getLUTliterals(stats_of_function[0], LUT_literals) != ITERATION_FINISH);
	
	if (outputResultMap(stats_of_function[0], stats_of_function[1], iter_flag) != ITERATION_FINISH)
	{
		/*means there are some terms not combined yet, need to iterate*/
		do
		{
			iter_flag++;

			LUT_literals[0] = 0;
			LUT_literals[1] = 1;
			invalid_number = 0;

			printf("\n*****************************************************\n");
			fprintf(ofp, "\n*****************************************************\n");
			printf("\nBeginning ITERATION %d, invalid extracted literal set will be skipped.\n", iter_flag);
			fprintf(ofp, "\nBeginning ITERATION %d, invalid extracted literal set will be skipped.\n", iter_flag);

			iteration_array = (struct LI*)realloc(iteration_array, (iter_flag + 1) * sizeof(struct LI));

			if (iter_flag == 1)
			{
				getExternalSpace(stats_of_function[0], stats_of_function[1], iter_flag - 1);
			}

			getExternalSpace(stats_of_function[0], stats_of_function[1], iter_flag);
			iteration_array[iter_flag].number_of_combinations = -1;
			iteration_array[iter_flag].opt_column = -1;

			do
			{
				if (invalid_LUT_set[LUT_literals[0]][LUT_literals[1]] == 0)
				{
					invalid_number++;
					continue;
				}
				else if (iter_flag == 1 && memcmp(logical_info.LUT_literals, LUT_literals, NUMBER_OF_LUT_LITERALS * sizeof(long int)) == 0)
				{
					continue;
				}
				else if (iter_flag != 1 && memcmp(iteration_array[iter_flag - 1].LUT_literals, LUT_literals, NUMBER_OF_LUT_LITERALS * sizeof(long int)) == 0)
				{
					continue;
				}
				//printf("\nLUT literals:%d and %d\n", LUT_literals[0], LUT_literals[1]);

				itcflag = iteratedCombine(stats_of_function[0], stats_of_function[1], LUT_literals, iter_flag);

			} while (getLUTliterals(stats_of_function[0], LUT_literals) != ITERATION_FINISH);

			printf("\nSEARCH RESULT:\n");
			fprintf(ofp, "\nSEARCH RESULT:\n");
			if (iteration_array[iter_flag].number_of_combinations > 0)
			{
				printf("The optELS is: literal %ld and literal %ld\nMakes %ld combinations, reduced by %ld columns.\n", iteration_array[iter_flag].LUT_literals[0], iteration_array[iter_flag].LUT_literals[1], iteration_array[iter_flag].number_of_combinations, iteration_array[iter_flag].opt_column);
				fprintf(ofp, "The optELS is: literal %ld and literal %ld\nMakes %ld combinations, reduced by %ld columns.\n", iteration_array[iter_flag].LUT_literals[0], iteration_array[iter_flag].LUT_literals[1], iteration_array[iter_flag].number_of_combinations, iteration_array[iter_flag].opt_column);
				if (outputIterationMap(stats_of_function[0], stats_of_function[1], iter_flag) == ITERATION_FINISH)
				{
					break;
				}
			}
			else
			{
				printf("No terms can be combined.\n");
				fprintf(ofp, "No terms can be combined.\n");
			}

		} while (iteration_array[iter_flag].number_of_combinations > 0);

		if (itcflag != 1)
		{
			/*The function may not be called when all LUT sets are proved invalid or being used, while a valid combination
			has been generated from the original iteration. In this case, this branch(outputResultMap != ITERATION_FINISH)
			was chosen, while iteration 1 was completely not executed.*/
			iteration_array[0] = logical_info;
			setUncombinedTerms(stats_of_function[0], iter_flag);
		}
		else
		{
			setUncombinedTerms(stats_of_function[0], iter_flag + 1);
		}
	}
	else
	{
		/*all dependent terms have been combined, copy the information data to iteration_array[0]*/
		iteration_array = (struct LI*)realloc(iteration_array, (iter_flag + 1) * sizeof(struct LI));
		iteration_array[0] = logical_info;
		setUncombinedTerms(stats_of_function[0], iter_flag + 1);
	}
    printf("\nLogic Mergence has finished.\n");
	fprintf(ofp, "\nLogic Mergence has finished.\n");

	getAdversedValue(stats_of_function[0], iter_flag, adversed_seed, adversed_array);
	red_upperbound = getOriginalValue(stats_of_function[0], iter_flag, original_seed, original_array);
//TODO:fix the bug that a function have multiple terms with less than 2 literals is merged wrongly
    do
    {
        printf("*****************************************************\n");
        printf("Implementing spatial mergence...\n");
        printf("Input mode of synthesis method:\n");
        printf("0: quit\n");
        printf("1: area-first mode (default)\n");
        printf("2: row-first mode\n");
        printf("3: column-first mode\n");

        scanf("%d", &syn_mode);

        if (syn_mode != 0 && syn_mode != 2 && syn_mode != 3)
        {
            syn_mode = 1;
        }

        getRedundancy(stats_of_function[0], red_upperbound);
        blossomMethod(syn_mode, spatial_array[0].cluster_group[0], spatial_array[0].cluster_group[1], red_upperbound);
    } while (syn_mode != 0);

	fclose(ofp);
    freeAllMemory(stats_of_function, iter_flag);
    return final_area;

}

int freeAllMemory
	(const long int stats_of_function[], long int iter_flag)
{
	int free_i, free_j, free_k, free_l, array_count;
    long int conn_count;
	long int** iter_count;
	long int** comb_info;

	iter_count = (long int**)malloc(iter_flag * sizeof(long int*));
	comb_info = (long int**)malloc(iter_flag * sizeof(long int*));

	for (free_i = 0; free_i < iter_flag; free_i++)
	{
		iter_count[free_i] = (long int*)malloc(stats_of_function[0] * sizeof(long int));
		comb_info[free_i] = (long int*)malloc(stats_of_function[0] * sizeof(long int));
		for (free_j = 0; free_j < stats_of_function[0]; free_j++)
		{
			iter_count[free_i][free_j] = iteration_array[free_i].iteration_info[free_j][0].order_of_term;
			comb_info[free_i][free_j] = iteration_array[free_i].comb_info[free_j];
			comb_info[0][free_j] = logical_info.comb_info[free_j];
		}
	}

	array_count = (long int)original_array[0].order_of_term;
	conn_count = spatial_array[0].valid_flag;

	for (free_i = 0; free_i < stats_of_function[1]; free_i++)
	{
		free(process_info.func_info[free_i]);
	}
	free(process_info.func_info);
	for (free_i = 0; free_i < stats_of_function[0]; free_i++)
	{
		free(process_info.variables_info[free_i]);
		free(process_info.ternary_code[free_i]);
		free(process_info.independent_info[free_i]);
		/*if (stats_of_function[0] == 1)
		{
			free(invalid_LUT_set);
		}
		else
		{
			free(invalid_LUT_set[free_i]);
		}*/
	}
	for ( free_i = 0; free_i < iter_flag; free_i++)
	{
		free(iteration_array[free_i].comb_info);

		if (free_i > 0)
		{
			/*iteration_array[0] = logical_info means they share one pointer. When logical_info is freed,
			the other one will be freed simultaneously. Here we shouldn't free it again.*/
			for (free_j = 0; free_j < stats_of_function[0]; free_j++)
			{
				for (free_k = 0; free_k < comb_info[free_i][free_j]; free_k++)
				{
					free(iteration_array[free_i].result_map[free_j][free_k]);
				}
                for (free_k = 0; free_k < stats_of_function[1]; ++free_k)
                {
                    free(iteration_array[free_i].iteration_info[free_j][free_k].code);
                }
            	free(iteration_array[free_i].iteration_info[free_j]);
			}
		}
	}

	for (free_i = 0; free_i < array_count; free_i++)
	{
		free(original_array[free_i + 1].cluster_code);
		free(adversed_array[free_i + 1].cluster_code);
		free(spatial_triangle[free_i]);
	}

	for (free_i = 0; free_i < conn_count; free_i++)
	{
		free(spatial_matrix[free_i]);
	}

	for ( free_i = 0; free_i < iter_flag; free_i++)
	{
		free(iter_count[free_i]);
		free(comb_info[free_i]);
	}

	free(iter_count);
	free(comb_info);
	//free(logical_info.comb_info);
	//free(iteration_array);
	free(process_info.variables_info);
	free(process_info.ternary_code);
	free(process_info.independent_info);
	free(original_array);
	free(adversed_array);
	free(spatial_array);
	free(spatial_triangle);
	free(spatial_matrix);
	return 0;
}
