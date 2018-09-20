#include "stdafx.h"
#include "PatientInfo.h"

PatientInfo::PatientInfo(): _gender(0), _age(0), _height(0), _weight(0), _systolicBloodPressure(0),
                            _diastolicBloodPressure(0), _heartRate(0), _elevation(0)
{
}

const CString& PatientInfo::getName()
{
	return _name;
}

void PatientInfo::setName(const CString& name)
{
	_name=name;
}

const CString& PatientInfo::getId()
{
	return _id;
}

void PatientInfo::setId(const CString& id)
{
	_id=id;
}

const CString& PatientInfo::getBirthday()
{
	return _birthday;
}

void PatientInfo::setBirthday(const CString& date)
{
	_birthday=date;
}

TCHAR PatientInfo::getGender()
{
	return _gender;
}

void PatientInfo::setGender(TCHAR gender)
{
	_gender=gender;
}

int PatientInfo::getAge()
{
	return _age;
}

void PatientInfo::setAge(int age)
{
	_age=age;
}

float PatientInfo::getHeight()
{
	return _height;
}

void PatientInfo::setHeight(float height)
{
	_height = height;
}

float PatientInfo::getWeight()
{
	return _weight;
}

void PatientInfo::setWeight(float weight)
{
	_weight = weight;
}

float PatientInfo::getSystolicBloodPressure()
{
	return _systolicBloodPressure;
}

void PatientInfo::setSystolicBloodPressure(float systolicBloodPressure)
{
	_systolicBloodPressure = systolicBloodPressure;
}

float PatientInfo::getDiastolicBloodPressure()
{
	return _diastolicBloodPressure;
}

void PatientInfo::setDiastolicBloodPressure(float diastolicBloodPressure)
{
	_diastolicBloodPressure = diastolicBloodPressure;
}

float PatientInfo::getHeartRate()
{
	return _heartRate;
}

void PatientInfo::setHeartRate(float heartRate)
{
	_heartRate = heartRate;
}

float PatientInfo::getElevation()
{
	return _elevation;
}

void PatientInfo::setElevation(float elevation)
{
	_elevation = elevation;
}

