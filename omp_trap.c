/* 
 * Copyrights http://www.cs.usfca.edu/~peter/cs625/code/trap/omp_trap.c
 * File:    omp_trap.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: Using gcc
 *    gcc -g -Wall -fopenmp -o omp_trap omp_trap.c
 * Usage:   ./omp_trap <number of threads>
 *
 * Notes:    
 * 1.  The function f(x) is hardwired.
 * 2.  This version uses OpenMP's parallel for with variable
 *     scope specified, and static partitioning.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int thread_count;

double Trap(double a, double b, int n);
double f(double x);    /* Function we're integrating */

int main(int argc, char* argv[]) {
   double  integral;   /* Store result in integral   */
   double  a, b;       /* Left and right endpoints   */
   int     n;          /* Number of trapezoids       */

   if (argc != 2) {
      fprintf(stderr, "usage: %s <number of threads>\n", argv[0]);
      exit(0);
   }
   thread_count = strtol(argv[1], NULL, 10);

   printf("Enter a, b, and n\n");
   scanf("%lf %lf %d", &a, &b, &n);
   
   /* OpenMP starts from here */
   integral = Trap(a, b, n);

   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %19.15e\n",
      a, b, integral);

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Use trapezoidal rule to compute definite integral
 * Input args:  
 *    a: left endpoint
 *    b: right endpoint
 *    n: number of trapezoids
 * Return value:  Estimate of Integral from a to b of f(x)
 */
double Trap(double a, double b, int n) {
   double  h, x, integral = 0.0;
   int  i; 

   h = (b-a)/n;
   integral += (f(a) + f(b))/2.0;
#  pragma omp parallel for schedule(static) default(none) \
      shared(a, h, n) private(i, x) \
      reduction(+: integral) num_threads(thread_count)
   for (i = 1; i <= n-1; i++) {
      x = a + i*h;
      integral += f(x);
   }

   integral = integral*h;

   return integral;
}  /* Trap */

/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input arg:   x
 * Return val:  f(x)
 */
double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */
