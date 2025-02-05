#pragma once
#include <QWidget>
#include "ui_ConditionerImitation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ConditionerImitationClass; };
QT_END_NAMESPACE

/**

	@class   ConditionerImitation
	@brief   Класс, представляющий собой имитацию системы кондиционирования.
	Передает сигналы и параметры в основное окно приложения так, как они
	могли бы приходить от реальной системы кондиционирования.

**/
class ConditionerImitation : public QWidget {
	Q_OBJECT

public:
	ConditionerImitation(const QString& temperature, const QString& humidity, const QString& pressure, QWidget* parentPtr, QWidget* parent = nullptr);
	~ConditionerImitation();

public slots:
	void setTemperature(QString& newTemperature);
	void setPressure(QString& newPressure);
	void setDirection(QString& newDirection);

private:
	void initializeFieldsAndButtons();

	QIntValidator* humidityValidator;
	QIntValidator* pressureValidator;

	QWidget* parentPtr;
	Ui::ConditionerImitationClass* ui;
};
