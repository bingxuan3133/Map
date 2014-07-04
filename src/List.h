#ifndef List_H
#define List_H

#define listAdd listNew

typedef struct List List;

struct List {
  List *next;
  void *data;
};

List *listNew(void *data, List* next);
List *listAdd(void *newData, List* next);
void listDump(List *list, void (*dump)(void *));

#endif // List_H