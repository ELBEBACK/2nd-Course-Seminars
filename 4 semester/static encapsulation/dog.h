#ifndef _DOGDOG_
#define _DOGDOG_

typedef struct Dog Dog;

Dog* dog_create();
Dog* dog_create_with_name(const char* name);
void dog_voice(const Dog*);
void dog_destroy(Dog*);

#endif