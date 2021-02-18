#include "Rational.h"

Rational::Rational(int n, int m)
{
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

Rational* Rational::Add(Rational* x, Rational* y)
{
    return new Rational(x->n * y->m + y->n * x->m, x->m * y->m);
}

Rational* Rational::Sub(Rational* x, Rational* y)
{
    return new Rational(x->n * y->m - y->n * x->m, x->m * y->m);
}

Rational* Rational::Mult(Rational* x, Rational* y)
{
    return new Rational(x->n * y->n, x->m * y->m);
}

int Rational::GetNumerator(void)
{
    return n;
}

int Rational::GetDenumerator(void)
{
    return m;
}

Rational* Rational::Div(Rational* x, Rational* y)
{
    return InitRational(x->n * y->m, x->m * y->n);
}

Boolean Rational::LE(Rational* x, Rational* y)
{
    Rational* result = Sub(x, y);
    if (result->GetNumerator() * result->GetDenumerator() <= 0)
    {
        delete(result);
        return TRUE;
    }
    else
    {
        delete(result);
        return FALSE;
    }
}

Boolean Rational::GT(Rational* x, Rational* y)
{
    Rational* result = Sub(x, y);
    if (result->GetNumerator() * result->GetDenumerator() > 0)
    {
        delete(result);
        return TRUE;
    }
    else
    {
        delete(result);
        return FALSE;
    }
}

void Rational::Print(void)
{
    if (m == 1 || n == 0)
        std::cout << n << std::endl;
    else
        std::cout << n << "/" << m << std::endl;
}