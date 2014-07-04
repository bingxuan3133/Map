#ifndef Person_H
#define Person_H

typedef struct Person Person;

struct Person {
  char *name;
  int age;
  float weight;
};

Person *personNew(char *name, int age, float weight);
void personDump(void *data);

#endif // Person_H