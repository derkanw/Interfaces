#include "Supporting.h"

Rational::Rational(int n, int m)
{
    if (m < 0)
    {
        m *= -1;
        n *= -1;
    }
    this->n = n;
    this->m = m;
}

Rational* Rational::InitRational(int n, int m)
{
    if (m == 0)
    {
        std::cout << "The denumerator is 0" << std::endl;
        return nullptr;
    }
    return new Rational(n, m);
}

int Rational::GetNumerator(void)
{
    return n;
}

int Rational::GetDenumerator(void)
{
    return m;
}

Rational* Rational::Add(Rational* const x, Rational* const y)
{
    int numX, numY, denumX, denumY;
    GetParts(x, y, &numX, &numY, &denumX, &denumY);
    return new Rational(numX * denumY + numY * denumX, denumX * denumY);
}

Rational* Rational::Sub(Rational* const x, Rational* const y)
{
    int numX, numY, denumX, denumY;
    GetParts(x, y, &numX, &numY, &denumX, &denumY);
    return new Rational(numX * denumY - numY * denumX, denumX * denumY);
}

Rational* Rational::Mult(Rational* const x, Rational* const y)
{
    int numX, numY, denumX, denumY;
    GetParts(x, y, &numX, &numY, &denumX, &denumY);
    return new Rational(numX * numY, denumX * denumY);
}

Rational* Rational::Div(Rational* const x, Rational* const y)
{
    int numX, numY, denumX, denumY;
    GetParts(x, y, &numX, &numY, &denumX, &denumY);
    return InitRational(numX * denumY, denumX * numY);
}

Boolean Rational::LE(Rational* const x, Rational* const y)
{
    Rational* result = Sub(x, y);
    if (result->GetNumerator() * result->GetDenumerator() <= 0)
    {
        delete result;
        return True;
    }
    delete result;
    return False;
}

Boolean Rational::GT(Rational* const x, Rational* const y)
{
    if (False == Rational::LE(x, y))
        return True;
    return False;
}

void Rational::Print(void)
{
    if (m == 1 || n == 0)
        std::cout << n;
    else
        std::cout << n << "/" << m;
}
