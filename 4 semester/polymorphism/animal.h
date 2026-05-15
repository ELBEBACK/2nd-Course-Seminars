#ifndef ANIMAL_H
#define ANIMAL_H

typedef struct Animal Animal;

typedef struct {
    void (*voice)  (const Animal *obj);
    void (*destroy)(Animal *obj);
} AnimalVtbl;

struct Animal {
    const AnimalVtbl *vtbl;
    const char       *name_;
};

void animal_voice  (const Animal *obj);
void animal_destroy(Animal *obj);

#endif 