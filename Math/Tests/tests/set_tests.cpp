#include "set_tests.h"

void testCreate()
{
    LOGGER
    SET1
    if (set1 == nullptr)
        std::cout << "Error in testCreat()" << std::endl;
    DELETE_SET1
    DELETE_LOGGER
}

void testClone()
{
    LOGGER
    VECTOR1
    SET1

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testClone()" << std::endl;
    ISet* set2 = set1->clone();
    if (!ISet::equals(set1, set2, DEFAULT_NORM, TOL))
        std::cout << "Error in testClone()" << std::endl;
    delete set2;

    DELETE_SET1
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testGetDim()
{
    LOGGER
    SET1

    if (set1->getDim() != 0)
        std::cout << "Error in testGetDim()" << std::endl;

    DELETE_SET1
    DELETE_LOGGER
}

void testGetSize()
{
    LOGGER
    SET1

    if (set1->getDim() != 0)
        std::cout << "Error in testGetSize()" << std::endl;

    DELETE_SET1
    DELETE_LOGGER
}

void testGetCopy()
{
    LOGGER
    VECTOR1
    SET1

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testGetCopy()" << std::endl;

    IVector* temp;
    if (set1->getCopy(0, temp) != RC::SUCCESS || !IVector::equals(vector1, temp, DEFAULT_NORM, TOL))
        std::cout << "Error in testGetCopy()" << std::endl;
    delete temp;

    DELETE_SET1
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testFindFirstAndCopy()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1

   if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS)
       std::cout << "Error in testFindFirstAndCopy()" << std::endl;

    IVector* temp;
    if (set1->findFirstAndCopy(vector1, DEFAULT_NORM, TOL, temp) != RC::SUCCESS || !IVector::equals(temp, vector1, DEFAULT_NORM, TOL))
        std::cout << "Error in testFindFirstAndCopy()" << std::endl;
    delete temp;

    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testGetCoords()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->getCoords(0, vector2) != RC::SUCCESS)
        std::cout << "Error in testGetCoords()" << std::endl;
    if (!IVector::equals(vector1, vector2, DEFAULT_NORM, TOL))
        std::cout << "Error in testGetCoords()" << std::endl;

    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testFindFirstAndCopyCoords()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->findFirstAndCopyCoords(vector1, DEFAULT_NORM, TOL, vector2) != RC::SUCCESS)
        std::cout << "Error in testFindFirstAndCopyCoords()" << std::endl;
    if (!IVector::equals(vector1, vector2, DEFAULT_NORM, TOL))
        std::cout << "Error in testFindFirstAndCopyCoords()" << std::endl;

    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testFindFirst()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->findFirst(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testFindFirst()" << std::endl;
    if (set1->findFirst(vector2, DEFAULT_NORM, TOL) != RC::VECTOR_NOT_FOUND)
        std::cout << "Error in testFindFirst()" << std::endl;

    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testInsert()
{
    LOGGER
    VECTOR1
    SET1

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->getSize() != 1 || set1->getDim() != vector1->getDim())
        std::cout << "Error in testInsert()" << std::endl;
    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::VECTOR_ALREADY_EXIST || set1->getSize() != 1)
        std::cout << "Error in testInsert()" << std::endl;

    DELETE_SET1
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testRemoveByIndex()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testRemoveByIndex()" << std::endl;
    if (set1->remove(1) != RC::SUCCESS || set1->getSize() != 1)
        std::cout << "Error in testRemoveByIndex()" << std::endl;
    if (set1->remove(1) != RC::INDEX_OUT_OF_BOUND || set1->getSize() != 1)
        std::cout << "Error in testRemoveByIndex()" << std::endl;

    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testRemoveByPattern()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testRemoveByPattern()" << std::endl;
    if (set1->remove(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->getSize() != 1)
        std::cout << "Error in testRemoveByPattern()" << std::endl;
    IVector* temp;
    if (set1->getCopy(0, temp) != RC::SUCCESS || !IVector::equals(temp, vector1, DEFAULT_NORM, TOL))
        std::cout << "Error in testRemoveByPattern()" << std::endl;
    delete temp;

    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testMakeIntersection()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1
    SET2

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS
       || set2->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testMakeIntersection()" << std::endl;
    ISet* interSet = ISet::makeIntersection(set1, set2, DEFAULT_NORM, TOL);
    if (interSet == nullptr || !ISet::equals(interSet, set2, DEFAULT_NORM, TOL))
        std::cout << "Error in testMakeIntersection()" << std::endl;
    delete interSet;

    DELETE_SET2
    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testMakeUnion()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1
    SET2

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS
        || set2->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testMakeUnion()" << std::endl;
    ISet* unionSet = ISet::makeUnion(set1, set2, DEFAULT_NORM, TOL);
    if (unionSet == nullptr || !ISet::equals(unionSet, set1, DEFAULT_NORM, TOL))
        std::cout << "Error in testMakeUnion()" << std::endl;
    delete unionSet;

    DELETE_SET2
    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testSub()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1
    SET2
    SET3

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS
        || set2->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set3->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testSub()" << std::endl;
    ISet* subSet = ISet::sub(set1, set2, DEFAULT_NORM, TOL);
    if (subSet == nullptr || !ISet::equals(subSet, set3, DEFAULT_NORM, TOL))
        std::cout << "Error in testSub()" << std::endl;
    delete subSet;

    DELETE_SET3
    DELETE_SET2
    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testSymSub()
{
    LOGGER
    VECTOR1
    VECTOR2
    VECTOR3
    SET1
    SET2
    SET3

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS
        || set2->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set2->insert(vector3, DEFAULT_NORM, TOL) != RC::SUCCESS
        || set3->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS || set3->insert(vector3, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testSymSub()" << std::endl;
    ISet* subSet = ISet::symSub(set1, set2, DEFAULT_NORM, TOL);
    if (subSet == nullptr || !ISet::equals(subSet, set3, DEFAULT_NORM, TOL))
        std::cout << "Error in testSymSub()" << std::endl;
    delete subSet;

    DELETE_SET3
    DELETE_SET2
    DELETE_SET1
    DELETE_VECTOR3
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testEquals()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1
    SET2

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set2->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testEquals()" << std::endl;
    if (!ISet::equals(set1, set2, DEFAULT_NORM, TOL))
        std::cout << "Error in testEquals()" << std::endl;
    if (set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS || ISet::equals(set1, set2, DEFAULT_NORM, TOL))
        std::cout << "Error in testEquals()" << std::endl;

    DELETE_SET2
    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void testSubSet()
{
    LOGGER
    VECTOR1
    VECTOR2
    SET1
    SET2

    if (set1->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS || set1->insert(vector2, DEFAULT_NORM, TOL) != RC::SUCCESS ||
        set2->insert(vector1, DEFAULT_NORM, TOL) != RC::SUCCESS)
        std::cout << "Error in testSubSet()" << std::endl;
    if (!ISet::subSet(set2, set1, DEFAULT_NORM, TOL))
        std::cout << "Error in testSubSet()" << std::endl;
    if (set1->remove(0) != RC::SUCCESS || ISet::subSet(set2, set1, DEFAULT_NORM, TOL))
        std::cout << "Error in testSubSet()" << std::endl;

    DELETE_SET2
    DELETE_SET1
    DELETE_VECTOR2
    DELETE_VECTOR1
    DELETE_LOGGER
}

void setTests()
{
    testCreate();
    testClone();
    testGetDim();
    testGetSize();
    testGetCopy();
    testFindFirstAndCopy();
    testGetCoords();
    testFindFirstAndCopyCoords();
    testFindFirst();
    testInsert();
    testRemoveByIndex();
    testRemoveByPattern();
    testMakeIntersection();
    testMakeUnion();
    testSub();
    testSymSub();
    testEquals();
    testSubSet();
}
