
int result; //rax
int rbx;
int rbp;
int r12d;

int funct4(int x, int y){
//x is equal to rdi (which starts as 6), y = 2nd input (rsi)

	rbx = x;
	
	if(x <= 0){

		return 0;

	}else{
		rbp = y;
		result = y; //this is the return register

		if(x == 1){

			return result;

		}else{

			x--; //decrement x
			funct4(x, y);
			
			//go through this 5 times
			//starting conditions: x = 1, rbx = 2, rbp = y, result  = y;
			r12d = result + rbp * 1;
			x = rbx - 2;
			y = rbp;
	
			funct4(x, y); //last callq

			result += r12d; //
		}

	}

	return result;
}

#include <stdio.h>

int main(){
	//the second argument after being returned must equal whatever our first input 
	int printout;
	//for(int i = 4; i < 100; i++){
		printout = funct4(6, 4);
		printf("OUTPUT: %d\n", printout); //look for 4
	//}	
	

	
}
	
	
