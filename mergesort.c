#include <stdio.h>
#include <stdlib.h>

// source: https://www.geeksforgeeks.org/merge-sort/, modified by chi-n-nguyen
// merge 2 subarrays of arr[]
// 1st subarray is arr[left..middle], 2nd subarray is arr[middle+1..right]
// left: start index off left subarray, middle: end index of left subarray, right: end index of right subarray
void merge(int arr[], int left, int middle, int right) {
    int left_index, right_index, merge_index;
    int left_size = middle - left + 1;
    int right_size = right - middle;

    // crate temporary arrays
    int left_array[left_size], right_array[right_size];

    // copy data to temp arrays left_array[] and right_array[]
    for (left_index = 0; left_index < left_size; left_index++) {
        left_array[left_index] = arr[left + left_index];
    }

    for (right_index = 0; right_index < right_size; right_index++) {
        right_array[right_index] = arr[middle + 1 + right_index];
    }

    // merge the temp arrays back into arr[]
    left_index = 0; 
    right_index = 0;
    merge_index = left;
    while (left_index < left_size && right_index < right_size) {
        if (left_array[left_index] <= right_array[right_index]) {
            arr[merge_index] = left_array[left_index];
            left_index++;
        } else {
            arr[merge_index] = right_array[right_index];
            right_index++;
        }
        merge_index++;
    }

    // copy the remaining elements of left_array[], if there are any
    while (left_index < left_size) {
        arr[merge_index] = left_array[left_index];
        left_index++;
        merge_index++;
    }

    // copy the remaining elements of right_array[], if there are any   
    while (right_index < right_size) {
        arr[merge_index] = right_array[right_index];
        right_index++;
        merge_index++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        // find the middle point
        int middle = left + (right - left) / 2;

        // sort first and second halves
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        // merge the sorted halves
        merge(arr, left, middle, right);
    }
}

void printArray(int array[], int size) {
    int index;
    for (index = 0; index < size; index++)
        printf("%d ", array[index]);
    printf("\n");
}

int main(int argc, char *argv[]) {
    int arr[] = {1, 3, 5, 7, 6, 2, 4};
    int arr_size = sizeof(arr) / sizeof(arr[0]);

    printf("Given array is \n");
    printArray(arr, arr_size);

    mergeSort(arr, 0, arr_size - 1);

    printf("\nSorted array is \n");
    printArray(arr, arr_size);
    return 0;
}