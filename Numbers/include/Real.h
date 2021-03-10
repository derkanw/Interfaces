#pragma once
#include "iostream"
#include "Rational.h"
#include "Boolean.h"

class Real
{
private:
    Rational *left, *right;

protected:
    Real(Rational* const left, Rational* const right);

public:
    ~Real(void);

    static Real* InitReal(Rational* const left, Rational* const right);
    static Real* Add(Real* const x, Real* const y);
    static Real* Sub(Real* const x, Real* const y);
    static Real* Mult(Real* const x, Real* const y);
    static Real* Div(Real* const x, Real* const y);
    static Boolean LE(Real* const x, Real* const y);
    static Boolean GT(Real* const x, Real* const y);

    void Print(void);
    Rational* GetLeft(void);
    Rational* GetRight(void);
};
