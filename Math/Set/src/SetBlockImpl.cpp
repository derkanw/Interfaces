#include "SetBlockImpl.h"
#include "SetImpl.h"

ILogger* SetBlockImpl::logger = nullptr;

RC SetBlockImpl::getNext(IVector *const &vec, size_t &index, size_t indexInc) const
{
    if (setImpl == nullptr)
    {
        logger->warning(RC::SOURCE_SET_DESTROYED);
        return RC::SOURCE_SET_DESTROYED;
    }
    return setImpl->getNextIterator(vec, index, indexInc);
}

RC SetBlockImpl::getPrevious(IVector *const &vec, size_t &index, size_t indexInc) const
{
    if (setImpl == nullptr)
    {
        logger->warning(RC::SOURCE_SET_DESTROYED);
        return RC::SOURCE_SET_DESTROYED;
    }
    return setImpl->getPreviousIterator(vec, index, indexInc);
}

RC SetBlockImpl::getBegin(IVector *const &vec, size_t &index) const
{
    if (setImpl == nullptr)
    {
        logger->warning(RC::SOURCE_SET_DESTROYED);
        return RC::SOURCE_SET_DESTROYED;
    }
    if (setImpl->getSize() == 0)
    {
        logger->severe(RC::SOURCE_SET_EMPTY);
        return RC::SOURCE_SET_EMPTY;
    }
    return setImpl->getBeginIterator(vec, index);
}

RC SetBlockImpl::getEnd(IVector *const &vec, size_t &index) const
{
    if (setImpl == nullptr)
    {
        logger->warning(RC::SOURCE_SET_DESTROYED);
        return RC::SOURCE_SET_DESTROYED;
    }
    if (setImpl->getSize() == 0)
    {
        logger->severe(RC::SOURCE_SET_EMPTY);
        return RC::SOURCE_SET_EMPTY;
    }
    return setImpl->getEndIterator(vec, index);
}

SetBlockImpl* SetBlockImpl::createControlBlock(SetImpl* const& set)
{
    if (set == nullptr)
        return nullptr;
    SetBlockImpl* block = new SetBlockImpl();
    if (block != nullptr)
        block->setImpl = set;
    block->logger = SetImpl::getLogger();
    return block;
}

ISetControlBlock::~ISetControlBlock() = default;
