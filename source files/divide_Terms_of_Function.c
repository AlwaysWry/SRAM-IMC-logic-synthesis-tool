/*get number of variables of each term, and divide them into different groups*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header files/divide_Terms_of_Function.h"
#include "../header files/Global.h"

/*get the matrix of the information of number of variables that terms contain. each row refers to a certain 
number of variables, the elements refers to the order of terms in the function*/
int getNumberofVariables
	(long int max_number_of_literals, long int total_number_of_terms, long int stats_of_function[])
{
	long int number_of_literals, actual_literals = 0;
	long int group_order = 1, terms_order = 0;
	int count;
	char* term;

	process_info.variables_info = (long int**)malloc((max_number_of_literals + 1) * sizeof(long int*));
	for ( count = 0; count < max_number_of_literals; count++)
	{
		/*need 2 more memory unit to store number_of_literals and group_order, so length of array is 
		number_of_terms + 3*/
		process_info.variables_info[count] = (long int*)malloc((total_number_of_terms + 3) * sizeof(long int));
		/*initialize memory space.reset steps have to be executed repeatly, for the memory space is not 
		consistent in memory, the initialization need to be done array by array*/
		memset(process_info.variables_info[count], 0, (total_number_of_terms + 3) * sizeof(long int));
	}
    //reinitialize the counter

	printf("\nDividing terms...\n");
    fprintf(ofp, "\nDividing terms...\n");

	term = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
	while (terms_order < total_number_of_terms)
	{
		strcpy(term, process_info.func_info[terms_order]);
		number_of_literals = max_number_of_literals;
		for (count = 0; count <= max_number_of_literals; count++)
		{
			if (term[count] == '-')
			//check if there's '-' in the term, which means the corresponding literal/variable is not exist
			{
				number_of_literals = number_of_literals - 1;
			}
		}
		if (number_of_literals > actual_literals)
		{
			actual_literals = number_of_literals;
		}
		//the first element of each row means the number of variables of this row refers to
        if (number_of_literals == 0)
        {
            printf("term%ld: %ld variables, %ld terms in this group now.\n", terms_order, number_of_literals, group_order);
            fprintf(ofp, "term%ld: %ld variables, %ld terms in this group now.\n", terms_order, number_of_literals, group_order);
            terms_order++;
            continue;
        }
		if (number_of_literals != process_info.variables_info[number_of_literals - 1][0])
		{
			process_info.variables_info[number_of_literals - 1][0] = number_of_literals;
			group_order = 1;
			process_info.variables_info[number_of_literals - 1][1] = group_order;
			printf("term%ld: %ld variables, %ld terms in this group now.\n", terms_order, number_of_literals, group_order);
            fprintf(ofp, "term%ld: %ld variables, %ld terms in this group now.\n", terms_order, number_of_literals, group_order);
		}
		else
		{
			//counter of array length
			group_order = process_info.variables_info[number_of_literals - 1][1];
			group_order++;
			process_info.variables_info[number_of_literals - 1][1] = group_order;
			printf("term%ld: %ld variables, %ld terms in this group now.\n", terms_order, number_of_literals, group_order);
            fprintf(ofp, "term%ld: %ld variables, %ld terms in this group now.\n", terms_order, number_of_literals, group_order);
		}
		//write a new order of term to the rear of this row(array)
		process_info.variables_info[number_of_literals - 1][group_order + 1] = terms_order;
 		terms_order++;
	}
	free(term);//free the memory space only used in this function
	stats_of_function[2] = actual_literals;//the actual max number of literals among these terms
	printf("\nSeparation finished. The primitive area of SOP-based scheme is %ld.\n", stats_of_function[2] * stats_of_function[1]);
    fprintf(ofp, "\nSeparation finished. The primitive area of SOP-based scheme is %ld.\n", stats_of_function[2] * stats_of_function[1]);

	return 0;
}

int separateTerms
    (long int max_number_of_literals, long int total_number_of_terms)
{
    long int group_order = 1,term_order;
    int count,number_of_literals = 1;

    process_info.ternary_code = (encode_info**)malloc((max_number_of_literals + 1) * sizeof(encode_info*));
    process_info.independent_info = (term_info**)malloc((max_number_of_literals + 1) * sizeof(term_info*));

    for (count = 0; count < max_number_of_literals; count++)
    {
        /*allocate and initialize memory space. reset steps have to be executed repeatedly, for the memory space is not
        consistent in memory, the initialization need to be done array by array*/
        process_info.ternary_code[count] = (encode_info*)malloc((total_number_of_terms + 1) * sizeof(encode_info));
        //if digit is less than 40, the ternary number is smaller than 2^64-1, or the code has to be stored in a string
        if (max_number_of_literals <= TERNARY_UPPERDIGIT)
        {
            for (int code_count = 0; code_count < total_number_of_terms; ++code_count)
            {
                process_info.ternary_code[count][code_count].digital_value = 0;
            }
        } else
            for (int code_count = 0; code_count < total_number_of_terms; ++code_count)
            {
                process_info.ternary_code[count][code_count].code = (char*) malloc((max_number_of_literals + 1) * sizeof(char));
                process_info.ternary_code[count][code_count].digital_value = 0;
            }

        process_info.independent_info[count] = (term_info*)malloc((total_number_of_terms + 1) * sizeof(term_info));
        memset(process_info.independent_info[count], 0, (total_number_of_terms + 1) * sizeof(term_info));
        if (max_number_of_literals <= TERNARY_UPPERDIGIT)
        {
            for (int code_count = 0; code_count < total_number_of_terms; ++code_count)
            {
                process_info.independent_info[count][code_count].value_of_term = 0;
            }
        } else
            for (int code_count = 0; code_count < total_number_of_terms; ++code_count)
            {
                process_info.independent_info[count][code_count].code = (char*) malloc((max_number_of_literals + 1) * sizeof(char));
                process_info.independent_info[count][code_count].value_of_term = 0;
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
        //else if (process_info.variables_info[number_of_literals - 1][0] < 2)
        //{
            /*in a simplified logic function, terms contain less than 3 literals must be an independent term*/
        //    while (group_order <= process_info.variables_info[number_of_literals - 1][1])
        //    {
        /*        term_order = process_info.variables_info[number_of_literals - 1][group_order + 1];
                process_info.independent_info[number_of_literals - 1][group_order].order_of_term = term_order;
                process_info.independent_info[number_of_literals - 1][group_order].number_of_literals = number_of_literals;
                group_order++;
            }
            process_info.independent_info[number_of_literals - 1][0].order_of_term = group_order - 1;//using the first element to save the amount
            group_order = 1;
            number_of_literals++;
            continue;
        }*/
        else if (process_info.variables_info[number_of_literals - 1][1] < 2)
        {
            /*if there's only 1 term in this group, it must be an independent term*/
            term_order = process_info.variables_info[number_of_literals - 1][group_order + 1];
            process_info.independent_info[number_of_literals - 1][group_order].order_of_term = term_order;
            process_info.independent_info[number_of_literals - 1][group_order].number_of_literals = number_of_literals;
            process_info.independent_info[number_of_literals -
                                          1][0].order_of_term = group_order;//using the first element to save the amount
            number_of_literals++;
            continue;
        }
        else
        {
            number_of_literals++;//to next row
            group_order = 1;//reinitialize when a certain row's encoding is finished
        }
    }
    return 0;
}