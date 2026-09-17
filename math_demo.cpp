//
// Created by even on 2022/2/11.
//

#include <iostream>
#include "config.h"

#ifdef USE_MYMATH
    #include "math/MathFunctions.h"
#else
    #include <math.h>
#endif

using namespace std;

int main()
{
    double base;
    cin >> base;
    int exponent;
    cin >> exponent;
    #ifdef USE_MYMATH
        printf("Now we use our own Math library. \n");
        double result = power(base, exponent);
    #else
        printf("Now we use the standard library. \n");
        double result = pow(base, exponent);
    #endif
    printf("%g ^ %d is %g\n", base, exponent, result);
    return 0;
}

