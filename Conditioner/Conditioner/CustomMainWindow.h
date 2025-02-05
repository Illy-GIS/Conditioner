#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CustomMainWindow.h"
#include "ConditionerImitation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CustomMainWindowClass; };
QT_END_NAMESPACE

/**

    @class   CustomMainWindow
    @brief    ласс, представл€ющий собой основное окно приложени€.
	–еализует графический интерфейс дл€ взаимодействи€ пользовател€
	с системой кондиционировани€, а также основные элементы управлени€
	этой системой.

**/
class CustomMainWindow : public QMainWindow {
	Q_OBJECT

public:
	CustomMainWindow(QWidget* parent = nullptr);
	~CustomMainWindow();

	QString& getLightStyle();
	QString& getDarkStyle();

signals:
	void temperatureChanged(QString& newTemperature);
	void pressureChanged(QString& newPressure);
	void directionChanged(QString& newDirection);

public slots:
	void getTemperatureEdited(double newValue);
	void getHumidityEdited(const QString& newValue);
	void getPressureEdited(const QString& newValue);
	void getInnerBlockError();
	void getInnerBlockOk();
	void getOuterBlockError();
	void getOuterBlockOk();

private:
	void initializeStyles();
	void initializeButtons();

	void turnOnLightMode();
	void turnOnDarkMode();

	void switchPower();
	void saveSettings();
	void loadSettings();
	void setDefaultSettings();

	void increaseTemperature();
	void decreaseTemperature();
	bool validateTemperature(double newValue);
	void changeTemperatureModeFromCelsius();
	void changeTemperatureModeFromFahrenheit();
	void changeTemperatureModeFromKelvin();

	bool validateHumidity(const QString& newValue);

	void changePressureModeFromMmHg();
	void changePressureModeFromPascal();

	void changeDirectionToLeft();
	void changeDirectionToCenter();
	void changeDirectionToRight();

	void closeEvent(QCloseEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;

	QString lightStyle;
	QString darkStyle;

	QButtonGroup* modeGroup;
	QButtonGroup* temperatureGroup;
	QButtonGroup* pressureGroup;
	QButtonGroup* directionGroup;

	bool power = false;

	QSize resizeBuffer;

	Ui::CustomMainWindowClass* ui;
	ConditionerImitation* subWindow;
};
