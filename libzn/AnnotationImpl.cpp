#include "stdafx.h"
#include "AnnotationImpl.h"

AnnotationImpl::AnnotationImpl(): _offset(0), _pChannel(nullptr)
{
}

AnnotationImpl::~AnnotationImpl()
{
}

SIZE_T AnnotationImpl::getOffset() const
{
	return _offset;
}

void AnnotationImpl::setOffset(SIZE_T offset)
{
	_offset=offset;
}

const CString& AnnotationImpl::getLabel() const
{
	return _label;
}

void AnnotationImpl::setLabel(const CString& label)
{
	_label= label;
}

const SignalChannel* AnnotationImpl::getOwner() const
{
	return _pChannel;
}

SignalChannel* AnnotationImpl::getOwner()
{
	return _pChannel;
}

void AnnotationImpl::setOwner(SignalChannel* pChannel)
{
	_pChannel=pChannel;
}

