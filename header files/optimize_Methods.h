#define ORIGINAL_RED 0
#define MC_TRYTIMES 100
#define ASC_ORDER 0
#define DES_ORDER 1
#define OUTPUT_HOLD 0
#define OUTPUT_GO 1

#define AREA_PRIORITY 1
#define ROW_PRIORITY 2
#define COL_PRIORITY 3

typedef struct
{
	long int cluster_order;
	long int redundancy;
	long int sum;
	int select_flag;
}priority;

int blossomMethod
	(int mode, long int red_max, long int red_min, long int red_upperbound);
long int blossom
	(spatial_info* spatial_array, long int vertice_num);
long int* updatelimit
	(long int* para_lim);
int outputBlossomResult
	(spatial_info** spatial_triangle, long int vertice_num);
long int* outputLimit
	(spatial_info** spatial_triangle, long int vertice_num);
void free_all
	(long int vertice_num);
int compareRedundancy
	(const void* sa, const void* sb);
int descendingPriority
	(const void* sa, const void* sb);
int ascendingPriority
	(const void* sa, const void* sb);

