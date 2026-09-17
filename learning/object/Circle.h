//
// Created by even on 2022/3/7.
//

#ifndef CPP_LEARNING_CIRCLE_H
#define CPP_LEARNING_CIRCLE_H


class Circle {

private:
    const double PI = 3.14;
    int r;

public:

    double getCircumference(){
        return 2 * PI * r;
    }

    double getArea(){
        return 2 * PI * r * r;
    }

};


#endif //CPP_LEARNING_CIRCLE_H
