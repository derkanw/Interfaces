#include "../include/ISet.h"
#define CAPACITY 100

namespace
{
    class SetImpl : public ISet
    {
    private:
        double* data;
        size_t capacity, setSize, dim;
    public:
        static ILogger* logger;

        SetImpl()
        {
            capacity = CAPACITY;
            data = new double[capacity];
            setSize = 0;
            dim = 0;
        }

        SetImpl* clone() const override
        {
            SetImpl* newSet = new SetImpl();
            if (newSet != nullptr)
            {
                newSet->dim = this->dim;
                newSet->setSize = this->setSize;
                newSet->capacity = this->capacity;
                newSet->data = new double[capacity];
//если не работает, то убрать dim
                memcpy(newSet->data, data, setSize * dim * sizeof(double));
            }
            return newSet;
        }

        size_t getDim() const override
        {
            return dim;
        }

        size_t getSize() const override
        {
            return setSize;
        }

        RC getCopy(size_t index, IVector *& val) const override
        {
            val = IVector::createVector(dim, new double[dim]);
            if (getCoords(index, val) != RC::SUCCESS)
            {
                delete val;
                return RC::VECTOR_NOT_FOUND;
            }
            return RC::SUCCESS;
        }

        RC findFirstAndCopy(IVector const * const& pat, IVector::NORM n, double tol, IVector *& val) const override
        {
            if (pat == nulptr)
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

            val = IVector::createVector(dim, new double[dim]);
            if (val == nullptr)
                return RC::ALLOCATION_ERROR;

            int i = 0;
            while (true)
            {
                val->setData(dim, data + i * dim];
                if (IVector::equals(pat, val, n, tol))
                    return RC::SUCCESS;
                if (i >= setSize)
                {
                    delete val;
                    return RC::VECTOR_NOT_FOUND;
                }
                ++i;
            }
        }

        RC getCoords(size_t index, IVector * const& val) const override
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
            delete temp;
            return error;
        }
        RC findFirstAndCopyCoords(IVector const * const& pat, IVector::NORM n, double tol, IVector * const& val) const override
        {
            IVector* temp;
            RC error = findFirstAndCopy(pat, n, tol, temp);
            if (error != RC::SUCCESS)
                return error;
            error = val->setData(temp->getData());
            delete temp;
            return error;
        }

        RC findFirst(IVector const * const& pat, IVector::NORM n, double tol) const override
        {
            IVector* temp;
            RC error = findFirstAndCopy(pat, n, tol, temp);
            delete temp;
            return error;
        }

        RC insert(IVector const * const& val, IVector::NORM n, double tol) override
        {
            if (val == nullptr)
            {
                logger->warning(RC::NULLPTR_ERROR);
                return RC::NULLPTR_ERROR;
            }

            size_t valDim = val->getDim();
            if (setSize != 0)
            {
                if (dim != dimVal)
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

            double* temp = val->getData();
            for (size_t i = 0; i < dim; ++i)
                data[setSize * dim + i] = temp[i];
            ++setSize;
            deletep[] temp;
            return RC::SUCCESS;
        }

        RC remove(size_t index) override
        {
            if (index >= setSize)
            {
                logger->severe(RC::INDEX_OUT_OF_BOUND);
                return RC::INDEX_OUT_OF_BOUND;
            }

            double* newData = new double[capacity];
            for (size_t iSet = 0, iVector = 0; iSet < setSize; ++iSet)
                if (iSet != index)
                    for (size_t i = 0; i < dim; ++i, ++iVector)
                        newData[iVector] = data[iSet * setSize + i];
            --setSize;
            delete data[];
            data = newData;
            return RC::SUCCESS;
        }

        RC remove(IVector const * const& pat, IVector::NORM n, double tol) override
        {
            if (pat == nullptr)
            {
                logger->warning(RC::NULLPTR_ERROR);
                return RC::NULLPTR_ERROR;
            }

            IVector* val;
            RC error = findFirstAndCopy(pat, n, tol, val);
            if (error != RC::SUCCESS)
                return error;

            double* newData = new double[capacity];
            for (size_t iSet = 0, iVector = 0; iSet < setSize; ++iSet)
            {
                IVector* temp;
                if (getCopy(iSet, temp) == RC::SUCCESS && !IVector::equals(temp, val, n, tol))
                    for (size_t i = 0; i < dim; ++i, ++iVector)
                        newData[iVector] = data[iSet * setSize + i];
            }

            --setSize;
            delete data[];
            delete val;
            delete temp;
            data = newData;
            return RC::SUCCESS;
        }

        ~VectorImpl()
        {
            delete[] data;
        }
    };

    ILogger* SetImpl::logger = nullptr;
}

RC ISet::setLogger(ILogger* const pLogger)
{
    if (logger == nullptr)
        return RC::NULLPTR_ERROR;
    VectorImpl::logger = logger;
    return RC::SUCCESS;
}

ILogger* ISet::getLogger()
{
    return logger;
}

ISet* ISet::createSet()
{
    SetImpl* setPtr = new(std::nothrow)SetImpl;
    if (setPtr == nullptr)
    {
        VectorImpl::logger->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }
    return setPtr;
}

ISet* ISet::makeIntersection(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    IVector *data, *value;
    ISet* newSet = ISet::createSet();
    if (newSet == nullptr)
        return nullptr;

    size_t setSize1 = op1->getSize();
    for (size_t i = 0; i < setSize1; ++i)
    {
        if (op1.getCopy(i, data) != RC::SUCCESS)
            continue;
        if (op2.findFirstAndCopy(data, n, tol, value) != RC::SUCCESS)
            continue;
        if (newSet.insert(value, n, tol) != RC::SUCCESS)
            continue;
        delete data;
        delete value;
    }
    return newSet;
}

ISet* ISet::makeUnion(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }
    ISet* newSet = op1->clone();
    if (newSet == nullptr)
        return nullptr;

    size_t size2 = op2->getSize();
    IVector* value;
    for (size_t i = 0; i < size2; ++i)
    {
        if (op1.getCopy(i, value) != RC::SUCCESS)
            continue;
        if (newSet.insert(value, n, tol) != RC::SUCCESS)
            continue;
        delete value;
    }
    return newSet;
}

ISet* ISet::sub(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return nullptr;
    }

    ISet* newSet = op1->clone();
    ISet* interSet = ISet::makeIntersection(op1, op2, n, tol);
    if (newSet == nullptr || interSet == nullptr)
        return nullptr;

    IVector* value;
    size_t interSize = interSet->getSize();
    for (size_t i = 0; i < interSize; ++i)
    {
        if (interSet.getCopy(i, value) != RC::SUCCESS)
            continue;
        if (newSet.remove(value, n, tol) != RC::SUCCESS)
            continue;
        delete value;
    }

    delete unionSet;
    return newSet;
}

ISet* ISet::symSub(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
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
        logger->warning(RC::NULLPTR_ERROR);
        return false;
    }
    ISet* temp = ISet::sub(op1, op2, n, tol);
    if (temp == nullptr || temp->getSize() != 0)
    {
        delete temp
        return false;
    }
    delete temp;
    return true;
}

bool ISet::subSet(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol)
{
    if (op1 == nullptr || op2 == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return false;
    }
    ISet* temp = ISet::makeIntersection(op1, op2, n, tol);
    if (temp == nullptr || !ISet::equals(temp, op2, n, tol))
    {
        delete temp;
        return false;
    }
    delete temp;
    return true;
}

ISet::~ISet() = default;



namespace {
    class SetImpl : public ISet {
    public:
        /*
        * Iterator object can be created with ISet methods ISet::getIterator, ISet::getBegin, ISet::getEnd
        */
        class IteratorImpl : public IIterator {
        public:
            /*
            * Create iterator associated with next/previous position
            *
            * @param [in] indexInc Quantity of steps forward
            */
            IIterator * getNext(size_t indexInc = 1) const { return nullptr; }
            IIterator * getPrevious(size_t indexInc = 1) const { return nullptr; }
            IIterator * clone() const { return nullptr; }

            static RC setLogger(ILogger * const pLogger) { return RC::SUCCESS; }

            /*
            * Moves iterator forward/backward
            */
            RC next(size_t indexInc = 1)  { return RC::SUCCESS; }
            RC previous(size_t indexInc = 1)  { return RC::SUCCESS; }

            bool isValid() const { return false; }

            RC makeBegin() { return RC::SUCCESS; }
            RC makeEnd() { return RC::SUCCESS; }

            /*
            * Getter of value (same semantic as ISet::getCopy)
            */
            RC getVectorCopy(IVector *& val) const { return RC::SUCCESS; }
            /*
            * Getter of value (same semantic as ISet::getCoords)
            */
            RC getVectorCoords(IVector * const& val) const { return RC::SUCCESS; }

            ~IteratorImpl()  = default;

        protected:
            /*
            * As long as iterator refers to vector in ISet, which corresponds to unique index, we can compare iterators by this index
            */
            size_t getIndex() const { return 0; }
            IteratorImpl() = default;
        };

        IIterator *getIterator(size_t index) const { return nullptr; }
        IIterator *getBegin() const { return nullptr; }
        IIterator *getEnd() const { return nullptr; }
};
