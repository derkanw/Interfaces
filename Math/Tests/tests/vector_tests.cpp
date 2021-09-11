#include "vector_tests.h"

void creatingTest()
{
    LOGGER
    VECTOR1
    VECTOR2
    IVector* vector3 = IVector::createVector(1, nullptr);

    if (vector1 == nullptr || vector2 == nullptr || vector3 != nullptr)
    std::cout << "Error in creatingTest()" << std::endl;

    delete vector3;
    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void copyingTest()
{
    LOGGER
    VECTOR1
    VECTOR2

    if (IVector::copyInstance(vector2, vector1) != RC::SUCCESS)
        return;

    const double* data2 = vector2->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (arr1[i] != data2[i])
        {
            std::cout << "Error in copyingTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void movingTest()
{
    LOGGER
    VECTOR1
    VECTOR2

    if (IVector::moveInstance(vector2, vector1) != RC::SUCCESS)
        return;

    if (vector1 != nullptr)
        std::cout << "Error in movingTest()" << std::endl;

    const double* data2 = vector2->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (arr1[i] != data2[i])
        {
            std::cout << "Error in movingTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void cloningTest()
{
    LOGGER
    VECTOR1

    IVector* vector2 = vector1->clone();
    const double* data2 = vector2->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (arr1[i] != data2[i])
        {
            std::cout << "Error in cloningTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void gettingDataTest()
{
    LOGGER
    VECTOR1

    const double* data1 = vector1->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (arr1[i] != data1[i])
        {
            std::cout << "Error in gettingDataTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
}

void settingDataTest()
{
    LOGGER
    VECTOR1
    VECTOR2

    vector1->setData(DIM, arr2);
    const double* data1 = vector1->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (arr2[i] != data1[i])
        {
            std::cout << "Error in settingDataTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void gettingCordTest()
{
    LOGGER
    VECTOR1

    double value;
    vector1->getCord(2, value);
    if (value != arr1[2])
        std::cout << "Error in gettingCordTest()" << std::endl;

    DELETE_LOGGER
    DELETE_VECTOR1
}

void settingCordTest()
{
    LOGGER
    VECTOR1

    double value = 5;
    vector1->setCord(2, value);

    if (value != vector1->getData()[2])
        std::cout << "Error in settingCordTest()" << std::endl;

    DELETE_LOGGER
    DELETE_VECTOR1
}

void scaleTest()
{
    LOGGER
    VECTOR1

    double value = 5;
    vector1->scale(value);

    const double* data1 = vector1->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (fabs(data1[i] - arr1[i] * value) >= TOL)
        {
            std::cout << "Error in scaleTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
}

void gettingDimTest()
{
    LOGGER
    VECTOR1

    if (vector1->getDim() != DIM)
        {
            std::cout << "Error in gettingDiTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
}

void incTest()
{
    LOGGER
    VECTOR1
    VECTOR2

    vector1->inc(vector2);

    const double* data1 = vector1->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (fabs(data1[i] - arr1[i] - arr2[i]) >= TOL)
        {
            std::cout << "Error in incTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void decTest()
{
    LOGGER
    VECTOR1
    VECTOR2

    vector1->dec(vector2);

    const double* data1 = vector1->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (fabs(data1[i] - arr1[i] + arr2[i]) >= TOL)
        {
            std::cout << "Error in decTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void addTest()
{
    LOGGER
    VECTOR1
    VECTOR2

    IVector* vector3 = IVector::add(vector1, vector2);

    const double* data3 = vector3->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (fabs(data3[i] - arr1[i] - arr2[i]) >= TOL)
        {
            std::cout << "Error in addTest()" << std::endl;
            return;
        }

    delete vector3;
    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void subTest()
{
    LOGGER
    VECTOR1
    VECTOR2

    IVector* vector3 = IVector::sub(vector1, vector2);

    const double* data3 = vector3->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (fabs(data3[i] - arr1[i] + arr2[i]) >= TOL)
        {
            std::cout << "Error in subTest()" << std::endl;
            return;
        }
    delete vector3;

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void dotTest()
{
    LOGGER
    VECTOR1
    VECTOR2

    if (fabs(IVector::dot(vector1, vector2) - 18) >= TOL)
        std::cout << "Error in dotTest()" << std::endl;

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void equalsTest()
{
    LOGGER
    VECTOR1
    IVector* vector2 = vector1->clone();

    if (!IVector::equals(vector1, vector2, IVector::NORM::CHEBYSHEV, TOL))
        std::cout << "Error in equalsTest()" << std::endl;

    DELETE_LOGGER
    DELETE_VECTOR1
    DELETE_VECTOR2
}

void normTest()
{
    LOGGER
    VECTOR1

    bool firstNorm = vector1->norm(IVector::NORM::FIRST) == 6.9;
    bool secondNorm = fabs(vector1->norm(IVector::NORM::SECOND) - 4.33013) < TOL;
    bool chebyshevNorm = vector1->norm(IVector::NORM::CHEBYSHEV) == 3.5;

    if (!firstNorm || !secondNorm || !chebyshevNorm)
        std::cout << "Error in normTest()" << std::endl;

    DELETE_LOGGER
    DELETE_VECTOR1
}

void applyingTest()
{
    LOGGER
    VECTOR1

    double value = 5.3;
    vector1->applyFunction([value](double element){return element + value; });

    const double* data1 = vector1->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (fabs(data1[i] - arr1[i] - value) >= TOL)
        {
            std::cout << "Error in applyingTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
}

void foreachTest()
{
    LOGGER
    VECTOR1

    vector1->foreach([](double element){return; });

    const double* data1 = vector1->getData();
    for (size_t i = 0; i < DIM; ++i)
        if (data1[i] != arr1[i])
        {
            std::cout << "Error in foreachTest()" << std::endl;
            return;
        }

    DELETE_LOGGER
    DELETE_VECTOR1
}

void vectorTests()
{
    creatingTest();
    copyingTest();
    movingTest();
    cloningTest();
    gettingDataTest();
    settingDataTest();
    gettingCordTest();
    settingCordTest();
    scaleTest();
    gettingDimTest();
    incTest();
    decTest();
    addTest();
    subTest();
    dotTest();
    equalsTest();
    normTest();
    applyingTest();
    foreachTest();
}
