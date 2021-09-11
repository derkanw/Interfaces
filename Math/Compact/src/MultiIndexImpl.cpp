#include <cmath>
#include <cstring>
#include <cstdint>
#include <new>
#include "MultiIndexImpl.h"
#define PTR_DATA (size_t*)((uint8_t*)this + sizeof(MultiIndexImpl))

ILogger* MultiIndexImpl::logger = nullptr;

IMultiIndex* IMultiIndex::createMultiIndex(size_t dim, const size_t* indices)
{
    if (dim == 0)
        {
            MultiIndexImpl::logger->severe(RC::INVALID_ARGUMENT);
            return nullptr;
        }
        if (indices == nullptr)
        {
            MultiIndexImpl::logger->warning(RC::NULLPTR_ERROR);
            return nullptr;
        }

        size_t size = sizeof(MultiIndexImpl) + dim * sizeof(size_t);
        uint8_t* ptr = new(std::nothrow)uint8_t[size];
        if (ptr == nullptr)
        {
            MultiIndexImpl::logger->warning(RC::ALLOCATION_ERROR);
            return nullptr;
        }

        IMultiIndex* multiIndex = new(ptr)MultiIndexImpl(dim);
        memcpy((uint8_t*)ptr + sizeof(MultiIndexImpl), indices, dim * sizeof(size_t));

        return multiIndex;
}

RC IMultiIndex::setLogger(ILogger * const pLogger)
{
    if (pLogger == nullptr)
        return RC::NULLPTR_ERROR;
    MultiIndexImpl::logger = pLogger;
    return RC::SUCCESS;
}

ILogger* IMultiIndex::getLogger()
{
    return MultiIndexImpl::logger;
}

MultiIndexImpl::MultiIndexImpl(size_t dim)
{
    this->dim = dim;
}

MultiIndexImpl* MultiIndexImpl::clone() const
{
    return (MultiIndexImpl*)createMultiIndex(dim, PTR_DATA);
}

size_t MultiIndexImpl::getDim() const
{
    return dim;
}

const size_t* MultiIndexImpl::getData() const
{
    return PTR_DATA;
}

RC MultiIndexImpl::setData(size_t dim, size_t const* const& ptr_data)
{
    if (dim != this->dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }

    if (ptr_data == nullptr)
        {
            logger->severe(RC::NULLPTR_ERROR);
            return RC::NULLPTR_ERROR;
        }

    for (size_t i = 0; i < dim; ++i)
        if (std::isnan(ptr_data[i]) || std::isinf(ptr_data[i]))
        {
            logger->severe(RC::NOT_NUMBER);
            return RC::NOT_NUMBER;
        }
    memcpy(PTR_DATA, ptr_data, dim * sizeof(size_t));
    return RC::SUCCESS;
}

RC MultiIndexImpl::getAxisIndex(size_t axisIndex, size_t &val) const
{
    if (this->dim <= axisIndex)
    {
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }
    val = *(PTR_DATA + axisIndex);
    return RC::SUCCESS;
}

RC MultiIndexImpl::setAxisIndex(size_t axisIndex, size_t val)
{
    if (this->dim <= axisIndex)
    {
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }
    if (std::isnan(val) || std::isinf(val))
    {
        logger->severe(RC::NOT_NUMBER);
        return RC::NOT_NUMBER;
    }
    *(PTR_DATA + axisIndex) = val;
    return RC::SUCCESS;
}

RC MultiIndexImpl::incAxisIndex(size_t axisIndex, ssize_t val)
{
    if (axisIndex >= dim)
    {
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }
    size_t temp = *(PTR_DATA + axisIndex) + val;
    if (temp >= 0)
        *(PTR_DATA + axisIndex) = temp;
    else
    {
        logger->severe(RC::INVALID_ARGUMENT);
        return RC::INVALID_ARGUMENT;
    }
    return RC::SUCCESS;
}

IMultiIndex::~IMultiIndex() = default;
