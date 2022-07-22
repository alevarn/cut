#ifndef CUT_H
#define CUT_H

#include <stdio.h>

// An enum for the status of a unit test which can be either passing or failing.
typedef enum CutUnitTestStatus
{
    PASS,
    FAIL
} CutUnitTestStatus;

// The data structure used to store information about one unit test.
typedef struct CutUnitTest
{
    const char *name;
    void (*fun)(void);
    CutUnitTestStatus status;
} CutUnitTest;

// The currently running unit test. 
static CutUnitTest *cut_current_unit_test = NULL;

// Runs all unit tests in the array and prints the result to stdout. 
int cut_run_unit_tests(CutUnitTest tests[], int length);

// Sets the status of the currently running unit test to failing. 
void cut_set_unit_test_failed();

// Creates a new unit test.
#define CUT_UNIT_TEST(fun) \
    (CutUnitTest) { #fun, fun, PASS }

// Runs all unit tests in the array. 
#define CUT_RUN_UNIT_TESTS(tests) \
    cut_run_unit_tests(tests, sizeof(tests) / sizeof(tests[0]))

// Ensures that the condition holds true, otherwise prints an error message to stdout. 
#define ASSERT(cond, err_msg)                                       \
    do                                                              \
    {                                                               \
        if (!(cond))                                                \
        {                                                           \
            cut_set_unit_test_failed();                             \
            printf("  - %s:%d: %s\n", __FILE__, __LINE__, err_msg); \
        }                                                           \
    } while (0)

// Ensures that a condition evaluates to true. 
#define ASSERT_TRUE(cond) \
    ASSERT(cond, "ASSERT_TRUE(" #cond ") FAILED")

// Ensures that a condition evaluates to false. 
#define ASSERT_FALSE(cond) \
    ASSERT(!(cond), "ASSERT_FALSE(" #cond ") FAILED")

// Ensures that two values are equal. 
#define ASSERT_EQ(expected, actual) \
    ASSERT(expected == actual, "ASSERT_EQ(" #expected "," #actual ") FAILED")

// Ensures that a value is equal to another within a specified error margin.
#define ASSERT_EQ_ERR(expected, actual, err) \
    ASSERT(expected - err <= actual && actual <= expected + err, "ASSERT_EQ_ERR(" #expected "," #actual "," #err ") FAILED")

// Asserts for handling exceptions, which is only relevant if we are compiling C++ code.
#ifdef __cplusplus

// Ensures that a piece of code throws an exception of a certain type. 
#define ASSERT_THROWS(code, ex)                                                                   \
    do                                                                                            \
    {                                                                                             \
        try                                                                                       \
        {                                                                                         \
            code;                                                                                 \
        }                                                                                         \
        catch (ex &)                                                                              \
        {                                                                                         \
            break;                                                                                \
        }                                                                                         \
        catch (...)                                                                               \
        {                                                                                         \
        }                                                                                         \
        cut_set_unit_test_failed();                                                               \
        printf("  - %s:%d: %s\n", __FILE__, __LINE__, "ASSERT_THROWS(" #code "," #ex ") FAILED"); \
    } while (0)

// Ensures that a piece of code does not throw any exception at all.
#define ASSERT_NOT_THROWS(code)                                                                   \
    do                                                                                            \
    {                                                                                             \
        try                                                                                       \
        {                                                                                         \
            code;                                                                                 \
        }                                                                                         \
        catch (...)                                                                               \
        {                                                                                         \
            cut_set_unit_test_failed();                                                           \
            printf("  - %s:%d: %s\n", __FILE__, __LINE__, "ASSERT_NOT_THROWS(" #code ") FAILED"); \
        }                                                                                         \
    } while (0)

#endif

int cut_run_unit_tests(CutUnitTest tests[], int length)
{
    printf("Running %d test(s)\n", length);

    int passed = 0;

    for (int i = 0; i < length; i++)
    {
        cut_current_unit_test = &tests[i];

        printf("RUN\n- %s\n", cut_current_unit_test->name);

        cut_current_unit_test->fun();

        if (cut_current_unit_test->status == PASS)
        {
            printf("PASS\n- %s\n", cut_current_unit_test->name);
            passed++;
        }
    }

    printf("SUMMARY\n- Executed %d test(s)\n- Passed %d test(s)\n- Failed %d test(s)", length, passed, length - passed);

    if (passed != length)
    {
        printf(", listed below\n");

        for (int i = 0; i < length; i++)
        {
            CutUnitTest *current = &tests[i];

            if (current->status == FAIL)
            {
                printf("  - %s\n", current->name);
            }
        }
    }
    else
    {
        printf("\n");
    }

    return passed != length;
}

void cut_set_unit_test_failed()
{
    if (cut_current_unit_test->status == PASS)
    {
        cut_current_unit_test->status = FAIL;
        printf("FAIL\n- %s\n", cut_current_unit_test->name);
    }
}

#endif