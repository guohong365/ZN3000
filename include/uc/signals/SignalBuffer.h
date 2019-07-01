#pragma once
#include <uc/libcore.h>
namespace uc
{
    namespace signals
    {
        template <typename Type1, typename Type2>
        Type1 *copyBufferNormalized(Type1 *pTarget,const Type2 * pSource, size_t count, double gain, Type2 baseValue)
        {
            for(size_t i=0; i< count; i++ )
            {
                pTarget[i]=static_cast<Type1>((pSource[i]-baseValue)*gain);
            }
            return pTarget;
        }
        template <typename Type1, typename Type2>
        Type1 *copyBuffer(Type1 *pTarget,const Type2 * pSource, size_t count)
        {
            for(size_t i=0; i< count; i++ )
            {
                pTarget[i]=static_cast<Type1>(pSource[i]);
            }
            return pTarget;
        }
        template <typename ValueType>
        ValueType minValue(ValueType * pBuffer, size_t count)
        {
            ValueType m = pBuffer[0];
            for(size_t i=1; i< count; i++)
            {
                if(m > pBuffer[i])
                {
                    m=pBuffer[i];
                }
            }
            return m;
        }
        template <typename ValueType>
        ValueType maxValue(ValueType * pBuffer, size_t count)
        {
            ValueType m = pBuffer[0];
            for(size_t i=1; i< count; i++)
            {
                if(m < pBuffer[i])
                {
                    m=pBuffer[i];
                }
            }
            return m;
        }

        template <typename ValueType>
        class SignalBuffer 
        {
        public:
            template<typename ValueType2>
            explicit SignalBuffer(const SignalBuffer<ValueType2>& buffer);
            explicit SignalBuffer(size_t size);

            size_t length() const;
            void length(size_t length);
            size_t size() const;
            ValueType* buffer();
            const ValueType* buffer() const;
            ValueType gain() const;
            ValueType operator[](int index) const;
            ValueType& operator[](int index);
            template<typename T> void append(T val);
            template<typename T>
            void append(const T* pData, size_t count);
            ValueType maximum() const;
            ValueType minimum() const;
            ValueType average() const;
            void normalize();
            bool normalized() const;
            void scale(ValueType scale);
            void clear();
            virtual ~SignalBuffer();

            template <typename ValueType2>
            SignalBuffer<ValueType>& operator =(const SignalBuffer<ValueType2>& buffer);

            void attach(ValueType* pBuffer, size_t size, size_t length = 0, ValueType gain = 1, bool normalized = false);

            ValueType* detach(size_t* pSize, size_t* pLength, ValueType* pGain, bool* pNormalized);
        private:
            SignalBuffer();
            template <typename ValueType1>
            static void _copy(const SignalBuffer<ValueType1>& source, SignalBuffer<ValueType>& dest);	
            size_t _size;
            size_t _length;
            ValueType * _pBuffer;
            bool _normalized;
            ValueType _gain;
            ValueType _max;
            ValueType _min;
            ValueType _average;
        };

        template <typename ValueType>
        size_t SignalBuffer<ValueType>::length() const
        {
            return _length;
        }

        template <typename ValueType>
        ValueType SignalBuffer<ValueType>::average() const
        {
            if (_normalized) return _average;
            if (_length == 0) return 0;

            const size_t begin = _length >= _size ? _length % _size : 0;
            ValueType sum = 0;
            for (size_t i = begin; i < _length; i++)
            {
                sum += _pBuffer[i % _size];
            }
            return sum / _length;
        }

        template <typename ValueType>
        template <typename T>
        void SignalBuffer<ValueType>::append(const T* pData, size_t count)
        {
            for (size_t i = 0; i < count; ++i)
            {
                append(pData[i]);
            }
        }

        template <typename ValueType>
        template <typename ValueType2>
        SignalBuffer<ValueType>::SignalBuffer(const SignalBuffer<ValueType2>& buffer)
            :_pBuffer(nullptr)
        {
            _copy(buffer, *this);
        }

        template <typename ValueType>
        SignalBuffer<ValueType>::SignalBuffer(const size_t size)
            : _size(size), _length(0), _normalized(false), _gain(1)
        {
            _pBuffer = new ValueType[_size];
        }

        template <typename ValueType>
        void SignalBuffer<ValueType>::length(size_t length)
        {
            _length = length;
        }

        template <typename ValueType>
        size_t SignalBuffer<ValueType>::size() const
        {
            return _size;
        }

        template <typename ValueType>
        ValueType* SignalBuffer<ValueType>::buffer()
        {
            return _pBuffer;
        }

        template <typename ValueType>
        const ValueType* SignalBuffer<ValueType>::buffer() const
        {
            return _pBuffer;
        }

        template <typename ValueType>
        ValueType SignalBuffer<ValueType>::gain() const
        {
            return _gain;
        }


        template <typename ValueType>
        ValueType SignalBuffer<ValueType>::operator[](const int index) const
        {
            return _pBuffer[(_length + index - 1) % _size];
        }

        template <typename ValueType>
        ValueType& SignalBuffer<ValueType>::operator[](const int index)
        {
            return _pBuffer[(_length + index - 1) % _size];
        }

        template <typename ValueType>
        template <typename T>
        void SignalBuffer<ValueType>::append(T val)
        {
            _pBuffer[_length % _size] = static_cast<ValueType>(val);
            _length ++;
        }

        template <typename ValueType>
        ValueType SignalBuffer<ValueType>::maximum() const
        {            
            if(_length == 0) return 0;
            if(_normalized) return _max;
            size_t begin = _length >= _size ? _length % _size : 0;            
            ValueType maxValue = _pBuffer[begin];
            for (size_t i = begin; i < _length; i++)
            {
                if (maxValue < _pBuffer[i % _size])
                {
                    maxValue = _pBuffer[i % _size];
                }
            }
            return maxValue;
        }

        template <typename ValueType>
        ValueType SignalBuffer<ValueType>::minimum() const
        {
            if(_length == 0) return  0;
            if(_normalized) return _min;

            size_t begin = _length >= _size ? _length % _size : 0;
            ValueType minValue = _pBuffer[begin];
            for (size_t i = begin; i < _length; i++)
            {
                if (minValue > _pBuffer[i % _size])
                {
                    minValue = _pBuffer[i % _size];
                }
            }
            return minValue;
        }

        template <typename ValueType>
        void SignalBuffer<ValueType>::normalize()
        {
            if(_length == 0) return;
            const size_t begin = _length >= _size ? _length % _size : 0;
            _min = _pBuffer[0];
            _max = _pBuffer[1];
            ValueType sum = 0;
            for (size_t i = begin; i < _length; i++)
            {
                if(_min > _pBuffer[i%_size]) _min = _pBuffer[i % _size];
                if(_max < _pBuffer[i%_size]) _max = _pBuffer[i % _size];
                sum += _pBuffer[i%_size];                
            }
            _average = sum / _length;
            _gain = _max - _min;
            if(!(_gain > 0)) return;
            for (size_t i = begin; i < _length; i++)
            {
                _pBuffer[i % _size] = _pBuffer[i % _size] / _gain;
            }
            _normalized = true;
        }

        template <typename ValueType>
        bool SignalBuffer<ValueType>::normalized() const
        {
            return _normalized;
        }

        template <typename ValueType>
        void SignalBuffer<ValueType>::scale(ValueType scale)
        {
            if(_normalized)
            {
                _gain *=scale;
                return;
            }
            const auto begin = _length >= _size ? _length % _size : 0;
            for (auto i = begin; i < _length; i++)
            {
                _pBuffer[i % _size] = _pBuffer[i % _size] * scale;
            }
        }

        template <typename ValueType>
        void SignalBuffer<ValueType>::clear()
        {
            _length = 0;
            memset(_pBuffer, 0, sizeof(ValueType) * _size);
        }

        template <typename ValueType>
        SignalBuffer<ValueType>::~SignalBuffer()
        {
            delete _pBuffer;
        }

        template <typename ValueType>
        template <typename ValueType2>
        SignalBuffer<ValueType>& SignalBuffer<ValueType>::operator=(const SignalBuffer<ValueType2>& buffer)
        {
            _copy(buffer, *this);
            return *this;
        }

        template <typename ValueType>
        void SignalBuffer<ValueType>::attach(ValueType* pBuffer, size_t size, size_t length, ValueType gain, bool normalized)
        {
            delete[] _pBuffer;
            _pBuffer = pBuffer;
            _size = size;
            _length = length;
            if(normalized){
                normalize();
                _gain *= gain;
                _normalized = true;
            }
            else
            {
                _gain = gain;
                _normalized = false;
            }

        }

        template <typename ValueType>
        ValueType* SignalBuffer<ValueType>::detach(size_t* pSize, size_t* pLength, ValueType* pGain, bool* pNormalized)
        {
            if (pSize) *pSize = _size;
            if (pLength) *pLength = _length;
            if (pGain) *pGain = _gain;
            if (pNormalized) *pNormalized = _normalized;
            ValueType* pResult = _pBuffer;
            _size = 0;
            _length = 0;
            _gain = 1;
            _pBuffer = nullptr;
            _normalized = false;
            return pResult;
        }

        template <typename ValueType>
        template <typename ValueType1>
        void SignalBuffer<ValueType>::_copy(const SignalBuffer<ValueType1>& source, SignalBuffer<ValueType>& dest)
        {
            if (dest._pBuffer && dest._size != source.getSize())
            {
                delete dest._pBuffer;
                dest._pBuffer=nullptr;
            }
            if(dest._pBuffer==nullptr)
            {
                dest._pBuffer = new ValueType[source.getSize()];
                dest._size = source.getSize();
            }
            dest._normalized = source.isNormalized();
            dest._length = source.getLength();
            dest._gain = source.getGain();
            dest._max = source.getMax();
            dest._min = source.getMin();
            dest._average=source.getAverage();
            copyBuffer(dest._pBuffer, source.getBuffer(), source.getLength());
        }

        LIB_CORE_EXTERN template class LIB_CORE_API SignalBuffer<double>;
        LIB_CORE_EXTERN template class LIB_CORE_API SignalBuffer<float>;
        LIB_CORE_EXTERN template class LIB_CORE_API SignalBuffer<long>;
        LIB_CORE_EXTERN template class LIB_CORE_API SignalBuffer<short>;
    }
}