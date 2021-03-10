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
    static Rational* Add(Rational* const x, Rational* const y);
    static Rational* Sub(Rational* const x, Rational* const y);
    static Rational* Mult(Rational* const x, Rational* const y);
    static Rational* Div(Rational* const x, Rational* const y);
    static Boolean LE(Rational* const x, Rational* const y);
    static Boolean GT(Rational* const x, Rational* const y);

    void Print(void);
    int GetNumerator(void);
    int GetDenumerator(void);
};
