#ifndef _MODEL_
#define _MODEL_

enum Direction { LEFT, UP, RIGHT, DOWN };

class Model {

    unsigned short width_;
    unsigned short height_;
    //Snake snake_;
    //Rabbit rabbit_;
    unsigned int score_;
    Direction currentD_;
    bool Gameover_;

public:
    Model(unsigned short width, unsigned short height) : width_(width), height_(height) {}

public:

    void update() {}

    bool isGameover() const { return true; }

    void setDirection() {}
    Direction getDirection() const { return UP; }

private:
    bool checkInterception() const { return true; }
    void growSnake() {}

};

#endif