# C Unit Testing (CUT)

A basic single header unit testing framework for C/C++.

Example usage:

````
#include "cut.h"

void test1()
{
    ASSERT_TRUE(1 == 1);            // Pass because 1 == 1 evaluates to true 
    ASSERT_FALSE(1 == 2);           // Pass because 1 == 2 evaluates to false
    ASSERT_EQ(5.1, 5.1);            // Pass because 5.1 is equal to 5.1
    ASSERT_EQ_ERR(5, 5.1, 0.1);     // Pass because (5 - 0.1) <= 5.1 <= (5 + 0.1) 
    ASSERT_THROWS(throw 20, int);   // Pass because code throws an exception of type int
    ASSERT_NOT_THROWS(1+1);         // Pass because code does not throw
}

void test2()
{
    ASSERT_TRUE(4 < 3);             // Fail because 4 < 3 evaluates to false. 
}

int main()
{
    CutUnitTest tests[] = {
        CUT_UNIT_TEST(test1),
        CUT_UNIT_TEST(test2)
    };

    return CUT_RUN_UNIT_TESTS(tests);
}
```

Resulting prints if the code above is executed: 

```
Running 2 test(s)
RUN
- test1
PASS
- test1
RUN
- test2
FAIL
- test2
  - my_test.cpp:15: ASSERT_TRUE(4 < 3) FAILED
SUMMARY
- Executed 2 test(s)
- Passed 1 test(s)
- Failed 1 test(s), listed below
  - test2
```