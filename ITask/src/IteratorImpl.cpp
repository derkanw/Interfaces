#include "SetImpl.h"

SetImpl::IteratorImpl::IteratorImpl()
{
    valid = true;
}

ISet::IIterator* SetImpl::IteratorImpl::getNext(size_t indexInc) const
{
    IIterator* newIterator = clone();
    if (newIterator == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }
    RC error = newIterator->next(indexInc);
    if (error != RC::SUCCESS)
        delete newIterator;
    return newIterator;
}

ISet::IIterator* SetImpl::IteratorImpl::getPrevious(size_t indexInc) const
{
    IIterator* newIterator = clone();
    if (newIterator == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return nullptr;
    }
    RC error = newIterator->previous(indexInc);
    if (error != RC::SUCCESS)
        delete newIterator;
    return newIterator;
}

ISet::IIterator* SetImpl::IteratorImpl::clone() const
{
    return createIterator(block, currentIndex, currentVector->clone());
}

RC SetImpl::IteratorImpl::next(size_t indexInc)
{
    RC error = block->getNext(currentVector, currentIndex, indexInc);
    if (error == RC::INDEX_OUT_OF_BOUND)
        valid = false;
    return error;
}

RC SetImpl::IteratorImpl::previous(size_t indexInc)
{
    RC error = block->getPrevious(currentVector, currentIndex, indexInc);
    if (error == RC::INDEX_OUT_OF_BOUND)
        valid = false;
    return error;
}

bool SetImpl::IteratorImpl::isValid() const
{
    return valid;
}

RC SetImpl::IteratorImpl::makeBegin()
{
    return block->getBegin(currentVector, currentIndex);
}

RC SetImpl::IteratorImpl::makeEnd()
{
    return block->getEnd(currentVector, currentIndex);
}

RC SetImpl::IteratorImpl::getVectorCopy(IVector *& val) const
{
    IVector* newVector = currentVector->clone();
    if (newVector == nullptr)
    {
        return RC::ALLOCATION_ERROR;
    }
    val = newVector;
    return RC::SUCCESS;
}

RC SetImpl::IteratorImpl::getVectorCoords(IVector * const& val) const
{
    return val->setData(currentVector->getDim(), currentVector->getData());
}

SetImpl::IteratorImpl::~IteratorImpl()
{
    delete currentVector;
}

SetImpl::IteratorImpl* SetImpl::IteratorImpl::createIterator(SetBlockImpl* const& block, size_t index, IVector* vector)
{
    if (block == nullptr || vector == nullptr)
        return nullptr;
    IteratorImpl* iterator = new IteratorImpl();
    if (iterator != nullptr)
    {
        iterator->block = block;
        iterator->currentVector = vector;
        iterator->currentIndex = index;
    }
    return iterator;
}

RC ISet::IIterator::setLogger(ILogger * const pLogger)
{
    if (pLogger == nullptr)
        return RC::NULLPTR_ERROR;
    SetImpl::IteratorImpl::logger = pLogger;
    return RC::SUCCESS;
}

ILogger* ISet::IIterator::getLogger()
{
    return SetImpl::IteratorImpl::logger;
}

ISet::IIterator::~IIterator()  = default;
