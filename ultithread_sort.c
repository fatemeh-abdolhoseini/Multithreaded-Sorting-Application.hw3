#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100
#define NUM_THREADS 2

int array[MAX_SIZE];
int array_size;

typedef struct {
    int start;
    int end;
} thread_data;

void* bubble_sort(void* arg) {
    thread_data* data = (thread_data*)arg;
    int start = data->start;
    int end = data->end;
    
    printf("Thread sorting from index %d to %d\n", start, end-1);
    
    for(int i = start; i < end; i++) {
        for(int j = start; j < end - 1; j++) {
            if(array[j] > array[j+1]) {
                
                int temp = array[j];
                array[j] = array[j+1];
                array[j+1] = temp;
            }
        }
    }
    
    pthread_exit(NULL);
}


void merge_sections(int start1, int end1, int start2, int end2) {
    int temp[MAX_SIZE];
    int i = start1, j = start2, k = 0;
    
    while(i <= end1 && j <= end2) {
        if(array[i] < array[j]) {
            temp[k++] = array[i++];
        } else {
            temp[k++] = array[j++];
        }
    }
    
    while(i <= end1) {
        temp[k++] = array[i++];
    }
    
    while(j <= end2) {
        temp[k++] = array[j++];
    }
    

    for(i = start1, j = 0; i <= end2; i++, j++) {
        array[i] = temp[j];
    }
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data data[NUM_THREADS];
    
    
    printf("Enter the size of array (max %d): ", MAX_SIZE);
    scanf("%d", &array_size);
    
    printf("Enter %d integers separated by spaces:\n", array_size);
    for(int i = 0; i < array_size; i++) {
        scanf("%d", &array[i]);
    }
    
    printf("Original array: ");
    for(int i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    
    int section_size = array_size / NUM_THREADS;
    
    for(int i = 0; i < NUM_THREADS; i++) {
        data[i].start = i * section_size;
        data[i].end = (i == NUM_THREADS-1) ? array_size : (i+1) * section_size;
        pthread_create(&threads[i], NULL, bubble_sort, &data[i]);
    }
    
    
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    
    if(NUM_THREADS > 1) {
        merge_sections(0, section_size-1, section_size, array_size-1);
    }
    
    
    printf("Sorted array: ");
    for(int i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    return 0;
}