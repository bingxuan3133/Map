#include "unity.h"
#include "List.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_listNew_given_5_should_create_a_List(void) {
  List *list;
  int int5 = 5;
  int *ptrInt;
  
  list = listNew(&int5, NULL);
  ptrInt = (int *)list->data;
  
  TEST_ASSERT_NOT_NULL(list);
  TEST_ASSERT_EQUAL(5, *(int *) list->data);
  TEST_ASSERT_EQUAL(5, *ptrInt);
}