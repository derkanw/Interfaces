#include "../src/Supporting.h"

int main(void)
{
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    std::cout << "The first rational value: ";
    std::cin >> x1 >> y1;
    std::cout << "The second rational value: ";
    std::cin >> x2 >> y2;
    Rational* num1 = Rational::InitRational(x1, y1);
    Rational* num2 = Rational::InitRational(x2, y2);
    if (!num1 || !num2)
        return 0;

    QArithmetics(Rational::Add, num1, num2, "Addition: ");
    QArithmetics(Rational::Sub, num1, num2, "Subtraction: ");
    QArithmetics(Rational::Mult, num1, num2, "Multiplication: ");
    QArithmetics(Rational::Div, num1, num2, "Division: ");

    std::cout << "LE: " << ToString(Rational::LE(num1, num2)) << std::endl;
    std::cout << "GT: " << ToString(Rational::GT(num1, num2)) << std::endl;
    std::cout << "===================================================" << std::endl;

    Real* num3 = Real::InitReal(num1, num2);
    Real* num4 = Real::InitReal(Rational::Sub(num1, num2), Rational::Add(num1, num2));
    if (!num3 || !num4)
        return 0;

    std::cout << "The first real value: ";
    num3->Print();
    std::cout << "\nThe second real value: ";
    num4->Print();
    std::cout << std::endl;

    RArithmetics(Real::Add, num3, num4, "Addition: ");
    RArithmetics(Real::Sub, num3, num4, "Subtraction: ");
    RArithmetics(Real::Mult, num3, num4, "Multiplication: ");
    RArithmetics(Real::Div, num3, num4, "Division: ");

    std::cout << "LE: " << ToString(Real::LE(num3, num4)) << std::endl;
    std::cout << "GT: " << ToString(Real::GT(num3, num4)) << std::endl;
    return 0;
}
