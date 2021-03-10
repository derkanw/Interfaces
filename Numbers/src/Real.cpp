#include "Supporting.h"

Real::Real(Rational* const left, Rational* const right)
{
    this->left = left;
    this->right = right;
}

Real::~Real(void)
{
    delete left;
    delete right;
}

Real* Real::InitReal(Rational* const left, Rational* const right)
{
    if (left == nullptr || right == nullptr)
    {
        std::cout << "The parameters are not valid" << std::endl;
        return nullptr;
    }
    if (True == Rational::GT(left, right))
    {
        std::cout << "The interval is set incorrectly" << std::endl;
        return nullptr;
    }
    return new Real(left, right);
}

Rational* Real::GetLeft(void)
{
    return left;
}

Rational* Real::GetRight(void)
{
    return right;
}

Real* Real::Add(Real* const x, Real* const y)
{
    Rational *leftX = nullptr, *leftY = nullptr, *rightX = nullptr, *rightY = nullptr;
    GetBoarders(x, y, &leftX, &leftY, &rightX, &rightY);
    return new Real(Rational::Add(leftX, leftY), Rational::Add(rightX, rightY));
}

Real* Real::Sub(Real* const x, Real* const y)
{
    Rational* leftX = nullptr, * leftY = nullptr, * rightX = nullptr, * rightY = nullptr;
    GetBoarders(x, y, &leftX, &leftY, &rightX, &rightY);
    return new Real(Rational::Sub(leftX, rightY), Rational::Sub(rightX, leftY));
}

Real* Real::Mult(Real* const x, Real* const y)
{
    Rational* leftX = nullptr, * leftY = nullptr, * rightX = nullptr, * rightY = nullptr;
    Rational *elem1, *elem2, *elem3, *elem4;
    GetBoarders(x, y, &leftX, &leftY, &rightX, &rightY);

    elem1 = Rational::Mult(leftX, leftY);
    elem2 = Rational::Mult(leftX, rightY);
    elem3 = Rational::Mult(rightX, leftY);
    elem4 = Rational::Mult(rightX, rightY);
    Rational* combo[] = {elem1, elem2, elem3, elem4};
    return CheckCombo(combo);
}

Real* Real::Div(Real* const x, Real* const y)
{
    Rational* leftX = nullptr, * leftY = nullptr, * rightX = nullptr, * rightY = nullptr;
    Rational *elem1, *elem2, *elem3, *elem4;
    GetBoarders(x, y, &leftX, &leftY, &rightX, &rightY);

    Rational* zero = Rational::InitRational(0, 1);
    if (True == Rational::LE(leftY, zero) && True == Rational::GT(leftY, zero))
        return nullptr;
    delete zero;

    elem1 = Rational::Div(leftX, leftY);
    elem2 = Rational::Div(leftX, rightY);
    elem3 = Rational::Div(rightX, leftY);
    elem4 = Rational::Div(rightX, rightY);
    Rational* combo[] = {elem1, elem2, elem3, elem4};
    return CheckCombo(combo);
}

Boolean Real::LE(Real* const x, Real* const y)
{
    return Rational::LE(x->GetRight(), y->GetLeft());
}

Boolean Real::GT(Real* const x, Real* const y)
{
    return Rational::GT(x->GetLeft(), y->GetRight());
}

void Real::Print(void)
{
    if (Rational::Sub(left, right)->GetNumerator() == 0)
        left->Print();
    else
    {
        std::cout << "[";
        left->Print();
        std::cout << ", ";
        right->Print();
        std::cout << "]" << std::endl;
    }
}
