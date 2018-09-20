#pragma once
#include "Record.h"

class AnnotationImpl : public Annotation
{
public:
	AnnotationImpl();
	virtual ~AnnotationImpl();;
	virtual SIZE_T getOffset() const;
	virtual void setOffset(SIZE_T offset);
	virtual const CString& getLabel() const;
	virtual void setLabel(const CString& label);
	virtual const SignalChannel* getOwner() const;
	virtual SignalChannel* getOwner();
	virtual void setOwner(SignalChannel* pChannel);
private:
	AnnotationImpl(const AnnotationImpl & annotation);
	AnnotationImpl & operator=(const AnnotationImpl & annotation);
	SIZE_T _offset;
	CString _label;
	SignalChannel * _pChannel;
};

