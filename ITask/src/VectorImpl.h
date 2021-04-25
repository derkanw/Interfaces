#pragma once
#include "../include/IVector.h"

namespace
{
    class VectorImpl : public IVector
    {
    private:
        size_t dim;
        static ILogger* logger;

    protected:
        VectorImpl(size_t dim);

    public:
        static IVector* createVector(size_t dim, double const* const& ptr_data);
        static RC setLogger(ILogger* const logger);


        IVector* clone() const override;
        ILogger* getLogger() const override;
        double const* getData() const override;
        RC setData(size_t dim, double const* const& ptr_data) override;
        RC getCord(size_t index, double& val) const override;
        RC setCord(size_t index, double val) override;
        RC scale(double multiplier) override;
        size_t getDim() const override;

        RC inc(IVector const* const& op) override;
        RC dec(IVector const* const& op) override;
        double norm(NORM n) const override;

        RC applyFunction(const std::function<double(double)>& fun) override;
        RC foreach(const std::function<void(double)>& fun) const override;

        size_t sizeAllocated() const override;

        ~VectorImpl() = default;
    };

    ILogger* VectorImpl::logger = nullptr;
}; //end namespace anonymous