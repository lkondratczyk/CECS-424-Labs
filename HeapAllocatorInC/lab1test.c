#include <stdio.h>
#include <stdlib.h>

const int msize = 8; //increased from sizeof(int) to avoid seg. fault
const int psize = sizeof(void*);
void* free_head;
void* buffer; //separate var for this since free_head changes reference

void my_initialize_heap(int);
void* my_alloc(int);
void my_free();
void test_run();
void print_contents(int);
void populate_block(void*, int);
void free_block(void*, int);

int main(){
	test_run();
	

	return 0;
}

void my_initialize_heap(int size){
	int overhead = msize + psize;
	while(size % psize != 0){
		size ++;
	}
	printf("Size is: %i \n", size);	
	buffer = malloc(size);
	free_head = buffer;
	*((int*)free_head) = (int)(size - overhead);
	*((void**)(free_head + msize)) = NULL;
}

void* my_alloc(int size){
	while(size % psize != 0){
		size ++;
	}	
	if((free_head != NULL)){
		printf("input size is: %i \n", size);
		void* current = free_head;
		void* next = current + msize;
		void* previous = NULL;
		while(*((int*)current) < size && (*((void**)next) != NULL)){
			previous = current;
			current = next;
			next = current + msize;
		}
		if(*((int*)current) >= size) {
			if(*((int*)current) >= size + psize + psize + msize){
				printf("Creating new block \n");
				void* new_block = current + size + msize + psize;
				printf("new block index: %ld \n", new_block - buffer);
				*((void**)(new_block + msize)) = *((void**)(next));
				*((int*)new_block) = *((int*)current) - (int)(size + msize + psize);
				printf("new block size: %i \n", *((int*)new_block));
				*((int*)current) = size;
				
				if (previous == NULL){
					printf("Moving free head \n");
					free_head = new_block;
				} else{
					printf("Re-linking previous \n");
					*((void**)(previous + msize)) = new_block;	
				}
			} else{
				printf("size available: %i \n", *((int*)current));
				printf("Not creating new block \n");
				if (previous != NULL){
				
					printf("Re-linking previous \n");
					*((void**)(previous + msize)) = *((void**)(current + msize));
				}
				else{
					printf("Moving free head \n");
					free_head = *((void**)(free_head + msize));
				}
			}
			*((void**)(current + msize)) = NULL;
			//
			
			populate_block((current + msize + psize), size);
			
			//
			return (void*)(current + msize + psize);
		} else {
			printf("Not enough space!! \n");
			return NULL;
		}
	}	
	else {
		printf("Not enough space!! \n");
		return NULL;
	}
}
void my_free(void* data){
	if(data != NULL){
		printf("Removing data \n");

		*((void**)(data - psize)) = free_head;
		free_head = data - msize - psize;
	}
	else{
		printf("oops\n");
	}

//
	free_block(data, *((int*)(data - msize - psize)));
	//
}

void test_run(){
	
	int size = 72;
	my_initialize_heap(size);	


	void* entry1 = my_alloc(4);
	print_contents(size);
	
	void* entry2 = my_alloc(8);	
	print_contents(size);

	void* entry3 = my_alloc(4);
	print_contents(size);	
	
	void* entry4 = my_alloc(16);
	print_contents(size);
	
	my_free(entry1);
	free_block(entry1, 4);
	print_contents(size);
	
	my_free(entry2);
	free_block(entry2, 8);
	print_contents(size);
	
	
	my_free(entry3);
	free_block(entry3, 4);	
	print_contents(size);
	
	entry2 = my_alloc(8);
	print_contents(size);
	
	entry3 = my_alloc(8);
	print_contents(size);
	
	entry4 = my_alloc(16);
	print_contents(size);
	
	entry1 = my_alloc(8);
	print_contents(size);
	
}

void special_test_run(){
	int size = 120;
	my_initialize_heap(size);

	printf("Test 3:\n");
	int* test3a = my_alloc(sizeof(int));
	populate_block(test3a, (sizeof(int)));
	print_contents(120);
	
	int* test3b = my_alloc(sizeof(int));
	populate_block(test3b, (sizeof(int)));
	print_contents(120);
	
	int* test3c = my_alloc(sizeof(int));
	populate_block(test3c, (sizeof(int)));
	print_contents(120);
	
	my_free(test3b);
	free_block(test3b, (sizeof(int)));
	print_contents(120);
	
	double* test3d = my_alloc(2* sizeof(double));
	populate_block(test3d, (2* sizeof(double)));
	print_contents(120);
	
	int* test3e = my_alloc(sizeof(int));
	populate_block(test3e, (sizeof(int)));
	print_contents(120);
	
	free(buffer);
}

void print_contents(int size){
	int i;
	for(i = 0; i < size; i ++){
		if((buffer + i) == free_head){
			printf("[%i]: free_head \n", i);
		}
		if(i == 0){
			printf("[%i]:%i \n", i, *((int*)buffer));
		}
		else{
			printf("[%i]:%c \n", i, *((char*)buffer + i));
		}
	}
}
void populate_block(void* dataBlock, int size){
	int i;
	for(i = 0; i < size; i++){
		*(((char*)dataBlock + i)) = 'A';
	}
}
void free_block(void* dataBlock, int size){
	if(dataBlock == NULL){
		printf("oops\n");
	}
	else{
		int i;
		for(i = 0; i < size; i++){
			*(((char*)dataBlock + i)) = 'Z';
		}
	}
}
