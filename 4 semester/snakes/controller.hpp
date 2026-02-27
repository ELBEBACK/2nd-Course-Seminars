#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "model.hpp"
#include "view.hpp"


class Controller {
    Model model_;
    View view_;

public:
    Controller(Model& model, View& view) : model_(model), view_(view) {};

    void run() {}

    void pause() {}
    void exit() {}

};


#endif