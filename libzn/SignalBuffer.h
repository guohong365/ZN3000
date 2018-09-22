#pragma once

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
ValueType minBuffer(ValueType * pBuffer, size_t count)
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
ValueType maxBuffer(ValueType * pBuffer, size_t count)
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
	explicit SignalBuffer(SIZE_T size);

	SIZE_T getLength() const;
	void setLength(SIZE_T length);
	SIZE_T getSize() const;
	ValueType* getBuffer();
	const ValueType* getBuffer() const;
	ValueType operator[](int index) const;
	ValueType& operator[](int index);
	void append(ValueType val);
	ValueType maxValue();
	ValueType minValue();
	void normalize();
	bool isNormalized() const;
	void scale(DOUBLE scale);
	void clear();
	~SignalBuffer();

	template <typename ValueType2>
	SignalBuffer<ValueType>& operator =(const SignalBuffer<ValueType2>& buffer);
private:
	SignalBuffer();
	template <typename ValueType1>
	static void _copy(const SignalBuffer<ValueType1>& source, SignalBuffer<ValueType>& dest);	
	SIZE_T _size;
	SIZE_T _length;
	ValueType * _pBuffer;
	bool _normalized;
};

template <typename ValueType>
template <typename ValueType2>
SignalBuffer<ValueType>::SignalBuffer(const SignalBuffer<ValueType2>& buffer)
	:_pBuffer(nullptr)
{
	_copy(buffer, *this);
}

template <typename ValueType>
SignalBuffer<ValueType>::SignalBuffer(const SIZE_T size)
	: _size(size), _length(0), _normalized(false)
{
	ASSERT(_size > 0);
	_pBuffer = new ValueType[_size];
}

template <typename ValueType>
SIZE_T SignalBuffer<ValueType>::getLength() const
{
	return _length;
}

template <typename ValueType>
void SignalBuffer<ValueType>::setLength(SIZE_T length)
{
	_length = length;
}

template <typename ValueType>
SIZE_T SignalBuffer<ValueType>::getSize() const
{
	return _size;
}

template <typename ValueType>
ValueType* SignalBuffer<ValueType>::getBuffer()
{
	return _pBuffer;
}

template <typename ValueType>
const ValueType* SignalBuffer<ValueType>::getBuffer() const
{
	return _pBuffer;
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
void SignalBuffer<ValueType>::append(ValueType val)
{
	_pBuffer[_length % _size] = val;
	_length ++;
}

template <typename ValueType>
ValueType SignalBuffer<ValueType>::maxValue()
{
	SIZE_T begin = _length >= _size ? _length % _size : 0;
	ValueType maxValue = _pBuffer[begin];
	for (SIZE_T i = begin; i < _length; i++)
	{
		if (maxValue < _pBuffer[i % _size])
		{
			maxValue = _pBuffer[i % _size];
		}
	}
	return maxValue;
}

template <typename ValueType>
ValueType SignalBuffer<ValueType>::minValue()
{
	SIZE_T begin = _length >= _size ? _length % _size : 0;
	ValueType minValue = _pBuffer[begin];
	for (SIZE_T i = begin; i < _length; i++)
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
	const SIZE_T begin = _length >= _size ? _length % _size : 0;
	ValueType minValue = minValue();
	ValueType maxValue = maxValue();
	ValueType scale = maxValue - minValue;
	if (scale == 0) return;
	for (SIZE_T i = begin; i < _length; i++)
	{
		_pBuffer[i % _size] = _pBuffer[i % _size] / scale;
	}
	_normalized = true;
}

template <typename ValueType>
bool SignalBuffer<ValueType>::isNormalized() const
{
	return _normalized;
}

template <typename ValueType>
void SignalBuffer<ValueType>::scale(DOUBLE scale)
{
	const auto begin = _length >= _size ? _length % _size : 0;
	for (auto i = begin; i < _length; i++)
	{
		_pBuffer[i % _size] = _pBuffer[i % _size] * scale;
	}
	if (scale != 1)
	{
		_normalized = false;
	}
}

template <typename ValueType>
void SignalBuffer<ValueType>::clear()
{
	_length = 0;
#ifdef _DEBUG
	memset(_pBuffer, 0, sizeof(ValueType) * _size);
#endif
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
	copyBuffer(dest._pBuffer, source.getBuffer(), source.getLength());
}
