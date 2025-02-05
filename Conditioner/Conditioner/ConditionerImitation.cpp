#include "ConditionerImitation.h"
#include "CustomMainWindow.h"
#include "AppData.h"
#include <QObject>
#include <QDebug>

/**
    @brief Конструктор класса-имитатора системы кондиционирования.
    @param temperature - Передаваемый параметр температуры.
    @param humidity    - Передаваемый параметр влажности.
    @param pressure    - Передаваемый параметр давления.
    @param parentPtr   - Указатель для взаимодействия с основным окном приложения.
    @param parent      - Параметр для обозначения иерархии виджетов, унаследованный
	от класса QWidget, по умолчанию равен nullptr.
**/
ConditionerImitation::ConditionerImitation(const QString& temperature, const QString& humidity, const QString& pressure, QWidget* parentPtr, QWidget* parent)
	: QWidget(parent), ui(new Ui::ConditionerImitationClass()) {
	ui->setupUi(this);
	ui->temperatureBox->setValue(temperature.toDouble());
	ui->humidityEdit->setText(humidity.chopped(1));
	ui->pressureEdit->setText(pressure);
	this->parentPtr = parentPtr;
	initializeFieldsAndButtons();
}

/**
    @brief Деструктор класса-имитатора системы кондиционирования.
**/
ConditionerImitation::~ConditionerImitation() {
	delete humidityValidator;
	delete pressureValidator;
	delete ui;
}

/**
    @brief Метод, отвечающий за инициализацию полей и управляющих элементов
	класса. Вызывается в конструкторе класса.
**/
void ConditionerImitation::initializeFieldsAndButtons() {
	humidityValidator = new QIntValidator(HUMIDITY_MIN, HUMIDITY_MAX);
	ui->humidityEdit->setValidator(humidityValidator);
	pressureValidator = new QIntValidator(-1000000, 10000000);
	ui->pressureEdit->setValidator(pressureValidator);
	connect(parentPtr, SIGNAL(temperatureChanged(QString&)), this, SLOT(setTemperature(QString&)));
	connect(ui->temperatureBox, SIGNAL(valueChanged(double)), parentPtr, SLOT(getTemperatureEdited(double)));
	connect(ui->humidityEdit, SIGNAL(textEdited(const QString&)), parentPtr, SLOT(getHumidityEdited(const QString&)));
	connect(parentPtr, SIGNAL(pressureChanged(QString&)), this, SLOT(setPressure(QString&)));
	connect(ui->pressureEdit, SIGNAL(textEdited(const QString&)), parentPtr, SLOT(getPressureEdited(const QString&)));
	connect(parentPtr, SIGNAL(directionChanged(QString&)), this, SLOT(setDirection(QString&)));
	connect(ui->sendInnerIssueButton, SIGNAL(clicked(bool)), parentPtr, SLOT(getInnerBlockError()));
	connect(ui->solveInnerIssueButton, SIGNAL(clicked(bool)), parentPtr, SLOT(getInnerBlockOk()));
	connect(ui->sendOuterIssueButton, SIGNAL(clicked(bool)), parentPtr, SLOT(getOuterBlockError()));
	connect(ui->solveOuterIssueButton, SIGNAL(clicked(bool)), parentPtr, SLOT(getOuterBlockOk()));
}

/**
    @brief Метод для синхронизации значения поля температуры в классе с
	соответствующим	значением в основном окне приложения.
    @param newTemperature - Новое значение параметра температуры, измененное
	в основном окне приложения.
**/
void ConditionerImitation::setTemperature(QString& newTemperature) {
	ui->temperatureBox->setValue(newTemperature.toDouble());
}

/**
	@brief Метод для синхронизации значения поля давления в классе с
	соответствующим значением в основном окне приложения.
	@param newPressure - Новое значение параметра давления, измененное
	в основном окне приложения.
**/
void ConditionerImitation::setPressure(QString& newPressure) {
	ui->pressureEdit->setText(newPressure);
}

/**
	@brief Метод для синхронизации направления выдува системы кондиционирования
	с соответствующей опцией в основном окне приложения.
	@param newTemperature - Новое заданное в основном окне приложения
	направление выдува.
**/
void ConditionerImitation::setDirection(QString& newDirection) {
	//Изменить направление выдува
	//qDebug() << newDirection;
}
