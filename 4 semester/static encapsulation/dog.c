#include "dog.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Dog{
    const char* name_;
    const char* voiceline_;
} Dog;

Dog* dog_create() {
    return dog_create_with_name("Sobaka Defaultovna");
}

Dog* dog_create_with_name(const char* name) {
    
    Dog* tmp = (Dog *) malloc(sizeof(Dog));
    if (!tmp) return NULL;
    
    tmp->name_ = strdup(name);

    tmp->voiceline_ = strdup("Bark");

    return tmp;
}

void dog_voice(const Dog* obj) {
    if (!obj) return fprintf(stderr, "This dog doesn't exist!\n");
    printf("%s barks: \"%s\"\n", obj->name_, obj->voiceline_);
    return;
}

void dog_destroy(Dog* obj) {
    if (!obj) return;

    printf("%s was destroyed!\n", obj->name_);
    free(obj->name_);
    free(obj->voiceline_);
    free(obj);
    obj = NULL;
}