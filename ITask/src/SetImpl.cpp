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
                memcpy(newSet->data, data, setSize * sizeof(double));
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
            {
                logger->warning(RC::ALLOCATION_ERROR);
                return RC::ALLOCATION_ERROR;
            }

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
            size_t dimVal = val->getDim();
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
            {
                dim = dimVal;
            }


        }

            while (capacity < size * dim + val->getDim()) {
                capacity *= 2;
                double *tmp = new double[capacity];
                for (size_t idx = 0; idx < size * dim; ++idx)
                    tmp[idx] = data[idx];
                delete [] data;
                data = tmp;
            }

            for (size_t idx = 0; idx < dim; ++idx)
                data[size * dim + idx] = val->getData()[idx];

            ++size;

            return RC::SUCCESS;
        }

    virtual RC remove(size_t index) = 0;
    virtual RC remove(IVector const * const& pat, IVector::NORM n, double tol) = 0;
    };

    ILogger* SetImpl::logger = nullptr;
}


namespace {
    class SetImpl : public ISet {
    public:
        static RC setLogger(ILogger* const pLogger) {
            if (pLogger == nullptr)
                return RC::NULLPTR_ERROR;

            logger = pLogger;
            return RC::SUCCESS;
        }
        static ILogger* getLogger() {
            return logger;
        }

        static ISet* createSet() { return new SetImpl; }

        RC remove(size_t index) {
            if (size == 0) {
                logger->warning(RC::SOURCE_SET_EMPTY, __FILE__, __func__, __LINE__);
                return RC::SOURCE_SET_EMPTY;
            }
            if (index > size - 1) {
                logger->warning(RC::INDEX_OUT_OF_BOUND, __FILE__, __func__, __LINE__);
                return RC::INDEX_OUT_OF_BOUND;
            }

            double* new_data = new double[capacity];
            for (size_t vec_idx = 0, new_idx = 0; vec_idx < size; ++vec_idx) {
                if (vec_idx != index)
                    for (size_t idx = 0; idx < dim; ++idx, ++new_idx)
                        new_data[new_idx] = data[vec_idx * size + idx];
            }
            delete[] data;
            data = new_data;
            --size;

            return RC::SUCCESS;
        }
        RC remove(IVector const * const& pat, IVector::NORM n, double tol) {
            if (size == 0) {
                logger->warning(RC::SOURCE_SET_EMPTY, __FILE__, __func__, __LINE__);
                return RC::SOURCE_SET_EMPTY;
            }

            return RC::SUCCESS;
         }

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

        ~SetImpl() = default;

};

RC ISet::setLogger(ILogger* const logger) {
    return SetImpl::setLogger(logger);
}
ILogger* ISet::getLogger() {
    return SetImpl::getLogger();
}
ISet* ISet::createSet() {
    return SetImpl::createSet();
}
ISet* ISet::makeIntersection(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol) {
    if (op1->getDim() != op2->getDim()) {
        getLogger()->severe(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    size_t dim = op1->getDim();
    ISet* new_set = ISet::createSet();

    for (size_t vec_idx = 0; vec_idx < op1->getSize(); ++vec_idx) {
        double* empty_data = new double[dim];

        IVector* vec1 = IVector::createVector(dim, empty_data);
        RC err = op1->getCoords(vec_idx, vec1);
        if (err != RC::SUCCESS) {
            delete vec1;
            delete[] empty_data;

            getLogger()->warning(err, __FILE__, __func__, __LINE__);
            return new_set;
        }

        IVector* vec2 = IVector::createVector(op1->getDim(), new double[op1->getDim()]);
        err = op2->findFirstAndCopyCoords(vec1, n, tol, vec2);
        if (err != RC::SUCCESS) {
            delete vec1;
            delete vec2;
            delete[] empty_data;

            getLogger()->warning(err, __FILE__, __func__, __LINE__);
            return new_set;
        }

        new_set->insert(vec1, n, tol);

        delete vec1;
        delete vec2;
        delete[] empty_data;
    }

    return new_set;
}
ISet* ISet::makeUnion(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol) {
    if (op1->getDim() != op2->getDim()) {
        getLogger()->severe(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return nullptr;
    }

    size_t dim = op1->getDim();
    ISet* new_set = op1->clone();

    for (size_t vec_idx = 0; vec_idx < op1->getSize(); ++vec_idx) {
        double* empty_data = new double[dim];

        IVector* vec1 = IVector::createVector(dim, empty_data);
        RC err = op1->getCoords(vec_idx, vec1);
        if (err != RC::SUCCESS) {
            delete vec1;
            delete[] empty_data;

            getLogger()->warning(err, __FILE__, __func__, __LINE__);
            return new_set;
        }

        IVector* vec2 = IVector::createVector(op1->getDim(), new double[op1->getDim()]);
        err = op2->findFirstAndCopyCoords(vec1, n, tol, vec2);
        if (err == RC::VECTOR_NOT_FOUND) {
            new_set->insert(vec1, n, tol);
        }
        else if (err != RC::SUCCESS) {
            delete vec1;
            delete vec2;
            delete[] empty_data;

            getLogger()->warning(err, __FILE__, __func__, __LINE__);
            return new_set;
        }

        delete vec1;
        delete vec2;
        delete[] empty_data;
    }

    return new_set;
}
ISet* ISet::sub(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol) {
    return nullptr;
}
ISet* ISet::symSub(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol) {
    return nullptr;
}
bool ISet::equals(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol) {
    if (op1->getDim() != op2->getDim()) {
        getLogger()->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return false;
    }
    if (op1->getSize() != op2->getSize()) {
        getLogger()->warning(RC::MISMATCHING_DIMENSIONS, __FILE__, __func__, __LINE__);
        return false;
    }

    return true;
}
bool ISet::subSet(ISet const * const& op1, ISet const * const& op2, IVector::NORM n, double tol) {
    return false;
}
RC ISet::IIterator::setLogger(ILogger * const pLogger) {
    return SetImpl::IteratorImpl::setLogger(pLogger);
}
ILogger* ISet::IIterator::getLogger() {
    return SetImpl::IteratorImpl::getLogger();
}
ISet::~ISet() = default;

