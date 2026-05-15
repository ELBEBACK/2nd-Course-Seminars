#ifndef DOG_H
#define DOG_H

#include "animal.h"

Animal *dog_create(void);
Animal *dog_create_with_name(const char *name);

#endif