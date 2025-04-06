/// @file cu_test.h
/// Simple testing "framework" as a header only library.
/// It provides some simple MACROS:
///     - CU_RUN_TESTS(title)
///     - CU_RUN_END()
///     - CU_TEST_START(title, desc)
///     - CU_TEST_END()
///     - CU_TEST_CHECK(condition)
///     - CU_TEST_REQUIRE(condition)
///     - CU_TEST_COMMENT(msg)
/// @def CU_TEST_SILENT
/// @brief Define this macro before including to silence all test output (other than the summary at the end).
/// @def CU_FOUT
/// @brief Define this before including to redirect output (e.g., to `stderr` or a file).
/// For usage and examples see the doccomments and the tests for the other headers under tests/.

#ifndef CU_TEST_H
#define CU_TEST_H

#include <stdio.h>

#ifndef CU_FOUT
#define CU_FOUT stdout
#endif // CU_FOUT

#ifndef CU_TEST_SILENT
/// @brief Simply prints to `CU_FOUT`.
/// @param msg Message, should be double quoted.
/// @note Can be disabled by defining `CU_TEST_SILENT`.
#define CU_TEST_COMMENT(msg)   \
    do                         \
    {                          \
        fprintf(CU_FOUT, msg); \
    } while (0)
#else
/// @brief Simply prints to `CU_FOUT`.
/// @param msg Message, should be double quoted.
/// @note Can be disabled by defining `CU_TEST_SILENT`.
#define CU_TEST_COMMENT(msg)
#endif

/// @brief Shouldn't be used by the user, it is called by `CU_RUN_TESTS`.
#define __CU_TEST_INIT(title)                   \
    int __g_cu_unit_test_result = 0;            \
    const char *__g_cu_unit_test_title = title; \
    size_t __g_cu_num_tests = 0;                \
    size_t __g_cu_num_tests_passed = 0;         \
    size_t __g_cu_num_checks = 0;               \
    size_t __g_cu_num_checks_passed = 0;

/// @brief Should be called at the end of the test scope. Prints a summary of the tests.
#define CU_RUN_END()                                                                             \
    fprintf(CU_FOUT, "Summary:\n");                                                              \
    if (__g_cu_unit_test_result == 0)                                                            \
    {                                                                                            \
        fprintf(CU_FOUT, "%s PASSED\n", __g_cu_unit_test_title);                                 \
    }                                                                                            \
    else                                                                                         \
    {                                                                                            \
        fprintf(CU_FOUT, "%s FAILED\n", __g_cu_unit_test_title);                                 \
    }                                                                                            \
    fprintf(CU_FOUT, "Tests passed: %zu / %zu\n", __g_cu_num_tests_passed, __g_cu_num_tests);    \
    fprintf(CU_FOUT, "Checks passed: %zu / %zu\n", __g_cu_num_checks_passed, __g_cu_num_checks); \
    return __g_cu_unit_test_result;

#ifndef CU_TEST_SILENT
/// @brief Should be called at the start of each test.
/// @param title Title of the test
/// @param desc Description of the test
/// @note By defining `CU_TEST_SILENT`, it doesn't print anything.
#define CU_TEST_START(title, desc)                             \
    int __cu_test_result = 0;                                  \
    const char *__cu_test_title = title;                       \
    __g_cu_num_tests += 1;                                     \
    do                                                         \
    {                                                          \
        fprintf(CU_FOUT, "TEST START: " title "\n" desc "\n"); \
    } while (0);
#else
/// @brief Should be called at the start of each test.
/// @param title Title of the test
/// @param desc Description of the test
/// @note By defining `CU_TEST_SILENT`, it doesn't print anything.
#define CU_TEST_START(title, desc) \
    int __cu_test_result = 0;      \
    __g_cu_num_tests += 1;
#endif

#ifndef CU_TEST_SILENT
/// @brief Should be called at the end of each test.
/// @note By defining `CU_TEST_SILENT`, it doesn't print anything.
#define CU_TEST_END()                                                 \
    if (__cu_test_result == 0)                                        \
    {                                                                 \
        fprintf(CU_FOUT, "TEST END: %s PASSED\n\n", __cu_test_title); \
        __g_cu_num_tests_passed += 1;                                 \
    }                                                                 \
    else                                                              \
    {                                                                 \
        fprintf(CU_FOUT, "TEST END: %s FAILED\n\n", __cu_test_title); \
        __g_cu_unit_test_result = 1;                                  \
    }                                                                 \
    return __cu_test_result;
#else
/// @brief Should be called at the end of each test.
/// @note By defining `CU_TEST_SILENT`, it doesn't print anything.
#define CU_TEST_END()                 \
    if (__cu_test_result == 0)        \
    {                                 \
        __g_cu_num_tests_passed += 1; \
    }                                 \
    else                              \
    {                                 \
        __g_cu_unit_test_result = 1;  \
    }                                 \
    return __cu_test_result;
#endif

#ifndef CU_TEST_SILENT
/// @brief Checks the condition.
/// @param condition Condition: if evaluates to true, the check is PASSED, else FAILED.
/// @note By defining `CU_TEST_SILENT`, it doesn't print anything.
#define CU_TEST_CHECK(condition)                                  \
    do                                                            \
    {                                                             \
        __g_cu_num_checks += 1;                                   \
        if (condition)                                            \
        {                                                         \
            fprintf(CU_FOUT, "\tCheck PASSED: %s\n", #condition); \
            __g_cu_num_checks_passed += 1;                        \
        }                                                         \
        else                                                      \
        {                                                         \
            fprintf(CU_FOUT, "\tCheck FAILED: %s\n", #condition); \
            __cu_test_result = 1;                                 \
        }                                                         \
                                                                  \
    } while (0)
#else
/// @brief Checks the condition.
/// @param condition Condition: if evaluates to true, the check is PASSED, else FAILED.
/// @note By defining `CU_TEST_SILENT`, it doesn't print anything.
#define CU_TEST_CHECK(condition)           \
    do                                     \
    {                                      \
        __g_cu_num_checks += 1;            \
        if (condition)                     \
        {                                  \
            __g_cu_num_checks_passed += 1; \
        }                                  \
        else                               \
        {                                  \
            __cu_test_result = 1;          \
        }                                  \
                                           \
    } while (0)
#endif

#ifndef CU_TEST_SILENT
/// @brief Same as CU_TEST_CHECK, but it returns on failure.
/// @param condition Condition: if evaluates to true, the check is PASSED, else FAILED.
/// @note By defining `CU_TEST_SILENT`, it doesn't print anything.
#define CU_TEST_REQUIRE(cond)                                   \
    do                                                          \
    {                                                           \
        __g_cu_num_checks += 1;                                 \
        if (!(cond))                                            \
        {                                                       \
            fprintf(CU_FOUT, "\tREQUIRED FAILED: %s\n", #cond); \
            __cu_test_result = 1;                               \
            return __cu_test_result;                            \
        }                                                       \
        else                                                    \
        {                                                       \
            fprintf(CU_FOUT, "\tCheck PASSED: %s\n", #cond);    \
            __g_cu_num_checks_passed += 1;                      \
        }                                                       \
    } while (0)
#else
/// @brief Same as CU_TEST_CHECK, but it returns on failure.
/// @param condition Condition: if evaluates to true, the check is PASSED, else FAILED.
/// @note By defining `CU_TEST_SILENT`, it doesn't print anything.
#define CU_TEST_REQUIRE(cond)              \
    do                                     \
    {                                      \
        __g_cu_num_checks += 1;            \
        if (!(cond))                       \
        {                                  \
            __cu_test_result = 1;          \
            return __cu_test_result;       \
        }                                  \
        else                               \
        {                                  \
            __g_cu_num_checks_passed += 1; \
        }                                  \
    } while (0)
#endif

/// @brief Wrapper around `main`.
/// ```C
/// CU_RUN_TESTS("Title") {
///     test_something();
///     ...
///     CU_RUN_END();
/// }
/// ```
/// @param title Title of the unit test.
#define CU_RUN_TESTS(title) \
    __CU_TEST_INIT(title)   \
    int main(void)

#endif // CU_TEST_H
