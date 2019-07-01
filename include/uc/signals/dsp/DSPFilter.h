#pragma once

namespace uc
{
    namespace dsp
    {
        namespace filter
        {
            class LIB_UC_API FilterBuffer
            {
            public:
                FilterBuffer(const double* pBuffer, size_t count);

                explicit FilterBuffer(const size_t count);

                FilterBuffer(const FilterBuffer& buffer);
                ~FilterBuffer();

                FilterBuffer& operator=(const FilterBuffer& buffer);

                double* get();

                const double* get() const;

                size_t count() const;
            private:
                size_t _count;
                double * _pValues;
                void release();
            };

            class LIB_UC_API Filter
            {
            public:
                virtual ~Filter(){}
                virtual FilterBuffer & filter(const FilterBuffer & input)=0; 
            };
        }
    }
}