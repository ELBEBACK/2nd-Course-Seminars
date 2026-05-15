#include "cat.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Cat{
    const char* name_;
    const char* voiceline_;
} Cat;

Cat* cat_create() {
    return cat_create_with_name("Koshka Defaultovna");
}

Cat* cat_create_with_name(const char* name) {
    
    Cat* tmp = (Cat *) malloc(sizeof(Cat));
    if (!tmp) return NULL;
    
    tmp->name_ = strdup(name);

    tmp->voiceline_ = strdup("Meow");

    return tmp;
}

void cat_voice(const Cat* obj) {
    if (!obj) return fprintf(stderr, "This cat doesn't exist!\n");
    printf("%s meows: \"%s\"\n", obj->name_, obj->voiceline_);
    return;
}

void cat_destroy(Cat* obj) {
    if (!obj) return;

    printf("%s was destroyed!\n", obj->name_);
    free(obj->name_);
    free(obj->voiceline_);
    free(obj);
    obj = NULL;
}