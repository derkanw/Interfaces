#pragma once
#include "iostream"
#include "Boolean.h"

class Rational
{
private:
    int n, m;

protected:
    Rational(int n, int m);

public:
    ~Rational(void) = default;

    static Rational* InitRational(int n, int m = 1);
    static Rational* Add(Rational* x, Rational* y);
    static Rational* Sub(Rational* x, Rational* y);
    static Rational* Mult(Rational* x, Rational* y);
    static Rational* Div(Rational* x, Rational* y);
    static Boolean LE(Rational* x, Rational* y);
    static Boolean GT(Rational* x, Rational* y);

    void Print(void);
    int GetNumerator(void);
    int GetDenumerator(void);
};
