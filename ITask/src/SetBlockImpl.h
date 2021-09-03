#pragma once
#include "../include/ISetControlBlock.h"

class SetBlockImpl
{
public:
    RC getNext(IVector *const &vec, size_t &index, size_t indexInc = 1) const override;
    RC getPrevious(IVector *const &vec, size_t &index, size_t indexInc = 1) const override;
    RC getBegin(IVector *const &vec, size_t &index) const override;
    RC getEnd(IVector *const &vec, size_t &index) const override;
    ~ISetControlBlock() = default;

protected:
    ISetControlBlock() = default;
};
