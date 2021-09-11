#include "multiIndex_tests.h"

void testCreateIndex()
{
    LOGGER
    INDEX1

    IMultiIndex* temp = IMultiIndex::createMultiIndex(2, nullptr);
    if (index1 == nullptr || temp != nullptr)
        std::cout << "Error in testCreateIndex()" << std::endl;
    delete temp;

    DELETE_LOGGER
    DELETE_INDEX1
}

void testCloneIndex()
{
    LOGGER
    INDEX1

    IMultiIndex* index2 = index1->clone();
    const size_t *data1 = index1->getData(), *data2 = index2->getData();
    size_t dim = index1->getDim();
    for (size_t i = 0; i < dim; ++i)
        if (data1[i] != data2[i])
            std::cout << "Error in testCloneIndex()" << std::endl;
    delete index2;

    DELETE_LOGGER
    DELETE_INDEX1
}

void testGetDimIndex()
{
    LOGGER
    INDEX1

    if (index1->getDim() == sizeof(arr1))
        std::cout << "Error in testGetDimIndex()" << std::endl;

    DELETE_LOGGER
    DELETE_INDEX1
}

void testGetDataIndex()
{
    LOGGER
    INDEX1

    size_t dim = index1->getDim();
    const size_t* data1 = index1->getData();
    for (size_t i = 0; i < dim; ++i)
        if (data1[i] != arr1[i])
            std::cout << "Error in testGetDataIndex()" << std::endl;

    DELETE_LOGGER
    DELETE_INDEX1
}

void testSetDataIndex()
{
    LOGGER
    INDEX1
    INDEX2

    size_t dim = index1->getDim();
    index2->setData(index1->getDim(), index1->getData());
    const size_t* data = index2->getData();
    for (size_t i = 0; i < dim; ++i)
        if (data[i] != arr1[i])
            std::cout << "Error in testSetDataIndex()" << std::endl;

    DELETE_LOGGER
    DELETE_INDEX1
    DELETE_INDEX2
}

void testGetAxisIndex()
{
    LOGGER
    INDEX1

    size_t value;
    index1->getAxisIndex(0, value);
    if (value != arr1[0])
        std::cout << "Error in testGetAxisIndex()" << std::endl;

    DELETE_LOGGER
    DELETE_INDEX1
}

void testSetAxisIndex()
{
    LOGGER
    INDEX1

    index1->setAxisIndex(0, 15);
    if (index1->getData()[0] != 15)
        std::cout << "Error in testSetAxisIndex()" << std::endl;

    DELETE_LOGGER
    DELETE_INDEX1
}

void testIncAxisIndex()
{
    LOGGER
    INDEX1

    index1->incAxisIndex(0, 9);
    if (index1->getData()[0] != arr1[0] + 9)
        std::cout << "Error in testIncAxisIndex()" << std::endl;

    DELETE_LOGGER
    DELETE_INDEX1
}

void multiIndexTests()
{
    testCreateIndex();
    testCloneIndex();
    testGetDimIndex();
    testGetDataIndex();
    testSetDataIndex();
    testGetAxisIndex();
    testSetAxisIndex();
    testIncAxisIndex();
}
