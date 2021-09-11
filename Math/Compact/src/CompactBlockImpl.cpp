#include "CompactBlockImpl.h"
#include "CompactImpl.h"

ILogger* CompactBlockImpl::logger = nullptr;

CompactBlockImpl::CompactBlockImpl(CompactImpl* compact)
{
    this->compact = compact;
}

CompactBlockImpl* CompactBlockImpl::createCompactBlock(CompactImpl* compact)
{
    if (compact == nullptr)
        return nullptr;
    CompactBlockImpl* block = new CompactBlockImpl(compact);
    block->logger = CompactImpl::getLogger();
    return block;
}

RC CompactBlockImpl::get(IMultiIndex *const &currentIndex, IMultiIndex const *const &bypassOrder) const
{
    if (currentIndex == nullptr || bypassOrder == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    size_t dim = currentIndex->getDim();
    if (dim != bypassOrder->getDim())
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        return RC::MISMATCHING_DIMENSIONS;
    }

    IMultiIndex* grid = compact->getGrid();
    if (grid == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return RC::ALLOCATION_ERROR;
    }
    const size_t *gridData = grid->getData(), *orderData = bypassOrder->getData(), *indexData = currentIndex->getData();
    if (gridData == nullptr || orderData == nullptr || indexData == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }

    for (size_t i = 0; i < dim; ++i)
    {
        size_t j = orderData[i];
        if (indexData[j] == gridData[j])
            currentIndex->setAxisIndex(j, 1);
        else
            if (indexData[j] < gridData[j])
            {
                currentIndex->incAxisIndex(j, 1);
                delete grid;
                return RC::SUCCESS;
            }
    }
    logger->severe(RC::INDEX_OUT_OF_BOUND);
    delete grid;
    return RC::INDEX_OUT_OF_BOUND;
}

RC CompactBlockImpl::get(IMultiIndex const *const &currentIndex, IVector *const &val) const
{
    if (currentIndex == nullptr || val == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    IMultiIndex* grid = compact->getGrid();
    if (grid == nullptr)
    {
        logger->warning(RC::ALLOCATION_ERROR);
        return RC::ALLOCATION_ERROR;
    }
    if (currentIndex->getDim() != grid->getDim())
    {
        logger->severe(RC::MISMATCHING_DIMENSIONS);
        delete grid;
        return RC::MISMATCHING_DIMENSIONS;
    }

    const size_t *indexData = currentIndex->getData(), *gridData = grid->getData();
    if (indexData == nullptr || gridData == nullptr)
    {
        logger->warning(RC::NULLPTR_ERROR);
        return RC::NULLPTR_ERROR;
    }
    size_t dim = compact->getDim();
    for (size_t i = 0; i < dim; ++i)
        if (indexData[i] > gridData[i])
        {
            logger->severe(RC::INDEX_OUT_OF_BOUND);
            delete grid;
            return RC::INDEX_OUT_OF_BOUND;
        }
    delete grid;

    IVector* vectorCopy;
    if (compact->getVectorCopy(currentIndex, vectorCopy) != RC::SUCCESS)
        return RC::ALLOCATION_ERROR;
    val->setData(currentIndex->getDim(), vectorCopy->getData());
    delete vectorCopy;
    return RC::SUCCESS;
}

ICompactControlBlock::~ICompactControlBlock() = default;
