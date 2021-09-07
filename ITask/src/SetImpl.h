#pragma once
#include <vector>
#include "../include/ISet.h"
#include "SetBlockImpl.h"

class SetImpl : public ISet
{
private:
    std::vector<size_t> uniqueIndexes;
    size_t lastIndex;
    double* data;
    size_t capacity, setSize, dim;
    SetBlockImpl* setBlock;

    size_t indexToPos(size_t index) const;

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

    class IteratorImpl : public ISet::IIterator
    {
    public:
        static ILogger* logger;

        static IteratorImpl* createIterator(SetBlockImpl* const& block, size_t index, IVector* vector);
        IIterator* getNext(size_t indexInc = 1) const override;
        IIterator* getPrevious(size_t indexInc = 1) const override;
        IIterator* clone() const override;
        RC next(size_t indexInc = 1) override;
        RC previous(size_t indexInc = 1) override;
        bool isValid() const override;
        RC makeBegin() override;
        RC makeEnd() override;
        RC getVectorCopy(IVector *& val) const override;
        RC getVectorCoords(IVector * const& val) const override;
        ~IteratorImpl();

    protected:
        IteratorImpl();

    private:
        SetBlockImpl* block;
        IVector* currentVector;
        size_t currentIndex;
        bool valid;
    };

    RC getNextIterator(IVector *const &vec, size_t &index, size_t indexInc = 1) const;
    RC getPreviousIterator(IVector *const &vec, size_t &index, size_t indexInc = 1) const;
    RC getBeginIterator(IVector *const &vec, size_t &index) const;
    RC getEndIterator(IVector *const &vec, size_t &index) const;

    IIterator* getIterator(size_t index) const override;
    IIterator* getBegin() const override;
    IIterator* getEnd() const override;
};
