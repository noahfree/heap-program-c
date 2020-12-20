

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// NOTE: Please use this symbollic constant to define the name of the input file, currently "input.txt"
#define FILENAME "input.txt"
#define ARRAYSIZE 1000

// function declarations:
	void output(char* fileName);
	// BUILD-MAX-HEAP
	int* buildMaxHeap(FILE* fPtr);
	// MAX-HEAPIFY
	void maxHeapify(int* array, int index);
	// HEAP-CHANGE-KEY
	int changeKey(int* array, int index, int key);
	void restoreMaxHeap(int* array, int index);
	void executeCommands(FILE* fPtr, int* array);
	// INSERT
	void insert(int* array, int key);
	// DELETE
	int delete(int* array, int index);
	void printHeap(int* array);

/*
	Program details:  **NOTE Indexing begins at 1 instead of 0 for commands since the heap size is stored at index 0**
	The first line of the input file specifies the size (n) of the heap to be created. The next n lines specify values for which
	the heap should be created with. Using the size & these keys, a max heap is created. The following commands are then executed:
	
		E:  means extract max
		I v: means insert new key v
		C i v: change key at A[i] to v
		D i: delete key at A[i]
		
	An error message will be printed if at any point an index that doesn't currently exist is being removed or changed or if the size
	is 0 & the max is trying to be extracted.  Note: all operations should be implemented using the cyclic-shift idea
*/

// In order to obtain maximum data encapsulation, the main function merely calls a single function using the file name, which will calloc
// every function necessary to construct the heap, execute the commands, and print its contents.
int main(void) {
	output(FILENAME);
}

// In reality, output() is practically the main(). Using the inputted file name, it calls other functions to construct the max heap, execute
// the specified commands, and print the contents.
void output(char* fileName){
	FILE* fPtr = fopen(fileName, "r");
	if (fPtr == NULL){
		puts("\n\n      Error: Unable to open file\n");
	}
	else {
		int *array = buildMaxHeap(fPtr);
		maxHeapify(array, array[0]);
		puts("\n\n    Initial heap");
		printHeap(array);
		executeCommands(fPtr, array);
		puts("\n    Final heap");
		printHeap(array);
		puts("\n\n");
		fclose(fPtr);
		free(array);
	}
	
	puts("\n\n    Program Author: Noah Free, nsfq94\n\n");
}

// Function buildMaxHeap() takes a file pointer and creates an array of size ARRAYSIZE (a symbollic constant) that will hold the heap.
// Using the first line of the file, which is the size of the array, the specified contents of the heap in the file are added to the array.
// EFFICIENCY: O(n), n being the number of contents initially in the heap, also the first line of the file
int* buildMaxHeap(FILE* fPtr){
	int *array = malloc(sizeof(int)*ARRAYSIZE);
	if (array == NULL){
		puts("\n\nError: Unable to allocate memory (1)\n");
		return NULL;
	}
	// the array's contents are initialized to -1
	for (int i = 0; i < 1000; i++) array[i] = -1;
	
	fscanf(fPtr, "%d\n", &array[0]);
	for (int i = 1; i <= array[0]; i++){
		fscanf(fPtr, "%d\n", &array[i]);
	}
	
	return array;
}

// Function maxHeapify() calls itself recursively for every element in the heap in order to create a max-heap
// maxHeapify() first compares the element at the given index to its parent and then if it is smaller than its parents
// compares it to its children and switches accordingly and calls maxHeapify() accordingly
// maxHeapify() is different than restoreMaxHeap() primarily because maxHeapify() is called once on an unsorted array &
// it creates a max heap with the array, whereas restoreMaxHeap() is called to ensure a singular element that may be out
// of place in a max heap is in the correct place.
void maxHeapify(int* array, int index){
	int key = array[index];
	if (key > array[(int)floor(index/2)]){
		array[index] = array[(int)floor(index/2)];
		array[(int)floor(index/2)] = key;
		maxHeapify(array, (int)floor(index/2));
	}
	else {
		int largest = index; // then, if the key is less than its parent a variable called largest is set equal to the index holding
							 // the largest value of the inputted index & its children
		if (array[index*2] > array[largest]){
			largest = index*2;
		}
		if (array[(index*2)+1] > array[largest]){
			largest = (index*2)+1;
		}
		if (largest != index){ // if largest != index then largest is less than one or both of its children, so the keys are switch and	
							   // restoreMaxHeap() is called on the index now holding the key possibly out of place
			array[index] = array[largest];
			array[largest] = key;
			maxHeapify(array, largest);
		}
	} 
	if (index != 1) maxHeapify(array, (index - 1));
}

// Function changeKey() merely replaces the key at the inputted index with the inputted key & calls restoreMaxHeap to maintani max heap property
// Note that the removed key is also returned to the calling program in case something needs to be done with this key.
// EFFICIENCY: O(n), n actually being the height of the heap, since that is what restoreMaxHeap()'s running time depends on
int changeKey(int* array, int index, int key){
	int removed = array[index];
	array[index] = key;
	restoreMaxHeap(array, index);
	return removed;
}

// Function restoreMaxHeap() compares the key at the given index to that key's parents and children in order to see if the max heap property is
// maintained. If not, it switches keys accordingly & recursively calls restoreMaxHeap() on the index the key is now at.
// EFFICIENCY: O(n), n technically representing the height of the heap, but height depends on the number of elements, so this simplifies to just O(n)
void restoreMaxHeap(int* array, int index){
	if (index > array[0]) return;
	
	int key = array[index];
	if (index % 2 == 0){ // if the index is even:
		if (key <= array[index/2]){ // key is first compared to its parent
			int largest = index; // then, if the key is less than its parent a variable called largest is set equal to the index holding
								 // the largest value of the inputted index & its children
			if (array[index*2] > array[largest]){
				largest = index*2;
			}
			if (array[(index*2)+1] > array[largest]){
				largest = (index*2)+1;
			}
			if (largest != index){ // if largest != index then largest is less than one or both of its children, so the keys are switch and	
								   // restoreMaxHeap() is called on the index now holding the key possibly out of place
				array[index] = array[largest];
				array[largest] = key;
				restoreMaxHeap(array, largest);
			}
		}
		else { // if the index holds a key less than its parent, the keys are switched and restoreMaxHeap() is called on the parent
			array[index] = array[index/2];
			array[index/2] = key;
			restoreMaxHeap(array, index/2);
		}
	}
	else if (index != 1) { // else the index is odd & if not equal to 1/not the head of the heap
		if (key <= array[(index-1)/2]){ // key is first compared to its parent
			int largest = index; // then, if the key is less than its parent a variable called largest is set equal to the index holding
								 // the largest value of the inputted index & its children
			if (array[index*2] > array[largest]){
				largest = index*2;
			}
			if (array[(index*2)+1] > array[largest]){
				largest = (index*2)+1;
			}
			if (largest != index){ // if largest != index then largest is less than one or both of its children, so the keys are switch and	
								   // restoreMaxHeap() is called on the index now holding the key possibly out of place
				array[index] = array[largest];
				array[largest] = key;
				restoreMaxHeap(array, largest);
			}
		}
		else { // if the index holds a key less than its parent, the keys are switched and restoreMaxHeap() is called on the parent
			array[index] = array[(index-1)/2];
			array[(index-1)/2] = key;
			restoreMaxHeap(array, (index-1)/2);
		}
	}
	else { // this else only runs if the index is 1, meaning restoreMaxHeap is being called on the head of the heap
		   // the index's value is compared to its children and the max heap property is restored if at least one of its children are greater
		int largest = index;
		if (array[index*2] > array[largest]){
			largest = index*2;
		}
		if (array[(index*2)+1] > array[largest]){
			largest = (index*2)+1;
		}
		if (largest != index){
			array[index] = array[largest];
			array[largest] = key;
			restoreMaxHeap(array, largest);
		}
	}
}

// Function executeCommands() executes the specified commands on the created heap.
// EFFICIENCY: O(n*c) where c is the number of commands and n is the number of elements in the heap
void executeCommands(FILE* fPtr, int* array){
	char command;
	int removed, index, key;
	// while there are still more commands to be executed:
	puts("\n\n\n    Commands Executed:");
	while (!feof(fPtr)){
		fscanf(fPtr, "%c", &command);
		// a switch statement is used in order to execute the correct command based on the reading from the file
		switch (command){
			case 'E': // extract the max
				if (array[0] == 0){
					puts("      Error: Heap is currently empty; Cannot extract max");
					fscanf(fPtr, "\n");
					break;
				}
				removed = array[1];
				array[1] = array[array[0]];
				array[array[0]] = -1;
				array[0]--;
				restoreMaxHeap(array, 1);
				fscanf(fPtr, "\n");
				printf("      E:\tKey %d removed from index 1 of the heap; Max heap property restored\n", removed);
				break;
			case 'I': // insert new key v
				fscanf(fPtr, " %d\n", &key);
				insert(array, key);
				printf("      I %d:\tKey %d inserted into the heap; Max heap property restored\n", key, key);
				break;
			case 'C': // change key at A[i] to v
				fscanf(fPtr, " %d %d\n", &index, &key);
				if (index > array[0]){ 
					puts("      Error: Specified index to change does not currently exist in the heap");
					break;
				}
				removed = changeKey(array, index, key);
				printf("      C %d %d:\tKey %d at index %d changed to %d; Max heap property restored\n", index, key, removed, index, key);
				break;
			case 'D': // delete key at A[i]
				fscanf(fPtr, " %d\n", &index);
				if (index > array[0]){ 
					puts("      Error: Specified index to delete does not currently exist in the heap");
					break;
				}
				else {
					removed = delete(array, index);
					printf("      D %d:\tKey %d at index %d deleted; Max heap property restored\n", index, removed, index);
				}
				break;
		}
	}
	puts("\n");
}

// Function insert() inserts the given key into the heap and then calls restoreMaxHeap to maintain the max heap property.
// EFFICIENCY: insertion is O(1) & restoring max heap is O(n), n depending on the height of the heap which depends on its size
void insert(int* array, int key){
	array[0]++;
	array[array[0]] = key;
	restoreMaxHeap(array, array[0]);
}

// Function delete() deletes key at the inputted index and then calls restoreMaxHeap to maintain the max heap property
// EFFICIENCY: deletion is O(1) & restoring max heap is O(n), n depending on the height of the heap which depends on its size
int delete(int* array, int index){
	int removed = array[index];
	array[index] = array[array[0]];
	array[array[0]] = -1;
	array[0]--;
	restoreMaxHeap(array, index);
	return removed;
}

// Function printHeap() first prints the output of the heap and then prints each key's index, parent, & children.
// EFFICIENCY: O(n), n being the size of the heap
void printHeap(int* array){
	if (array[0] == 0){
		puts("      Heap is empty; No contents to print\n");
		return;
	}
	
	// prints the output, the contents of the heap
	printf("      Output: ");
	for (int i = 1; i <= array[0]; i++){
		printf(" %d", array[i]);
	}
	puts("");
	
	// loops through the array again and prints each key's index, parent, and children
	for (int i = 1; i <= array[0]; i++){
		printf("\n      Key: %d (index %d),", array[i], i);
		if (i > 1){
			if (i % 2 == 0){
				printf("  Parent: %4d,", array[i/2]);
			}
			else {
				printf("  Parent: %4d,", array[(i-1)/2]);
			}
		}
		else {
			printf("  Parent: none,");
		}
		if (array[2*i] != -1){
			printf("  Left: %4d,", array[2*i]);
			if (array[(2*i)+1] != -1){
				printf("  Right: %4d\n", array[(2*i)+1]);
			}
			else {
				printf("  Right: none\n");
			}
		}
		else {
			printf("  Left: none,  Right: none\n");
		}
	}
}