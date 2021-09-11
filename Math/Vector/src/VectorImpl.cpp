#include <cmath>
#include <limits>
#include <cstdint>
#include <cstring>
#define PTR_DATA (double*)((uint8_t*)this + sizeof(VectorImpl))
#include "../include/IVector.h"

namespace
{
    class VectorImpl : public IVector
    {
    private:
        size_t dim;

    public:
        static ILogger* logger;

        VectorImpl(size_t dim)
        {
            this->dim = dim;
        }
        VectorImpl* clone() const override
        {
            return (VectorImpl*)createVector(dim, PTR_DATA);
        }

        double const* getData() const override
        {
            return PTR_DATA;
        }

        RC setData(size_t dim, double const* const& ptr_data) override
        {
            if (dim != this->dim)
            {
                logger->severe(RC::MISMATCHING_DIMENSIONS);
                return RC::MISMATCHING_DIMENSIONS;
            }

            for (size_t i = 0; i < dim; ++i)
                if (std::isnan(ptr_data[i]) || std::isinf(ptr_data[i]))
                {
                    logger->severe(RC::NOT_NUMBER);
                    return RC::NOT_NUMBER;
                }
            memcpy(PTR_DATA, ptr_data, dim * sizeof(double));
            return RC::SUCCESS;
        }

        RC getCord(size_t index, double& val) const override
        {
            if (this->dim <= index)
            {
                logger->severe(RC::INDEX_OUT_OF_BOUND);
                return RC::INDEX_OUT_OF_BOUND;
            }
            val = *(PTR_DATA + index);
            return RC::SUCCESS;
        }

        RC setCord(size_t index, double val) override
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

        RC scale(double multiplier) override
        {
            if (std::isnan(multiplier) || std::isinf(multiplier))
            {
                logger->severe(RC::NOT_NUMBER);
                return RC::NOT_NUMBER;
            }
            applyFunction([multiplier](double element)->double {return element * multiplier; });
            return RC::SUCCESS;
        }

        size_t getDim() const override
        {
            return dim;
        }

        RC inc(IVector const* const& op) override
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

        RC dec(IVector const* const& op) override
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

        double norm(NORM n) const override
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
            case NORM::CHEBYSHEV:
                for (size_t i = 0; i < dim; ++i)
                    if (fabs(*(PTR_DATA + i)) > max)
                        max = fabs(*(PTR_DATA + i));
                return max;
            default:
                logger->warning(RC::INVALID_ARGUMENT);
                return std::numeric_limits<double>::quiet_NaN();
            }
        }

        RC applyFunction(const std::function<double(double)>& fun) override
        {
            for (size_t i = 0; i < dim; ++i)
                *(PTR_DATA + i) = fun(*(PTR_DATA + i));
            return RC::SUCCESS;
        }

        RC foreach(const std::function<void(double)>& fun) const override
        {
            for (size_t i = 0; i < dim; ++i)
                fun(*(PTR_DATA + i));
            return RC::SUCCESS;
        }

        size_t sizeAllocated() const override
        {
            return sizeof(VectorImpl) + dim * sizeof(double);
        }

        ~VectorImpl() = default;
    };

    ILogger* VectorImpl::logger = nullptr;
}; //end namespace anonymous

IVector* IVector::createVector(size_t dim, double const* const& ptr_data)
{
    if (dim == 0)
    {
        VectorImpl::logger->severe(RC::INVALID_ARGUMENT);
        return nullptr;
    }
    if (ptr_data == nullptr)
    {
        VectorImpl::logger->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }

    size_t size = sizeof(VectorImpl) + dim * sizeof(double);
    uint8_t* ptr = new(std::nothrow)uint8_t[size];
    if (ptr == nullptr)
    {
        VectorImpl::logger->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }

    VectorImpl* vector = new(ptr)VectorImpl(dim);
    memcpy((uint8_t*)ptr + sizeof(VectorImpl), ptr_data, dim * sizeof(double));

    return vector;
}

RC IVector::setLogger(ILogger* const logger)
{
    if (logger == nullptr)
        return RC::NULLPTR_ERROR;
    VectorImpl::logger = logger;
    return RC::SUCCESS;
}

ILogger* IVector::getLogger()
{
    return VectorImpl::logger;
}

RC IVector::copyInstance(IVector* const dest, IVector const* const& src)
{
    if (dest->getDim() != src->getDim())
    {
        src->getLogger()->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }
    if (std::abs((uint8_t*)dest - (uint8_t*)src) > src->sizeAllocated())
    {
        std::memcpy((uint8_t*)dest, (uint8_t*)src, src->sizeAllocated());
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

    std::memmove((uint8_t*)dest, (uint8_t*)src, src->sizeAllocated());
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
    delete res;
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
    delete res;
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
        scalar += data1[i] * data2[i];
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
    bool res = false;
    if (sub_op != nullptr)
        res = sub_op->norm(n) <= tol;
    else
        op1->getLogger()->warning(RC::NULLPTR_ERROR);
    delete sub_op;

    return res;
}

IVector::~IVector() = default;
