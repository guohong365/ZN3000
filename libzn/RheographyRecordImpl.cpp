#include "stdafx.h"

#include "RheographyRecordImpl.h"

RheographyRecordImpl::RheographyRecordImpl()
	: _gender(0)
	  , _age(0)
	  , _height(0)
	  , _weight(0)
	  , _systolicBloodPressure(0)
	  , _diastolicBloodPressure(0)
	  , _heartRate(0)
	  , _elevation(0)
	  , _startTime(0)
{
}


RheographyRecordImpl::~RheographyRecordImpl()
{
	for(Annotations::const_iterator i=_annotations.begin(); i!= _annotations.end(); ++i)
	{
		delete *(i);
	}
	for(SignalChannels::const_iterator i=_signalChannels.begin(); i!=_signalChannels.end(); ++i)
	{
		delete *(i);
	}
}

const CString& RheographyRecordImpl::getPatientName() const
{
	return _patientName;
}

void RheographyRecordImpl::setPatientName(const CString& name)
{
	_patientName=name;
}

const CString& RheographyRecordImpl::getPatientId() const
{
	return _patientId;
}

void RheographyRecordImpl::setPatientId(const CString& id)
{
	_patientId=id;
}

const CString& RheographyRecordImpl::getBirthday() const
{
	return _birthday;
}

void RheographyRecordImpl::setBirthday(const CString& date)
{
	_birthday=date;
}

TCHAR RheographyRecordImpl::getGender() const
{
	return _gender;
}

void RheographyRecordImpl::setGender(TCHAR gender)
{
	_gender=gender;
}

INT RheographyRecordImpl::getAge() const
{
	return _age;
}

void RheographyRecordImpl::setAge(INT age)
{
	_age=age;
}

FLOAT RheographyRecordImpl::getHeight() const
{
	return _height;
}

void RheographyRecordImpl::setHeight(FLOAT height)
{
	_height=height;
}

FLOAT RheographyRecordImpl::getWeight() const
{
	return _weight;
}

void RheographyRecordImpl::setWeight(FLOAT weight)
{
	_weight=weight;
}

FLOAT RheographyRecordImpl::getSystolicBloodPressure() const
{
	return _systolicBloodPressure;
}

void RheographyRecordImpl::setSystolicBloodPressure(FLOAT systolicBloodPressure)
{
	_systolicBloodPressure=systolicBloodPressure;
}

FLOAT RheographyRecordImpl::getDiastolicBloodPressure() const
{
	return _diastolicBloodPressure;
}

void RheographyRecordImpl::setDiastolicBloodPressure(FLOAT diastolicBloodPressure)
{
	_diastolicBloodPressure=diastolicBloodPressure;
}

FLOAT RheographyRecordImpl::getHeartRate() const
{
	return _heartRate;
}

void RheographyRecordImpl::setHeartRate(FLOAT heartRate)
{
	_heartRate=heartRate;
}

FLOAT RheographyRecordImpl::getElevation() const
{
	return _elevation;
}

void RheographyRecordImpl::setElevation(FLOAT elevation)
{
	_elevation=elevation;
}

const CString& RheographyRecordImpl::getRecordId() const
{
	return _recordId;
}

void RheographyRecordImpl::setRecordId(const CString& id)
{
	_recordId=id;
}

const CString& RheographyRecordImpl::getHospital() const
{
	return _hospital;
}

void RheographyRecordImpl::setHospital(const CString& hospital)
{
	_hospital=hospital;
}

const CString& RheographyRecordImpl::getDepartment() const
{
	return _department;
}

void RheographyRecordImpl::setDepartment(const CString& department)
{
	_department=department;
}

__time64_t RheographyRecordImpl::getStartTime() const
{
	return _startTime;
}

void RheographyRecordImpl::setStartTime(__time64_t startTime)
{
	_startTime = startTime;
}

SignalChannels& RheographyRecordImpl::getSignalChannels()
{
	return _signalChannels;
}

const SignalChannels& RheographyRecordImpl::GetSignalChannels() const
{
	return _signalChannels;
}
