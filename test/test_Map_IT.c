#include "unity.h"
#include "Map.h"
#include "List.h"
#include "Person.h"
#include "ComparePerson.h"
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

/**
 *  Case 1
 */
void test_mapNew_given_length_of_10_should_create_a_new_Map(void) {
  Map *map = mapNew(10);
  
  TEST_ASSERT_NOT_NULL(map);
  TEST_ASSERT_NOT_NULL(map->bucket);
  TEST_ASSERT_EQUAL(10, map->length);
  TEST_ASSERT_EQUAL(0, map->size);
}

/////////////////////////////
// mapStore
/////////////////////////////
/**
 *  Case 2
 *
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

/**
 *  Case 3
 *
 *  Add Ali into a map. Ali is first hashed and hash value 3 is obtained.
 *  Ali is then placed into bucket 3.
 *  But there is already a person called "Ali".
 */
void test_mapStore_given_Ali_should_throw_exception_when_there_is_already_an_Ali_stored(){
  CEXCEPTION_T e;
  Person *person = personNew("Ali", 25, 70.3);
  List *list = listNew(person, NULL);
  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(person, 3);
	
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

/**
 *  Case 4
 *
 *  Add Zorro into a map. Zorro is first hashed and hash value 3 is obtained.
 *  Ali is already placed in the bucket 3.
 *  Zorro will then be added to the head of the bucket 3.
 */
void test_mapStore_given_Zorro_should_add_into_the_head_when_there_is_already_an_Ali_stored(){
  CEXCEPTION_T e;
  Person *person1 = personNew("Ali", 25, 70.3);
  Person *person2 = personNew("Zorro", 60, 70.3);
  List *list = listNew(person1, NULL);
  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(person2, 3);
	
	Try{
		mapStore(map, person2, comparePerson, hash);
    TEST_ASSERT_NOT_NULL(map->bucket[3]);
    TEST_ASSERT_EQUAL_Person(person2, getPersonFromBucket(map->bucket[3]));
    TEST_ASSERT_EQUAL_Person(person1, getPersonFromBucket(((List *) map->bucket[3] )->next));
	}
	Catch(e){
    TEST_FAIL_MESSAGE("Expect not to throw exception but thrown.");
	}
}

/////////////////////////////
// mapFind
/////////////////////////////
/**
 *  Case 1
 *
 *  Ali is in the map.
 *  Find Ali.
 *  Return Ali.
 */
void test_mapFind_given_ali_and_ali_is_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  List *list = listNew(ali, NULL);
  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);

  person = mapFind(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
}

/**
 *  Case 2
 *
 *  Ali is not in the map.
 *  Find Ali.
 *  Return NULL.
 */
void test_mapFind_given_ali_and_ali_is_not_in_the_map_return_NULL_since_no_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  List *list = listNew(ali, NULL);
  Map *map = mapNew(5);

  hash_ExpectAndReturn(ali, 3);

  person = mapFind(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NULL(person);
}

/**
 *  Case 3
 *
 *  The map contains a list: suparman -> zorro -> ali.
 *  Find Ali.
 *  Return Ali.
 */
void test_mapFind_given_ali_and_ali_is_in_the_linkedList_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *suparman = personNew("Suparman", 25, 70.3);
  
  List *list = listNew(ali, NULL);
  list = listAdd(zorro, list);
  list = listAdd(suparman, list);

  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);

  person = mapFind(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
}

/**
 *  Case 4
 *
 *  The map contains a list: suparman -> zorro -> abu.
 *  Find Ali.
 *  Return NULL.
 */
void test_mapFind_given_ali_and_ali_is_not_in_the_linkedList_in_the_map_return_NULL() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *abu = personNew("Abu", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *suparman = personNew("Suparman", 25, 70.3);
  
  List *list = listNew(abu, NULL);
  list = listAdd(zorro, list);
  list = listAdd(suparman, list);

  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);
  
  person = mapFind(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NULL(person);
}

/////////////////////////////
// mapRemove
/////////////////////////////
/**
 *  Case 1
 *
 *  Ali is in the map.
 *  Find Ali.
 *  Return Ali.
 */
void test_mapRemove_given_ali_and_ali_is_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  List *list = listNew(ali, NULL);
  Map *map = mapNew(5);
  
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);

  printf("case 1: before\n");
  listDump(map->bucket[3], personDump);
  
  person = mapRemove(map, ali, comparePerson, hash);
  
  printf("case 1: after\n");
  listDump(map->bucket[3], personDump);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
  TEST_ASSERT_NULL(map->bucket[3]);
}

/**
 *  Case 2
 *
 *  Ali is not in the map.
 *  Find Ali.
 *  Return NULL.
 */
void test_mapRemove_given_ali_and_ali_is_not_in_the_map_return_NULL_since_no_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *abu = personNew("Abu", 25, 70.3);
  List *list = listNew(abu, NULL);
  Map *map = mapNew(5);

  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);
  
  printf("case 2: before\n");
  listDump(map->bucket[3], personDump);

  person = mapRemove(map, ali, comparePerson, hash);
  
  printf("case 2: after\n");
  listDump(map->bucket[3], personDump);
    
  TEST_ASSERT_NULL(person);
  TEST_ASSERT_NOT_NULL(map->bucket[3]);
  TEST_ASSERT_EQUAL_Person(abu, getPersonFromBucket(map->bucket[3]));
}

/**
 *  Case 3
 *
 *  The map contains a list: suparman -> zorro -> ali.
 *  Find Ali.
 *  Return Ali.
 */
void test_mapRemove_given_ali_and_ali_is_the_last_element_of_the_linkedList_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *suparman = personNew("Suparman", 25, 70.3);
  
  List *list = listNew(ali, NULL);
  list = listAdd(zorro, list);
  list = listAdd(suparman, list);

  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);
  
  printf("case 3: before\n");
  listDump(map->bucket[3], personDump);

  person = mapRemove(map, ali, comparePerson, hash);
  
  printf("case 3: after\n");
  listDump(map->bucket[3], personDump);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_EQUAL_Person(suparman, getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(((List *)map->bucket[3])->next));
  TEST_ASSERT_EQUAL_Person(zorro, getPersonFromBucket(((List *)map->bucket[3])->next));
}

/**
 *  Case 3b
 *
 *  The map contains a list: suparman -> ali -> zorro.
 *  Find Ali.
 *  Return Ali.
 */
void test_mapRemove_given_ali_and_ali_is_the_middle_element_of_the_linkedList_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *suparman = personNew("Suparman", 25, 70.3);
  
  List *list = listNew(zorro, NULL);
  list = listAdd(ali, list);
  list = listAdd(suparman, list);

  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);
  
  printf("case 3b: before\n");
  listDump(map->bucket[3], personDump);

  person = mapRemove(map, ali, comparePerson, hash);
  
  printf("case 3b: after\n");
  listDump(map->bucket[3], personDump);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_EQUAL_Person(suparman, getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(((List *)map->bucket[3])->next));
  TEST_ASSERT_EQUAL_Person(zorro, getPersonFromBucket(((List *)map->bucket[3])->next));
}

/**
 *  Case 3c
 *
 *  The map contains a list: ali -> suparman -> zorro.
 *  Find Ali.
 *  Return Ali.
 */
void test_mapRemove_given_ali_and_ali_is_the_first_element_of_the_linkedList_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *suparman = personNew("Suparman", 25, 70.3);
  
  List *list = listNew(zorro, NULL);
  list = listAdd(suparman, list);
  list = listAdd(ali, list);

  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);
  
  printf("case 3c: before\n");
  listDump(map->bucket[3], personDump);

  person = mapRemove(map, ali, comparePerson, hash);
  
  printf("case 3c: after\n");
  listDump(map->bucket[3], personDump);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_EQUAL_Person(suparman, getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(((List *)map->bucket[3])->next));
  TEST_ASSERT_EQUAL_Person(zorro, getPersonFromBucket(((List *)map->bucket[3])->next));
}

/**
 *  Case 4
 *
 *  The map contains a list: suparman -> zorro -> abu.
 *  Find Ali.
 *  Return NULL.
 */
void test_mapRemove_given_ali_and_ali_is_not_in_the_linkedList_in_the_map_return_NULL() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *abu = personNew("Abu", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *suparman = personNew("Suparman", 25, 70.3);
  
  List *list = listNew(abu, NULL);
  list = listAdd(zorro, list);
  list = listAdd(suparman, list);
  
  Map *map = mapNew(5);
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);

  printf("case 4: before\n");
  listDump(map->bucket[3], personDump);

  person = mapRemove(map, ali, comparePerson, hash);
  
  printf("case 4: after\n");
  listDump(map->bucket[3], personDump);
  
  TEST_ASSERT_NULL(person);
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_EQUAL_Person(suparman, getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(((List *)map->bucket[3])->next));
  TEST_ASSERT_EQUAL_Person(zorro, getPersonFromBucket(((List *)map->bucket[3])->next));
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(((List *)map->bucket[3])->next->next));
  TEST_ASSERT_EQUAL_Person(abu, getPersonFromBucket(((List *)map->bucket[3])->next->next));
}

