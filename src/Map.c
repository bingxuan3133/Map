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

void mapStore(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *)){

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
        
          printf("temp: ");
          listDump(temp, personDump);
          
        return removedElement;
      }
      temp = temp->next;
    }
    return NULL;
  }
  
}