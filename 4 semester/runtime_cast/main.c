#include <stdlib.h>
#include "animal.h"
#include "cat.h"
#include "dog.h"

int main() {

    Animal *animals[] = {
        dog_create(),
        dog_create_with_name("Bibu"),
        cat_create(),
        cat_create_with_name("Bubi"),
    };
    size_t count = sizeof(animals) / sizeof(animals[0]);


    for (size_t i = 0; i < count; ++i)
        animal_voice(animals[i]);


    for (size_t i = 0; i < count; ++i) {
        Cat *c = animal_as_cat(animals[i]);
        if (c) cat_scratch(c);

        Dog *d = animal_as_dog(animals[i]);
        if (d) dog_bite(d);
    }

    for (size_t i = 0; i < count; ++i)
        animal_destroy(animals[i]);

    return EXIT_SUCCESS;
}