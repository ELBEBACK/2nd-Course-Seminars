#include <stdlib.h>
#include "cat.h"
#include "dog.h"

int main() {

    Dog* dog1 = dog_create();
    Dog* dog2 = dog_create_with_name("Bibu");
    Cat* cat1 = cat_create();
    Cat* cat2 = cat_create_with_name("Bubi");


    dog_voice(dog1);
    dog_voice(dog2);

    cat_voice(cat1);
    cat_voice(cat2);


    dog_destroy(dog1);
    dog_destroy(dog2);

    cat_destroy(cat1);
    cat_destroy(cat2);

    return 0;
}