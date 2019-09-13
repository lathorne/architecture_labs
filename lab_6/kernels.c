/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "Chubby Bunnies",			/* Team name */

    "Hyrum Gunther",		/* First member full name */
    "hygunth@gmail.com",		/* First member email address */

    "Logan Thorneloe",		  /* Second member full name (leave blank if none) */
    "loganthorneloe@gmail.com"		  /* Second member email addr (leave blank if none) */
};

#define MIN_MACRO(a, b) (a < b ? a : b)
#define MAX_MACRO(a, b) (a > b ? a : b)

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 0; i < dim; i+=16)
	for (j = 0; j < dim; j++){
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
		dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
		dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)];
		dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];
		dst[RIDX(dim-1-j, i+4, dim)] = src[RIDX(i+4, j, dim)];
		dst[RIDX(dim-1-j, i+5, dim)] = src[RIDX(i+5, j, dim)];
		dst[RIDX(dim-1-j, i+6, dim)] = src[RIDX(i+6, j, dim)];
		dst[RIDX(dim-1-j, i+7, dim)] = src[RIDX(i+7, j, dim)];
		dst[RIDX(dim-1-j, i+8, dim)] = src[RIDX(i+8, j, dim)];
		dst[RIDX(dim-1-j, i+9, dim)] = src[RIDX(i+9, j, dim)];
		dst[RIDX(dim-1-j, i+10, dim)] = src[RIDX(i+10, j, dim)];
		dst[RIDX(dim-1-j, i+11, dim)] = src[RIDX(i+11, j, dim)];
		dst[RIDX(dim-1-j, i+12, dim)] = src[RIDX(i+12, j, dim)];
		dst[RIDX(dim-1-j, i+13, dim)] = src[RIDX(i+13, j, dim)];
		dst[RIDX(dim-1-j, i+14, dim)] = src[RIDX(i+14, j, dim)];
		dst[RIDX(dim-1-j, i+15, dim)] = src[RIDX(i+15, j, dim)];
	}
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

#define ASSIGN_SUM_TO_PIXEL(current_pixel, sum) current_pixel.red = (unsigned short) (sum.red/sum.num);\
    current_pixel.green = (unsigned short) (sum.green/sum.num);\
    current_pixel.blue = (unsigned short) (sum.blue/sum.num);

#define ACCUMULATE_SUM(sum, p) sum.red += (int) p.red;\
    sum.green += (int) p.green;\
    sum.blue += (int) p.blue;\
    sum.num++;

#define SUM_IDX(sum, src, idx) sum.red += (int) src[idx].red;\
	sum.green += (int) src[idx].green;\
	sum.blue += (int) src[idx].blue;

#define AVG_SUM(current_pixel, sum) current_pixel.red = (unsigned short) (sum.red/sum.num);\
	current_pixel.green = (unsigned short) (sum.green/sum.num);\
	current_pixel.blue = (unsigned short) (sum.blue/sum.num);

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++){ 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);
	}
    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
	int i, j;
	int length = dim*dim;
	int edge_length = length - 1;

		pixel_sum sum;
		pixel current_pixel;

		//top left corner
		sum.red = sum.green = sum.blue = 0;
    	sum.num = 4;
		//all four cases
		sum.red += (int) src[RIDX(0, 0, dim)].red;
		sum.green += (int) src[RIDX(0, 0, dim)].green;
		sum.blue += (int) src[RIDX(0, 0, dim)].blue;

		sum.red += (int) src[RIDX(0, 1, dim)].red;
		sum.green += (int) src[RIDX(0, 1, dim)].green;
		sum.blue += (int) src[RIDX(0, 1, dim)].blue;

		sum.red += (int) src[RIDX(1, 0, dim)].red;
		sum.green += (int) src[RIDX(1, 0, dim)].green;
		sum.blue += (int) src[RIDX(1, 0, dim)].blue;

		sum.red += (int) src[RIDX(1, 1, dim)].red;
		sum.green += (int) src[RIDX(1, 1, dim)].green;
		sum.blue += (int) src[RIDX(1, 1, dim)].blue;

		
		AVG_SUM(current_pixel, sum);
		dst[0] = current_pixel; //setting the current pixel here

		//topmost row w/o corners
		for(i = 1; i < dim - 1; i++){
			sum.red = sum.green = sum.blue = 0;
			sum.num = 6;
			//all four cases
			sum.red += (int) src[RIDX(i - 1, 0, dim)].red;
			sum.green += (int) src[RIDX(i - 1, 0, dim)].green;
			sum.blue += (int) src[RIDX(i - 1, 0, dim)].blue;

			sum.red += (int) src[RIDX(i - 1, 1, dim)].red;
			sum.green += (int) src[RIDX(i - 1, 1, dim)].green;
			sum.blue += (int) src[RIDX(i - 1, 1, dim)].blue;

			sum.red += (int) src[RIDX(i, 0, dim)].red;
			sum.green += (int) src[RIDX(i, 0, dim)].green;
			sum.blue += (int) src[RIDX(i, 0, dim)].blue;

			sum.red += (int) src[RIDX(i, 1, dim)].red;
			sum.green += (int) src[RIDX(i, 1, dim)].green;
			sum.blue += (int) src[RIDX(i, 1, dim)].blue;

			sum.red += (int) src[RIDX(i + 1, 0, dim)].red;
			sum.green += (int) src[RIDX(i + 1, 0, dim)].green;
			sum.blue += (int) src[RIDX(i + 1, 0, dim)].blue;

			sum.red += (int) src[RIDX(i + 1, 1, dim)].red;
			sum.green += (int) src[RIDX(i + 1, 1, dim)].green;
			sum.blue += (int) src[RIDX(i + 1, 1, dim)].blue;

			AVG_SUM(current_pixel, sum);
			dst[RIDX(i, 0, dim)] = current_pixel; //setting the current pixel here
		}

		//top right corner
		sum.red = sum.green = sum.blue = 0;
    	sum.num = 4;
		//all four cases
		sum.red += (int) src[RIDX(dim - 1, 0, dim)].red;
		sum.green += (int) src[RIDX(dim - 1, 0, dim)].green;
		sum.blue += (int) src[RIDX(dim - 1, 0, dim)].blue;

		sum.red += (int) src[RIDX(dim - 2, 0, dim)].red;
		sum.green += (int) src[RIDX(dim - 2, 0, dim)].green;
		sum.blue += (int) src[RIDX(dim - 2, 0, dim)].blue;

		sum.red += (int) src[RIDX(dim - 1, 1, dim)].red;
		sum.green += (int) src[RIDX(dim - 1, 1, dim)].green;
		sum.blue += (int) src[RIDX(dim - 1, 1, dim)].blue;

		sum.red += (int) src[RIDX(dim - 2, 1, dim)].red;
		sum.green += (int) src[RIDX(dim - 2, 1, dim)].green;
		sum.blue += (int) src[RIDX(dim - 2, 1, dim)].blue;

		AVG_SUM(current_pixel, sum);
		dst[RIDX(dim - 1, 0, dim)] = current_pixel; //setting the current pixel here

		//leftmost column w/o corners
		for(i = 1; i < dim - 1; i++){
			sum.red = sum.green = sum.blue = 0;
			sum.num = 6;
			//all four cases
			sum.red += (int) src[RIDX(0, i, dim)].red;
			sum.green += (int) src[RIDX(0, i, dim)].green;
			sum.blue += (int) src[RIDX(0, i, dim)].blue;

			sum.red += (int) src[RIDX(1, i, dim)].red;
			sum.green += (int) src[RIDX(1, i, dim)].green;
			sum.blue += (int) src[RIDX(1, i, dim)].blue;

			sum.red += (int) src[RIDX(0, i + 1, dim)].red;
			sum.green += (int) src[RIDX(0, i + 1, dim)].green;
			sum.blue += (int) src[RIDX(0, i + 1, dim)].blue;

			sum.red += (int) src[RIDX(1, i + 1, dim)].red;
			sum.green += (int) src[RIDX(1, i + 1, dim)].green;
			sum.blue += (int) src[RIDX(1, i + 1, dim)].blue;

			sum.red += (int) src[RIDX(0, i - 1, dim)].red;
			sum.green += (int) src[RIDX(0, i - 1, dim)].green;
			sum.blue += (int) src[RIDX(0, i - 1, dim)].blue;

			sum.red += (int) src[RIDX(1, i - 1, dim)].red;
			sum.green += (int) src[RIDX(1, i - 1, dim)].green;
			sum.blue += (int) src[RIDX(1, i - 1, dim)].blue;

			AVG_SUM(current_pixel, sum);
			dst[RIDX(0, i, dim)] = current_pixel; //setting the current pixel here
		}

		//loop for all inner pixels
		for(i = 1; i < dim-1; i++){

			//the first column


			
			for(j = 1; j < dim-1; j++){
				//initialize the pixel
				
				sum.red = sum.green = sum.blue = 0;
    			sum.num = 9;

				//all nine cases
				sum.red += (int) src[RIDX(i-1, j-1, dim)].red;
				sum.green += (int) src[RIDX(i-1, j-1, dim)].green;
				sum.blue += (int) src[RIDX(i-1, j-1, dim)].blue;

				sum.red += (int) src[RIDX(i, j, dim)].red;
				sum.green += (int) src[RIDX(i, j, dim)].green;
				sum.blue += (int) src[RIDX(i, j, dim)].blue;

				sum.red += (int) src[RIDX(i+1, j+1, dim)].red;
				sum.green += (int) src[RIDX(i+1, j+1, dim)].green;
				sum.blue += (int) src[RIDX(i+1, j+1, dim)].blue;

				sum.red += (int) src[RIDX(i-1, j+1, dim)].red;
				sum.green += (int) src[RIDX(i-1, j+1, dim)].green;
				sum.blue += (int) src[RIDX(i-1, j+1, dim)].blue;

				sum.red += (int) src[RIDX(i+1, j-1, dim)].red;
				sum.green += (int) src[RIDX(i+1, j-1, dim)].green;
				sum.blue += (int) src[RIDX(i+1, j-1, dim)].blue;

				sum.red += (int) src[RIDX(i+1, j, dim)].red;
				sum.green += (int) src[RIDX(i+1, j, dim)].green;
				sum.blue += (int) src[RIDX(i+1, j, dim)].blue;

				sum.red += (int) src[RIDX(i-1, j, dim)].red;
				sum.green += (int) src[RIDX(i-1, j, dim)].green;
				sum.blue += (int) src[RIDX(i-1, j, dim)].blue;

				sum.red += (int) src[RIDX(i, j+1, dim)].red;
				sum.green += (int) src[RIDX(i, j+1, dim)].green;
				sum.blue += (int) src[RIDX(i, j+1, dim)].blue;

				sum.red += (int) src[RIDX(i, j-1, dim)].red;
				sum.green += (int) src[RIDX(i, j-1, dim)].green;
				sum.blue += (int) src[RIDX(i, j-1, dim)].blue;

				AVG_SUM(current_pixel, sum);
				dst[RIDX(i, j, dim)] = current_pixel; //setting the current pixel here
			}

			
		}

		//rightmost column w/o corners
		for(i = 1; i < dim - 1; i++){
			sum.red = sum.green = sum.blue = 0;
			sum.num = 6;
			//all four cases
			sum.red += (int) src[RIDX(dim - 1, i, dim)].red;
			sum.green += (int) src[RIDX(dim - 1, i, dim)].green;
			sum.blue += (int) src[RIDX(dim - 1, i, dim)].blue;

			sum.red += (int) src[RIDX(dim - 2, i, dim)].red;
			sum.green += (int) src[RIDX(dim - 2, i, dim)].green;
			sum.blue += (int) src[RIDX(dim - 2, i, dim)].blue;

			sum.red += (int) src[RIDX(dim - 1, i + 1, dim)].red;
			sum.green += (int) src[RIDX(dim - 1, i + 1, dim)].green;
			sum.blue += (int) src[RIDX(dim - 1, i + 1, dim)].blue;

			sum.red += (int) src[RIDX(dim - 2, i + 1, dim)].red;
			sum.green += (int) src[RIDX(dim - 2, i + 1, dim)].green;
			sum.blue += (int) src[RIDX(dim - 2, i + 1, dim)].blue;

			sum.red += (int) src[RIDX(dim - 1, i - 1, dim)].red;
			sum.green += (int) src[RIDX(dim - 1, i - 1, dim)].green;
			sum.blue += (int) src[RIDX(dim - 1, i - 1, dim)].blue;

			sum.red += (int) src[RIDX(dim - 2, i - 1, dim)].red;
			sum.green += (int) src[RIDX(dim - 2, i - 1, dim)].green;
			sum.blue += (int) src[RIDX(dim - 2, i - 1, dim)].blue;

			AVG_SUM(current_pixel, sum);
			dst[RIDX(dim - 1, i, dim)] = current_pixel; //setting the current pixel here
		}

		//bottom left corner
		sum.red = sum.green = sum.blue = 0;
    	sum.num = 4;
		//all four cases
		sum.red += (int) src[RIDX(0, dim - 1, dim)].red;
		sum.green += (int) src[RIDX(0, dim - 1, dim)].green;
		sum.blue += (int) src[RIDX(0, dim - 1, dim)].blue;

		sum.red += (int) src[RIDX(0, dim - 2, dim)].red;
		sum.green += (int) src[RIDX(0, dim - 2, dim)].green;
		sum.blue += (int) src[RIDX(0, dim - 2, dim)].blue;

		sum.red += (int) src[RIDX(1, dim - 2, dim)].red;
		sum.green += (int) src[RIDX(1, dim - 2, dim)].green;
		sum.blue += (int) src[RIDX(1, dim - 2, dim)].blue;

		sum.red += (int) src[RIDX(1, dim - 1, dim)].red;
		sum.green += (int) src[RIDX(1, dim - 1, dim)].green;
		sum.blue += (int) src[RIDX(1, dim - 1, dim)].blue;
		
		AVG_SUM(current_pixel, sum);
		dst[RIDX(0, dim - 1, dim)] = current_pixel; //setting the current pixel here

		//bottommost row w/o corners
		for(i = 1; i < dim - 1; i++){
			sum.red = sum.green = sum.blue = 0;
			sum.num = 6;
			//all four cases
			sum.red += (int) src[RIDX(i, dim - 1, dim)].red;
			sum.green += (int) src[RIDX(i, dim - 1, dim)].green;
			sum.blue += (int) src[RIDX(i, dim - 1, dim)].blue;

			sum.red += (int) src[RIDX(i, dim - 2, dim)].red;
			sum.green += (int) src[RIDX(i, dim - 2, dim)].green;
			sum.blue += (int) src[RIDX(i, dim - 2, dim)].blue;

			sum.red += (int) src[RIDX(i - 1, dim - 1, dim)].red;
			sum.green += (int) src[RIDX(i - 1, dim - 1, dim)].green;
			sum.blue += (int) src[RIDX(i - 1, dim - 1, dim)].blue;

			sum.red += (int) src[RIDX(i - 1, dim - 2, dim)].red;
			sum.green += (int) src[RIDX(i - 1, dim - 2, dim)].green;
			sum.blue += (int) src[RIDX(i - 1, dim - 2, dim)].blue;

			sum.red += (int) src[RIDX(i + 1, dim - 1, dim)].red;
			sum.green += (int) src[RIDX(i + 1, dim - 1, dim)].green;
			sum.blue += (int) src[RIDX(i + 1, dim - 1, dim)].blue;

			sum.red += (int) src[RIDX(i + 1, dim - 2, dim)].red;
			sum.green += (int) src[RIDX(i + 1, dim - 2, dim)].green;
			sum.blue += (int) src[RIDX(i + 1, dim - 2, dim)].blue;

			AVG_SUM(current_pixel, sum);
			dst[RIDX(i, dim - 1, dim)] = current_pixel; //setting the current pixel here
		}

		//bottom right corner
		sum.red = sum.green = sum.blue = 0;
    	sum.num = 4;
		//all four cases
		sum.red += (int) src[RIDX(dim - 1, dim - 1, dim)].red;
		sum.green += (int) src[RIDX(dim - 1, dim - 1, dim)].green;
		sum.blue += (int) src[RIDX(dim - 1, dim - 1, dim)].blue;

		sum.red += (int) src[RIDX(dim - 1, dim - 2, dim)].red;
		sum.green += (int) src[RIDX(dim - 1, dim - 2, dim)].green;
		sum.blue += (int) src[RIDX(dim - 1, dim - 2, dim)].blue;

		sum.red += (int) src[RIDX(dim - 2, dim - 2, dim)].red;
		sum.green += (int) src[RIDX(dim - 2, dim - 2, dim)].green;
		sum.blue += (int) src[RIDX(dim - 2, dim - 2, dim)].blue;

		sum.red += (int) src[RIDX(dim - 2, dim - 1, dim)].red;
		sum.green += (int) src[RIDX(dim - 2, dim - 1, dim)].green;
		sum.blue += (int) src[RIDX(dim - 2, dim - 1, dim)].blue;

		AVG_SUM(current_pixel, sum);
		dst[RIDX(dim - 1, dim - 1, dim)] = current_pixel; //setting the current pixel here
}

#define BS 32

char smooth3_descr[] = "smooth3";
void smooth3(int dim, pixel *src, pixel *dst) 
{
    int i, j;
	int length = dim*dim;
	int edge_length = length - 1;

		pixel_sum sum;
		pixel current_pixel;

		//top left corner
		sum.red = sum.green = sum.blue = 0;
    	sum.num = 4;
		//all four cases
		sum.red += (int) src[RIDX(0, 0, dim)].red;
		sum.green += (int) src[RIDX(0, 0, dim)].green;
		sum.blue += (int) src[RIDX(0, 0, dim)].blue;

		sum.red += (int) src[RIDX(0, 1, dim)].red;
		sum.green += (int) src[RIDX(0, 1, dim)].green;
		sum.blue += (int) src[RIDX(0, 1, dim)].blue;

		sum.red += (int) src[RIDX(1, 0, dim)].red;
		sum.green += (int) src[RIDX(1, 0, dim)].green;
		sum.blue += (int) src[RIDX(1, 0, dim)].blue;

		sum.red += (int) src[RIDX(1, 1, dim)].red;
		sum.green += (int) src[RIDX(1, 1, dim)].green;
		sum.blue += (int) src[RIDX(1, 1, dim)].blue;

		
		AVG_SUM(current_pixel, sum);
		dst[0] = current_pixel; //setting the current pixel here

		//topmost row w/o corners
		for(i = 1; i < dim - 1; i++){
			sum.red = sum.green = sum.blue = 0;
			sum.num = 6;
			//all four cases
			sum.red += (int) src[RIDX(i - 1, 0, dim)].red;
			sum.green += (int) src[RIDX(i - 1, 0, dim)].green;
			sum.blue += (int) src[RIDX(i - 1, 0, dim)].blue;

			sum.red += (int) src[RIDX(i - 1, 1, dim)].red;
			sum.green += (int) src[RIDX(i - 1, 1, dim)].green;
			sum.blue += (int) src[RIDX(i - 1, 1, dim)].blue;

			sum.red += (int) src[RIDX(i, 0, dim)].red;
			sum.green += (int) src[RIDX(i, 0, dim)].green;
			sum.blue += (int) src[RIDX(i, 0, dim)].blue;

			sum.red += (int) src[RIDX(i, 1, dim)].red;
			sum.green += (int) src[RIDX(i, 1, dim)].green;
			sum.blue += (int) src[RIDX(i, 1, dim)].blue;

			sum.red += (int) src[RIDX(i + 1, 0, dim)].red;
			sum.green += (int) src[RIDX(i + 1, 0, dim)].green;
			sum.blue += (int) src[RIDX(i + 1, 0, dim)].blue;

			sum.red += (int) src[RIDX(i + 1, 1, dim)].red;
			sum.green += (int) src[RIDX(i + 1, 1, dim)].green;
			sum.blue += (int) src[RIDX(i + 1, 1, dim)].blue;

			AVG_SUM(current_pixel, sum);
			dst[RIDX(i, 0, dim)] = current_pixel; //setting the current pixel here
		}

		//top right corner
		sum.red = sum.green = sum.blue = 0;
    	sum.num = 4;
		//all four cases
		sum.red += (int) src[RIDX(dim - 1, 0, dim)].red;
		sum.green += (int) src[RIDX(dim - 1, 0, dim)].green;
		sum.blue += (int) src[RIDX(dim - 1, 0, dim)].blue;

		sum.red += (int) src[RIDX(dim - 2, 0, dim)].red;
		sum.green += (int) src[RIDX(dim - 2, 0, dim)].green;
		sum.blue += (int) src[RIDX(dim - 2, 0, dim)].blue;

		sum.red += (int) src[RIDX(dim - 1, 1, dim)].red;
		sum.green += (int) src[RIDX(dim - 1, 1, dim)].green;
		sum.blue += (int) src[RIDX(dim - 1, 1, dim)].blue;

		sum.red += (int) src[RIDX(dim - 2, 1, dim)].red;
		sum.green += (int) src[RIDX(dim - 2, 1, dim)].green;
		sum.blue += (int) src[RIDX(dim - 2, 1, dim)].blue;

		AVG_SUM(current_pixel, sum);
		dst[RIDX(dim - 1, 0, dim)] = current_pixel; //setting the current pixel here

		//leftmost column w/o corners
		for(i = 1; i < dim - 1; i++){
			sum.red = sum.green = sum.blue = 0;
			sum.num = 6;
			//all four cases
			sum.red += (int) src[RIDX(0, i, dim)].red;
			sum.green += (int) src[RIDX(0, i, dim)].green;
			sum.blue += (int) src[RIDX(0, i, dim)].blue;

			sum.red += (int) src[RIDX(1, i, dim)].red;
			sum.green += (int) src[RIDX(1, i, dim)].green;
			sum.blue += (int) src[RIDX(1, i, dim)].blue;

			sum.red += (int) src[RIDX(0, i + 1, dim)].red;
			sum.green += (int) src[RIDX(0, i + 1, dim)].green;
			sum.blue += (int) src[RIDX(0, i + 1, dim)].blue;

			sum.red += (int) src[RIDX(1, i + 1, dim)].red;
			sum.green += (int) src[RIDX(1, i + 1, dim)].green;
			sum.blue += (int) src[RIDX(1, i + 1, dim)].blue;

			sum.red += (int) src[RIDX(0, i - 1, dim)].red;
			sum.green += (int) src[RIDX(0, i - 1, dim)].green;
			sum.blue += (int) src[RIDX(0, i - 1, dim)].blue;

			sum.red += (int) src[RIDX(1, i - 1, dim)].red;
			sum.green += (int) src[RIDX(1, i - 1, dim)].green;
			sum.blue += (int) src[RIDX(1, i - 1, dim)].blue;

			AVG_SUM(current_pixel, sum);
			dst[RIDX(0, i, dim)] = current_pixel; //setting the current pixel here
		}

		//loop for all inner pixels
		for(i = 1; i < dim-1; i++){

			//the first column


			
			for(j = 1; j < dim-1; j++){
				//initialize the pixel
				
				sum.red = sum.green = sum.blue = 0;
    			sum.num = 9;

				//all nine cases
				sum.red += (int) src[RIDX(i-1, j-1, dim)].red;
				sum.green += (int) src[RIDX(i-1, j-1, dim)].green;
				sum.blue += (int) src[RIDX(i-1, j-1, dim)].blue;

				sum.red += (int) src[RIDX(i, j, dim)].red;
				sum.green += (int) src[RIDX(i, j, dim)].green;
				sum.blue += (int) src[RIDX(i, j, dim)].blue;

				sum.red += (int) src[RIDX(i+1, j+1, dim)].red;
				sum.green += (int) src[RIDX(i+1, j+1, dim)].green;
				sum.blue += (int) src[RIDX(i+1, j+1, dim)].blue;

				sum.red += (int) src[RIDX(i-1, j+1, dim)].red;
				sum.green += (int) src[RIDX(i-1, j+1, dim)].green;
				sum.blue += (int) src[RIDX(i-1, j+1, dim)].blue;

				sum.red += (int) src[RIDX(i+1, j-1, dim)].red;
				sum.green += (int) src[RIDX(i+1, j-1, dim)].green;
				sum.blue += (int) src[RIDX(i+1, j-1, dim)].blue;

				sum.red += (int) src[RIDX(i+1, j, dim)].red;
				sum.green += (int) src[RIDX(i+1, j, dim)].green;
				sum.blue += (int) src[RIDX(i+1, j, dim)].blue;

				sum.red += (int) src[RIDX(i-1, j, dim)].red;
				sum.green += (int) src[RIDX(i-1, j, dim)].green;
				sum.blue += (int) src[RIDX(i-1, j, dim)].blue;

				sum.red += (int) src[RIDX(i, j+1, dim)].red;
				sum.green += (int) src[RIDX(i, j+1, dim)].green;
				sum.blue += (int) src[RIDX(i, j+1, dim)].blue;

				sum.red += (int) src[RIDX(i, j-1, dim)].red;
				sum.green += (int) src[RIDX(i, j-1, dim)].green;
				sum.blue += (int) src[RIDX(i, j-1, dim)].blue;


				AVG_SUM(current_pixel, sum);
				dst[RIDX(i, j, dim)] = current_pixel; //setting the current pixel here
			}

			
		}

		//rightmost column w/o corners
		for(i = 1; i < dim - 1; i++){
			sum.red = sum.green = sum.blue = 0;
			sum.num = 6;
			//all four cases
			sum.red += (int) src[RIDX(dim - 1, i, dim)].red;
			sum.green += (int) src[RIDX(dim - 1, i, dim)].green;
			sum.blue += (int) src[RIDX(dim - 1, i, dim)].blue;

			sum.red += (int) src[RIDX(dim - 2, i, dim)].red;
			sum.green += (int) src[RIDX(dim - 2, i, dim)].green;
			sum.blue += (int) src[RIDX(dim - 2, i, dim)].blue;

			sum.red += (int) src[RIDX(dim - 1, i + 1, dim)].red;
			sum.green += (int) src[RIDX(dim - 1, i + 1, dim)].green;
			sum.blue += (int) src[RIDX(dim - 1, i + 1, dim)].blue;

			sum.red += (int) src[RIDX(dim - 2, i + 1, dim)].red;
			sum.green += (int) src[RIDX(dim - 2, i + 1, dim)].green;
			sum.blue += (int) src[RIDX(dim - 2, i + 1, dim)].blue;

			sum.red += (int) src[RIDX(dim - 1, i - 1, dim)].red;
			sum.green += (int) src[RIDX(dim - 1, i - 1, dim)].green;
			sum.blue += (int) src[RIDX(dim - 1, i - 1, dim)].blue;

			sum.red += (int) src[RIDX(dim - 2, i - 1, dim)].red;
			sum.green += (int) src[RIDX(dim - 2, i - 1, dim)].green;
			sum.blue += (int) src[RIDX(dim - 2, i - 1, dim)].blue;

			AVG_SUM(current_pixel, sum);
			dst[RIDX(dim - 1, i, dim)] = current_pixel; //setting the current pixel here
		}

		//bottom left corner
		sum.red = sum.green = sum.blue = 0;
    	sum.num = 4;
		//all four cases
		sum.red += (int) src[RIDX(0, dim - 1, dim)].red;
		sum.green += (int) src[RIDX(0, dim - 1, dim)].green;
		sum.blue += (int) src[RIDX(0, dim - 1, dim)].blue;

		sum.red += (int) src[RIDX(0, dim - 2, dim)].red;
		sum.green += (int) src[RIDX(0, dim - 2, dim)].green;
		sum.blue += (int) src[RIDX(0, dim - 2, dim)].blue;

		sum.red += (int) src[RIDX(1, dim - 2, dim)].red;
		sum.green += (int) src[RIDX(1, dim - 2, dim)].green;
		sum.blue += (int) src[RIDX(1, dim - 2, dim)].blue;

		sum.red += (int) src[RIDX(1, dim - 1, dim)].red;
		sum.green += (int) src[RIDX(1, dim - 1, dim)].green;
		sum.blue += (int) src[RIDX(1, dim - 1, dim)].blue;
		
		AVG_SUM(current_pixel, sum);
		dst[RIDX(0, dim - 1, dim)] = current_pixel; //setting the current pixel here

		//bottommost row w/o corners
		for(i = 1; i < dim - 1; i++){
			sum.red = sum.green = sum.blue = 0;
			sum.num = 6;
			//all four cases
			sum.red += (int) src[RIDX(i, dim - 1, dim)].red;
			sum.green += (int) src[RIDX(i, dim - 1, dim)].green;
			sum.blue += (int) src[RIDX(i, dim - 1, dim)].blue;

			sum.red += (int) src[RIDX(i, dim - 2, dim)].red;
			sum.green += (int) src[RIDX(i, dim - 2, dim)].green;
			sum.blue += (int) src[RIDX(i, dim - 2, dim)].blue;

			sum.red += (int) src[RIDX(i - 1, dim - 1, dim)].red;
			sum.green += (int) src[RIDX(i - 1, dim - 1, dim)].green;
			sum.blue += (int) src[RIDX(i - 1, dim - 1, dim)].blue;

			sum.red += (int) src[RIDX(i - 1, dim - 2, dim)].red;
			sum.green += (int) src[RIDX(i - 1, dim - 2, dim)].green;
			sum.blue += (int) src[RIDX(i - 1, dim - 2, dim)].blue;

			sum.red += (int) src[RIDX(i + 1, dim - 1, dim)].red;
			sum.green += (int) src[RIDX(i + 1, dim - 1, dim)].green;
			sum.blue += (int) src[RIDX(i + 1, dim - 1, dim)].blue;

			sum.red += (int) src[RIDX(i + 1, dim - 2, dim)].red;
			sum.green += (int) src[RIDX(i + 1, dim - 2, dim)].green;
			sum.blue += (int) src[RIDX(i + 1, dim - 2, dim)].blue;

			AVG_SUM(current_pixel, sum);
			dst[RIDX(i, dim - 1, dim)] = current_pixel; //setting the current pixel here
		}

		//bottom right corner
		sum.red = sum.green = sum.blue = 0;
    	sum.num = 4;
		//all four cases
		sum.red += (int) src[RIDX(dim - 1, dim - 1, dim)].red;
		sum.green += (int) src[RIDX(dim - 1, dim - 1, dim)].green;
		sum.blue += (int) src[RIDX(dim - 1, dim - 1, dim)].blue;

		sum.red += (int) src[RIDX(dim - 1, dim - 2, dim)].red;
		sum.green += (int) src[RIDX(dim - 1, dim - 2, dim)].green;
		sum.blue += (int) src[RIDX(dim - 1, dim - 2, dim)].blue;

		sum.red += (int) src[RIDX(dim - 2, dim - 2, dim)].red;
		sum.green += (int) src[RIDX(dim - 2, dim - 2, dim)].green;
		sum.blue += (int) src[RIDX(dim - 2, dim - 2, dim)].blue;

		sum.red += (int) src[RIDX(dim - 2, dim - 1, dim)].red;
		sum.green += (int) src[RIDX(dim - 2, dim - 1, dim)].green;
		sum.blue += (int) src[RIDX(dim - 2, dim - 1, dim)].blue;

		AVG_SUM(current_pixel, sum);
		dst[RIDX(dim - 1, dim - 1, dim)] = current_pixel; //setting the current pixel here
		
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */

char smooth2_descr[] = "super smooth";
void smooth2(int n, pixel *src, pixel *dst){
	pixel_sum sum;
	pixel current_pixel;
	int length = n*n;
	int edge_length = length - n;
	int i;
	int col, loc;

#if 1
	if(n == 32 && BS == 32){
		//first pixel
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, 0);
		SUM_IDX(sum, src, 1);
		SUM_IDX(sum, src, n);
		SUM_IDX(sum, src, n+1);
		sum.num = 4;
		AVG_SUM(current_pixel, sum);
		dst[0] = current_pixel;
		for(i = 1; i < BS-1; i++){
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, i);
			SUM_IDX(sum, src, i+1);
			SUM_IDX(sum, src, i-1);
			SUM_IDX(sum, src, i+n);
			SUM_IDX(sum, src, i+n+1);
			SUM_IDX(sum, src, i+n-1);
			sum.num = 6;
			AVG_SUM(current_pixel, sum);
			dst[i] = current_pixel;
		}
		//last pixel
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, n-1);
		SUM_IDX(sum, src, n-2);
		SUM_IDX(sum, src, n+n-2);
		SUM_IDX(sum, src, n+n-1);
		sum.num = 4;
		AVG_SUM(current_pixel, sum);
		dst[n-1] = current_pixel;
		//first column blocks
		for(loc = n; loc < edge_length; loc += (n+n)){
			//first pixel	
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, loc);
			SUM_IDX(sum, src, loc+1);
			SUM_IDX(sum, src, loc-n);
			SUM_IDX(sum, src, loc-n+1);
			SUM_IDX(sum, src, loc+n);
			SUM_IDX(sum, src, loc+n+1);
			sum.num = 6;
			AVG_SUM(current_pixel, sum);
			dst[loc] = current_pixel;
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, loc+n);
			SUM_IDX(sum, src, loc+n+1);
			SUM_IDX(sum, src, loc+n-n);
			SUM_IDX(sum, src, loc+n-n+1);
			SUM_IDX(sum, src, loc+n+n);
			SUM_IDX(sum, src, loc+n+n+1);
			sum.num = 6;
			AVG_SUM(current_pixel, sum);
			dst[loc+n] = current_pixel;
			//rest of the block
			for(i = loc + 1; i < loc+BS-1; i++){
				sum.red = sum.green = sum.blue = 0;
				SUM_IDX(sum, src, i);
				SUM_IDX(sum, src, i-1);
				SUM_IDX(sum, src, i+1);
				SUM_IDX(sum, src, i+n);
				SUM_IDX(sum, src, i+n+1);
				SUM_IDX(sum, src, i+n-1);
				SUM_IDX(sum, src, i-n);
				SUM_IDX(sum, src, i-n+1);
				SUM_IDX(sum, src, i-n-1);
				sum.num = 9;
				AVG_SUM(current_pixel, sum);
				dst[i] = current_pixel;

				sum.red = sum.green = sum.blue = 0;
				SUM_IDX(sum, src, (i+n));
				SUM_IDX(sum, src, (i+n)-1);
				SUM_IDX(sum, src, (i+n)+1);
				SUM_IDX(sum, src, (i+n)+n);
				SUM_IDX(sum, src, (i+n)+n+1);
				SUM_IDX(sum, src, (i+n)+n-1);
				SUM_IDX(sum, src, (i+n)-n);
				SUM_IDX(sum, src, (i+n)-n+1);
				SUM_IDX(sum, src, (i+n)-n-1);
				sum.num = 9;
				AVG_SUM(current_pixel, sum);
				dst[(i+n)] = current_pixel;
			}
			//last pixel
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, loc+BS-1);
			SUM_IDX(sum, src, loc+BS-2);
			SUM_IDX(sum, src, loc+BS-1-n);
			SUM_IDX(sum, src, loc+BS-2-n);
			SUM_IDX(sum, src, loc+BS-1+n);
			SUM_IDX(sum, src, loc+BS-2+n);
			sum.num = 6;
			AVG_SUM(current_pixel, sum);
			dst[loc+BS-1] = current_pixel;
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, loc+n+BS-1);
			SUM_IDX(sum, src, loc+n+BS-2);
			SUM_IDX(sum, src, loc+n+BS-1-n);
			SUM_IDX(sum, src, loc+n+BS-2-n);
			SUM_IDX(sum, src, loc+n+BS-1+n);
			SUM_IDX(sum, src, loc+n+BS-2+n);
			sum.num = 6;
			AVG_SUM(current_pixel, sum);
			dst[loc+n+BS-1] = current_pixel;
		}
		//first column last block
		//first pixel
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, edge_length);
		SUM_IDX(sum, src, edge_length+1);
		SUM_IDX(sum, src, edge_length-n);
		SUM_IDX(sum, src, edge_length-n+1);
		sum.num = 4;
		AVG_SUM(current_pixel, sum);
		dst[edge_length] = current_pixel;
		//rest of last block
		for(i = edge_length+1; i < edge_length+BS-1; i++){
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, i);
			SUM_IDX(sum, src, i+1);
			SUM_IDX(sum, src, i-1);
			SUM_IDX(sum, src, i-n);
			SUM_IDX(sum, src, i-n+1);
			SUM_IDX(sum, src, i-n-1);
			sum.num = 6;
			AVG_SUM(current_pixel, sum);
			dst[i] = current_pixel;
		}
		//last pixel
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, length-1);
		SUM_IDX(sum, src, length-2);
		SUM_IDX(sum, src, length-n-1);
		SUM_IDX(sum, src, length-n-2);
		sum.num = 4;
		AVG_SUM(current_pixel, sum);
		dst[length-1] = current_pixel;
		return;
	}
#endif




	//first column first block
	//first pixel
	sum.red = sum.green = sum.blue = 0;
	SUM_IDX(sum, src, 0);
	SUM_IDX(sum, src, 1);
	SUM_IDX(sum, src, n);
	SUM_IDX(sum, src, n+1);
	sum.num = 4;
	AVG_SUM(current_pixel, sum);
	dst[0] = current_pixel;
	//rest of first block
	for(i = 1; i < BS; i++){
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, i);
		SUM_IDX(sum, src, i+1);
		SUM_IDX(sum, src, i-1);
		SUM_IDX(sum, src, i+n);
		SUM_IDX(sum, src, i+n+1);
		SUM_IDX(sum, src, i+n-1);
		sum.num = 6;
		AVG_SUM(current_pixel, sum);
		dst[i] = current_pixel;
	}
	//first column blocks
	for(loc = n; loc < edge_length; loc += n){
		//first pixel	
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, loc);
		SUM_IDX(sum, src, loc+1);
		SUM_IDX(sum, src, loc-n);
		SUM_IDX(sum, src, loc-n+1);
		SUM_IDX(sum, src, loc+n);
		SUM_IDX(sum, src, loc+n+1);
		sum.num = 6;
		AVG_SUM(current_pixel, sum);
		dst[loc] = current_pixel;
		//rest of the block
		for(i = loc + 1; i < loc+BS; i++){
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, i);
			SUM_IDX(sum, src, i-1);
			SUM_IDX(sum, src, i+1);
			SUM_IDX(sum, src, i+n);
			SUM_IDX(sum, src, i+n+1);
			SUM_IDX(sum, src, i+n-1);
			SUM_IDX(sum, src, i-n);
			SUM_IDX(sum, src, i-n+1);
			SUM_IDX(sum, src, i-n-1);
			sum.num = 9;
			AVG_SUM(current_pixel, sum);
			dst[i] = current_pixel;
		}
	}

	//first column last block
	//first pixel
	sum.red = sum.green = sum.blue = 0;
	SUM_IDX(sum, src, edge_length);
	SUM_IDX(sum, src, edge_length+1);
	SUM_IDX(sum, src, edge_length-n);
	SUM_IDX(sum, src, edge_length-n+1);
	sum.num = 4;
	AVG_SUM(current_pixel, sum);
	dst[edge_length] = current_pixel;
	//rest of last block
	for(i = edge_length+1; i < edge_length+BS; i++){
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, i);
		SUM_IDX(sum, src, i+1);
		SUM_IDX(sum, src, i-1);
		SUM_IDX(sum, src, i-n);
		SUM_IDX(sum, src, i-n+1);
		SUM_IDX(sum, src, i-n-1);
		sum.num = 6;
		AVG_SUM(current_pixel, sum);
		dst[i] = current_pixel;
	}

	//all middle columns
	for(col = BS; col < n - BS; col += BS){
	//////////////////////////////////////
		/*first block in column*/
		for(i = col; i < col+BS; i++){
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, i);
			SUM_IDX(sum, src, i-1);
			SUM_IDX(sum, src, i+1);
			SUM_IDX(sum, src, i+n);
			SUM_IDX(sum, src, i+n+1);
			SUM_IDX(sum, src, i+n-1);
			sum.num = 6;
			AVG_SUM(current_pixel, sum);
			dst[i] = current_pixel;
		}
		/*blocks in column (except edges)*/
		for(loc = col + n; loc < edge_length; loc += (n+n)){
			//////////////////////////////////
			/*no-edge block*/
			for(i = loc; i < loc+BS; i+=2){
				sum.red = sum.green = sum.blue = 0;
				SUM_IDX(sum, src, i);
				SUM_IDX(sum, src, i-1);
				SUM_IDX(sum, src, i+1);
				SUM_IDX(sum, src, i+n);
				SUM_IDX(sum, src, i+n+1);
				SUM_IDX(sum, src, i+n-1);
				SUM_IDX(sum, src, i-n);
				SUM_IDX(sum, src, i-n+1);
				SUM_IDX(sum, src, i-n-1);
				sum.num = 9;
				AVG_SUM(current_pixel, sum);
				dst[i] = current_pixel;
				//delete this one
				sum.red = sum.green = sum.blue = 0;
				SUM_IDX(sum, src, (i+1));
				SUM_IDX(sum, src, (i+1)-1);
				SUM_IDX(sum, src, (i+1)+1);
				SUM_IDX(sum, src, (i+1)+n);
				SUM_IDX(sum, src, (i+1)+n+1);
				SUM_IDX(sum, src, (i+1)+n-1);
				SUM_IDX(sum, src, (i+1)-n);
				SUM_IDX(sum, src, (i+1)-n+1);
				SUM_IDX(sum, src, (i+1)-n-1);
				sum.num = 9;
				AVG_SUM(current_pixel, sum);
				dst[(i+1)] = current_pixel;

				sum.red = sum.green = sum.blue = 0;
				SUM_IDX(sum, src, (i+n));
				SUM_IDX(sum, src, (i+n)-1);
				SUM_IDX(sum, src, (i+n)+1);
				SUM_IDX(sum, src, (i+n)+n);
				SUM_IDX(sum, src, (i+n)+n+1);
				SUM_IDX(sum, src, (i+n)+n-1);
				SUM_IDX(sum, src, (i+n)-n);
				SUM_IDX(sum, src, (i+n)-n+1);
				SUM_IDX(sum, src, (i+n)-n-1);
				sum.num = 9;
				AVG_SUM(current_pixel, sum);
				dst[(i+n)] = current_pixel;

				sum.red = sum.green = sum.blue = 0;
				SUM_IDX(sum, src, ((i+1)+n));
				SUM_IDX(sum, src, ((i+1)+n)-1);
				SUM_IDX(sum, src, ((i+1)+n)+1);
				SUM_IDX(sum, src, ((i+1)+n)+n);
				SUM_IDX(sum, src, ((i+1)+n)+n+1);
				SUM_IDX(sum, src, ((i+1)+n)+n-1);
				SUM_IDX(sum, src, ((i+1)+n)-n);
				SUM_IDX(sum, src, ((i+1)+n)-n+1);
				SUM_IDX(sum, src, ((i+1)+n)-n-1);
				sum.num = 9;
				AVG_SUM(current_pixel, sum);
				dst[((i+1)+n)] = current_pixel;
			}
			/////////////////////////////////
		}
		/*last block in column*/
		for(i = edge_length+col; i < edge_length+col+BS; i++){
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, i);
			SUM_IDX(sum, src, i-1);
			SUM_IDX(sum, src, i+1);
			SUM_IDX(sum, src, i-n);
			SUM_IDX(sum, src, i-n+1);
			SUM_IDX(sum, src, i-n-1);
			sum.num = 6;
			AVG_SUM(current_pixel, sum);
			dst[i] = current_pixel;
		}
		//////////////////////////////////////
	}
	
	//last column first block
	//most of block
	for(i = n-BS; i < n-1; i++){
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, i);
		SUM_IDX(sum, src, i-1);
		SUM_IDX(sum, src, i+1);
		SUM_IDX(sum, src, i+n);
		SUM_IDX(sum, src, i+n+1);
		SUM_IDX(sum, src, i+n-1);
		sum.num = 6;
		AVG_SUM(current_pixel, sum);
		dst[i] = current_pixel;
	}
	//last pixel
	sum.red = sum.green = sum.blue = 0;
	SUM_IDX(sum, src, n-1);
	SUM_IDX(sum, src, n-2);
	SUM_IDX(sum, src, n+n-2);
	SUM_IDX(sum, src, n+n-1);
	sum.num = 4;
	AVG_SUM(current_pixel, sum);
	dst[n-1] = current_pixel;

	//last column blocks
	for(loc = n+n-BS; loc < edge_length; loc += n){
		for(i = loc; i < loc+BS-1; i++){
			sum.red = sum.green = sum.blue = 0;
			SUM_IDX(sum, src, i);
			SUM_IDX(sum, src, i-1);
			SUM_IDX(sum, src, i+1);
			SUM_IDX(sum, src, i+n);
			SUM_IDX(sum, src, i+n+1);
			SUM_IDX(sum, src, i+n-1);
			SUM_IDX(sum, src, i-n);
			SUM_IDX(sum, src, i-n+1);
			SUM_IDX(sum, src, i-n-1);
			sum.num = 9;
			AVG_SUM(current_pixel, sum);
			dst[i] = current_pixel;
		}
		//last pixel
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, loc+BS-1);
		SUM_IDX(sum, src, loc+BS-2);
		SUM_IDX(sum, src, loc+BS-1-n);
		SUM_IDX(sum, src, loc+BS-2-n);
		SUM_IDX(sum, src, loc+BS-1+n);
		SUM_IDX(sum, src, loc+BS-2+n);
		sum.num = 6;
		AVG_SUM(current_pixel, sum);
		dst[loc+BS-1] = current_pixel;
	}
	//last column last block
	//most of block
	for(i = length-BS; i < length-1; i++){
		sum.red = sum.green = sum.blue = 0;
		SUM_IDX(sum, src, i);
		SUM_IDX(sum, src, i-1);
		SUM_IDX(sum, src, i+1);
		SUM_IDX(sum, src, i-n);
		SUM_IDX(sum, src, i-n+1);
		SUM_IDX(sum, src, i-n-1);
		sum.num = 6;
		AVG_SUM(current_pixel, sum);
		dst[i] = current_pixel;
	}
	//last pixel
	sum.red = sum.green = sum.blue = 0;
	SUM_IDX(sum, src, length-1);
	SUM_IDX(sum, src, length-2);
	SUM_IDX(sum, src, length-n-1);
	SUM_IDX(sum, src, length-n-2);
	sum.num = 4;
	AVG_SUM(current_pixel, sum);
	dst[length-1] = current_pixel;
}



/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
	add_smooth_function(&smooth2, smooth2_descr);
	add_smooth_function(&smooth3, smooth3_descr);
    /* ... Register additional test functions here */
}

