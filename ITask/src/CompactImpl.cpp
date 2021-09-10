#include <cmath>
#include "CompactImpl.h"
#define DELETE_ALL delete left1; delete left2; delete right1; delete right2; delete resGrid; delete resLeft; delete resRight; delete start; delete end;

ILogger* CompactImpl::logger = nullptr;

CompactImpl::CompactImpl(const IVector* left, const IVector* right, const IMultiIndex* nodes)
{
    this->left = left->clone();
    this->right = right->clone();
    grid = nodes->clone();
    dim = left->getDim();
    block = CompactBlockImpl::createCompactBlock(this);
}

ICompact* CompactImpl::clone() const
{
    return (CompactImpl*)createCompact(left, right, grid);
}

bool CompactImpl::isInside(IVector const * const&vec) const
{
    if (vec == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return false;
    }
    if (vec->getDim() != dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return false;
    }

    const double* leftData = left->getData();
    const double* rightData = right->getData();
    const double* vecData = vec->getData();
    for (size_t i = 0; i < dim; ++i)
        if (vecData[i] < leftData[i] || vecData[i] > rightData[i])
            return false;
    return true;
}

RC CompactImpl::getVectorCopy(IMultiIndex const *index, IVector *& val) const
{
    double* temp = new double[dim];
    IVector* newVector = IVector::createVector(dim, temp);
    delete[] temp;
    if (newVector == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return RC::ALLOCATION_ERROR;
    }

    RC error = getVectorCoords(index, newVector);
    if (error != RC::SUCCESS)
        delete newVector;
    else
        val = newVector;
    return error;
}

RC CompactImpl::getVectorCoords(IMultiIndex const *index, IVector * const& val) const
{
    if (index == nullptr || val == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    if (index->getDim() != dim || val->getDim() != dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }

    const size_t* indexData = index->getData();
    const size_t* gridData = grid->getData();
    for (size_t i = 0; i < dim; ++i)
        if (indexData[i] > gridData[i])
        {
            logger->severe(RC::INDEX_OUT_OF_BOUND);
            return RC::INDEX_OUT_OF_BOUND;
        }

    const double* leftData = left->getData();
    const double* rightData = right->getData();
    for (size_t i = 0; i < dim; ++i)
    {
        double delta = (double)(indexData[i]) / (gridData[i] - 1);
        val->setCord(i, (1.0 - delta) * leftData[i] + delta * rightData[i]);
    }
    return RC::SUCCESS;
}

RC CompactImpl::getLeftBoundary(IVector *& vec) const
{
    IVector* copy = left->clone();
    vec = copy;
    if (copy == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    return RC::SUCCESS;
}

RC CompactImpl::getRightBoundary(IVector *& vec) const
{
    IVector* copy = right->clone();
    vec = copy;
    if (copy == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    return RC::SUCCESS;
}

size_t CompactImpl::getDim() const
{
    return dim;
}

IMultiIndex* CompactImpl::getGrid() const
{
    return grid->clone();
}

ICompact::IIterator* CompactImpl::getIterator(IMultiIndex const * const&index, IMultiIndex const * const &bypassOrder) const
{
    if (index == nullptr || bypassOrder == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    if (index->getDim() != bypassOrder->getDim())
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return nullptr;
    }
    IVector* vectorCopy = nullptr;
    if (getVectorCopy(index, vectorCopy) != RC::SUCCESS)
    {
        delete vectorCopy;
        return nullptr;
    }

    IIterator* newIterator = IteratorImpl::createIterator(block, vectorCopy, index, bypassOrder);
    delete vectorCopy;
    IIterator::setLogger(logger);
    return newIterator;
}

ICompact::IIterator* CompactImpl::getBegin(IMultiIndex const * const &bypassOrder) const
{
    if (bypassOrder == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    if (bypassOrder->getDim() != dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return nullptr;
    }

    size_t* temp = new (std::nothrow) size_t[dim];
    if (temp == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }
    for (size_t i = 0; i < dim; ++i)
        temp[i] = 1;

    IMultiIndex* index = IMultiIndex::createMultiIndex(dim, temp);
    delete temp;
    IVector* vectorCopy = nullptr;
    if (index == nullptr || getLeftBoundary(vectorCopy) != RC::SUCCESS)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        delete index;
        delete vectorCopy;
        return nullptr;
    }

    IIterator* newIterator = IteratorImpl::createIterator(block, vectorCopy, index, bypassOrder);
    delete vectorCopy;
    delete index;
    IIterator::setLogger(logger);
    return newIterator;
}

ICompact::IIterator* CompactImpl::getEnd(IMultiIndex const * const &bypassOrder) const
{
    if (bypassOrder == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    if (bypassOrder->getDim() != dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return nullptr;
    }

    size_t* temp = new (std::nothrow) size_t[dim];
    if (temp == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }
    const size_t* gridData = grid->getData();
    for (size_t i = 0; i < dim; ++i)
        temp[i] = gridData[i];

    IMultiIndex* index = IMultiIndex::createMultiIndex(dim, temp);
    delete[] temp;
    IVector* vectorCopy = nullptr;
    if (index == nullptr || getRightBoundary(vectorCopy) != RC::SUCCESS)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        delete index;
        delete vectorCopy;
        return nullptr;
    }

    IIterator* newIterator = IteratorImpl::createIterator(block, vectorCopy, index, bypassOrder);
    delete vectorCopy;
    delete index;
    IIterator::setLogger(logger);
    return newIterator;
}

CompactImpl::~CompactImpl()
{
    delete left;
    delete right;
    delete grid;
    delete block;
}

ICompact* ICompact::createCompact(IVector const * vec1, IVector const * vec2, IMultiIndex const *nodeQuantities)
{
    if (vec1 == nullptr || vec2 == nullptr || nodeQuantities == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    size_t dim = vec1->getDim();
    if (dim != vec2->getDim() || dim != nodeQuantities->getDim())
    {
        getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return nullptr;
    }
    CompactImpl* compactPtr = new(std::nothrow)CompactImpl(vec1, vec2, nodeQuantities);
    if (compactPtr == nullptr)
    {
        getLogger()->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }
    return compactPtr;
}

RC ICompact::setLogger(ILogger* const logger)
{
    if (logger == nullptr)
        return RC::NULLPTR_ERROR;
    CompactImpl::logger = logger;
    return RC::SUCCESS;
}

ILogger* ICompact::getLogger()
{
    return CompactImpl::logger;
}

static bool interCheck(double left1, double left2, double right1, double right2, double& resLeft, double& resRight)
{
    if (right1 < left2 || right2 < left1)
    {
        resLeft = 0;
        resRight = 0;
        return false;
    }
    if (left2 < left1 && right1 < right2)
    {
        resLeft = left1;
        resRight = right1;
        return true;
    }
    if (left1 < left2 && right2 < right1)
    {
        resLeft = left2;
        resRight = right2;
        return true;
    }
    if (left1 < left2 && left2 < right1 && right1 < right2)
    {
        resLeft = left2;
        resRight = right1;
        return true;
    }
    if (left2 < left1 && left1 < right2 && right2 < right1)
    {
        resLeft = left1;
        resRight = right1;
        return true;
    }
    else
        return false;
}

static void buildUnion(const double* data, double& resLeft, double& resRight)
{
    size_t size = 2;
    resLeft = data[0];
    resRight = data[0];
    for (size_t i = 1; i < size * 2; ++i)
    {
        if (data[i] < resLeft)
            resLeft = data[i];
        if (data[i] > resRight)
            resRight = data[i];
    }
}

ICompact* ICompact::createIntersection(ICompact const *op1, ICompact const *op2, IMultiIndex const* const grid, double tol)
{
    if (op1 == nullptr || op2 == nullptr || grid == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    size_t dim = op1->getDim();
    if (dim != op2->getDim() || dim != grid->getDim())
    {
        getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return nullptr;
    }
    if (std::isnan(tol) || std::isinf(tol))
    {
        getLogger()->severe(RC::NOT_NUMBER);
        return nullptr;
    }

    double resLeft[dim], resRight[dim];
    IMultiIndex* resGrid = grid->clone();
    IVector *left1 = nullptr, *left2 = nullptr, *right1 = nullptr, *right2 = nullptr, *start = nullptr, *end = nullptr;
    size_t size = 2;

    if (op1->getLeftBoundary(left1) != RC::SUCCESS || op2->getLeftBoundary(left2) != RC::SUCCESS || op1->getRightBoundary(right1) != RC::SUCCESS ||
        op2->getRightBoundary(right2) != RC::SUCCESS || resGrid == nullptr)
    {
        DELETE_ALL
        return nullptr;
    }
    const double *left1Data = left1->getData(), *left2Data = left2->getData(), *right1Data = right1->getData(), *right2Data = right2->getData();
    if (left1Data == nullptr || left2Data == nullptr || right1Data == nullptr || right2Data == nullptr)
    {
        DELETE_ALL
        return nullptr;
    }

    for (size_t i = 0; i < dim; ++i)
    {
        if (!interCheck(left1Data[i], left2Data[i], right1Data[i], right2Data[i], resLeft[i], resRight[i]))
        {
            DELETE_ALL
            return nullptr;
        }
        double tempData[size] = {resLeft[i], resRight[i]};
        IVector* tempInter = IVector::createVector(size, tempData);
        if (tempInter == nullptr)
        {
            delete tempData;
            delete tempInter;
            DELETE_ALL
            return nullptr;
        }
        if (tempInter->norm(IVector::NORM::FIRST) < tol)
            resGrid->setAxisIndex(i, 1);
        delete[] tempData;
        delete tempInter;
    }

    start = IVector::createVector(dim, resLeft);
    end = IVector::createVector(dim, resRight);
    if (start == nullptr || end == nullptr)
    {
        DELETE_ALL
        return nullptr;
    }
    ICompact* inter = ICompact::createCompact(start, end, resGrid);
    if (inter == nullptr)
    {
        DELETE_ALL
        return nullptr;
    }
    DELETE_ALL
    return inter;
}

ICompact* ICompact::createCompactSpan(ICompact const *op1, ICompact const *op2, IMultiIndex const* const grid)
{
    if (op1 == nullptr || op2 == nullptr || grid == nullptr)
    {
        getLogger()->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    size_t dim = op1->getDim();
    if (dim != op2->getDim())
    {
        getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return nullptr;
    }

    double resLeft[dim], resRight[dim];
    IMultiIndex* resGrid = nullptr;
    IVector *left1 = nullptr, *left2 = nullptr, *right1 = nullptr, *right2 = nullptr, *start = nullptr, *end = nullptr;

    if (op1->getLeftBoundary(left1) != RC::SUCCESS || op2->getLeftBoundary(left2) != RC::SUCCESS || op1->getRightBoundary(right1) != RC::SUCCESS ||
        op2->getRightBoundary(right2) != RC::SUCCESS)
    {
        DELETE_ALL
        return nullptr;
    }
    const double *left1Data = left1->getData(), *left2Data = left2->getData(), *right1Data = right1->getData(), *right2Data = right2->getData();
    if (left1Data == nullptr || left2Data == nullptr || right1Data == nullptr || right2Data == nullptr)
    {
        DELETE_ALL
        return nullptr;
    }

    for (size_t i = 0; i < dim; ++i)
    {
        double temp[4] = {left1Data[i], right1Data[i], left2[i], right2Data[i]};
        buildUnion(temp, resLeft[i], resRight[i]);
        delete temp;
    }

    start = IVector::createVector(dim, resLeft);
    end = IVector::createVector(dim, resRight);
    if (start == nullptr || end == nullptr)
    {
        DELETE_ALL
        return nullptr;
    }
    ICompact* unionCompact = ICompact::createCompact(start, end, grid);
    if (unionCompact == nullptr)
    {
        DELETE_ALL
        return nullptr;
    }
    DELETE_ALL
    return unionCompact;
}

ICompact::~ICompact() = default;
