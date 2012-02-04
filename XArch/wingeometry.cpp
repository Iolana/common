#include "wingeometry.h"
#include <QCoreApplication>
#include <QWidget>
#include <QSettings>

WinGeometry::WinGeometry(QWidget *parent)
	: QObject(parent)
{
	owner = parent; 
}

WinGeometry::~WinGeometry()
{

}
void WinGeometry::load()
{
	QSettings settings(QString("%1.ini").arg(QCoreApplication::applicationName()), QSettings::IniFormat);
	settings.beginGroup(owner->objectName()); 
	owner->resize(settings.value("size", QSize(800, 600)).toSize()); 
	owner->move(settings.value("pos", QPoint(50, 50)).toPoint()); 
	settings.endGroup(); 
}
void WinGeometry::save()
{
	QSettings settings(QString("%1.ini").arg(QCoreApplication::applicationName()), QSettings::IniFormat);
	settings.beginGroup(owner->objectName()); 
	settings.setValue("size", owner->size());
	settings.setValue("pos", owner->pos());
	settings.endGroup(); 
}