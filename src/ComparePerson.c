#include "ComparePerson.h"
#include "Person.h"
#include <string.h>

int comparePerson(void *person1, void *person2) {
  if(strcmp( ((Person *)person1)->name, ((Person *)person2)->name) == 0)
    return 1;
  else
    return 0;

}