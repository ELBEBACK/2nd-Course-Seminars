#ifndef CAT_H
#define CAT_H

#include <stdbool.h>

#include "animal.h"

typedef struct Cat Cat;

Animal *cat_create(void);
Animal *cat_create_with_name(const char *name);

bool    cat_is       (const Animal *obj);
Cat    *animal_as_cat(Animal *obj);  
void    cat_scratch  (const Cat *obj);

#endif
