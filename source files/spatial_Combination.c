//#define _CRT_SECURE_NO_WARNINGS
/*open this when need to check the redundancy list and matrix*/
//#define PRINTREDLIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../header files/Global.h"
#include "../header files/spatial_Combination.h"
#include "../header files/optimize_Methods.h"

logical_cluster* adversed_array;
logical_cluster* original_array;
spatial_info** spatial_triangle;
spatial_info* spatial_array;
long int original_area;

long int getAdversedValue
	(long int max_number_of_literals, int iter_flag, int code_seed[], logical_cluster* value_array)
{
	logical_cluster* temp_array;

	int iter_count, comb_count, independent_count;
    long int count;
	long int array_count = 0;
	long int LUT_literals[2];
	char* term_code;
	char* term;
	long int row_order, term_order, group_order;
	long int number_of_literals, number_of_terms;
	long int red_upperbound1 = 0, red_upperbound2 = 0, red_upperbound = 0;

	/*allocation of local arrays*/
	term = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
	memset(term, 0, (max_number_of_literals + 1) * sizeof(char));

	term_code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
	memset(term_code, 0, (max_number_of_literals + 1) * sizeof(char));

	temp_array = (logical_cluster*)malloc(sizeof(logical_cluster));

	for (iter_count = 0; iter_count <= iter_flag; iter_count++)
	{
		/*situations for logical clusters*/
		memmove(LUT_literals, iteration_array[iter_count].LUT_literals, 2 * sizeof(long int));

		for ( row_order = 0; row_order < max_number_of_literals; row_order++)
		{
			number_of_literals = process_info.variables_info[row_order][0];
			number_of_terms = process_info.variables_info[row_order][1];

			if (number_of_literals < 2 || number_of_terms < 2)
			{
				continue;
			}
			else
			{
				comb_count = iteration_array[iter_count].comb_info[row_order];
				for (group_order = 0; group_order < comb_count; group_order++)
				{
					term_order = iteration_array[iter_count].result_map[row_order][group_order][0];
					strcpy(term, process_info.func_info[term_order]);
					strcpy(term_code, term);
					for (count = 0; count < max_number_of_literals; count++)
					{
						if (count != LUT_literals[0] && count != LUT_literals[1])
						{
							switch (term[count])
							{
							case '0':
								term_code[count] = (char)(code_seed[0] + (int)'0');
								red_upperbound++;
								break;
							case '1':
								term_code[count] = (char)(code_seed[1] + (int)'0');
								red_upperbound++;
								break;
							case '-':
								term_code[count] = (char)(code_seed[2] + (int)'0');
								break;
							default:
								break;
							}
							//bit = count;
							/*calculate the value of each term. using decimal encoding system*/
							//term_value += pow(ADVERSE_CODE_SEED, bit) * term_code[count];
						}
						else
						{
							term_code[count] = (char)(code_seed[2] + (int)'0');
							continue;
						}
					}
					array_count++;
					temp_array = (logical_cluster*)realloc(temp_array, (array_count + 1) * sizeof(logical_cluster));
					temp_array[array_count].number_of_literals = number_of_literals;
					temp_array[array_count].number_of_iterations = iter_count;
					temp_array[array_count].order_of_combs = group_order;
					temp_array[array_count].order_of_term = iteration_array[iter_count].result_map[number_of_literals - 1][group_order];
					
					temp_array[array_count].cluster_code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
					strcpy(temp_array[array_count].cluster_code, term_code);
					memset(term_code, '0', max_number_of_literals * sizeof(char));
					temp_array[array_count].red_upperbound = red_upperbound;

					if (red_upperbound1 < red_upperbound)
					{
						red_upperbound1 = red_upperbound;
					}
					red_upperbound = 0;
				}
			}
		}
	}

	red_upperbound = 0;

	for (row_order = 0; row_order < max_number_of_literals; row_order++)
	{
		/*situations when the term is independent*/
		number_of_literals = process_info.variables_info[row_order][0];
		number_of_terms = process_info.variables_info[row_order][1];
		independent_count = process_info.independent_info[row_order][0].order_of_term;

		if (independent_count == 0)
		{
			continue;
		}
		else
		{
			for (group_order = 1; group_order <= independent_count; group_order++)
			{
				term_order = process_info.independent_info[row_order][group_order].order_of_term;
				strcpy(term, process_info.func_info[term_order]);

				/*no LUT literals*/
				for (count = 0; count < max_number_of_literals; count++)
				{
					//for independent terms, all existing literals are remaining literals
					switch (term[count])
					{
					case '0':
						term_code[count] = (char)(code_seed[0] + (int)'0');
						red_upperbound++;
						break;
					case '1':
						term_code[count] = (char)(code_seed[1] + (int)'0');
						red_upperbound++;
						break;
					case '-':
						term_code[count] = (char)(code_seed[2] + (int)'0');
						break;
					default:
						break;
					}	
					//bit = count;
					/*calculate the value of each term. using decimal encoding system*/
					//term_value += pow(ADVERSE_CODE_SEED, bit) * term_code[count];
				}
				array_count++;
				temp_array = (logical_cluster*)realloc(temp_array, (array_count + 1) * sizeof(logical_cluster));
				temp_array[array_count].number_of_literals = number_of_literals;
				temp_array[array_count].number_of_iterations = INDEPENDENT_VALUE;
				temp_array[array_count].order_of_combs = group_order;
				temp_array[array_count].order_of_term = NULL;
				
				temp_array[array_count].cluster_code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
				memset(temp_array[array_count].cluster_code, 0, (max_number_of_literals + 1) * sizeof(char));
				strcpy(temp_array[array_count].cluster_code, term_code);
				memset(term_code, 0, max_number_of_literals * sizeof(char));
				temp_array[array_count].red_upperbound = red_upperbound;

				if (red_upperbound2 < red_upperbound)
				{
					red_upperbound2 = red_upperbound;
				}
				red_upperbound = 0;
			}
		}
	}
	temp_array[0].order_of_term = (long int*)array_count;

	red_upperbound2 -= 2;//for independent terms, 2 literals can be put in LUT unit, so the upperbound of redundancy should be reduce by 2.
	red_upperbound = (long int)fmaxl(red_upperbound1, red_upperbound2);

	//printf("\n Based on the reslut of logical combination, there are %d clusters(including combination set of terms and independent terms)\n to be spatial combined.\n", array_count);
	//printf("the orders will be presented represent the order of clusters.\n");
	/*for ( count = 1; count <= array_count; count++)
	{
		printf("%lld\n", temp_array[count].value_of_cluster);
	}
	printf("\n");*/

	/*allocation for the cluster_code in global arrays*/
	if (value_array == adversed_array)
	{
		adversed_array = (logical_cluster*)malloc((array_count + 1) * sizeof(logical_cluster));
		memmove(adversed_array, temp_array, (array_count + 1) * sizeof(logical_cluster));
		for ( count = 1; count <= array_count; count++)
		{
			adversed_array[count].cluster_code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
			strcpy(adversed_array[count].cluster_code, temp_array[count].cluster_code);
			free(temp_array[count].cluster_code);
		}
	}
	else if (value_array == original_array)
	{
		original_array = (logical_cluster*)malloc((array_count + 1) * sizeof(logical_cluster));
		memmove(original_array, temp_array, (array_count + 1) * sizeof(logical_cluster));
		for (count = 1; count <= array_count; count++)
		{
			original_array[count].cluster_code = (char*)malloc((max_number_of_literals + 1) * sizeof(char));
			strcpy(original_array[count].cluster_code, temp_array[count].cluster_code);
			free(temp_array[count].cluster_code);
		}
		outputLogicalResult();
	}
	free(term);
	free(term_code);
	free(temp_array);
	//_CrtDumpMemoryLeaks();
	return red_upperbound;
}

int outputLogicalResult
	(void)
{
	long int count, array_count, comb_count = 0;
	long int number_of_literals, order_of_combs, number_of_iterations;
	long int* order_of_term;
	array_count = (long int)original_array[0].order_of_term;
	printf("\nSEARCH RESULT:\n");
	fprintf(ofp, "\nSEARCH RESULT:\n");
	printf("Order\tCluster\tLiteral set\n");
	fprintf(ofp, "Order\tCluster\tLiteral set\n");
	printf("---------------------------\n");
	fprintf(ofp, "---------------------------\n");

	for (count = 1; count <= array_count; count++)
	{
		comb_count = 0;
		number_of_iterations = original_array[count].number_of_iterations;
		number_of_literals = original_array[count].number_of_literals;
		order_of_combs = original_array[count].order_of_combs;
		order_of_term = original_array[count].order_of_term;

		printf("%5ld\t", count);
		fprintf(ofp, "%5ld\t", count);
		if (original_array[count].order_of_term != NULL)
		{
			printf("{");
			fprintf(ofp, "{");
			while (original_array[count].order_of_term[comb_count + 1] != -1)
			{
				printf("%ld,", original_array[count].order_of_term[comb_count]);
				fprintf(ofp, "%ld,", original_array[count].order_of_term[comb_count]);
				comb_count++;
			}
			printf("%ld}\t", original_array[count].order_of_term[comb_count]);
			fprintf(ofp, "%ld}\t", original_array[count].order_of_term[comb_count]);
			printf("(%ld,%ld)\n", iteration_array[number_of_iterations].LUT_literals[0], iteration_array[number_of_iterations].LUT_literals[1]);
			fprintf(ofp, "(%ld,%ld)\n", iteration_array[number_of_iterations].LUT_literals[0], iteration_array[number_of_iterations].LUT_literals[1]);
		}
		else
		{
			printf("{%ld}\t", process_info.independent_info[number_of_literals - 1][order_of_combs].order_of_term);
			fprintf(ofp, "{%ld}\t", process_info.independent_info[number_of_literals - 1][order_of_combs].order_of_term);
			printf("-\n");
			fprintf(ofp, "-\n");
		}

	}
	printf("---------------------------\n");
	fprintf(ofp, "---------------------------\n");
	return 0;
}

long int getOriginalValue
	(long int max_number_of_literals, int iter_flag, int code_seed[], logical_cluster* value_array)
{
	long int red_upperbound;
	red_upperbound = getAdversedValue(max_number_of_literals, iter_flag, code_seed, original_array);
	return red_upperbound;
}

long int getSlaveRedundancy
	(long int max_number_of_literals, long int red_upperbound)
{
	long int count = 0, array_count = 0, connection_count = 0, traverse_count = 0;
	long int red_count = 0, red_max = 0, red_min = red_upperbound;
	long int com_count = 0, com_min = red_upperbound, com_max = 0;
	int bit_original, bit_adverse;
	long long int difference;
	spatial_info spatial_temp;

	//the array for storing the super long integer of value_of_cluster
	char* original_code;
	char* adversed_code;
	long int code_length = 0;

	array_count = (long int)original_array[0].order_of_term;
	spatial_triangle = (spatial_info**)malloc(array_count * sizeof(spatial_info*));
	spatial_array = (spatial_info*)malloc(sizeof(spatial_info));

	/*initialize spatial_temp*/
	memset(spatial_temp.redundancy, 0, 4 * sizeof(long int));
	memset(spatial_temp.cluster_group, 0, 2 * sizeof(long int));
	spatial_temp.valid_flag = 0;

	for (count = 1; count <= array_count; count++)
	{
		spatial_triangle[count - 1] = (spatial_info*)malloc(sizeof(spatial_info));
		
		for ( traverse_count = count + 1; traverse_count <= array_count; traverse_count++)
		{
			connection_count++;

			original_code = original_array[traverse_count].cluster_code;
			adversed_code = adversed_array[count].cluster_code;

			code_length = strlen(original_code);

			while (code_length > 0)
			{
				bit_original = (int)(original_code[code_length - 1] - (int)'0');
				bit_adverse = (int)(adversed_code[code_length - 1] - (int)'0');

				/*check different bits, the number of different bits is redundancy*/
				difference = abs(bit_adverse - bit_original);
				if (difference != 0)
				{
					if (original_array[traverse_count].red_upperbound <= original_array[count].red_upperbound)
					{
						if (bit_original == 0)
						{
							code_length--;
							continue;
						}
					}
					else
					{
						if (bit_adverse == 0)
						{
							code_length--;
							continue;
						}
					}

					red_count++;
				}
				else if (bit_adverse != 0)
				{
					com_count++;
				}

				code_length--;
			}
			
			spatial_temp.cluster_group[0] = count;
			spatial_temp.cluster_group[1] = traverse_count;

			/*if the slave term is independent, the redundancy should reduce by 2, for the
			independent cluster can put 2 literals cannot be combined into LUT unit*/
			if (original_array[traverse_count].red_upperbound <= original_array[count].red_upperbound)
			{
				if (original_array[traverse_count].number_of_iterations == INDEPENDENT_VALUE)
				{
					red_count -= 2;
				}
			}
			else
			{
				if (adversed_array[count].number_of_iterations == INDEPENDENT_VALUE)
				{

					red_count -= 2;
				}
			}

			//red_count -= red_margin;


			/*when those 2 situations above happen, while the redundancy is less than 4(or 2), these two clusters 
			can be totally combined, while will not change the original physical row scale*/
			red_count = (long int)fmaxl(red_count, 0);

			spatial_temp.redundancy[0] = com_count;
			spatial_temp.redundancy[1] = 0;//just initialize
			spatial_temp.redundancy[2] = red_count;
			spatial_temp.redundancy[3] = 0;//just initialize
			spatial_temp.valid_flag = 0;

			spatial_triangle[count - 1] = (spatial_info*)realloc(spatial_triangle[count - 1], (traverse_count - count + 1) * sizeof(spatial_info));
			spatial_triangle[count - 1][traverse_count - count] = spatial_temp;

			spatial_array = (spatial_info*)realloc(spatial_array, (connection_count + 1) * sizeof(spatial_info));
			spatial_array[connection_count] = spatial_temp;

			if (spatial_temp.redundancy[2] > red_max)
			{
				red_max = spatial_temp.redundancy[2];
			}
			if (spatial_temp.redundancy[2] < red_min)
			{
				red_min = spatial_temp.redundancy[2];
			}

			red_count = 0;

			if (com_count > com_max)
			{
				com_max = com_count;
			}
			if (com_count < com_min)
			{
				com_min = com_count;
			}

			com_count = 0;
		}

		/*use first struct to store some data*/
		spatial_triangle[count - 1][0].redundancy[0] = array_count - count;//the number of connections of this cluster
	}

	/*avoid error when only 1 term exists*/
	if (com_count > com_max)
	{
		com_max = com_count;
	}
	if (com_count < com_min)
	{
		com_min = com_count;
	}
	if (spatial_temp.redundancy[2] > red_max)
	{
		red_max = spatial_temp.redundancy[2];
	}
	if (spatial_temp.redundancy[2] < red_min)
	{
		red_min = spatial_temp.redundancy[2];
	}

	/*use first struct to store some data*/
	spatial_array[0].valid_flag = connection_count;
	spatial_array[0].redundancy[0] = com_max;
	spatial_array[0].redundancy[1] = com_min;
	spatial_array[0].redundancy[2] = 0;//just initialize, for red_max of master terms
	spatial_array[0].redundancy[3] = 0;//just initialize, for red_min of master terms
	spatial_array[0].cluster_group[0] = red_max;//red_max of slave terms
	spatial_array[0].cluster_group[1] = red_min;//red_min of slave terms

	return 0;
}

int getMasterRedundancy
	(long int red_upperbound)
{
	long int master_index = 0, var_m = 0, num_com = 0, red_m = 0;
	long int red_max = 0, red_min = red_upperbound;
	long int count = 0;

	for (long int count = 1; count <= spatial_array[0].valid_flag; count++)
	{
		if (original_array[spatial_array[count].cluster_group[0]].red_upperbound < original_array[spatial_array[count].cluster_group[1]].red_upperbound)
		{
			master_index = spatial_array[count].cluster_group[1];
		}
		else
		{
			master_index = spatial_array[count].cluster_group[0];
		}

		var_m = original_array[master_index].red_upperbound;
		num_com = spatial_array[count].redundancy[0];

		red_m = var_m - num_com;

		if (original_array[master_index].number_of_iterations == INDEPENDENT_VALUE)
		{
			red_m -= 2;
		}

		red_m = (long int)fmaxl(0, red_m);

		if (red_m > red_max)
		{
			red_max = red_m;
		}
		if (red_m < red_min)
		{
			red_min = red_m;
		}

		spatial_array[count].redundancy[1] = red_m;
		spatial_triangle[spatial_array[count].cluster_group[0] - 1]\
		[spatial_array[count].cluster_group[1] - spatial_array[count].cluster_group[0]].redundancy[1] = red_m;
	}

	/*avoid error when only 1 term exists*/
	if (red_m > red_max)
	{
		red_max = red_m;
	}
	if (red_m < red_min)
	{
		red_min = red_m;
	}

	spatial_array[0].redundancy[2] = red_max;
	spatial_array[0].redundancy[3] = red_min;

	return 0;
}

int getRedundancy
	(long int max_number_of_literals, long int red_upperbound)
{
	getSlaveRedundancy(max_number_of_literals, red_upperbound);
	getMasterRedundancy(red_upperbound);
	return 0;
}

long int getSpatialParameter
	(long int red_upperbound, long int* result_lim, long int combination)
{
	long int array_count;
	long int redundancy, area_function, area_difference;
	long int max_physical_row, max_physical_column;

	array_count = (long int)original_array[0].order_of_term;

	redundancy = (long int)fmaxl(result_lim[0] + result_lim[1] + result_lim[2] - red_upperbound, 0);

	/*this is the number of rows in actual SRAM array. red_upperbound is the max number of remaining variables among terms. there will be 2 LUT units 
	(contains NOR-cell itself and a standard 10T cell), so the max_physical_row is red_upperbound + 4.*/
	max_physical_row = red_upperbound + 4;
	max_physical_column = array_count;

	original_area = max_physical_column * max_physical_row;
	area_function = (max_physical_row + redundancy) * (max_physical_column - combination);
	area_difference = original_area - area_function;

	//printf("area decreased by %ld cells\n", area_difference);

	return area_difference;
}

int removeBadMatching
	(long int* para_lim)
{
	long int matching_count, count = 0, case_count = 0;
	long int com_lim, rm_lim, rs_lim;

	com_lim = para_lim[0];
	rm_lim = para_lim[1];
	rs_lim = para_lim[2];

	matching_count = spatial_array[0].valid_flag;

	for (size_t count = 1; count <= matching_count; count++)
	{
		spatial_array[count].valid_flag = -1;

		for (size_t case_count = 1; case_count <= spatial_matrix[count - 1][0].valid_flag; case_count++)
		{
			spatial_matrix[count - 1][case_count].valid_flag = -1;
		}
	}

	for (size_t count = 1; count <= matching_count; count++)
	{
		for (size_t case_count = 1; case_count <= spatial_matrix[count - 1][0].valid_flag; case_count++)
		{
			if (spatial_matrix[count - 1][case_count].redundancy[0] > com_lim || spatial_matrix[count - 1][case_count].redundancy[1] > rm_lim || spatial_matrix[count - 1][case_count].redundancy[2] > rs_lim)
			{
				spatial_matrix[count - 1][case_count].valid_flag = -1;
				spatial_array[count].valid_flag = -1;
			}
			else
			{
				spatial_matrix[count - 1][case_count].valid_flag = 0;
				spatial_array[count] = spatial_matrix[count - 1][case_count];
				spatial_triangle[spatial_array[count].cluster_group[0] - 1]\
					[spatial_array[count].cluster_group[1] - spatial_array[count].cluster_group[0]] = spatial_array[count];
				break;
			}
		}
	}
	return 0;
}

/*build a matrix to store mapping tuples of each spatial pair*/
int expandMapCases
	()
{
	long int conn_count = 0;
	long int case_num = 0;
	long int rm_max = 0, rs_max = 0;

	rm_max = spatial_array[0].redundancy[2];
	rs_max = spatial_array[0].cluster_group[0];

	conn_count = spatial_array[0].valid_flag;
	spatial_matrix = (spatial_info**)malloc(conn_count * sizeof(spatial_info*));

	for (long int count = 1; count <= conn_count; count++)
	{
		case_num = (long int)fminl((long int)fminl(spatial_array[count].redundancy[0] + 1, rm_max - spatial_array[count].redundancy[1] + 1), rs_max - spatial_array[count].redundancy[2] + 1);
		
		if (case_num > 0)
		{
			spatial_matrix[count - 1] = (spatial_info*)malloc((case_num + 1) * sizeof(spatial_info));
			for (long int case_count = 0; case_count < case_num; case_count++)
			{
				spatial_matrix[count - 1][case_count + 1] = spatial_array[count];
				spatial_matrix[count - 1][case_count + 1].redundancy[0] -= case_count;
				spatial_matrix[count - 1][case_count + 1].redundancy[1] += case_count;
				spatial_matrix[count - 1][case_count + 1].redundancy[2] += case_count;
			}
		}

		spatial_matrix[count - 1][0].valid_flag = case_num;
	}

// to output the original redundancy list
#ifdef PRINTREDLIST
	FILE* fp;

	if ((fp = fopen("spredlist.txt", "w+")) != NULL)
	{
		fprintf(fp, "\nSP REDUNDANCY LIST\n");
		fprintf(fp, "Spatial pairs\tRedundancy\n");
		fprintf(fp, "--------------------------\n");
		for (size_t count = 1; count <= spatial_array[0].valid_flag; count++)
		{
			fprintf(fp, "{%ld,%ld}\t(%ld, %ld, %ld)\n", \
				spatial_array[count].cluster_group[0], \
				spatial_array[count].cluster_group[1], \
				spatial_array[count].redundancy[0], \
				spatial_array[count].redundancy[1], \
				spatial_array[count].redundancy[2]);
		}
		fprintf(fp, "--------------------------\n");
        fclose(fp);
	}
	else
	{
		fprintf(stdout, "Fail to open file!\n");
		exit(EXIT_FAILURE);
	}
#endif // PRINTREDLIST

	return 0;
}
