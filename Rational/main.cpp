#include <string>
#include "Rational.h"

void Arithmetics(Rational* (*function)(Rational* x, Rational* y), Rational* x, Rational* y, std::string message)
{
    Rational* result = function(x, y);
    if (result)
    {
        std::cout << message;
        result->Print();
    }
}

std::string ToString(Boolean value)
{
    switch (value)
    {
    case TRUE:
        return "true";
    case FALSE:
        return "false";
    default:
        return "nan";
    }
}

int main(void)
{
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    std::cout << "The first value: ";
    std::cin >> x1 >> y1;
    std::cout << "The second value: ";
    std::cin >> x2 >> y2;
    Rational* num1 = Rational::InitRational(x1, y1);
    Rational* num2 = Rational::InitRational(x2, y2);
    if (!num1 || !num2)
        return 0;

    Arithmetics(Rational::Add, num1, num2, "Addition: ");
    Arithmetics(Rational::Sub, num1, num2, "Subtraction: ");
    Arithmetics(Rational::Mult, num1, num2, "Multiplication: ");
    Arithmetics(Rational::Div, num1, num2, "Division: ");

    std::cout << "LE: " << ToString(Rational::LE(num1, num2)) << std::endl;
    std::cout << "GT: " << ToString(Rational::GT(num1, num2)) << std::endl;

    return 0;
}
