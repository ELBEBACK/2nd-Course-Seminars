#ifndef _CATCAT_
#define _CATCAT_

typedef struct Cat Cat;

Cat* cat_create();
Cat* cat_create_with_name(const char* name);
void cat_destroy(Cat*);
void cat_voice(const Cat*);

#endif