#include "ConditionerImitation.h"
#include "CustomMainWindow.h"
#include "AppData.h"
#include <QObject>
#include <QDebug>

/**
    @brief ����������� ������-��������� ������� �����������������.
    @param temperature - ������������ �������� �����������.
    @param humidity    - ������������ �������� ���������.
    @param pressure    - ������������ �������� ��������.
    @param parentPtr   - ��������� ��� �������������� � �������� ����� ����������.
    @param parent      - �������� ��� ����������� �������� ��������, ��������������
	�� ������ QWidget, �� ��������� ����� nullptr.
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
    @brief ���������� ������-��������� ������� �����������������.
**/
ConditionerImitation::~ConditionerImitation() {
	delete humidityValidator;
	delete pressureValidator;
	delete ui;
}

/**
    @brief �����, ���������� �� ������������� ����� � ����������� ���������
	������. ���������� � ������������ ������.
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
    @brief ����� ��� ������������� �������� ���� ����������� � ������ �
	���������������	��������� � �������� ���� ����������.
    @param newTemperature - ����� �������� ��������� �����������, ����������
	� �������� ���� ����������.
**/
void ConditionerImitation::setTemperature(QString& newTemperature) {
	ui->temperatureBox->setValue(newTemperature.toDouble());
}

/**
	@brief ����� ��� ������������� �������� ���� �������� � ������ �
	��������������� ��������� � �������� ���� ����������.
	@param newPressure - ����� �������� ��������� ��������, ����������
	� �������� ���� ����������.
**/
void ConditionerImitation::setPressure(QString& newPressure) {
	ui->pressureEdit->setText(newPressure);
}

/**
	@brief ����� ��� ������������� ����������� ������ ������� �����������������
	� ��������������� ������ � �������� ���� ����������.
	@param newTemperature - ����� �������� � �������� ���� ����������
	����������� ������.
**/
void ConditionerImitation::setDirection(QString& newDirection) {
	//�������� ����������� ������
	//qDebug() << newDirection;
}
