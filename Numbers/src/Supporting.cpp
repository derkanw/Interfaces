#include "Supporting.h"

void QArithmetics(Rational* (*func)(Rational*, Rational*), Rational* const x, Rational* const y, std::string message)
{
    Rational* result = func(x, y);
    if (result)
    {
        std::cout << message;
        result->Print();
        std::cout << std::endl;
    }
    delete result;
}

void RArithmetics(Real* (*func)(Real*, Real*), Real* const x, Real* const y, std::string message)
{
    Real* result = func(x, y);
    if (result)
    {
        std::cout << message;
        result->Print();
        std::cout << std::endl;
    }
    delete result;
}

std::string ToString(Boolean value)
{
    if (value == True)
        return "true";
    return "false";
}

void GetParts(Rational* const x, Rational* const y, int* numX, int* numY, int* denumX, int* denumY)
{
    (*numX) = x->GetNumerator();
    (*numY) = y->GetNumerator();
    (*denumX) = x->GetDenumerator();
    (*denumY) = y->GetDenumerator();
}

void GetBoarders(Real* const x, Real* const y, Rational** leftX, Rational** leftY, Rational** rightX, Rational** rightY)
{
    (*leftX) = x->GetLeft();
    (*leftY) = y->GetLeft();
    (*rightX) = x->GetRight();
    (*rightY) = y->GetRight();
}

Real* CheckCombo(Rational** combo)
{
    int size = 4;
    Rational* minElem = combo[0];
    for (int i = 1; i < size; ++i)
        if (True == Rational::GT(minElem, combo[i]))
            minElem = combo[i];

    Rational* maxElem = combo[0];
    for (int i = 1; i < size; ++i)
        if (True == Rational::GT(combo[i], maxElem))
            maxElem = combo[i];

    Real* result = Real::InitReal(minElem, maxElem);
    for (int i = 0; i < size; ++i)
        if (combo[i] != minElem && combo[i] != maxElem)
            delete combo[i];
    return result;
}
