#include "cat.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct Cat {
    Animal      base;
    const char *voiceline_;
} Cat;


static void cat_voice(const Animal *obj) {
    const Cat *cat = (const Cat *)obj;
    printf("%s meows: \"%s\"\n", cat->base.name_, cat->voiceline_);
}

static void cat_destroy(Animal *obj) {
    Cat *cat = (Cat *)obj;
    printf("%s was destroyed!\n", cat->base.name_);
    free((char *)cat->base.name_);
    free((char *)cat->voiceline_);
    free(cat);
}

static const AnimalVtbl cat_vtbl = { cat_voice, cat_destroy };


Animal *cat_create_with_name(const char *name) {
    Cat *tmp = malloc(sizeof(Cat));
    if (!tmp) return NULL;
    tmp->base.vtbl  = &cat_vtbl;
    tmp->base.name_ = strdup(name);
    tmp->voiceline_ = strdup("Meow");
    return (Animal *)tmp;
}

Animal *cat_create(void) {
    return cat_create_with_name("Koshka Defaultovna");
}

bool cat_is(const Animal *obj) {
    return obj && obj->vtbl == &cat_vtbl;
}

Cat *animal_as_cat(Animal *obj) {
    return cat_is(obj) ? (Cat *)obj : NULL;
}

void cat_scratch(const Cat *obj) {
    if (!obj) return;
    printf("%s scratches!\n", obj->base.name_);
}
