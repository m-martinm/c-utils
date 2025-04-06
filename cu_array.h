/// @file cu_array.h
/// @brief Simple, header-only dynamic array implementation for C.
///
/// This library provides a generic, resizable array structure (`cu_array_t`) that stores
/// arbitrary fixed-size items in a contiguous memory block.
///
/// Designed to be minimal and portable, this library avoids platform-specific dependencies
/// and can be easily included in any project.
///
/// To include function implementations, define:
///     #define CU_ARRAY_IMPL
/// before including this header in **one** `.c` or `.cpp` file.
///
/// ### Main API
/// - `cu_array_init()`
/// - `cu_array_deinit()`
/// - `cu_array_append()`
/// - `cu_array_insert()`
/// - `cu_array_extend()`
/// - `cu_array_remove_at()`
/// - `cu_array_clear()`
/// - `cu_array_qsort()`
/// - `cu_array_at()`
///
/// @see `tests/` for examples.
/// 
/// @def CU_ARRAY_IMPL
/// @brief Enables function definitions inside the header. Must be defined in exactly one .c or .cpp file.
/// @def CU_API
/// @brief Override linkage keyword (default: extern or empty if CU_ARRAY_IMPL is defined).
/// @def CU_UNIT
/// @brief Type used for raw array storage (default: unsigned char). Override it if you want to store word sized data for example. (Doesn't work at the moment, still needs some work with pointer arithmetics).
/// @def CU_ARRAY_DEFAULT_SIZE
/// @brief Initial array capacity (default: 32).
/// @def CU_ARRAY_MAX_ITEM_SIZE
/// @brief Maximum size used for internal static swap buffer during qsort (default: 128).
/// @def CU_GROWTH_RATE_SPEED
/// @brief Use fast power-of-two resizing. Might waste memory.
/// @def CU_GROWTH_RATE_SPACE
/// @brief Use conservative 2x growth. Mutually exclusive with CU_GROWTH_RATE_SPEED. Default behaviour.
/// @def CU_DEBUG
/// @brief Enables debug features (like _cu_array_debug_print() and __CU_DEBUG_HERE).
/// @def CU_NO_LIBC
/// @brief Disable all standard library includes. You should define cu_malloc, cu_free, cu_realloc, cu_memmove, cu_memcpy.
/// @def CU_NO_STDLIB_H
/// @brief Disable inclusion of <stdlib.h>. You should define cu_malloc, cu_free, cu_realloc.
/// @def CU_NO_STRINGS_H
/// @brief Disable inclusion of <string.h>. You should define cu_memmove, cu_memcpy.

#ifndef CU_ARRAY_H
#define CU_ARRAY_H

#ifndef CU_API
#ifdef CU_ARRAY_IMPL
#define CU_API
#else
#define CU_API extern
#endif // CU_ARRAY_IMPL
#endif // CU_API

#ifndef CU_UNIT
#define CU_UNIT unsigned char
#endif // CU_UNIT

#if (!defined(CU_NO_STDLIB_H)) && (!defined(CU_NO_LIBC))
#include <stdlib.h>
#endif // (!defined(CU_NO_STDLIB)) || (!defined(CU_NO_LIBC))

#if defined(CU_DEBUG)
#include <stdio.h>
#define __CU_DEBUG_HERE                      \
    {                                        \
        printf("HERE Line: %d\n", __LINE__); \
    }
#endif // defined(CU_DEBUG)

#if (defined(CU_GROWTH_RATE_SPEED)) && (defined(CU_GROWTH_RATE_SPACE))
#error "Only CU_GROWTH_RATE_SPEED or CU_GROWTH_RATE_SPACE can be defined, not both"
#endif // (defined(CU_GROWTH_RATE_SPEED)) && (defined(CU_GROWTH_RATE_SPACE))
#if (!defined(CU_GROWTH_RATE_SPEED)) && (!defined(CU_GROWTH_RATE_SPACE))
#define CU_GROWTH_RATE_SPACE
#endif // (!defined(CU_GROWTH_RATE_SPEED)) && (!defined(CU_GROWTH_RATE_SPACE))

#if ((defined(CU_GROWTH_RATE_SPEED)) && (!defined(SIZE_MAX))) || (defined(CU_NO_STDLIB_H))
#include <stddef.h>
#endif // (defined(CU_GROWTH_RATE_SPEED)) && (!defined(SIZE_MAX))

#if (!defined(CU_NO_STRINGS_H)) && (!defined(CU_NO_LIBC))
#include <string.h>
#endif // (!defined(CU_NO_STRINGS_H)) && (!defined(CU_NO_LIBC))

typedef struct cu_array_s
{
    CU_UNIT *data;
    size_t item_size;
    size_t length;
    size_t capacity;
} cu_array_t;

#ifdef __cplusplus
extern "C"
{
#endif

    // Declarations

    /// @brief Initializes a dynamic array with capacity `CU_ARRAY_DEFAULT_SIZE`.
    /// @param arr Pointer to the array.
    /// @param item_size Size of each item in `CU_UNIT`.
    /// @return `0` on success, `1` on error.
    CU_API int cu_array_init(cu_array_t *arr, size_t item_size);

    /// @brief Deinitializes a dynamic array, free the underlying data and set everything to zero.
    /// @param arr Pointer to the array.
    /// @return `0` on success, `1` on error.
    CU_API int cu_array_deinit(cu_array_t *arr);

    /// @brief Returns a pointer to the item at position `pos`.
    /// @note The returned pointer is of type `CU_UNIT *`. You should cast it to the actual data type.
    /// @param arr Pointer to the array.
    /// @param pos Index of the item.
    /// @return Pointer to item on success, `NULL` on error.
    CU_API CU_UNIT *cu_array_at(cu_array_t *arr, size_t pos);

    /// @brief Appends a single item to the end of the array.
    /// @param arr Pointer to the array.
    /// @param item Pointer to the item to append.
    /// @return `0` on success, `1` on error.
    CU_API int cu_array_append(cu_array_t *arr, void *item);

    /// @brief Extends the array with `num_items` items.
    /// @note Internally calls `cu_array_reserve`.
    /// @param arr Pointer to the array.
    /// @param items Pointer to the first item in the list.
    /// @param num_items Number of items to add.
    /// @return `0` on success, `1` on error.
    CU_API int cu_array_extend(cu_array_t *arr, void *items, size_t num_items);

    /// @brief Inserts an item at position `pos`.
    /// @param arr Pointer to the array.
    /// @param item Pointer to the item to insert.
    /// @param pos Insertion index.
    /// @return `0` on success, `1` on error.
    CU_API int cu_array_insert(cu_array_t *arr, void *item, size_t pos);

    /// @brief Reserves capacity for exactly `new_capacity` items.
    /// @note This does **NOT** add to the current capacity — it sets it to `new_capacity` if needed.
    /// @param arr Pointer to the array.
    /// @param new_capacity New capacity value.
    /// @return `0` on success, `1` on allocation failure.
    CU_API int cu_array_reserve(cu_array_t *arr, size_t new_capacity);

    /// @brief Removes the item at position `pos`.
    /// @param arr Pointer to the array.
    /// @param pos Index of the item to remove.
    /// @return `0` on success, `1` on error.
    CU_API int cu_array_remove_at(cu_array_t *arr, size_t pos);

    /// @brief Clears the array (equivalent to setting `arr->length = 0`).
    /// @note Capacity stays the same.
    /// @param arr Pointer to the array.
    /// @return `0` on success, `1` on error.
    CU_API int cu_array_clear(cu_array_t *arr);

    /// @brief Sorts the array in-place using quicksort.
    /// @warning Not thread-safe: uses a static internal buffer of size `CU_ARRAY_MAX_ITEM_SIZE`.
    /// @note To make it thread-safe, you can modify the internal sort function to use a user-provided swap buffer.
    /// @note If your items are larger than 128 bytes, redefine `CU_ARRAY_MAX_ITEM_SIZE` before including this header.
    /// @note For an example compare function, see `cu_compare_int`.
    /// @param arr Pointer to the array.
    /// @param compare Comparator function:
    ///                 `< 0` if `a < b`
    ///                 `0` if `a == b`
    ///                 `> 0` if `a > b`
    /// @return `0` on success, `1` on error.
    CU_API int cu_array_qsort(cu_array_t *arr, int (*compare)(void *, void *));

    /// @brief Example comparator for sorting arrays of `int`.
    /// @param a Pointer to the first `int`.
    /// @param b Pointer to the second `int`.
    /// @return `< 0` if `a < b`; `0` if equal; `> 0` if `a > b`.
    CU_API int cu_compare_int(void *a, void *b);

#ifdef CU_DEBUG
    CU_API int _cu_array_debug_print(cu_array_t *arr);
#endif // CU_DEBUG

#ifdef __cplusplus
}
#endif

#endif // CU_ARRAY_H

#ifdef CU_ARRAY_IMPL

#ifndef cu_malloc
#define cu_malloc malloc
#endif // cu_malloc
#ifndef cu_free
#define cu_free free
#endif // cu_free
#ifndef cu_realloc
#define cu_realloc realloc
#endif // cu_realloc
#ifndef cu_memcpy
#define cu_memcpy memcpy
#endif // cu_memcpy
#ifndef cu_memmove
#define cu_memmove memmove
#endif // cu_memmove
#ifndef CU_ARRAY_DEFAULT_SIZE
#define CU_ARRAY_DEFAULT_SIZE (32u)
#endif // CU_ARRAY_DEFAULT_SIZE
#ifndef CU_ARRAY_MAX_ITEM_SIZE
#define CU_ARRAY_MAX_ITEM_SIZE (128u)
#endif // CU_ARRAY_MAX_ITEM_SIZE

// Definitions

// Helper functions, not "public"

#ifdef CU_GROWTH_RATE_SPEED
/// The bit width of size_t is not less than 16. (since C99)
CU_API size_t __next_power_of_two(size_t v)
{
    v--;
#if SIZE_MAX == 0xFFFF
    // 16-bit
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
#elif SIZE_MAX == 0xFFFFFFFF
    // 32-bit
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
#elif SIZE_MAX == 0xFFFFFFFFFFFFFFFF
    // 64-bit
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
#else
#error "Unsupported size_t size"
#endif
    v++;
    return v;
}
#endif

CU_API void __cu_array_qsort_internal(CU_UNIT *data, size_t item_size, size_t len, int (*compare)(void *, void *))
{
    static CU_UNIT swap_buffer[CU_ARRAY_MAX_ITEM_SIZE];
    if (len < 2u)
    {
        return;
    }
    void *pivot = (void *)(data + item_size * (size_t)(len / 2));

    size_t i, j;
    for (i = 0, j = len - 1;; i++, j--)
    {

        while (compare(data + item_size * i, pivot) < 0)
        {
            i++;
        }
        while (compare(pivot, data + item_size * j) < 0)
        {
            j--;
        }
        if (i >= j)
        {
            break;
        }
        cu_memcpy(swap_buffer, data + item_size * i, item_size);
        cu_memcpy(data + item_size * i, data + item_size * j, item_size);
        cu_memcpy(data + item_size * j, swap_buffer, item_size);
    }

    __cu_array_qsort_internal(data, item_size, i, compare);
    __cu_array_qsort_internal(data + item_size * i, item_size, len - i, compare);
}

CU_API int _cu_array_debug_print(cu_array_t *arr)
{
    if (arr == NULL)
    {
        return 1;
    }
    printf("{\n"
           "\tdata:      %p\n"
           "\titem_size: %zu\n"
           "\tlength:    %zu\n"
           "\tcapacity:  %zu\n"
           "}\n",
           arr->data, arr->item_size, arr->length, arr->capacity);
    return 0;
}

// "Public" function definitions

CU_API int __cu_array_grow(cu_array_t *arr)
{
    if (arr == NULL || arr->data == NULL || arr->capacity == 0u)
    {
        return 1;
    }

#ifdef CU_GROWTH_RATE_SPEED
    size_t new_capacity = __next_power_of_two(arr->capacity);
    if (new_capacity == arr->capacity)
    {
        new_capacity = __next_power_of_two(arr->capacity + 1);
    }
#endif
#ifdef CU_GROWTH_RATE_SPACE
    size_t new_capacity = arr->capacity * 2u;
#endif
    if (new_capacity <= arr->length)
    {
        return 1;
    }

    CU_UNIT *tmp = (CU_UNIT *)cu_realloc(arr->data, arr->item_size * new_capacity);
    if (tmp == NULL)
    {
        return 1;
    }

    arr->data = tmp;
    arr->capacity = new_capacity;
    return 0;
}

CU_API int cu_array_init(cu_array_t *arr, size_t item_size)
{
    if (arr == NULL || arr->data != NULL || arr->capacity != 0u)
    {
        return 1;
    }

    *arr = (cu_array_t){0};
    arr->item_size = item_size;
    arr->data = (CU_UNIT *)cu_malloc(arr->item_size * CU_ARRAY_DEFAULT_SIZE);
    if (arr->data == NULL)
    {
        return 1;
    }

    arr->capacity = CU_ARRAY_DEFAULT_SIZE;
    return 0;
}

CU_API int cu_array_deinit(cu_array_t *arr)
{
    if (arr == NULL || arr->data == NULL || arr->capacity == 0u)
    {
        return 1;
    }

    cu_free(arr->data);
    *arr = (cu_array_t){0};
    return 0;
}

CU_API CU_UNIT *cu_array_at(cu_array_t *arr, size_t pos)
{
    if (arr == NULL || arr->data == NULL ||
        arr->capacity == 0u || pos >= arr->length)
    {
        return NULL;
    }

    return (arr->data + arr->item_size * pos);
}

CU_API int cu_array_append(cu_array_t *arr, void *item)
{
    if (item == NULL || arr == NULL || arr->data == NULL || arr->capacity == 0u)
    {
        return 1;
    }
    if (arr->length >= arr->capacity)
    {
        int res = __cu_array_grow(arr);
        if (res != 0)
        {
            return res;
        }
    }
    cu_memcpy(((arr->data) + (arr->item_size * arr->length)), item, arr->item_size);
    arr->length += 1;
    return 0;
}

CU_API int cu_array_extend(cu_array_t *arr, void *items, size_t num_items)
{
    if (items == NULL || arr == NULL || arr->data == NULL || arr->capacity == 0u)
    {
        return 1;
    }
    int res = cu_array_reserve(arr, arr->length + num_items);
    if (res != 0)
    {
        return 1;
    }
    cu_memcpy(arr->data + arr->item_size * arr->length, items, num_items * arr->item_size);
    arr->length += num_items;
    return 0;
}

CU_API int cu_array_insert(cu_array_t *arr, void *item, size_t pos)
{
    if (item == NULL || arr == NULL || arr->data == NULL || arr->capacity == 0u || pos > arr->length)
    {
        return 1;
    }
    if (arr->length >= arr->capacity)
    {
        int res = __cu_array_grow(arr);
        if (res != 0)
        {
            return res;
        }
    }

    cu_memmove(cu_array_at(arr, pos + 1), cu_array_at(arr, pos), (arr->length - pos) * arr->item_size);
    cu_memcpy((void *)cu_array_at(arr, pos), item, arr->item_size);
    arr->length += 1;
    return 0;
}

CU_API int cu_array_reserve(cu_array_t *arr, size_t new_capacity)
{
    if (arr == NULL || arr->data == NULL || arr->capacity == 0u)
    {
        return 1;
    }
    if (arr->capacity >= new_capacity)
    {
        return 0;
    }

    CU_UNIT *tmp = (CU_UNIT *)cu_realloc(arr->data, arr->item_size * new_capacity);
    if (tmp == NULL)
    {
        return 1;
    }

    arr->data = tmp;
    arr->capacity = new_capacity;
    return 0;
}

CU_API int cu_array_remove_at(cu_array_t *arr, size_t pos)
{
    if (arr == NULL || arr->data == NULL || arr->capacity == 0u || pos >= arr->length)
    {
        return 1;
    }

    cu_memmove(cu_array_at(arr, pos), cu_array_at(arr, pos + 1), (arr->length - pos) * arr->item_size);
    arr->length -= 1;
    return 0;
}

CU_API int cu_array_clear(cu_array_t *arr)
{
    if (arr == NULL)
    {
        return 1;
    }
    arr->length = 0;
    return 0;
}

CU_API int cu_array_qsort(cu_array_t *arr, int (*compare)(void *, void *))
{
    if (arr == NULL || arr->data == NULL || arr->capacity == 0u)
    {
        return 1;
    }
    __cu_array_qsort_internal(arr->data, arr->item_size, arr->length, compare);
    return 0;
}

CU_API int cu_compare_int(void *a, void *b)
{
    return (*(int *)a - *(int *)b);
}

#endif //  CU_ARRAY_IMPL