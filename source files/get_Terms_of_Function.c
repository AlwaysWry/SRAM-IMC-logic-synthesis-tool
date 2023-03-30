 /*get the terms from file*/
#define _CRT_SECURE_NO_WARNINGS
#define _COMPARATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header files/get_Terms_of_Function.h"
#include "../header files/Global.h"


long int outputSumofVariables
    (char* term)
{
    long int term_var_count = 0;
    for (int i = 0; i < strlen(term); ++i) {
        if (term[i] == '0' || term[i] == '1'){
            term_var_count++;
        }
    }
    return term_var_count;
}

int getTermsofFunction
	(const char file_name[], long int stats_of_function[],
	int file_name_length, int number_of_stats)
{
	char ch;
	int count;
	long int location_in_file,file_length;
	long int order_of_terms = 0,split_length = 0;
	long int max_number_of_literals,total_number_of_terms;
    long int var_count = 0;
	char file[FILE_NAME_LENGTH];
	FILE* fp;
	/*at the beginning, max_number_of_literals is unknown.TEST_NUMBER_OF_LITERALS is a test capability value, 
	which can be changed in Global.h*/
	char get_term[TEST_NUMBER_OF_LITERALS],term[TEST_NUMBER_OF_LITERALS];

	//get the name of file
	strcpy(file,file_name);

	//check if the file exists
	if ((fp = fopen(file,"rb")) == NULL)
	{
		printf("Illegal input file!\n");
		fprintf(ofp, "Illegal input file!\n");
		fclose(ofp);
        system("exit");
		exit(EXIT_FAILURE);
	}

	//get the number of literals of each term in the function
	fscanf(fp,"%s",get_term);
	max_number_of_literals = strlen(get_term);

	if (max_number_of_literals == 0)
	{
		printf("the file is empty!\n");
		fprintf(ofp, "the file is empty!\n");
		fclose(ofp);
        system("exit");
		exit(EXIT_FAILURE);
	}

	printf("\nGetting terms from the file...\n");
	fprintf(ofp, "Getting terms from the file...\n");

	//get the length of spliter between terms, including \n
	while((ch = getc(fp)) != EOF)
	{
		if (ch == '\r')//consider difference in windows and linux
		{
			split_length += 2;
			break;
		}
		else if (ch == '\n') {
            split_length++;
            break;
        } else
            split_length++;
    }

	//get the length of the file
	fseek(fp,0L,SEEK_END);
	file_length = ftell(fp);
	rewind(fp);

	//get the number of terms, also the number of rows of the file
	total_number_of_terms = file_length / (max_number_of_literals + split_length) + 1;

	//dynamic allocation of memory space for function info, according to the scale
	process_info.func_info = (char**)malloc((total_number_of_terms + 1) * sizeof(char*));
	for ( count = 0; count < total_number_of_terms; count++)
	{
		process_info.func_info[count] = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
		/*initialize memory space.reset steps have to be executed repeatly, for the memory space is not consistent in memory.
		the initialization need to be done array by array*/
		memset(process_info.func_info[count], 0, (max_number_of_literals + 1) * sizeof(char));
	}
	count = 0;//reinitialize the counter

	//try to get the code of each term of the file
	for(location_in_file = 1L; location_in_file <= file_length;)
	{
		//get one term from the beginning, the term string should be seperated by " " from spliters
		fgets(term,max_number_of_literals + 1,fp);
		//copy the string to a 2-D array, one term takes a row
		strcpy(process_info.func_info[order_of_terms],term);

        //output the sum of number of variables
#ifdef _COMPARATION
        var_count += outputSumofVariables(term);
#endif
		//print terms
		printf("term%ld of the function is:\n",order_of_terms);
		fprintf(ofp, "term%ld of the function is:\n",order_of_terms);
		puts(process_info.func_info[order_of_terms]);
		fprintf(ofp, "%s\n", process_info.func_info[order_of_terms]);
		//skip the spliter between terms
		fseek(fp,split_length,SEEK_CUR);
		//move the cursor to current location
		location_in_file = location_in_file + max_number_of_literals + split_length;
		//term counter
		order_of_terms++;
	}
	//check the calculation 
	if (order_of_terms != total_number_of_terms)
	{
		total_number_of_terms = order_of_terms;
	}
	stats_of_function[0] = max_number_of_literals;
	stats_of_function[1] = total_number_of_terms;
	fclose(fp);
	printf("\nthe number of literals is %ld\nthe number of terms is %ld\n",max_number_of_literals,total_number_of_terms);
	fprintf(ofp, "\nthe number of literals is %ld\nthe number of terms is %ld\n",max_number_of_literals,total_number_of_terms);
#ifdef _COMPARATION
    printf("the sum number of variables is %ld\n", var_count);
	fprintf(ofp, "the sum number of variables is %ld\n", var_count);
#endif
	return 0;

}
