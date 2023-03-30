#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../header files/Global.h"
#include "../header files/spatial_Combination.h"
#include "../header files/optimize_Methods.h"


long int* updatelimit
	(long int* para_lim)
{
	long int* update_lim;
	long int com_min, rm_min, rs_min;
	long int com_max, rm_max, rs_max;

	com_max = spatial_array[0].redundancy[0];
	com_min = spatial_array[0].redundancy[1];
	rm_max = spatial_array[0].redundancy[2];
	rm_min = spatial_array[0].redundancy[3];
	rs_max = spatial_array[0].cluster_group[0];
	rs_min = spatial_array[0].cluster_group[1];

    update_lim = (long int*) malloc(3 * sizeof (long int));
	memmove(update_lim, para_lim, 3 * sizeof(long int));

	if (update_lim[0] == com_max)
	{

		if (update_lim[1] == rm_max)
		{

			if (update_lim[2] == rs_max)
			{
				return update_lim;
			}
			else
			{
				update_lim[2]++;
			}
			update_lim[1] = rm_min;
		}
		else
		{
			update_lim[1]++;
		}
		update_lim[0] = com_min;
	}
	else
	{
		update_lim[0]++;
	}

	return update_lim;
}

int blossomMethod(int mode, long int red_max, long int red_min, long int red_upperbound)
{
	float diff_rate;
	long int match_num = -1, match_new = -1, area_diff;
	long int opt_match_num = 0, opt_red = 0, opt_area_diff = 0;
	long int array_count = (long int)original_array[0].order_of_term;
	long int para_lim[3], opt_lim[3];
	long int com_max, com_min, rm_max, rm_min, rs_max, rs_min;
	long int sum_upperbound, sum_lowerbound, lim_count = 0, lim_sum, lim_num;
	long int* result_lim;
    long int* temp_lim;
	long int*** lim_array;

	com_max = spatial_array[0].redundancy[0];
	com_min = spatial_array[0].redundancy[1];
	rm_max = spatial_array[0].redundancy[2];
	rm_min = spatial_array[0].redundancy[3];
	rs_max = spatial_array[0].cluster_group[0];
	rs_min = spatial_array[0].cluster_group[1];

	sum_lowerbound = com_min + rm_min + rs_min;
	sum_upperbound = com_max + rm_max + rs_max;

	lim_array = (long int***)malloc((sum_upperbound - sum_lowerbound + 1) * sizeof(long int**));
	temp_lim = (long int*)malloc(3 * sizeof(long int));

	para_lim[0] = com_min;
    //para_lim[0] = 0;
	para_lim[1] = rm_min;
	para_lim[2] = rs_min;

    memmove(temp_lim, para_lim, 3 * sizeof (long int));
	result_lim = temp_lim;

	/*create an array for traversing the limit search tree*/
	for (long int sum_count = 0; sum_count <= sum_upperbound - sum_lowerbound; sum_count++)
	{
		lim_array[sum_count] = (long int**)malloc(sizeof(long int*));
		lim_array[sum_count][0] = (long int*)malloc(3 * sizeof(long int));
		memset(lim_array[sum_count][0], 0, 3 * sizeof(long int));
	}
	/*sort limit groups by the sum of their three elements to build the search tree*/
	do
	{
		memmove(para_lim, result_lim, 3 * sizeof(long int));
        free(result_lim);
		lim_sum = para_lim[0] + para_lim[1] + para_lim[2];
		lim_num = ++lim_array[lim_sum - sum_lowerbound][0][0];
		lim_array[lim_sum - sum_lowerbound] = (long int**)realloc(lim_array[lim_sum - sum_lowerbound], (lim_num + 1) * sizeof(long int*));
		lim_array[lim_sum - sum_lowerbound][lim_num] = (long int*)malloc(3 * sizeof(long int));
		memmove(lim_array[lim_sum - sum_lowerbound][lim_num], para_lim, 3 * sizeof(long int));
		result_lim = updatelimit(para_lim);
	} while (para_lim[0] + para_lim[1] + para_lim[2] < sum_upperbound);

	/*get other mapping tuples of each spatial pair*/
	expandMapCases();

	/*level order traversal, visit them in the order that sum increases, record the optimal match limit group*/
	for (long int sum_count = 0; sum_count <= sum_upperbound - sum_lowerbound; sum_count++)
	{
        //row-first situation
		if (mode == ROW_PRIORITY)
		{
			if (sum_count + sum_lowerbound > red_upperbound)
			{
				sum_upperbound = fmaxl(sum_count - 1, 0) + sum_lowerbound;
				break;
			}
		}

		for (long int lim_count = 1; lim_count <= lim_array[sum_count][0][0]; lim_count++)
		{
			memmove(para_lim, lim_array[sum_count][lim_count], 3 * sizeof(long int));
			removeBadMatching(para_lim);
			match_new = blossom(spatial_array, array_count);
			free_all(array_count);

			if (match_new >= match_num)
			{
				match_num = match_new;
				lim_array[sum_count][0][2] = lim_count;
			}

			/*if get an maximum matching, the remaining limit groups can be discarded*/
			if (match_num == floor(array_count / 2))
			{
				break;
			}
		}

		lim_array[sum_count][0][1] = match_num;
		if (match_num == floor(array_count / 2))
		{
			sum_upperbound = sum_count + sum_lowerbound;
			break;
		}
		match_num = 0;
	}

	/*find the optimal limit group and match number*/
	for (long int sum_count = sum_upperbound - sum_lowerbound; sum_count >= 0; sum_count--)
	{
        //column-first situation
		if (mode == COL_PRIORITY)
		{
			if (lim_array[sum_count][0][1] < floor(array_count / 2))
			{
				continue;
			}
		}

		area_diff = getSpatialParameter(red_upperbound, lim_array[sum_count][lim_array[sum_count][0][2]], lim_array[sum_count][0][1]);
		if (area_diff >= opt_area_diff)
		{
			opt_area_diff = area_diff;
			memmove(opt_lim, lim_array[sum_count][lim_array[sum_count][0][2]], 3 * sizeof(long int));
		}
	}

	/*do EBA with the optimal limit group again*/
	removeBadMatching(opt_lim);
	opt_match_num = blossom(spatial_array, array_count);
	result_lim = outputLimit(spatial_triangle, array_count);
	opt_red = (long int)(fmaxl(result_lim[0] + result_lim[1] + result_lim[2] - red_upperbound, 0));
	opt_area_diff = getSpatialParameter(red_upperbound, result_lim, opt_match_num);

	/*record results*/
	for (size_t count = 1; count <= spatial_array[0].valid_flag; count++)
	{
		spatial_array[count].redundancy[3] = (long int)(fmaxl(spatial_array[count].redundancy[0] + spatial_array[count].redundancy[1]\
			+ spatial_array[count].redundancy[2] - red_upperbound, 0));
		spatial_triangle[spatial_array[count].cluster_group[0] - 1]\
			[spatial_array[count].cluster_group[1] - spatial_array[count].cluster_group[0]].redundancy[3] = spatial_array[count].redundancy[3];
	}
	outputBlossomResult(spatial_triangle, array_count);

	diff_rate = original_area == 0 ? 0 : ((float)opt_area_diff * 100 / (float)original_area);
	
	printf("Optimal number of spatial pairs is %ld, with minimum redundancy (%ld, %ld, %ld, %ld).\nArea was reduced by %.2lf%% after spatial mergence, from %ld to %ld, %ld 10T SRAM cells in total.\n", opt_match_num, result_lim[0],\
		result_lim[1], result_lim[2], opt_red, diff_rate, original_area, original_area - opt_area_diff, opt_area_diff);
	printf("Spatial mergence has finished.\n");
	printf("*****************************************************\n");
	fprintf(ofp, "Optimal number of spatial pairs is %ld, with minimum redundancy (%ld, %ld, %ld, %ld).\nArea was reduced by %.2lf%% after spatial mergence, from %ld to %ld, %ld 10T SRAM cells in total.\n", opt_match_num, result_lim[0],\
		result_lim[1], result_lim[2], opt_red, diff_rate, original_area, original_area - opt_area_diff, opt_area_diff);
	fprintf(ofp, "Spatial mergence has finished.\n");
	fprintf(ofp, "*****************************************************\n");

	/*free memories*/
	free_all(array_count);
	free(result_lim);
	for (long int sum_count = sum_upperbound - sum_lowerbound; sum_count >= 0; sum_count--)
	{
		lim_count = lim_array[sum_count][0][0];
		for (long int count = lim_count; count >= 0; count--)
		{
			free(lim_array[sum_count][count]);
		}
		free(lim_array[sum_count]);
	}
	return 0;
}

/*the comparison function for qsort()*/
int compareRedundancy
	(const void* sa, const void* sb)
{
	spatial_info* SA = (spatial_info*)sa;
	spatial_info* SB = (spatial_info*)sb;
	if (SA->redundancy != SB->redundancy)
	{
		return ((SA->redundancy) > (SB->redundancy)) ? 1 : -1;
	}
	else
	{
		return 0;
	}
}

/*the comparison function for qsort()*/
int descendingPriority
	(const void* sa, const void* sb)
{
	priority* SA = (priority*)sa;
	priority* SB = (priority*)sb;
	if ((SA->redundancy) != (SB->redundancy))
	{
		/*sort structs in descending order of redundancy*/
		return ((SA->redundancy) < (SB->redundancy)) ? 1 : -1;
	}
	else
	{
		if ((SA->sum) != (SB->sum))
		{
			/*second judge principle: cluster whose the sum of redundancy is largest will prior to others*/
			return ((SA->sum) < (SB->sum)) ? 1 : -1;
		}
		else
		{
			return 0;
		}
	}
}

/*the comparison function for qsort()*/
int ascendingPriority
(const void* sa, const void* sb)
{
	priority* SA = (priority*)sa;
	priority* SB = (priority*)sb;
	if ((SA->redundancy) != (SB->redundancy))
	{
		/*sort structs in ascending order of redundancy*/
		return ((SA->redundancy) > (SB->redundancy)) ? 1 : -1;
	}
	else
	{
		if ((SA->sum) != (SB->sum))
		{
			/*second judge principle: cluster whose the sum of redundancy is largest will prior to others*/
			return ((SA->sum) > (SB->sum)) ? 1 : -1;
		}
		else
		{
			return 0;
		}
	}
}