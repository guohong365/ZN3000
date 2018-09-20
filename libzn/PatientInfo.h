#pragma once

#include "patient.h"
class PatientInfo :public IPatient
{
public:
	PatientInfo();
	virtual const CString& getName();
	virtual void setName(const CString& name);
	virtual const CString& getId();
	virtual void setId(const CString& id);
	virtual const CString& getBirthday();
	virtual void setBirthday(const CString& date);
	virtual TCHAR getGender();
	virtual void setGender(TCHAR gender);
	virtual int getAge();
	virtual void setAge(int age);
	virtual float getHeight();
	virtual void setHeight(float height);
	virtual float getWeight();
	virtual void setWeight(float weight);
	virtual float getSystolicBloodPressure();
	virtual void setSystolicBloodPressure(float systolicBloodPressure);
	virtual float getDiastolicBloodPressure();
	virtual void setDiastolicBloodPressure(float diastolicBloodPressure);
	virtual float getHeartRate();
	virtual void setHeartRate(float heartRate);
	virtual float getElevation();
	virtual void setElevation(float elevation);
private:
	CString _name;
	CString _id;
	CString _birthday;
	char _gender;
	int _age;
	float _height;
	float _weight;
	float _systolicBloodPressure;   // ’Àı—π
	float  _diastolicBloodPressure; // Ê’≈—π
	float _heartRate;
	float _elevation;
};

