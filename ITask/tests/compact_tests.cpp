#include "compact_tests.h"

void testCreateCompact()
{
    LOGGER
    COMPACT1

    if (compact1 == nullptr)
        std::cout << "Error in testCreateCompact()" << std::endl;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testCloneCompact()
{
    LOGGER
    COMPACT1

    ICompact* temp = compact1->clone();
    if (temp == nullptr)
        std::cout << "Error in testCloneCompact()" << std::endl;
    delete temp;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testIsInside()
{
    LOGGER
    COMPACT1

    double temp[] = {3, 1};
    IVector* newVector = IVector::createVector(SIZE, temp);
    if (!compact1->isInside(newVector))
        std::cout << "Error in testIsInside()" << std::endl;
    delete newVector;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testGetVectorCoordsCompact()
{
    LOGGER
    COMPACT1

    size_t dim = compact1->getDim();
    double* temp = new double[dim];
    IVector* newVector = IVector::createVector(dim, temp);
    delete[] temp;

    size_t indexData[] = {2, 2}, result[] = {2, 2};
    IMultiIndex* index = IMultiIndex::createMultiIndex(SIZE, indexData);
    if (compact1->getVectorCoords(index, newVector) != RC::SUCCESS)
        std::cout << "Error in testGetVectorCoordsCompact()" << std::endl;

    const double* data = newVector->getData();
    for (size_t i = 0; i < SIZE; ++i)
        if (data[i] != result[i])
            std::cout << "Error in testGetVectorCoordsCompact()" << std::endl;
    delete newVector;
    delete index;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testGetVectorCopyCompact()
{
    LOGGER
    COMPACT1

    size_t dim = compact1->getDim();
    IVector* newVector;

    size_t indexData[] = {2, 2}, result[] = {2, 2};
    IMultiIndex* index = IMultiIndex::createMultiIndex(SIZE, indexData);
    if (compact1->getVectorCopy(index, newVector) != RC::SUCCESS)
        std::cout << "Error in testGetVectorCopyCompact()" << std::endl;

    const double* data = newVector->getData();
    for (size_t i = 0; i < SIZE; ++i)
        if (data[i] != result[i])
            std::cout << "Error in testGetVectorCopyCompact()" << std::endl;
    delete newVector;
    delete index;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testGetLeftBoundary()
{
    LOGGER
    COMPACT1

    IVector* left = nullptr;
    if (compact1->getLeftBoundary(left) != RC::SUCCESS || !IVector::equals(left1, left, DEFAULT_NORM, TOL))
        std::cout << "Error in testGetLeftBoundary()" << std::endl;
    delete left;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testGetRightBoundary()
{
    LOGGER
    COMPACT1

    IVector* right = nullptr;
    if (compact1->getRightBoundary(right) != RC::SUCCESS || !IVector::equals(right1, right, DEFAULT_NORM, TOL))
        std::cout << "Error in testGetRightBoundary()" << std::endl;
    delete right;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testGetDimCompact()
{
    LOGGER
    COMPACT1

    if (compact1->getDim() != left1->getDim())
        std::cout << "Error in testGetDimCompact()" << std::endl;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testGetGrid()
{
    LOGGER
    COMPACT1

    IMultiIndex* temp = compact1->getGrid();

    const size_t *tempData = temp->getData(), *indexData = grid1->getData();
    size_t dim = compact1->getDim();
    for (size_t i = 0; i < dim; ++i)
        if (tempData[i] != indexData[i])
            std::cout << "Error in testGetGrid()" << std::endl;
    delete temp;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void testIntersectionCompact()
{
    LOGGER
    COMPACT1
    COMPACT2

    ICompact* inter = ICompact::createIntersection(compact1, compact2, grid1, TOL);
    if (inter == nullptr)
        std::cout << "Error in testIntersectionCompact()" << std::endl;

    IVector *left, *right;
    if (inter->getLeftBoundary(left) != RC::SUCCESS || inter->getRightBoundary(right) != RC::SUCCESS)
        std::cout << "Error in testIntersectionCompact()" << std::endl;
    if (!IVector::equals(left, left2, DEFAULT_NORM, TOL) || !IVector::equals(right, right1, DEFAULT_NORM, TOL))
        std::cout << "Error in testIntersectionCompact()" << std::endl;

    IMultiIndex* grid = inter->getGrid();
    if (grid == nullptr)
        std::cout << "Error in testIntersectionCompact()" << std::endl;
    size_t dim = grid->getDim();
    const size_t *gridData = grid->getData(), *grid1Data = grid1->getData();
    for (size_t i = 0; i < dim; ++i)
        if (gridData[i] != grid1Data[i])
            std::cout << "Error in testIntersectionCompact()" << std::endl;

    delete grid;
    delete left;
    delete right;
    delete inter;

    DELETE_COMPACT2
    DELETE_COMPACT1
    DELETE_LOGGER
}

void testSpan()
{
    LOGGER
    COMPACT1
    COMPACT2

    ICompact* span = ICompact::createCompactSpan(compact1, compact2, grid1);
    if (span == nullptr)
        std::cout << "Error in testSpan()" << std::endl;

    IVector *left, *right;
    if (span->getLeftBoundary(left) != RC::SUCCESS || span->getRightBoundary(right) != RC::SUCCESS)
        std::cout << "Error in testSpan()" << std::endl;
    if (!IVector::equals(left, left1, DEFAULT_NORM, TOL) || !IVector::equals(right, right2, DEFAULT_NORM, TOL))
        std::cout << "Error in testSpan()" << std::endl;

    IMultiIndex* grid = span->getGrid();
    if (grid == nullptr)
        std::cout << "Error in testSpan()" << std::endl;
    size_t dim = grid->getDim();
    const size_t *gridData = grid->getData(), *grid1Data = grid1->getData();
    for (size_t i = 0; i < dim; ++i)
        if (gridData[i] != grid1Data[i])
            std::cout << "Error in testSpan()" << std::endl;

    delete grid;
    delete left;
    delete right;
    delete span;

    DELETE_COMPACT2
    DELETE_COMPACT1
    DELETE_LOGGER
}

void testIteratorCompact()
{
    LOGGER
    COMPACT1

    IVector* vector = nullptr;
    size_t orderData[] = {0, 1};
    IVector* result[] = {left1, right1};
    IMultiIndex* order = IMultiIndex::createMultiIndex(SIZE, orderData);
    ICompact::IIterator* iterator = compact1->getBegin(order);

    for (size_t i = 0; i < SIZE, iterator->isValid(); ++i, iterator->next())
    {
        if (iterator->getVectorCopy(vector) != RC::SUCCESS || vector == nullptr || !IVector::equals(vector, result[i], DEFAULT_NORM, TOL))
            std::cout << "Error in testIteratorCompact()" << std::endl;
        delete vector;
    }

    delete iterator;
    delete order;

    DELETE_COMPACT1
    DELETE_LOGGER
}

void compactTests()
{
    testCreateCompact();
    testCloneCompact();
    testGetDimCompact();
    testGetGrid();
    testGetLeftBoundary();
    testGetRightBoundary();
    testGetVectorCoordsCompact();
    testGetVectorCopyCompact();
    testIsInside();
    testIntersectionCompact();
    testSpan();
    testIteratorCompact();
}
