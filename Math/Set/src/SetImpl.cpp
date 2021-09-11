#include <cmath>
#include <cstring>
#include "SetImpl.h"
#define CAPACITY 100

ILogger* SetImpl::logger = nullptr;
ILogger* SetImpl::IteratorImpl::logger = nullptr;

SetImpl::SetImpl()
{
    capacity = CAPACITY;
    data = new double[capacity];
    setSize = 0;
    dim = 0;
    lastIndex = 0;
    setBlock = SetBlockImpl::createControlBlock(this);
}

SetImpl* SetImpl::clone() const
{
    SetImpl* newSet = new SetImpl();
    if (newSet != nullptr)
    {
        newSet->dim = this->dim;
        newSet->setSize = this->setSize;
        newSet->capacity = this->capacity;
        newSet->uniqueIndexes = this->uniqueIndexes;
        newSet->lastIndex = this->lastIndex;
        memcpy(newSet->data, data, setSize * dim * sizeof(double));
    }
    return newSet;
}

size_t SetImpl::getDim() const
{
    return dim;
}

size_t SetImpl::getSize() const
{
    return setSize;
}

RC SetImpl::getCopy(size_t index, IVector *& val) const
{
    double* temp = new double[dim];
    val = IVector::createVector(dim, temp);
    delete[] temp;

    if (getCoords(index, val) != RC::SUCCESS)
    {
        delete val;
        return RC::VECTOR_NOT_FOUND;
    }
    return RC::SUCCESS;
}

RC SetImpl::findFirstAndCopy(IVector const * const& pat, IVector::NORM n, double tol, IVector *& val) const
{
    if (pat == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    if (pat->getDim() != dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }
    if (n == IVector::NORM::AMOUNT || tol < 0)
    {
        logger->severe(RC::INVALID_ARGUMENT);
        return RC::INVALID_ARGUMENT;
    }
    if (std::isnan(tol))
    {
        logger->severe(RC::NOT_NUMBER);
        return RC::NOT_NUMBER;
    }
    if (std::isinf(tol))
    {
        logger->severe(RC::INFINITY_OVERFLOW);
        return RC::INFINITY_OVERFLOW;
    }

    double* temp = new double[dim];
    val = IVector::createVector(dim, temp);
    delete[] temp;
    if (val == nullptr)
        return RC::ALLOCATION_ERROR;

    size_t i = 0;
    while (true)
    {
        if (i >= setSize)
        {
            delete val;
            return RC::VECTOR_NOT_FOUND;
        }
        val->setData(dim, data + i * dim);
        if (IVector::equals(pat, val, n, tol))
            return RC::SUCCESS;
        ++i;
    }
}

RC SetImpl::getCoords(size_t index, IVector * const& val) const
{
    if (setSize == 0)
    {
        logger->severe(RC::SOURCE_SET_EMPTY);
        return RC::SOURCE_SET_EMPTY;
    }
    if (index >= setSize || index < 0)
    {
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }
    if (val == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return RC::ALLOCATION_ERROR;
    }

    double* temp = new double[dim];
    memcpy(temp, data + index * dim, dim * sizeof(double));
    RC error = val->setData(dim, temp);
    delete[] temp;
    return error;
}

RC SetImpl::findFirstAndCopyCoords(IVector const * const& pat, IVector::NORM n, double tol, IVector * const& val) const
{
    IVector* temp;
    RC error = findFirstAndCopy(pat, n, tol, temp);
    if (error != RC::SUCCESS)
        return error;
    error = val->setData(temp->getDim(), temp->getData());
    delete temp;
    return error;
}

RC SetImpl::findFirst(IVector const * const& pat, IVector::NORM n, double tol) const
{
    IVector* temp;
    RC error = findFirstAndCopy(pat, n, tol, temp);
    if (error == RC::SUCCESS)
        delete temp;
    return error;
}

RC SetImpl::insert(IVector const * const& val, IVector::NORM n, double tol)
{
    if (val == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }

    size_t valDim = val->getDim();
    if (setSize != 0)
    {
        if (dim != valDim)
        {
            logger->severe(RC::MISMATCHING_DIMENSIONS);
            return RC::MISMATCHING_DIMENSIONS;
        }
        RC error = findFirst(val, n, tol);
        if (error == RC::SUCCESS)
        {
            logger->info(RC::VECTOR_ALREADY_EXIST);
            return RC::VECTOR_ALREADY_EXIST;
        }
        if (error != RC::VECTOR_NOT_FOUND)
            return error;
    }
    else
        dim = valDim;

    while (capacity < (setSize + 1) * dim)
        capacity *= 2;

    double* temp = data;
    data = new double[capacity];
    memcpy(data, temp, setSize * dim * sizeof(double));
    delete[] temp;

    const double* tempVal = val->getData();
    for (size_t i = 0; i < dim; ++i)
        data[setSize * dim + i] = tempVal[i];
    ++setSize;
    ++lastIndex;
    uniqueIndexes.push_back(lastIndex);
    return RC::SUCCESS;
}

RC SetImpl::remove(size_t index)
{
    if (index >= setSize)
    {
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }

    double* newData = new double[capacity];
    for (size_t iSet = 0, iVector = 0; iSet < setSize; ++iSet)
        if (iSet != index)
        {
            for (size_t i = 0; i < dim; ++i, ++iVector)
                newData[iVector] = data[iSet * setSize + i];
            break;
        }
    --setSize;
    uniqueIndexes.erase(uniqueIndexes.begin() + index);
    if (setSize == 0)
        lastIndex = 0;
    delete[] data;
    data = newData;
    return RC::SUCCESS;
}

RC SetImpl::remove(IVector const * const& pat, IVector::NORM n, double tol)
{
    if (pat == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }

    IVector* value;
    RC error = findFirstAndCopy(pat, n, tol, value);
    if (error != RC::SUCCESS)
        return error;

    double* newData = new double[capacity];
    IVector* temp;
    for (size_t iSet = 0, iVector = 0; iSet < setSize; ++iSet)
    {
        if (getCopy(iSet, temp) == RC::SUCCESS && IVector::equals(temp, value, n, tol))
            uniqueIndexes.erase(uniqueIndexes.begin() + iSet);
        else
            for (size_t i = 0; i < dim; ++i, ++iVector)
                newData[iVector] = data[iSet * dim + i];
        delete temp;
    }
    --setSize;
    if (setSize == 0)
        lastIndex = 0;
    delete[] data;
    delete value;
    data = newData;
    return RC::SUCCESS;
}

ISet::IIterator* SetImpl::getIterator(size_t index) const
{
    if (index >= setSize)
    {
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return nullptr;
    }
    IVector* value;
    RC error = getCopy(index, value);
    if (error != RC::SUCCESS)
        return nullptr;
    IteratorImpl::setLogger(logger);
    return IteratorImpl::createIterator(setBlock, uniqueIndexes[index], value);
}

ISet::IIterator* SetImpl::getBegin() const
{
    return getIterator(0);
}

ISet::IIterator* SetImpl::getEnd() const
{
    return getIterator(setSize - 1);
}

size_t SetImpl::indexToPos(size_t index) const
{
    for (size_t i = 0; i < uniqueIndexes.size(); ++i)
        if (uniqueIndexes[i] == index)
            return i;
    return lastIndex;
}

RC SetImpl::getNextIterator(IVector *const &vec, size_t &index, size_t indexInc) const
{
    if (index > lastIndex || index < 0)
    {
        logger->severe(RC::INVALID_ARGUMENT);
        return RC::INVALID_ARGUMENT;
    }
    size_t temp = indexToPos(index) + indexInc;
    if (temp >= setSize)
    {
        index = lastIndex;
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }
    index = uniqueIndexes[temp];
    return getCoords(temp, vec);
}

RC SetImpl::getPreviousIterator(IVector *const &vec, size_t &index, size_t indexInc) const
{
    if (index > lastIndex || index < 0)
    {
        logger->severe(RC::INVALID_ARGUMENT);
        return RC::INVALID_ARGUMENT;
    }
    size_t temp = indexToPos(index) - indexInc;
    if (temp < 0)
    {
        index = lastIndex;
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }
    index = uniqueIndexes[temp];
    return getCoords(temp, vec);
}

RC SetImpl::getBeginIterator(IVector *const &vec, size_t &index) const
{
    index = uniqueIndexes.front();
    return getCoords(0, vec);
}

RC SetImpl::getEndIterator(IVector *const &vec, size_t &index) const
{
    index = uniqueIndexes.back();
    return getCoords(setSize - 1, vec);
}

SetImpl::~SetImpl()
{
    delete setBlock;
    delete[] data;
}

RC ISet::setLogger(ILogger* const pLogger)
{
    if (pLogger == nullptr)
        return RC::NULLPTR_ERROR;
    SetImpl::logger = pLogger;
    return RC::SUCCESS;
}

ILogger* ISet::getLogger()
{
    return SetImpl::logger;
}

ISet* ISet::createSet()
{
    SetImpl* setPtr = new(std::nothrow)SetImpl;
    if (setPtr == nullptr)
    {
        getLogger()->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }
    return setPtr;
}

ISet* ISet::makeIntersection(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    IVector *data, *value;
    ISet* newSet = ISet::createSet();
    if (newSet == nullptr)
        return nullptr;

    IIterator* it1;
    for (it1 = op1->getBegin(); it1->isValid(); it1->next())
    {
        if (it1->getVectorCopy(data) != RC::SUCCESS)
        {
            delete data;
            continue;
        }
        if (op2->findFirstAndCopy(data, n, tol, value) != RC::SUCCESS)
        {
            delete data;
            continue;
        }
        if (newSet->insert(value, n, tol) != RC::SUCCESS)
        {
            delete data;
            continue;
        }
        delete data;
        delete value;
    }
    delete it1;
    return newSet;
}

ISet* ISet::makeUnion(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    ISet* newSet = op1->clone();
    if (newSet == nullptr)
        return nullptr;

    IVector* value;
    IIterator* it2;
    for (it2 = op2->getBegin(); it2->isValid(); it2->next())
    {
        if (it2->getVectorCopy(value) != RC::SUCCESS || newSet->insert(value, n, tol) != RC::SUCCESS)
        {
            delete value;
            continue;
        }
        else
            delete value;
    }
    delete it2;
    return newSet;
}

ISet* ISet::sub(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }

    ISet* newSet = op1->clone();
    ISet* interSet = ISet::makeIntersection(op1, op2, n, tol);
    if (newSet == nullptr || interSet == nullptr)
        return nullptr;

    IVector* value = nullptr;
    IIterator* it;
    for (it = interSet->getBegin(); it->isValid(); it->next())
    {
        if (it->getVectorCopy(value) != RC::SUCCESS || newSet->remove(value, n, tol) != RC::SUCCESS)
        {
            delete value;
            continue;
        }
        else
            delete value;
    }
    delete interSet;
    delete it;
    return newSet;
}

ISet* ISet::symSub(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }

    ISet* unionSet = ISet::makeUnion(op1, op2, n, tol);
    ISet* interSet = ISet::makeIntersection(op1, op2, n, tol);
    if (unionSet == nullptr || interSet == nullptr)
        return nullptr;
    ISet* newSet = ISet::sub(unionSet, interSet, n, tol);

    delete unionSet;
    delete interSet;
    return newSet;
}

bool ISet::equals(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return false;
    }
    ISet* temp = ISet::sub(op1, op2, n, tol);
    if (temp == nullptr || temp->getSize() != 0)
    {
        delete temp;
        return false;
    }
    delete temp;
    return true;
}

bool ISet::subSet(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return false;
    }
    ISet* temp = ISet::makeIntersection(op1, op2, n, tol);
    if (temp == nullptr || temp->getSize() == 0 || !ISet::equals(temp, op2, n, tol))
    {
        delete temp;
        return false;
    }
    delete temp;
    return true;
}

ISet::~ISet() = default;
