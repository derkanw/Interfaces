#pragma once
#include "../include/ISetControlBlock.h"

class SetImpl;

class SetBlockImpl : public ISetControlBlock
{
public:
    static ILogger* logger;

    static SetBlockImpl* createControlBlock(SetImpl* const& set);

    RC getNext(IVector *const &vec, size_t &index, size_t indexInc = 1) const override;
    RC getPrevious(IVector *const &vec, size_t &index, size_t indexInc = 1) const override;
    RC getBegin(IVector *const &vec, size_t &index) const override;
    RC getEnd(IVector *const &vec, size_t &index) const override;
    ~SetBlockImpl() = default;

protected:
    SetBlockImpl() = default;

private:
    SetImpl* setImpl;
};
