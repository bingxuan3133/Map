#include "unity.h"
#include "Person.h"
#include "CustomAssert.h"

void assertEqualPerson(Person *expected, Person *actual, int line, char *msg) {
  UNITY_TEST_ASSERT_EQUAL_STRING(expected->name, actual->name, line, "Not the same name.");
  UNITY_TEST_ASSERT_EQUAL_INT(expected->age, actual->age, line, "Not the same age.");
  UNITY_TEST_ASSERT_FLOAT_WITHIN(0.0001, expected->weight, actual->weight, line, "Not the same weight.");
}