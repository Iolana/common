#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <QString>
#include <QDate>

struct Patient
{
	int id; 
	QString name; 
	QDate birthday; 
	int sex; 
	Patient()
	{
		id = -1; 
		sex = -1; 
	}; 
	Patient& operator=(const Patient& data)
	{
		if(this != &data)
		{
			id = data.id; 
			name = data.name; 
			birthday = data.birthday; 
			sex = data.sex; 
		}
		return *this; 
	}
}; 
struct Examination
{
	int id; 
	int patientId;
	int modalityId; 
	int deviceId; 
	int regionId; 
	int contrastId; 
	double contrastAmount; 
	int orderedById; 
	QDate examDate; 
	QString content; 
	QString conclusion; 
	Examination()
	{
		id = -1; 
		patientId = -1;
		modalityId = -1; 
		deviceId = -1; 
		regionId = -1; 
		contrastId = -1; 
		contrastAmount = -1; 
		orderedById = -1; 
	}; 
		Examination& operator=(const Examination& data)
	{
		if(this != &data)
		{
			id = data.id; 
			patientId = data.patientId; 
			modalityId = data.modalityId; 
			deviceId = data.deviceId; 
			regionId = data.regionId; 
			contrastId = data.contrastId; 
			contrastAmount = data.contrastAmount; 
			orderedById = data.orderedById; 
			examDate = data.examDate; 
			content = data.content; 
			conclusion = data.conclusion; 
		}
		return *this; 
	}
}; 

#endif // STRUCTURES_H