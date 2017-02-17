#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void swap(int *a, long x, long y) {
	int temp = a[x];
	a[x] = a[y];
	a[y] = temp;
}

void quicksort_parallel(int *a, long lo, long hi) {
	long i, div;
	
	if (lo < hi) {
		int x = a[lo];
		div = lo;
		
		for (i = lo+1; i < hi; i++) {
			if (a[i] <= x) {
				div++;
				swap(a, div, i);
			}
		}

		swap(a, lo, div);

		#pragma omp task
		quicksort_parallel(a, lo, div);

		#pragma omp task
		quicksort_parallel(a, div+1, hi);
	}
}

void quicksort_serial(int *a, long lo, long hi) {
	long i, div;
	
	if (lo < hi) {
		int x = a[lo];
		div = lo;
		
		for (i = lo+1; i < hi; i++) {
			if (a[i] <= x) {
				div++;
				swap(a, div, i);
			}
		}

		swap(a, lo, div);

		quicksort_serial(a, lo, div);
		quicksort_serial(a, div+1, hi);
	}
}

int main(int argc, char *argv[]) {
	int thread_count = strtol(argv[1], NULL, 10);
	int n;
	printf("Jumlah array : "); scanf("%d", &n);
	int a[n];
	int b[n];

	double start, end;

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		int x = rand() % 999999;
		a[i] = x;
		b[i] = x;
	}

	start = omp_get_wtime();
	quicksort_serial(a, 0, n);
	end = omp_get_wtime();
    double elapsed1 = (end - start);

	start = omp_get_wtime();
	#pragma omp parallel num_threads(thread_count)
	{
		#pragma omp single nowait
		quicksort_parallel(b, 0, n);
	}
	end = omp_get_wtime();
	double elapsed2 = (end - start);
    
    printf("Time elapsed in serial: %f second\n", elapsed1);
    printf("Time elapsed in parallel: %f second\n", elapsed2);

	return 0;
}