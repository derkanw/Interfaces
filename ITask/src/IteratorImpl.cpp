#include "SetImpl.h"

namespace
{
    SetImpl::IteratorImpl::IteratorImpl()
    {
        valid = true;
    }


}

IteratorImpl* SetImpl::IteratorImpl::createIterator(SetBlockImpl* const& block, size_t index, IVector* vector)
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
