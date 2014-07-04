#include "unity.h"
#include "Map.h"
#include "List.h"
#include "Person.h"
#include "mock_ComparePerson.h"
#include "mock_Hash.h"
#include "CustomAssert.h"
#include "ErrorCode.h"
#include "CException.h"

#define getPersonFromBucket(x) (Person *)((List *)(x))->data

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mapNew_given_length_of_10_should_create_a_new_Map(void) {
  Map *map = mapNew(10);
  
  TEST_ASSERT_NOT_NULL(map);
  TEST_ASSERT_NOT_NULL(map->bucket);
  TEST_ASSERT_EQUAL(10, map->length);
  TEST_ASSERT_EQUAL(0, map->size);
}

/**
 *  Add Ali into an empty map. Ali is first hashed and hash value 3 is obtained.
 *  Ali is then placed into bucket 3.
 */
void test_mapStore_given_Ali_should_add_it_to_map(void) {
  Person *person = personNew("Ali", 25, 70.3);
  Map *map = mapNew(5);
  
  hash_ExpectAndReturn(person, 3);
  
  mapStore(map, person, comparePerson, hash);

  TEST_ASSERT_NOT_NULL(map->bucket[3]);
  TEST_ASSERT_NOT_NULL(((Person *)((List *)map->bucket[3])->data)->name);
  
  TEST_ASSERT_EQUAL_Person(person, getPersonFromBucket(map->bucket[3]));
}


void test_mapStore_given_Ali_should_throw_exception_when_there_is_already_an_Ali_stored(){
  CEXCEPTION_T e;
	Person *person = personNew("Ali",25,70.3);
  List *list = listNew(person, NULL);
	Map *map = mapNew(5);
  
  // hash_ExpectAndReturn(person,3);
  // mapStore(map, person, comparePerson, hash);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(person,3);
	comparePerson_ExpectAndReturn(person, person, 1);
	
	Try{
		mapStore(map, person, comparePerson, hash);
		TEST_FAIL_MESSAGE("Expect throw exception but did not.");
    
	}
	Catch(e){
    TEST_ASSERT_EQUAL(ERR_SAME_ELEMENT, e);
    TEST_ASSERT_NOT_NULL(map->bucket[3]);
    TEST_ASSERT_EQUAL_Person(person, getPersonFromBucket(map->bucket[3]));
	}
}