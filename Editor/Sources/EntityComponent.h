#ifndef ENTITY_COMPONENT
#define ENTITY_COMPONENT

#include "Cross.h"

#include <QGroupBox.h>

class QLineEdit;

using namespace cross;

class EntityComponent : public QGroupBox
{
	Q_OBJECT
public:
	EntityComponent(QWidget* parent = 0);
	~EntityComponent();

	void Update(float sec);
	void SetEntity(Entity* entity);

protected:
	void showEvent(QShowEvent *event);

private:
	Entity* entity		= NULL;
	QLineEdit* posX		= NULL;
	QLineEdit* posY		= NULL;
	QLineEdit* posZ		= NULL;
	QLineEdit* rotX		= NULL;
	QLineEdit* rotY		= NULL;
	QLineEdit* rotZ		= NULL;
	QLineEdit* angle	= NULL;
	QLineEdit* scaleX	= NULL;
	QLineEdit* scaleY	= NULL;
	QLineEdit* scaleZ	= NULL;

	void PositionChanged();
	void RotationChanged();
	void ScaleChanged();

	void UpdateRotatioon();
};

#endif