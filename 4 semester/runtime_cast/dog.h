#ifndef DOG_H
#define DOG_H

#include <stdbool.h>

#include "animal.h"

typedef struct Dog Dog;

Animal *dog_create(void);
Animal *dog_create_with_name(const char *name);

bool    dog_is       (const Animal *obj);
Dog    *animal_as_dog(Animal *obj);
void    dog_bite     (const Dog *obj);

#endif
