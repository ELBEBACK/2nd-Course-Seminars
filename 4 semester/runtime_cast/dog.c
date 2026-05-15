#include "dog.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct Dog {
    Animal      base;
    const char *voiceline_;
} Dog;

static void dog_voice(const Animal *obj) {
    const Dog *dog = (const Dog *)obj;
    printf("%s barks: \"%s\"\n", dog->base.name_, dog->voiceline_);
}

static void dog_destroy(Animal *obj) {
    Dog *dog = (Dog *)obj;
    printf("%s was destroyed!\n", dog->base.name_);
    free((char *)dog->base.name_);
    free((char *)dog->voiceline_);
    free(dog);
}

static const AnimalVtbl dog_vtbl = { dog_voice, dog_destroy };


Animal *dog_create_with_name(const char *name) {
    Dog *tmp = malloc(sizeof(Dog));
    if (!tmp) return NULL;
    tmp->base.vtbl  = &dog_vtbl;
    tmp->base.name_ = strdup(name);
    tmp->voiceline_ = strdup("Bark");
    return (Animal *)tmp;
}

Animal *dog_create(void) {
    return dog_create_with_name("Sobaka Defaultovna");
}

bool dog_is(const Animal *obj) {
    return obj && obj->vtbl == &dog_vtbl;
}

Dog *animal_as_dog(Animal *obj) {
    return dog_is(obj) ? (Dog *)obj : NULL;
}

void dog_bite(const Dog *obj) {
    if (!obj) return;
    printf("%s bites!\n", obj->base.name_);
}