#include <iostream>
#include <deque>
#include <cstring>
#include <cstdio>
#include <malloc.h>
#include "../header files/Global.h"

using namespace std;

/*the number of vertices of the graph, which represents the terms of the logic function*/
//const int V = 50;

/*adjacent matrix of the graph*/
//bool adj_matrix[V][V];
bool** adj_matrix;

/*a double-ended queue, being used to store the alternative paths*/
//std::deque<int> alter_path[V];
deque<int>* alter_path;

/*record the spouse vertice of each vertices*/
//int match_index[V];
int* match_index;

/*parity information of each vertice. 0 means even vertice, 1 means odd vertice,
-1 means undecided*/
//int parity_info[V];
int* parity_info;

/*a queue used to store even vertices*/
//int queue[V], *qf, *qb;
int* queue, * qf, * qb;

extern "C" long int blossom(spatial_info* spatial_array, long int vertice_num);
extern "C" long int* outputLimit(spatial_info** spatial_triangle, long int vertice_num);
extern "C" int outputBlossomResult(spatial_info** spatial_triangle, long int vertice_num);
extern "C" void free_all(long int vertice_num);

int allocate(const long int vertice_num)
{
    match_index = new int[vertice_num];
	parity_info = new int[vertice_num];
	queue = new int[vertice_num];
	alter_path = new deque<int>[vertice_num];
	adj_matrix = new bool* [vertice_num];

	for (size_t count = 0; count < vertice_num; count++)
	{
		adj_matrix[count] = new bool[vertice_num];
	}
	return 0;
}
/*label_one_side is used to process the blossom, which means a cross edge
(from an even vertice to an even vertice) exists. In this case, all of the 
odd vertices are on a blossom of the alternating tree*/
void label_one_side(int x, int y, int b)
{
	for (int i = b + 1; i < alter_path[x].size(); i++)
	{
		int z = alter_path[x][i];
		if (parity_info[z] == 1)
		{
			/*if a vertice is an odd vertice, then turn it to an even vertice
			by letting the alternating path from root to the vertice contains
			the crossing edge*/
			alter_path[z] = alter_path[y];
			alter_path[z].insert(alter_path[z].end(), alter_path[x].rbegin(), alter_path[x].rend() - i);
			/*odd vertice turns to even vertice*/
			parity_info[z] = 0;
			*qb++ = z;
		}
	}
}

/*build alternating tree form an unmatch vertice r*/
bool BFS(int r, long int vertice_num)
{
	/*clear the deque*/
	for (int i = 0; i < vertice_num; i++)
	{
		alter_path[i].clear();
	}
	/*build the root of alternating tree*/
	alter_path[r].push_back(r);

	/*initialize all parity information of the alternating tree,
	the root is an even vertice*/
	memset(parity_info, -1, vertice_num * sizeof(int));
	parity_info[r] = 0;

	qf = qb = queue;
	*qb++ = r;

	while (qf < qb)
	{
		for (int x = *qf++, y = 0; y < vertice_num; y++)
		{
			if (adj_matrix[x][y] && match_index[y] != y)
			{
				if (parity_info[y] == -1)
				{
					if (match_index[y] == -1)
					{
						for (int i = 0; i + 1 < alter_path[x].size(); i += 2)
						{
							match_index[alter_path[x][i]] = alter_path[x][i + 1];
							match_index[alter_path[x][i + 1]] = alter_path[x][i];
						}
						match_index[x] = y;
						match_index[y] = x;
						return true;
					}
					else
					{
						int z = match_index[y];
						alter_path[z] = alter_path[x];
						alter_path[z].push_back(y);
						alter_path[z].push_back(z);
						parity_info[y] = 1;
						parity_info[z] = 0;
						*qb++ = z;
					}
				}
				else
				{
					if (parity_info[y] == 0)
					{
						int b = 0;
						while (b < alter_path[x].size() && b < alter_path[y].size() && alter_path[x][b] == alter_path[y][b])
						{
							b++;
						}
						b--;
						label_one_side(x, y, b);
						label_one_side(y, x, b);
					}
					else
					{
						;
					}
				}
			}
		}
	}
	return false;
}

long int match(long int vertice_num)
{
	memset(match_index, -1, vertice_num * sizeof(int));
	long int c = 0;
	for (int i = 0; i < vertice_num; i++)
	{
		if (match_index[i] == -1)
		{
			if (BFS(i, vertice_num))
			{
				c++;
			}
			else
			{
				match_index[i] = i;
			}
		}
	}
	return c;
}

long int* outputLimit
	(spatial_info** spatial_triangle, long int vertice_num)
{
	long int count = 0;
	long int* result_lim;

	result_lim = (long int*)malloc(3 * sizeof(long int));
	memset(result_lim, 0, 3 * sizeof(long int));

	for (size_t count = 1; count < vertice_num; count++)
	{
		if (count - 1 < match_index[count - 1])
		{
			if (spatial_triangle[count - 1][match_index[count - 1] + 1 - count].redundancy[0] > result_lim[0])
			{
				result_lim[0] = spatial_triangle[count - 1][match_index[count - 1] + 1 - count].redundancy[0];
			}
			if (spatial_triangle[count - 1][match_index[count - 1] + 1 - count].redundancy[1] > result_lim[1])
			{
				result_lim[1] = spatial_triangle[count - 1][match_index[count - 1] + 1 - count].redundancy[1];
			}
			if (spatial_triangle[count - 1][match_index[count - 1] + 1 - count].redundancy[2] > result_lim[2])
			{
				result_lim[2] = spatial_triangle[count - 1][match_index[count - 1] + 1 - count].redundancy[2];
			}
		}
	}

	return result_lim;
}

long int blossom(spatial_info* spatial_array, long int vertice_num)
{
	//cin >> V;

	int x, y;
	long int match_num;

	allocate(vertice_num);

	for (long int count = 1; count <= spatial_array[0].valid_flag; count++)
	{
		if (spatial_array[count].valid_flag == -1)
		{
			x = spatial_array[count].cluster_group[0] - 1;
			y = spatial_array[count].cluster_group[1] - 1;
			adj_matrix[x][y] = adj_matrix[y][x] = false;
		}
		else
		{
			x = spatial_array[count].cluster_group[0] - 1;
			y = spatial_array[count].cluster_group[1] - 1;
			adj_matrix[x][y] = adj_matrix[y][x] = true;
		}
	}

	match_num = match(vertice_num);

	return match_num;
}

int outputBlossomResult(spatial_info** spatial_triangle, long int vertice_num)
{
	long int red;

	printf("\nSEARCH RESULT:\n");
	printf("All orders in cluster sets are orders of logic cluster, in the result table of logic mergence.\n");
	printf("Cluster set\tRegion I\tRegion II\tRegion III\tTotal redundancy\n");
	printf("--------------------------------------------------------------------------------\n");
	fprintf(ofp, "\nSEARCH RESULT:\n");
	fprintf(ofp, "All orders in cluster sets are orders of logic cluster, in the result table of logic mergence.\n");
	fprintf(ofp, "Cluster set\tRegion I\tRegion II\tRegion III\tTotal redundancy\n");
	fprintf(ofp,"--------------------------------------------------------------------------------\n");

	for (int i = 1; i <= vertice_num; i++)
	{
		if (i - 1 < match_index[i - 1])
		{
			red = spatial_triangle[i - 1][match_index[i - 1] + 1 - i].redundancy[3];
			printf("{%d,%d}\t\t%ld\t\t%ld\t\t%ld\t\t%ld\n", i, match_index[i - 1] + 1, 
				spatial_triangle[i - 1][match_index[i - 1] + 1 - i].redundancy[0], 
				spatial_triangle[i - 1][match_index[i - 1] + 1 - i].redundancy[1], 
				spatial_triangle[i - 1][match_index[i - 1] + 1 - i].redundancy[2], red);
			fprintf(ofp, "{%d,%d}\t\t%ld\t\t%ld\t\t%ld\t\t%ld\n", i, match_index[i - 1] + 1, 
				spatial_triangle[i - 1][match_index[i - 1] + 1 - i].redundancy[0], 
				spatial_triangle[i - 1][match_index[i - 1] + 1 - i].redundancy[1], 
				spatial_triangle[i - 1][match_index[i - 1] + 1 - i].redundancy[2], red);
		}
	}
	printf("--------------------------------------------------------------------------------\n");
	fprintf(ofp, "--------------------------------------------------------------------------------\n");

	return 0;
}

void free_all(long int vertice_num)
{
	delete[] match_index;
	delete[] parity_info;
	delete[] queue;

	for (size_t count = 0; count < vertice_num; count++)
	{
		delete[] adj_matrix[count];
	}
	delete[] adj_matrix;
}