#pragma once
#include "../include/ICompact.h"
#include "CompactBlockImpl.h"

class CompactImpl : public ICompact
{
public:
    static ILogger* logger;

    ICompact* clone() const override;
    bool isInside(IVector const * const&vec) const override;
    RC getVectorCopy(IMultiIndex const *index, IVector *& val) const override;
    RC getVectorCoords(IMultiIndex const *index, IVector * const& val) const override;
    RC getLeftBoundary(IVector *& vec) const override;
    RC getRightBoundary(IVector *& vec) const override;
    size_t getDim() const override;
    IMultiIndex* getGrid() const override;

    class IteratorImpl : public ICompact::IIterator
    {
    public:
        static ILogger* logger;

        IIterator* getNext() override;
        IIterator* clone() const override;
        RC next() override;
        bool isValid() const override;
        RC getVectorCopy(IVector *& val) const override;
        RC getVectorCoords(IVector * const& val) const override;

        static IteratorImpl* createIterator(CompactBlockImpl* block, IVector* vector, IMultiIndex* index, IMultiIndex* order);
        ~IteratorImpl();

    private:
        bool valid;
        CompactBlockImpl* block;
        IVector* currentVector;
        IMultiIndex *currentIndex, *order;

    protected:
        IteratorImpl(CompactBlockImpl* block, IVector* vector, IMultiIndex* index, IMultiIndex* order);
    };

    IIterator* getIterator(IMultiIndex const * const&index, IMultiIndex const * const &bypassOrder) const override;
    IIterator* getBegin(IMultiIndex const * const &bypassOrder) const override;
    IIterator* getEnd(IMultiIndex const * const &bypassOrder) const override;

    CompactImpl(const IVector* left, const IVector* right, const IMultiIndex* nodes);
    ~CompactImpl();

private:
    size_t dim;
    const IVector* left;
    const IVector* right;
    const IMultiIndex* grid;
    CompactBlockImpl* block;
};

