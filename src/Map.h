#ifndef Map_H
#define Map_H

typedef struct Map Map;

struct Map {
  void **bucket;
  int length;
  int size;
};

Map *mapNew(int length);

void mapStore(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *));

void *mapFind(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *));
              
void *mapRemove(Map *map,
                void *element,
                int (*compare)(void *, void *),
                unsigned int (*hash)(void *));

#endif // Map_H
