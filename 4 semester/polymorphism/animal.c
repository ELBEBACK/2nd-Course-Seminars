#include "animal.h"
#include <stdio.h>

void animal_voice(const Animal *obj) {
    if (!obj) { fprintf(stderr, "Null animal\n"); return; }
    obj->vtbl->voice(obj);
}

void animal_destroy(Animal *obj) {
    if (!obj) return;
    obj->vtbl->destroy(obj);
}