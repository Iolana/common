#ifndef WINGEOMETRY_H
#define WINGEOMETRY_H

#include <QObject>

class QWidget; 

class WinGeometry : public QObject
{
	Q_OBJECT

public:
	WinGeometry(QWidget *parent);
	~WinGeometry();
	void load(); 
	void save(); 

private:
	QWidget* owner; 
};

#endif // WINGEOMETRY_H
