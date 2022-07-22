#ifndef CUT_H
#define CUT_H

#include <stdio.h>

typedef enum CutUnitTestStatus
{
    PASS,
    FAIL
} CutUnitTestStatus;

typedef struct CutUnitTest
{
    const char *name;
    void (*fun)(void);
    CutUnitTestStatus status;
} CutUnitTest;

static CutUnitTest *cut_current_unit_test = NULL;

int cut_run_unit_tests(CutUnitTest tests[], int length);

void cut_set_unit_test_failed();

#define CUT_UNIT_TEST(fun) \
    (CutUnitTest) { #fun, fun, PASS }

#define CUT_RUN_UNIT_TESTS(tests) \
    cut_run_unit_tests(tests, sizeof(tests) / sizeof(tests[0]))

#define ASSERT(cond, err_msg)                                       \
    do                                                              \
    {                                                               \
        if (!(cond))                                                \
        {                                                           \
            cut_set_unit_test_failed();                             \
            printf("  - %s:%d: %s\n", __FILE__, __LINE__, err_msg); \
        }                                                           \
    } while (0)

#define ASSERT_TRUE(cond) \
    ASSERT(cond, "ASSERT_TRUE(" #cond ") FAILED")

#define ASSERT_FALSE(cond) \
    ASSERT(!(cond), "ASSERT_FALSE(" #cond ") FAILED")

#define ASSERT_EQ(expected, actual) \
    ASSERT(expected == actual, "ASSERT_EQ(" #expected "," #actual ") FAILED")

#define ASSERT_EQ_ERR(expected, actual, err) \
    ASSERT(expected - err <= actual && actual <= expected + err, "ASSERT_EQ_ERR(" #expected "," #actual "," #err ") FAILED")

#ifdef __cplusplus

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