#pragma once
#include "../include/IMultiIndex.h"

class MultiIndexImpl : public IMultiIndex
{
public:
    static ILogger* logger;
    MultiIndexImpl(size_t dim);

    MultiIndexImpl* clone() const override;
    size_t getDim() const override;
    const size_t *getData() const override;
    RC setData(size_t dim, size_t const* const& ptr_data) override;
    RC getAxisIndex(size_t axisIndex, size_t &val) const override;
    RC setAxisIndex(size_t axisIndex, size_t val) override;
    RC incAxisIndex(size_t axisIndex, ssize_t val) override;
    ~MultiIndexImpl() = default;

private:
    size_t dim;
};

