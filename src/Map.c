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
  } else if (compare( ((List *)map->bucket[index])->data , element) == 1) {
    Throw(ERR_SAME_ELEMENT);
  } else {
    ; // chaining
  }

}

void *mapFind(Map *map,
             void *element,
             int (*compare)(void *, void *),
             unsigned int (*hash)(void *)) {
        
}