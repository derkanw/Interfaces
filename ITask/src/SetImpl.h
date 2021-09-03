#pragma once
#include <vector>
#include "../include/ISet.h"

class SetImpl : public ISet
{
private:
    std::vector<size_t> uniqueIndexes;
    double* data;
    size_t capacity, setSize, dim;
public:
    static ILogger* logger;

    SetImpl();
    SetImpl* clone() const override;
    size_t getDim() const override;
    size_t getSize() const override;
    RC getCopy(size_t index, IVector *& val) const override;
    RC findFirstAndCopy(IVector const * const& pat, IVector::NORM n, double tol, IVector *& val) const override;
    RC getCoords(size_t index, IVector * const& val) const override;
    RC findFirstAndCopyCoords(IVector const * const& pat, IVector::NORM n, double tol, IVector * const& val) const override;
    RC findFirst(IVector const * const& pat, IVector::NORM n, double tol) const override;
    RC insert(IVector const * const& val, IVector::NORM n, double tol) override;
    RC remove(size_t index) override;
    RC remove(IVector const * const& pat, IVector::NORM n, double tol) override;
    ~SetImpl();

    class IteratorImpl : public IIterator
    {
    public:
        static ILogger* logger;

        IIterator * getNext(size_t indexInc = 1) const override;
        IIterator * getPrevious(size_t indexInc = 1) const override;
        IIterator * clone() const override;
        RC next(size_t indexInc = 1) override;
        RC previous(size_t indexInc = 1) override;
        bool isValid() const override;
        RC makeBegin() override;
        RC makeEnd() override;
        RC getVectorCopy(IVector *& val) const override;
        RC getVectorCoords(IVector * const& val) const override;
        ~IteratorImpl()  = default;

    protected:
        IteratorImpl() = default;

    private:
        ISetControlBlock* block;
        IVector* currentVector;

    };

    IIterator *getIterator(size_t index) const override;
    IIterator *getBegin() const override;
    IIterator *getEnd() const override;
};
