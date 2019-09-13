/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	if(M == 32){
	/*the 32x32 case works by unrolling the loop by 8 and using 8 separate local variables to
	temporarily store data from A and then transfer the data to the B matrix*/
        int var1, var2, var3, var4, var5, var6, var7, var8;
        int i, j;
        for (i = 0; i < N; i+=8) {
            for (j = 0; j < M; j++) {
                var1 = A[i][j];
                var2 = A[i+1][j];
                var3 = A[i+2][j];
                var4 = A[i+3][j];   
                var5 = A[i+4][j];
                var6 = A[i+5][j];
                var7 = A[i+6][j];
                var8 = A[i+7][j];

                B[j][i] = var1;
                B[j][i+1] = var2;
                B[j][i+2] = var3;
                B[j][i+3] = var4;
                B[j][i+4] = var5;
                B[j][i+5] = var6;
                B[j][i+6] = var7;
                B[j][i+7] = var8;
            }
        }
    }
	else if(M == 64){
	/*the 64x64 case works by unrolling the outer loop by 4 and the inner loop by 2*/
		int i, j, var0, var1, var2, var3, var4, var5, var6, var7;
		for (i = 0; i < N; i+=4) {
			for (j = 0; j < M; j+=2) {
			    var0 = A[i  ][j  ];
				var1 = A[i+1][j  ];
			    var2 = A[i+2][j  ];
			    var3 = A[i+3][j  ];
			    var4 = A[i  ][j+1];
				var5 = A[i+1][j+1];
			    var6 = A[i+2][j+1];
			    var7 = A[i+3][j+1];

			    B[j  ][i  ] = var0;			    
			    B[j  ][i+1] = var1;
			    B[j  ][i+2] = var2;
			    B[j  ][i+3] = var3;
			    B[j+1][i  ] = var4;			    
			    B[j+1][i+1] = var5;
			    B[j+1][i+2] = var6;
			    B[j+1][i+3] = var7;
			}
		}
	}
	else if(M == 61) {
	/*the 61x67 case works by blocking with a blocksize of 16 and having a special case 
	for the elements along the diagonal that would otherwise cause thrashing*/
	
        int x, y, i, j, tmp, blocksize = 16; //declaring variables, blocksize chosen through trial and error
        for (x = 0; x < N; x+=blocksize) { //iterating through matrix rows
            for (y = 0; y < M; y+=blocksize) { //iterating through matrix columns
                for(i = x; (i < x+blocksize) && (i < N); i++){ //iterating through rows in block
                    for(j = y; (j < y+blocksize) && (j < M); j++){ //iterating through columns in block
                        if(i != j){ //check if the indexes are equal to avoid a cache overwrite 
                            B[j][i] = A[i][j]; //swap
                        }else{
                            tmp = A[i][j]; //store the swapping value in temporary variable
                        }
                    }
                    if(x == y){ //if the block is on the diagonal of the matrix
                        B[i][i] = tmp; //swap the B value with temporary variable
                    }
                }
            }
        }  
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

