#include "model.hpp"
#include "view.hpp"
#include "controller.hpp"

int main() {

    Model model(40, 60);
    View view = {};

    Controller controller(model, view);

    return 0;
}
