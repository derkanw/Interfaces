#include <iostream>
#include "../include/IVector.h"
#include "../include/ILogger.h"
#include "../include/RC.h"

int main()
{
    ILogger* logger = ILogger::createLogger("log.txt");
    IVector* vector1, * vector2;
    vector1->setLogger(logger);
    vector2->setLogger(logger);

    size_t dim = 3;
    double arr1[] = { 1.1, 2.3, 3.5 }, arr2[] = { 5.3, 2.4, 1.9 };
    vector1 = IVector::createVector(dim, arr1);
    vector2 = IVector::createVector(dim, arr2);

    IVector::add(vector1, vector2)->foreach([](double element) {std::cout << element << "; "; });
    std::cout << std::endl;
    vector1->setCord(dim - 2, 8.8);
    IVector::add(vector1, vector2)->foreach([](double element) {std::cout << element << "; "; });

    delete vector1;
    delete vector2;
    delete logger;
    return 0;
}