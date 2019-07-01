#pragma once

#include <vector>
#include "../signals/Record.h"
#include "RheographyRecord.h"
#define LEN_NAME 20
#define LEN_CODE 10
#define SEX_FEMALE 'F'
#define SEX_MALE   'M'


class RheographyRecordImpl : public RheographyRecord
{
public:
	RheographyRecordImpl();
	virtual ~RheographyRecordImpl();
	virtual CONST CString& getPatientName() CONST;
	virtual VOID setPatientName(CONST CString& name);
	virtual CONST CString& getPatientId() CONST;
	virtual VOID setPatientId(CONST CString& id);
	virtual CONST CString& getBirthday() CONST;
	virtual VOID setBirthday(CONST CString& date);
	virtual TCHAR getGender() CONST;
	virtual VOID setGender(TCHAR gender);
	virtual INT getAge() CONST;
	virtual VOID setAge(INT age);
	virtual FLOAT getHeight() CONST;
	virtual VOID setHeight(FLOAT height);
	virtual FLOAT getWeight() CONST;
	virtual VOID setWeight(FLOAT weight);
	virtual FLOAT getSystolicBloodPressure() CONST;
	virtual VOID setSystolicBloodPressure(FLOAT systolicBloodPressure);
	virtual FLOAT getDiastolicBloodPressure() CONST;
	virtual VOID setDiastolicBloodPressure(FLOAT diastolicBloodPressure);
	virtual FLOAT getHeartRate() CONST;
	virtual VOID setHeartRate(FLOAT heartRate);
	virtual FLOAT getElevation() CONST;
	virtual VOID setElevation(FLOAT elevation);

	virtual const CString& getRecordId() const;
	virtual void setRecordId(const CString& id);
	virtual const CString& getHospital() const;
	virtual void setHospital(const CString& hospital);
	virtual const CString& getDepartment() const;
	virtual void setDepartment(const CString& department);
	__time64_t getStartTime() const;
	void setStartTime(__time64_t startTime);
	virtual SignalChannels& getSignalChannels();
	virtual const SignalChannels& getSignalChannels() const;
private:
	CString _recordId;
	CString _hospital;
	CString _department;
	CString _patientName;
	CString _patientId;
	CString _birthday;
	TCHAR _gender;
	INT _age;
	FLOAT _height;
	FLOAT _weight;
	FLOAT _systolicBloodPressure;   // ’Àı—π
	FLOAT  _diastolicBloodPressure; // Ê’≈—π
	FLOAT _heartRate;
	FLOAT _elevation;
	__time64_t _startTime;
	Annotations _annotations;
	SignalChannels _signalChannels;
};

