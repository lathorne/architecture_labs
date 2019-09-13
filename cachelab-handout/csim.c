/*
Team:
Hyrum Gunther - hygunth - 49-553-4000
Logan Thornloe - lathorne - 71-211-4132
*/

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

struct line {
	bool valid;
	int tag_bits;
	int lru;
};

typedef char* string;

int main(int argc, char *argv[])
{

//******************Initialize Array
	string filename;
	bool verbose = false;
	char opt;

	int s = 0; //set index bits
	int E = 0; //number of lines per set
	int b = 0; //block size

	//****************Implement Command Line Arguments using getopt
	while((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1){
		switch(opt){
			case 'h':
				 printf("-h: Optional help flag that prints usage info\n-v: Optional verbose flag that displays trace info\n-s <s>: Number of set index bits (S = 2^s is the number of sets)\n-E <E>: Associativity (number of lines per set)\n-b <b>: Number of block bits (B = 2^b is the block size)\n-t <tracefile>: Name of the valgrind trace to replay\n");
			break;

			case 'v':
				verbose = true;
			break;

			case 's':
				s = atoi(optarg);
			break;

			case 'E':
				E = atoi(optarg);
			break;

			case 'b':
				b = atoi(optarg);
			break;

			case 't':
				filename = optarg;
			break;
		}
	}
	//****************Done Implementing Command Line Arguments
	int S = pow(2,s); //number of sets
	struct line *mem = calloc(S * E, sizeof(struct line)); //allocate memory and fill will 0's
//******************Done with Initializing Array

//******************Parsing and Reading File
	int hitCount = 0;
	int missCount = 0;
	int eviCount = 0;	

	FILE *ptr = fopen(filename, "r"); //open the file
	char op;
	int addr;
	int size;	

	while(!feof(ptr)){
		
		//read an instruction from the trace file and put the information in variables
		//(print it if verbose)
		fscanf(ptr, " %c %x,%x\r\n", &op, &addr, &size);
		if(verbose) printf(" %c %x,%x ", op, addr, size);

		//calculate set we are in
		unsigned set_num = addr << (64 - b - s);
		set_num = set_num >> (64 - s);

		//calculate block need
		unsigned block_offset = addr << (64 - b);
		block_offset = block_offset >> (64 - b);

		//calculate tag bits
		unsigned tag = addr >> (b + s);
		
		//select number of times to run code based on instruction type
		int times_to_run = (op == 'M') ? 2 : (op == 'I') ? 0 : 1;
		
		//run load or store
		struct line *set;
		while(times_to_run > 0){
			set = (mem + (E*set_num)); //this gets us to our set
			bool miss = true;
			for(int i = 0; i < E; i++){ //going through our lines
				if(((set + i)->tag_bits == tag) && ((set + i)->valid)){ //this is a hit
					hitCount++;
					if(verbose) printf("hit ");
					miss = false;
					(set + i)->lru = 0; //set more recently used to 0
					for(int j = 0; j < E; j++){ //go through set
						((set + j)->lru)++; //increment every lru
					}
				}
			}
			//printf("should have found a hit by now\n");
			if(miss){ //if its a miss
				missCount++;
				if(verbose) printf("miss ");
				//loading stuff in
				int line_rep = 0;
				int largest_lru = 0;
				bool eviction = true;
				for(int i = 0; i < E; i++){ //go through lines
					if(!((set + i)->valid)){
						line_rep = i;
						eviction = false;
						break;
					}
					if(((set + i)->lru) > largest_lru){
						largest_lru = ((set + i)->lru);
						line_rep = i;
					}
				} //now we know the line to replace
				(set + line_rep)->valid = true;
				(set + line_rep)->tag_bits = tag;
				(set + line_rep)->lru = 0;
				for(int j = 0; j < E; j++){ //go through set
					((set + j)->lru)++; //increment every lru
				}
				if(verbose && eviction){
					printf("eviction "); //if tag is valid
				}
				if(eviction){
					eviCount++;
				}
			}
			times_to_run--;
		}
		if(verbose) printf("\n"); //final print for loop
	}
//******************Done with Parsing and Reading File

//******************Print Results and close
    printSummary(hitCount, missCount, eviCount);
	free(mem);
    return 0;
}
