#include <stdio.h>
#include <malloc.h>
#include "List.h"

List *listNew(void *data, List* next) {
  List *list = malloc(sizeof(List));
  list->data = data;
  list->next = next;
  
  return list;
}

void listDump(List *list, void (*dump)(void *)) {
  while(list != NULL) {
    dump(list->data);
    list = list->next;
  }
}