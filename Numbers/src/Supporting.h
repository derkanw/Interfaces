#pragma once
#include <string>
#include <functional>
#include "../include/Rational.h"
#include "../include/Real.h"

void QArithmetics(Rational* (*func)(Rational*, Rational*), Rational* const x, Rational* const y, std::string message);
void RArithmetics(Real* (*func)(Real*, Real*), Real* const x, Real* const y, std::string message);
void GetParts(Rational* const x, Rational* const y, int* numX, int* numY, int* denumX, int* denumY);
void GetBoarders(Real* const x, Real* const y, Rational** leftX, Rational** leftY, Rational** rigthX, Rational** rightY);
std::string ToString(Boolean value);
Real* CheckCombo(Rational** combo);
