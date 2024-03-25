#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void swap(int *xp, int *yp){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void maxHeap(int arr[], int root, int treeSize){

    int biggest = root;

    int left = 2 * root + 1;

    int right = 2 * root + 2;

    if(left < treeSize && arr[left] > arr[biggest]){
        biggest = left;
    }

    if(right < treeSize && arr[right] > arr[biggest]){
        biggest = right;
    }

    if(biggest != root){
        swap(&arr[root], &arr[biggest]);

        maxHeap(arr, biggest, treeSize);
    }
}

void heapSort(int arr[], int n)
{
    for(int i = n/2 - 1; i >= 0; i--)
        maxHeap(arr, i, n);

    for(int i = n - 1; i >= 0; i--){
        swap(&arr[0], &arr[i]);

        maxHeap(arr, 0, i);
    }
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int pData[], int l, int m, int r){

    int i,j,k;

    int lengthLeft = m - l + 1;
    int lengthRight = r - m;

    int Left[lengthLeft], Right[lengthRight];


    for(i = 0; i < lengthLeft; i++)
        Left[i] = pData[l + i];
    for(j = 0; j < lengthRight; j++)
        Right[j] = pData[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while(i < lengthLeft && j < lengthRight){
        if(Left[i] <= Right[j]){
            pData[k] = Left[i];
            i++;
        }
        else{
            pData[k] = Right[j];
            j++;
        }
        k++;
    }

    while(i < lengthLeft){
        pData[k] = Left[i];
        i++;
        k++;
    }

    while(j < lengthRight){
        pData[k] = Right[j];
        j++;
        k++;
    }
}

void mergeSort(int pData[], int l, int r)
{
    if(l < r){
        int m = l + (r - l) / 2;

        mergeSort(pData, l,m);
        mergeSort(pData, m+1, r);

        merge(pData, l, m, r);
    }
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
    int i, j, item;

    for(i = 1; i < n; i++){
        item = pData[i];
        j = i - 1;

        while(j >= 0 && pData[j] > item){
            pData[j + 1] = pData[j];
            j = j - 1;
        }

        pData[j+1] = item;
    }

}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
    int i, j;
    bool isSwapped;

    for(i = 0; i < n - 1; i++){
        isSwapped = false;

        for(j = 0; j < n - i - 1; j++){
            if(pData[j] > pData[j + 1]){
                swap(&pData[j + 1], &pData[j]);
                isSwapped = true;
            }
        }

        if(isSwapped == false){
            break;
        }
    }
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
    int i, j, min;

    for(i = 0; i < n - 1; i++){

        min = i;
        for(j = i+1; j < n; j++) {
            if (pData[j] < pData[min]) {
                min = j;
            }
        }

        if(min != i){
            swap(&pData[min], &pData[i]);
        }
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
    int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i=0;i<dataSz;++i)
        {
            fscanf(inFile, "%d ",&n);
            data = *ppData + i;
            *data = n;
        }

        fclose(inFile);
    }
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy,  dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
