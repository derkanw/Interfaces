#include "CompactImpl.h"

ILogger* CompactImpl::IteratorImpl::logger = nullptr;

CompactImpl::IteratorImpl::IteratorImpl(CompactBlockImpl* block, IVector* vector, IMultiIndex* index, IMultiIndex* order)
{
    valid = true;
    this->block = block;
    currentVector = vector;
    currentIndex = index;
    this->order = order;
}

CompactImpl::IteratorImpl* CompactImpl::IteratorImpl::createIterator(CompactBlockImpl* block, IVector* vector, IMultiIndex* index, IMultiIndex* order)
{
    if (block == nullptr || vector == nullptr || index == nullptr || order == nullptr)
        return nullptr;
    return new IteratorImpl(block, vector->clone(), index->clone(), order->clone());
}

RC ICompact::IIterator::setLogger(ILogger * const pLogger)
{
    if (pLogger == nullptr)
        return RC::NULLPTR_ERROR;
    CompactImpl::IteratorImpl::logger = pLogger;
    return RC::SUCCESS;

}

ILogger* ICompact::IIterator::getLogger()
{
    return CompactImpl::IteratorImpl::logger;
}

ICompact::IIterator* CompactImpl::IteratorImpl::getNext()
{
    IIterator* newIterator = clone();
    if (newIterator == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }
    RC error = newIterator->next();
    if (error != RC::SUCCESS)
        delete newIterator;
    return newIterator;
}

ICompact::IIterator* CompactImpl::IteratorImpl::clone() const
{
    return createIterator(block, currentVector->clone(), currentIndex->clone(), order->clone());
}

RC CompactImpl::IteratorImpl::next()
{
    RC error = block->get(currentIndex, order);
    if (error != RC::SUCCESS)
        if (error == RC::INDEX_OUT_OF_BOUND)
            valid = false;
        return error;
    return block->get(currentIndex, currentVector);
}

bool CompactImpl::IteratorImpl::isValid() const
{
    return valid;
}

RC CompactImpl::IteratorImpl::getVectorCopy(IVector *& val) const
{
    val = currentVector->clone();
    if (val == nullptr)
        return RC::ALLOCATION_ERROR;
    return RC::SUCCESS;
}

RC CompactImpl::IteratorImpl::getVectorCoords(IVector * const& val) const
{
    if (val == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    if (val->getDim() != currentVector->getDim())
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }
    return val->setData(currentVector->getDim(), currentVector->getData());
}

CompactImpl::IteratorImpl::~IteratorImpl()
{
    delete currentIndex;
    delete currentVector;
    delete order;
}

ICompact::IIterator::~IIterator()  = default;

