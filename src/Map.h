#ifndef Map_H
#define Map_H

#define isBucketEmpty(x)  ((x) == NULL || (x) == (void *) -1)
#define isBucketMarked(x)  ((x) == (void *) -1)

typedef struct Map Map;

struct Map {
  void **bucket;
  int length;     // Total number bucket
  int size;       // Number of buckets used
};

Map *mapNew(int length);

// Implementing Separate Chaining Method
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
                
// Implementing Linear Probing Method
void mapLinearStore(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *));

void *mapLinearFind(Map *map,
              void *element,
              int (*compare)(void *, void *),
              unsigned int (*hash)(void *));
              
void *mapLinearRemove(Map *map,
                void *element,
                int (*compare)(void *, void *),
                unsigned int (*hash)(void *));
                
#endif // Map_H
