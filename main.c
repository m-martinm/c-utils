#define CU_ARRAY_IMPL
#define CU_DEBUG
#include "cu_array.h"

int main()
{

    cu_array_t arr = {0};
    cu_array_init(&arr, sizeof(int));
    for (int i = 0; i < 15; i++)
    {
        cu_array_append(&arr, (void *)&i);
        printf("i: %d\n", i);
        _cu_array_debug_print(&arr);
    }

    printf("Items:\n");
    for (size_t j = 0; j < arr.length; j++)
    {
        printf("%d ", *(int *)cu_array_at(&arr, j));
    }
    printf("\n");
    int src[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    cu_array_extend(&arr, (void *)src, 20);
    _cu_array_debug_print(&arr);
    printf("Items:\n");
    for (size_t j = 0; j < arr.length; j++)
    {
        printf("%d ", *(int *)cu_array_at(&arr, j));
    }
    printf("\n");
    int val = 99;
    cu_array_insert(&arr, &val, 15);
    val = -1;
    cu_array_insert(&arr, &val, 0);
    printf("Items:\n");
    for (size_t j = 0; j < arr.length; j++)
    {
        printf("%d ", *(int *)cu_array_at(&arr, j));
    }
    printf("\n");
    cu_array_remove_at(&arr, 7);
    printf("Items:\n");
    for (size_t j = 0; j < arr.length; j++)
    {
        printf("%d ", *(int *)cu_array_at(&arr, j));
    }
    printf("\n");
    cu_array_qsort(&arr, cu_compare_int);
    printf("Items:\n");
    for (size_t j = 0; j < arr.length; j++)
    {
        printf("%d ", *(int *)cu_array_at(&arr, j));
    }
    printf("\n");
    cu_array_deinit(&arr);
    return 0;
}