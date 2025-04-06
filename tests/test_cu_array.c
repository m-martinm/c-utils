#define CU_ARRAY_IMPL
#define CU_DEBUG
#include "../cu_array.h"
#define CU_TEST_SILENT
#include "../cu_test.h"

int test_cu_array_tc_1();
int test_cu_array_tc_2();
int test_cu_array_tc_3();
int test_cu_array_tc_4();
int test_cu_array_tc_5();
int test_cu_array_tc_6();
int test_cu_array_tc_7();

CU_RUN_TESTS("cu_array unit test")
{
    test_cu_array_tc_1();
    test_cu_array_tc_2();
    test_cu_array_tc_3();
    test_cu_array_tc_4();
    test_cu_array_tc_5();
    test_cu_array_tc_6();
    test_cu_array_tc_7();

    CU_RUN_END();
}

int test_cu_array_tc_1()
{
    CU_TEST_START("cu_array init and deinit", "Checks if the initialisation and deinit works.");

    cu_array_t arr = (cu_array_t){0};
    cu_array_init(&arr, sizeof(int));
    CU_TEST_CHECK(arr.capacity == CU_ARRAY_DEFAULT_SIZE);

    cu_array_deinit(&arr);
    CU_TEST_CHECK(arr.capacity == 0);
    CU_TEST_CHECK(arr.length == 0);
    CU_TEST_CHECK(arr.data == NULL);

    CU_TEST_END();
}

int test_cu_array_tc_2()
{
    CU_TEST_START("cu_array append and at", "Checks if append and at works properly.");

    cu_array_t arr = (cu_array_t){0};
    cu_array_init(&arr, sizeof(int));

    int to_append = 1;
    cu_array_append(&arr, &to_append);

    int result = *(int *)cu_array_at(&arr, 0);
    CU_TEST_CHECK(result == to_append);

    cu_array_deinit(&arr);

    CU_TEST_END();
}

int test_cu_array_tc_3()
{
    CU_TEST_START("cu_array extend", "Checks if extend works properly.");

    cu_array_t arr = (cu_array_t){0};
    cu_array_init(&arr, sizeof(int));

    int to_extend[5] = {0, 1, 2, 3, 4};
    cu_array_extend(&arr, to_extend, 5);

    for (int i = 0; i < 5; i++)
    {
        CU_TEST_CHECK(*(int *)cu_array_at(&arr, i) == i);
    }

    cu_array_deinit(&arr);

    CU_TEST_END();
}

int test_cu_array_tc_4()
{
    CU_TEST_START("cu_array insert", "Checks if insertion works properly.");

    cu_array_t arr = (cu_array_t){0};
    cu_array_init(&arr, sizeof(int));

    int to_extend[5] = {0, 1, 2, 3, 4};
    cu_array_extend(&arr, to_extend, 5);

    int to_insert = 3;
    size_t pos = 2;

    cu_array_insert(&arr, &to_insert, pos);
    CU_TEST_CHECK(*(int *)cu_array_at(&arr, pos) == to_insert);

    cu_array_deinit(&arr);
    CU_TEST_END();
}

int test_cu_array_tc_5()
{
    CU_TEST_START("cu_array remove at", "Checks if remove works properly.");

    cu_array_t arr = (cu_array_t){0};
    cu_array_init(&arr, sizeof(int));

    int to_extend[5] = {0, 1, 2, 3, 4};
    cu_array_extend(&arr, to_extend, 5);

    size_t pos = 2;

    cu_array_remove_at(&arr, pos);
    cu_array_remove_at(&arr, pos);
    CU_TEST_CHECK(*(int *)cu_array_at(&arr, 0) == 0);
    CU_TEST_CHECK(*(int *)cu_array_at(&arr, 1) == 1);
    CU_TEST_CHECK(*(int *)cu_array_at(&arr, 2) == 4);

    cu_array_deinit(&arr);
    CU_TEST_END();
}

int test_cu_array_tc_6()
{
    CU_TEST_START("cu_array qsort", "Checks if sorting works properly.");

    cu_array_t arr = (cu_array_t){0};
    cu_array_init(&arr, sizeof(int));

    int to_extend[5] = {0, 3, 4, 1, 2};
    cu_array_extend(&arr, to_extend, 5);

    cu_array_qsort(&arr, cu_compare_int);

    for (int i = 0; i < 5; i++)
    {
        CU_TEST_CHECK(*(int *)cu_array_at(&arr, i) == i);
    }

    cu_array_deinit(&arr);
    CU_TEST_END();
}

int test_cu_array_tc_7()
{
    CU_TEST_START("cu_array 'real-world'",
                  "Checks if the array works with structs "
                  "and in real word use cases.");

    CU_TEST_COMMENT("Create a dummy struct.");
    typedef struct dummy_s
    {
        size_t id;
        char *ptr;
        const char *key;
        int value;
    } dummy_t;

    const char *keys[5] = {"KEY1", "KEY2", "KEY3", "KEY4", "KEY5"};

    cu_array_t arr = (cu_array_t){0};
    cu_array_init(&arr, sizeof(dummy_t));

    CU_TEST_COMMENT("Fill the array with the dummy structs.");
    dummy_t to_append;
    for (size_t i = 0; i < 71; i++)
    {
        const char *key = keys[i % 5];
        to_append = (dummy_t){.id = i, .key = key, .ptr = NULL, .value = i * i};
        cu_array_append(&arr, &to_append);
    }

    CU_TEST_CHECK(arr.length == 71);

    CU_TEST_COMMENT("Check every struct with a for-loop.");
    for (size_t i = 0; i < 71; i++)
    {
        const char *key_should = keys[i % 5];
        dummy_t at = *(dummy_t *)cu_array_at(&arr, i);
        CU_TEST_CHECK(at.id == i);
        CU_TEST_CHECK(at.value == (int)(i * i));
        CU_TEST_CHECK(at.ptr == NULL);
        CU_TEST_CHECK(strcmp(key_should, at.key) == 0);
    }

    CU_TEST_COMMENT("Insert a struct at pos=24.\n");
    char tmp = 'a';
    dummy_t to_insert = (dummy_t){.id = 99, .key = "INSERT", .ptr = &tmp, .value = 69};
    cu_array_insert(&arr, &to_insert, 24);

    CU_TEST_COMMENT("Check the inserted struct.\n");
    dummy_t at = *(dummy_t *)cu_array_at(&arr, 24);
    CU_TEST_CHECK(at.id == 99);
    CU_TEST_CHECK(at.value == 69);
    CU_TEST_CHECK(at.ptr == &tmp);
    CU_TEST_CHECK(strcmp("INSERT", at.key) == 0);

    CU_TEST_COMMENT("Remove the inserted struct.\n");
    cu_array_remove_at(&arr, 24);
    at = *(dummy_t *)cu_array_at(&arr, 24);
    CU_TEST_CHECK(at.id != 99);
    CU_TEST_CHECK(at.value != 69);
    CU_TEST_CHECK(at.ptr != &tmp);
    CU_TEST_CHECK(strcmp("INSERT", at.key) != 0);

    cu_array_deinit(&arr);
    CU_TEST_END();
}