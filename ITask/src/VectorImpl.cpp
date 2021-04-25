#include <cmath>
#include "VectorImpl.h"
#define PTR_DATA (double*)((uint8_t*)this + sizeof(VectorImpl))

IVector* IVector::createVector(size_t dim, double const* const& ptr_data)
{
    return VectorImpl::createVector(dim, ptr_data);
}

RC IVector::setLogger(ILogger* const logger)
{
    return VectorImpl::setLogger(logger);
}

RC VectorImpl::setLogger(ILogger* const logger)
{
    if (logger == nullptr)
        return RC::NULLPTR_ERROR;
    VectorImpl::logger = logger;
    return RC::SUCCESS;
}

IVector* VectorImpl::createVector(size_t dim, double const* const& ptr_data)
{
    if (dim <= 0)
    {
        logger->severe(RC::INVALID_ARGUMENT);
        return nullptr;
    }
    if (ptr_data == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    
    size_t size = sizeof(VectorImpl) + dim * sizeof(double);
    void* ptr = new(std::nothrow)uint8_t[size];
    if (ptr == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }

    VectorImpl* vector = new(ptr)VectorImpl(dim);
    memcpy((uint8_t*)ptr + sizeof(VectorImpl), ptr_data, dim * sizeof(double));

    return vector;
}

RC IVector::copyInstance(IVector* const dest, IVector const* const& src)
{
    if (dest->getDim() != src->getDim())
    {
        src->getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }
    if (fabs((uint8_t*)dest - (uint8_t*)src) > src->sizeAllocated())
    {
        std::memcpy(dest, src, src->sizeAllocated());
        return RC::SUCCESS;
    }
    src->getLogger()->warning(RC::MEMORY_INTERSECTION);
    return RC::MEMORY_INTERSECTION;
}

RC IVector::moveInstance(IVector* const dest, IVector*& src)
{
    if (dest->getDim() != src->getDim())
    {
        src->getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }

    std::memmove(dest, src, src->sizeAllocated());
    delete src;
    src = nullptr;
    return RC::SUCCESS;
}

IVector* IVector::add(IVector const* const& op1, IVector const* const& op2)
{
    if (op1 == nullptr || op2 == nullptr)
        return nullptr;
    if (op1->getDim() != op2->getDim())
    {
        op1->getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return nullptr;
    }

    IVector* res = op1->clone();
    if (res != nullptr && RC::SUCCESS == res->inc(op2))
        return res;
    op1->getLogger()->warning(RC::ALLOCATION_ERROR);
    return nullptr;
}

IVector* IVector::sub(IVector const* const& op1, IVector const* const& op2)
{
    if (op1 == nullptr || op2 == nullptr)
        return nullptr;
    if (op1->getDim() != op2->getDim())
    {
        op1->getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return nullptr;
    }

    IVector* res = op1->clone();
    if (res != nullptr && RC::SUCCESS == res->dec(op2))
        return res;
    op1->getLogger()->warning(RC::ALLOCATION_ERROR);
    return nullptr;
}

double IVector::dot(IVector const* const& op1, IVector const* const& op2)
{
    if (op1 == nullptr || op2 == nullptr)
        return std::numeric_limits<double>::quiet_NaN();
    size_t dim_op = op1->getDim();
    if (dim_op != op2->getDim())
    {
        op1->getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return std::numeric_limits<double>::quiet_NaN();
    }

    double scalar = 0;
    double const *data1 = op1->getData(), *data2 = op2->getData();
    for (size_t i = 0; i < dim_op; ++i)
        scalar += data1[i] + data2[i];
    return scalar;
}

bool IVector::equals(IVector const* const& op1, IVector const* const& op2, NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
        return false;
    size_t dim_op = op1->getDim();
    if (dim_op != op2->getDim())
    {
        op1->getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return false;
    }
    if (tol < 0)
    {
        op1->getLogger()->warning(RC::INVALID_ARGUMENT);
        return false;
    }

    IVector* sub_op = sub(op1, op2);
    bool res = sub_op != nullptr && sub_op->norm(n) <= tol;
    delete sub_op;
    if (!res)
        op1->getLogger()->warning(RC::ALLOCATION_ERROR);
    return res;
}

IVector::~IVector() = default;

VectorImpl::VectorImpl(size_t dim)
{
    this->dim = dim;
}

IVector* VectorImpl::clone() const
{
    return createVector(dim, PTR_DATA);
}

ILogger* VectorImpl::getLogger() const
{
    return logger;
}

double const* VectorImpl::getData() const
{
    return PTR_DATA;
}

RC VectorImpl::setData(size_t dim, double const* const& ptr_data)
{
    if (dim != this->dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }

    for (size_t i = 0; i < dim; ++i)
    {
        if (std::isnan(ptr_data[i]) || std::isinf(ptr_data[i]))
        {
            logger->severe(RC::NOT_NUMBER);
            return RC::NOT_NUMBER;
        }
        *(PTR_DATA + i) = ptr_data[i];
    }
    return RC::SUCCESS;
}

RC VectorImpl::getCord(size_t index, double& val) const
{
    if (this->dim <= index)
    {
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }
    val = *(PTR_DATA + index);
    return RC::SUCCESS;
}

RC VectorImpl::setCord(size_t index, double val)
{
    if (this->dim <= index)
    {
        logger->severe(RC::INDEX_OUT_OF_BOUND);
        return RC::INDEX_OUT_OF_BOUND;
    }
    if (std::isnan(val) || std::isinf(val))
    {
        logger->severe(RC::NOT_NUMBER);
        return RC::NOT_NUMBER;
    }
    *(PTR_DATA + index) = val;
    return RC::SUCCESS;
}

RC VectorImpl::scale(double multiplier)
{
    if (std::isnan(multiplier) || std::isinf(multiplier))
    {
        logger->severe(RC::NOT_NUMBER);
        return RC::NOT_NUMBER;
    }
    applyFunction([multiplier](double element)->double {return element * multiplier; });
    return RC::SUCCESS;
}

size_t VectorImpl::getDim() const
{
    return dim;
}

RC VectorImpl::inc(IVector const* const& op)
{
    if (op == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    if (op->getDim() != dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }

    double const *data_op = op->getData();
    for (size_t i = 0; i < dim; ++i)
        *(PTR_DATA + i) += data_op[i];
    return RC::SUCCESS;
}

RC VectorImpl::dec(IVector const* const& op)
{
    if (op == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    if (op->getDim() != dim)
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }
    double const* data_op = op->getData();
    for (size_t i = 0; i < dim; ++i)
        *(PTR_DATA + i) -= data_op[i];
    return RC::SUCCESS;
}

double VectorImpl::norm(NORM n) const
{
    double max = 0, sum = 0;
    switch (n)
    {
    case NORM::FIRST:
        for (size_t i = 0; i < dim; ++i)
            sum += fabs(*(PTR_DATA + i));
        return sum;
    case NORM::SECOND:
        for (size_t i = 0; i < dim; ++i)
            sum += *(PTR_DATA + i) * *(PTR_DATA + i);
        return sqrt(sum);
    case NORM::AMOUNT:
        for (size_t i = 0; i < dim; ++i)
            if (fabs(*(PTR_DATA + i)) > max)
                max = fabs(*(PTR_DATA + i));
        return max;
    default:
        logger->warning(RC::INVALID_ARGUMENT);
        return std::numeric_limits<double>::quiet_NaN();
    }
}

RC VectorImpl::applyFunction(const std::function<double(double)>& fun)
{
    for (size_t i = 0; i < dim; ++i)
        *(PTR_DATA + i) = fun(*(PTR_DATA + i));
    return RC::SUCCESS;
}

RC VectorImpl::foreach(const std::function<void(double)>& fun) const
{
    for (size_t i = 0; i < dim; ++i)
        fun(*(PTR_DATA + i));
    return RC::SUCCESS;
}

size_t VectorImpl::sizeAllocated() const
{
    return sizeof(VectorImpl) + dim * sizeof(double);
}