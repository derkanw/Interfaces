#pragma once
#include "../include/ICompactControlBlock.h"

class CompactImpl;

class CompactBlockImpl : public ICompactControlBlock
{
  public:
    static ILogger* logger;

    static CompactBlockImpl* createCompactBlock(CompactImpl* compact);

    RC get(IMultiIndex *const &currentIndex, IMultiIndex const *const &bypassOrder) const override;
    RC get(IMultiIndex const *const &currentIndex, IVector *const &val) const override;

  private:
    CompactImpl* compact;

  protected:
    CompactBlockImpl(CompactImpl* compact);
};
