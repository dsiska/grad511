#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void sort(int arr[], int beg, int end) {
    if (end > beg + 1) {
        
        int piv = arr[beg], l = beg + 1, r = end;
        
        while (l < r) {
            
            if (arr[l] <= piv)
                l++;
            else
                swap(&arr[l], &arr[--r]);
        }
        
        swap(&arr[--l], &arr[beg]);
        
        sort(arr, beg, l);
        sort(arr, r, end);    
    }
}

int main(int argc, char** argv) {
    int IntArray[1000];
    int x = 1000;
    FILE *ofp;
    ofp = fopen("a.sorted", "w");

    for (x=0; x<1000; x++)
        IntArray[x] = random() %1000;

    sort(IntArray, 0, 999);
            
    for (x=0; x<1000; x++)
        fprintf(ofp, "\n%d", IntArray[x]);

	

    
    return (EXIT_SUCCESS);
}

