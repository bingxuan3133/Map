#include <stdio.h>
#include <malloc.h>
#include "Map.h"
#include "List.h"
#include "Person.h"
#include "ErrorCode.h"

Map *mapNew(int length) {
  Map *map = malloc(sizeof(Map));
  map->bucket = calloc(sizeof(void *), length);
  map->length = length;
  map->size = 0;
  
  return map;
}


// Implementing Separate Chaining Method

void mapStore(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *)) {

  int index = hash(element);
  List *list = listNew(element, NULL);

  if(((List *)map->bucket[index]) == NULL) {
    map->bucket[index] = (void *) list;
  } else if (compare(((List *)map->bucket[index])->data , element) == 1) { // compare(in-map, element)
    Throw(ERR_SAME_ELEMENT);
  } else {
    map->bucket[index] = listAdd(element, map->bucket[index]); // chaining
  }
}

void *mapFind(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *)) {
  
  int index = hash(element);
  List *temp = (List *)map->bucket[index];
  
  if(((List *)map->bucket[index]) == NULL) {
    return NULL;
  } else if (compare(temp->data , element) == 1) { // the first attempt: compare(in-map, element)
    return temp->data;
  } else {
    while((temp = temp->next) != NULL) { // the second or more attempts: go thru linkedlist and find element
      if(compare(temp->data, element) == 1)
        return temp->data;
    }
    return NULL;
  }
}

void *mapRemove(Map *map,
                void *element,
                int (*compare)(void *, void *),
                unsigned int (*hash)(void *)) {
  
  int index = hash(element);
  List *temp = (List *)map->bucket[index];
  void *removedElement;

  if(((List *)map->bucket[index]) == NULL) {
    return NULL;
  } else if (compare(temp->data, element) == 1) { // the first attempt: compare(in-map, element)
    removedElement = temp->data;
    if(temp->next != NULL)
      map->bucket[index] = temp->next;
    else
      map->bucket[index] = NULL;
    return removedElement;
    
  } else {
    while((temp->next) != NULL) { // the second or more attempts: go thru linkedlist and find element
      if(compare(temp->next->data, element) == 1) {
        removedElement = temp->next->data;
        if(temp->next->next != NULL)
          temp->next = temp->next->next;
        else
          temp->next = NULL;

          listDump(temp, personDump);
          
        return removedElement;
      }
      temp = temp->next;
    }
    return NULL;
  }
}

// Implementing Linear Probing Method

void mapLinearStore(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *)) {

  int index = hash(element);

  if(isBucketEmpty(map->bucket[index])) {
    map->bucket[index] = element;
  } else {
      while(map->bucket[index] != NULL) {
        if(index == map->length)
          Throw(ERR_BUCKET_FULL);
        if (compare(map->bucket[index], element) == 1) { // compare(in-map, element)
          Throw(ERR_SAME_ELEMENT);
        }
        index++;
      }
      map->bucket[index] = element;
  }
}

void *mapLinearFind(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *)) {

  int index = hash(element);

  if(map->bucket[index] == NULL) {
    return NULL;
  } else {
    while(map->bucket[index] != NULL && index < map->length) {
      if(isBucketMarked(map->bucket[index])) {
        index++;
      } else if (compare(map->bucket[index], element) == 1) { // compare(in-map, element)
        return map->bucket[index];
      } else
        return NULL;
    }
    return NULL;
  }
}

void *mapLinearRemove(Map *map,
                void *element,
                int (*compare)(void *, void *),
                unsigned int (*hash)(void *)) {

  int index = hash(element);
  int hashNumber = index;
  void *removedElement;
  printf("index: %d\n", index);
  if(map->bucket[index] == NULL) {
    return NULL;
  } else {
    while(map->bucket[index] != NULL && index < map->length) {
      if(!isBucketMarked(map->bucket[index])) {
        if(compare(map->bucket[index], element) == 1) { // compare(in-map, element)
          removedElement = map->bucket[index];
          
          if(index+1 < map->length) {
            if(map->bucket[index+1] == NULL) { // next is null
              map->bucket[index] = NULL;
              while(isBucketMarked(map->bucket[index-1])) {  // check previous if marked
                printf("came here?");
                map->bucket[--index] = NULL;
              }
              printf("index: %d\n", index);
            } else {
              if(hashNumber == hash(map->bucket[index+1])) {  // check the next hash number
                map->bucket[index] = (void *) -1;
              } else {                                        // check the next hash number
                map->bucket[index] = NULL;
              }
            }
          } else {
              map->bucket[index] = NULL;
          }
            
          return removedElement;
        }
      }
      index++;
    }
    return NULL;
  }
}