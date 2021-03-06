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
 *  Remove Ali from the map.
 */
void test_mapRemove_given_ali_and_ali_is_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  List *list = listNew(ali, NULL);
  Map *map = mapNew(5);
  
  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);

  person = mapRemove(map, ali, comparePerson, hash);

  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
  TEST_ASSERT_NULL(map->bucket[3]);
}

/**
 *  Case 2
 *
 *  Ali is not in the map.
 *  Find Ali.
 *  Remove NULL.
 */
void test_mapRemove_given_ali_and_ali_is_not_in_the_map_return_NULL_since_no_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *abu = personNew("Abu", 25, 70.3);
  List *list = listNew(abu, NULL);
  Map *map = mapNew(5);

  map->bucket[3] = list;
  
  hash_ExpectAndReturn(ali, 3);

  person = mapRemove(map, ali, comparePerson, hash);

  TEST_ASSERT_NULL(person);
  TEST_ASSERT_NOT_NULL(map->bucket[3]);
  TEST_ASSERT_EQUAL_Person(abu, getPersonFromBucket(map->bucket[3]));
}

/**
 *  Case 3
 *
 *  The map contains a list: suparman -> zorro -> ali.
 *  Find Ali.
 *  Remove Ali from the map.
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

  person = mapRemove(map, ali, comparePerson, hash);

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
 *  Remove Ali from the map.
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

  person = mapRemove(map, ali, comparePerson, hash);

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
 *  Remove Ali from the map.
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

  person = mapRemove(map, ali, comparePerson, hash);

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

  person = mapRemove(map, ali, comparePerson, hash);

  TEST_ASSERT_NULL(person);
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_EQUAL_Person(suparman, getPersonFromBucket(map->bucket[3]));
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(((List *)map->bucket[3])->next));
  TEST_ASSERT_EQUAL_Person(zorro, getPersonFromBucket(((List *)map->bucket[3])->next));
  TEST_ASSERT_NOT_NULL(getPersonFromBucket(((List *)map->bucket[3])->next->next));
  TEST_ASSERT_EQUAL_Person(abu, getPersonFromBucket(((List *)map->bucket[3])->next->next));
}

/////////////////////////////
// mapLinearStore
/////////////////////////////
/**
 *  Case 1
 *
 *  Add Ali into an empty map. Ali is first hashed and hash value 3 is obtained.
 *  Ali is then placed into bucket 3.
 */
void test_mapLinearStore_given_Ali_should_add_it_to_map(void) {
  Person *person = personNew("Ali", 25, 70.3);
  Map *map = mapNew(5);

  hash_ExpectAndReturn(person, 3);
  
  mapLinearStore(map, person, comparePerson, hash);

  TEST_ASSERT_NOT_NULL(map->bucket[3]);
  TEST_ASSERT_NOT_NULL((Person *)map->bucket[3]);
  
  TEST_ASSERT_EQUAL_Person(person, map->bucket[3]);
}

/**
 *  Case 2
 *
 *  Add Ali into a map. Ali is first hashed and hash value 3 is obtained.
 *  But there is already a person called "Ali".
 */
void test_mapLinearStore_given_Ali_should_throw_exception_when_there_is_already_an_Ali_stored() {
  CEXCEPTION_T e;
  Person *ali = personNew("Ali", 25, 70.3);
  
  Map *map = mapNew(5);
  map->bucket[3] = ali;
  
  hash_ExpectAndReturn(ali, 3);
	
	Try{
		mapLinearStore(map, ali, comparePerson, hash);
		TEST_FAIL_MESSAGE("Expect throw exception but did not.");
	}
	Catch(e){
    TEST_ASSERT_EQUAL(ERR_SAME_ELEMENT, e);
    TEST_ASSERT_NOT_NULL(map->bucket[3]);
    TEST_ASSERT_EQUAL_Person(ali, map->bucket[3]);
	}
}

/**
 *  Case 3
 *
 *  Add Abu into a map. Abu is first hashed and hash value 3 is obtained.
 *  Ali is already placed in the bucket[3].
 *  Abu will then be added to the head of the bucket[4].
 */
void test_mapLinearStore_given_Abu_should_add_into_the_next_bucket_when_there_is_already_an_Ali_stored() {
  CEXCEPTION_T e;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *abu = personNew("Abu", 60, 70.3);

  Map *map = mapNew(5);
  map->bucket[3] = ali;
  
  hash_ExpectAndReturn(abu, 3);
	
	Try{
		mapLinearStore(map, abu, comparePerson, hash);
    TEST_ASSERT_NOT_NULL(map->bucket[3]);
    TEST_ASSERT_NOT_NULL(map->bucket[4]);
    TEST_ASSERT_EQUAL_Person(ali, map->bucket[3]);
    TEST_ASSERT_EQUAL_Person(abu, map->bucket[4]);
	}
	Catch(e){
    TEST_FAIL_MESSAGE("Expect not to throw exception but thrown.");
	}
}

/**
 *  Case 4
 *
 *  Add Alice into a map. Alice is first hashed and hash value 3 is obtained.
 *  Ali & Abu is already placed in the bucket[3] & [4].
 *  Alice will then be added to the bucket[5].
 */
void test_mapLinearStore_given_Alice_should_add_into_the_next_next_bucket_when_there_are_already_Ali_and_Abu_stored() {
  CEXCEPTION_T e;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *abu = personNew("Abu", 60, 70.3);
  Person *alice = personNew("Alice", 18, 40.0);

  Map *map = mapNew(6);
  map->bucket[3] = ali;
  map->bucket[4] = abu;
  
  hash_ExpectAndReturn(alice, 3);
	
	Try{
		mapLinearStore(map, alice, comparePerson, hash);
    TEST_ASSERT_NOT_NULL(map->bucket[3]);
    TEST_ASSERT_NOT_NULL(map->bucket[4]);
    TEST_ASSERT_NOT_NULL(map->bucket[5]);
    TEST_ASSERT_EQUAL_Person(ali, map->bucket[3]);
    TEST_ASSERT_EQUAL_Person(abu, map->bucket[4]);
    TEST_ASSERT_EQUAL_Person(alice, map->bucket[5]);
	}
	Catch(e){
    TEST_FAIL_MESSAGE("Expect not to throw exception but thrown.");
	}
}

/**
 *  Case 5
 *
 *  Add Along into a map. Abu is first hashed and hash value 3 is obtained.
 *  Ali is already placed in the bucket[3].
 *  Abu is already placed in the bucket[4].
 *  Alice is already placed in the bucket[5].
 *  Along have no space to be placed. Thus, throw error ERR_BUCKET_FULL.
 */
void test_mapLinearStore_given_Along_should_return_an_exception_if_the_bucket_is_full() {
  CEXCEPTION_T e;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *abu = personNew("Abu", 60, 70.3);
  Person *alice = personNew("Alice", 18, 40.0);
  Person *along = personNew("Along", 30, 65.0);

  Map *map = mapNew(6);
  map->bucket[3] = ali;
  map->bucket[4] = abu;
  map->bucket[5] = alice;
  
  hash_ExpectAndReturn(along, 3);
	
	Try{
		mapLinearStore(map, along, comparePerson, hash);
    TEST_FAIL_MESSAGE("Expect throw exception but no error been not thrown");
	}
	Catch(e){
    TEST_ASSERT_EQUAL(e, ERR_BUCKET_FULL);
	}
}

/////////////////////////////
// mapLinearFind
/////////////////////////////
/**
 *  Case 1
 *
 *  Ali is already placed in the bucket[3].
 *  Find Ali.
 *  Return Ali.
 */
void test_mapLinearFind_given_ali_and_ali_is_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Map *map = mapNew(5);
  map->bucket[3] = ali;
  
  hash_ExpectAndReturn(ali, 3);
  
  person = mapLinearFind(map, ali, comparePerson, hash);
  
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
void test_mapLinearFind_given_ali_and_ali_is_not_in_the_map_return_NULL_since_no_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  List *list = listNew(ali, NULL);
  Map *map = mapNew(5);

  hash_ExpectAndReturn(ali, 3);

  person = mapLinearFind(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NULL(person);
}

/**
 *  Case 3
 *
 *  Suparman is already placed in the bucket[3].
 *  Zorro is already placed in the bucket[4].
 *  Ali is already placed in the bucket[5].
 *  Find Ali.
 *  Return Ali.
 */
void test_mapLinearFind_given_suparman_and_suparman_zorro_ali_is_in_bucket_3_4_5() {
  Person *person;
  Person *suparman = personNew("Suparman", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *ali = personNew("Ali", 25, 70.3);

  Map *map = mapNew(6);
  map->bucket[3] = suparman;
  map->bucket[4] = zorro;
  map->bucket[5] = ali;
  
  hash_ExpectAndReturn(suparman, 3);

  person = mapLinearFind(map, suparman, comparePerson, hash);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(suparman, person);
}

/**
 *  Case 4
 *
 *  bucket[3] is marked.
 *  bucket[4] is marked.
 *  Ali is placed in the bucket[5].
 *  Find Ali.
 *  Return Ali.
 */
void test_mapLinearFind_given_ali_and_bucket_3_and_4_are_marked_and_ali_is_in_bucket_5() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);

  Map *map = mapNew(6);
  map->bucket[3] = (void *) -1;
  map->bucket[4] = (void *) -1;
  map->bucket[5] = ali;
  
  hash_ExpectAndReturn(ali, 3);

  person = mapLinearFind(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
}

/////////////////////////////
// mapLinearRemove
/////////////////////////////
/**
 *  Case 1
 *
 *  Ali is in the map.
 *  Find Ali.
 *  Remove Ali from the map.
 */
void test_mapLinearRemove_given_ali_and_ali_is_in_the_map_return_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Map *map = mapNew(5);
  
  map->bucket[3] = ali;
  
  hash_ExpectAndReturn(ali, 3);

  person = mapLinearRemove(map, ali, comparePerson, hash);

  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
  TEST_ASSERT_NULL(map->bucket[3]);
}

/**
 *  Case 2
 *
 *  Ali is not in the map.
 *  Find Ali.
 *  Remove NULL.
 */
void test_mapLinearRemove_given_ali_and_ali_is_not_in_the_map_return_NULL_since_no_ali_object() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);
  Person *abu = personNew("Abu", 25, 70.3);

  Map *map = mapNew(5);
  map->bucket[3] = abu;
  
  hash_ExpectAndReturn(ali, 3);

  person = mapLinearRemove(map, ali, comparePerson, hash);

  TEST_ASSERT_NULL(person);
  TEST_ASSERT_NOT_NULL(map->bucket[3]);
  TEST_ASSERT_EQUAL_Person(abu, map->bucket[3]);
}

/**
 *  Case 3
 *
 *  Suparman is already placed in the bucket[3].
 *  Zorro is already placed in the bucket[4].
 *  Ali is already placed in the bucket[5].
 *  Find Ali.
 *  Remove Ali.
 */
void test_mapLinearRemove_given_ali_and_suparman_zorro_ali_is_in_bucket_3_4_5_remove_ali() {
  Person *person;
  Person *suparman = personNew("Suparman", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *ali = personNew("Ali", 25, 70.3);

  Map *map = mapNew(6);
  map->bucket[3] = suparman;
  map->bucket[4] = zorro;
  map->bucket[5] = ali;
  
  hash_ExpectAndReturn(ali, 3);

  person = mapLinearRemove(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_NULL(map->bucket[5]);
  TEST_ASSERT_EQUAL_Person(ali, person);
}

/**
 *  Case 4
 *
 *  Suparman is already placed in the bucket[3].
 *  Zorro is already placed in the bucket[4].
 *  Ali is already placed in the bucket[5].
 *  Find Zorro.
 *  Remove Zorro and replace with -1.
 */
void test_mapLinearRemove_given_zorro_and_suparman_zorro_ali_is_in_bucket_3_4_5_should_remove_zorro_and_replace_with_neg1() {
  Person *person;
  Person *suparman = personNew("Suparman", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *ali = personNew("Ali", 25, 70.3);

  Map *map = mapNew(6);
  map->bucket[3] = suparman;
  map->bucket[4] = zorro;
  map->bucket[5] = ali;
  
  hash_ExpectAndReturn(zorro, 3);
  hash_ExpectAndReturn(ali, 3);

  person = mapLinearRemove(map, zorro, comparePerson, hash);

  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_NOT_NULL(map->bucket[4]);
  TEST_ASSERT_EQUAL_Person(zorro, person);
  TEST_ASSERT_EQUAL(1, isBucketMarked(map->bucket[4]));
}

/**
 *  Case 5
 *
 *  Suparman is already placed in the bucket[3].
 *  Zorro is already placed in the bucket[4].
 *  Ali is already placed in the bucket[5].
 *  Find Suparman.
 *  Remove Suparman and replace with -1.
 */
void test_mapLinearRemove_given_suparman_and_suparman_zorro_ali_is_in_bucket_3_4_5_should_remove_suparman_and_replace_with_neg1() {
  Person *person;
  Person *suparman = personNew("Suparman", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *ali = personNew("Ali", 25, 70.3);

  Map *map = mapNew(6);
  map->bucket[3] = suparman;
  map->bucket[4] = zorro;
  map->bucket[5] = ali;
  
  hash_ExpectAndReturn(suparman, 3);
  hash_ExpectAndReturn(zorro, 3);

  person = mapLinearRemove(map, suparman, comparePerson, hash);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_NOT_NULL(map->bucket[3]);
  TEST_ASSERT_EQUAL_Person(suparman, person);
  TEST_ASSERT_EQUAL(1, isBucketMarked(map->bucket[3]));
}

/**
 *  Case 6
 *
 *  -1 is already placed in the bucket[3].
 *  Zorro is already placed in the bucket[4].
 *  Ali is already placed in the bucket[5].
 *  Find Suparman.
 *  Return null.
 */
void test_mapLinearRemove_given_suparman_and_neg1_zorro_ali_is_in_bucket_3_4_5_should_return_null() {
  Person *person;
  Person *suparman = personNew("Suparman", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *ali = personNew("Ali", 25, 70.3);

  Map *map = mapNew(6);
  map->bucket[3] = (void *) -1;
  map->bucket[4] = zorro;
  map->bucket[5] = ali;
  
  hash_ExpectAndReturn(suparman, 3);
  
  person = mapLinearRemove(map, suparman, comparePerson, hash);
  
  TEST_ASSERT_NULL(person);
}

/**
 *  Case 7
 *
 *  -1 is already placed in the bucket[3].
 *  Zorro is already placed in the bucket[4].
 *  Ali is already placed in the bucket[5].
 *  Find Ali.
 *  Remove Ali and replace with -1.
 */
void test_mapLinearRemove_given_ali_and_neg1_zorro_ali_is_in_bucket_3_4_5_should_remove_ali() {
  Person *person;
  Person *suparman = personNew("Suparman", 25, 70.3);
  Person *zorro = personNew("Zorro", 25, 70.3);
  Person *ali = personNew("Ali", 25, 70.3);

  Map *map = mapNew(6);
  map->bucket[3] = (void *) -1;
  map->bucket[4] = zorro;
  map->bucket[5] = ali;
  
  hash_ExpectAndReturn(ali, 3);
  
  person = mapLinearRemove(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
  TEST_ASSERT_NULL(map->bucket[5]);
}

/**
 *  Case 8
 *
 *  -1 is already placed in the bucket[3].
 *  Ali is already placed in the bucket[4].
 *  Find Ali.
 *  Remove Ali and replace with -1.
 */
void test_mapLinearRemove_given_ali_and_neg1_ali_is_in_bucket_3_4_should_remove_ali_and_make_bucket_3_and_4_to_null() {
  Person *person;
  Person *ali = personNew("Ali", 25, 70.3);

  Map *map = mapNew(6);
  map->bucket[3] = (void *) -1;
  map->bucket[4] = ali;
  
  hash_ExpectAndReturn(ali, 3);
  printf("this");
  person = mapLinearRemove(map, ali, comparePerson, hash);
  
  TEST_ASSERT_NOT_NULL(person);
  TEST_ASSERT_EQUAL_Person(ali, person);
  TEST_ASSERT_NULL(map->bucket[3]);
  TEST_ASSERT_NULL(map->bucket[4]);
}
