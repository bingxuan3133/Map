#include <stdio.h>
#include <malloc.h>
#include "Person.h"

Person *personNew(char *name, int age, float weight) {
  Person *person = malloc(sizeof(Person));
  person->name = name;
  person->age = age;
  person->weight = weight;
  
  return person;
}

void personDump(void *data) {
  Person *person = (Person *) data;

  if(person == NULL) return;
   
  printf("%s:%d:%f\n", person->name, person->age, person->weight);
}