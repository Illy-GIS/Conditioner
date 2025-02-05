#include "CustomMainWindow.h"
#include "AppData.h"
#include <QFile>
#include <QButtonGroup>
#include <QIcon>
#include <QCloseEvent>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QDebug>

/**
    @brief ����������� ������ ��������� ���� ����������.
    @param parent - �������� ��� ����������� �������� ��������, ��������������
	�� ������ QWidget, �� ��������� ����� nullptr.
**/
CustomMainWindow::CustomMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::CustomMainWindowClass()) {
	ui->setupUi(this);
	initializeStyles();
	initializeButtons();
	loadSettings();
	resizeBuffer = QSize(0, 0);
}

/**
    @brief ���������� ������ ��������� ���� ����������.
**/
CustomMainWindow::~CustomMainWindow() {
	delete modeGroup;
	delete temperatureGroup;
	delete pressureGroup;
	delete directionGroup;
	delete ui;
}

/**
    @brief �����, ���������� �� ������������� ������ ����������� ����.
	���������� � ������������ ������.
**/
void CustomMainWindow::initializeStyles() {
	QFile styleFile(lightStyleSheetPath);
	styleFile.open(QFile::ReadOnly);
	lightStyle = styleFile.readAll();
	styleFile.close();
	styleFile.setFileName(darkStyleSheetPath);
	styleFile.open(QFile::ReadOnly);
	darkStyle = styleFile.readAll();
	styleFile.close();
}

/**
    @brief  �����, �������������� ������ � �������� ����� ����������� ����.
    @retval  - ������, ���������� ������� ����� ���� � ������� QSS.
**/
QString& CustomMainWindow::getLightStyle() {
	return this->lightStyle;
}

/**
    @brief  �����, �������������� ������ � ������� ����� ����������� ����.
    @retval  - ������, ���������� ������ ����� ���� � ������� QSS.
**/
QString& CustomMainWindow::getDarkStyle() {
	return this->darkStyle;
}

/**
    @brief �����, ���������� �� ������������� ����������� ��������� ������.
	���������� � ������������ ������.
**/
void CustomMainWindow::initializeButtons() {
	modeGroup = new QButtonGroup(this);
	modeGroup->addButton(ui->lightToggle);
	modeGroup->addButton(ui->darkToggle);
	temperatureGroup = new QButtonGroup(this);
	temperatureGroup->addButton(ui->celsiusButton);
	temperatureGroup->addButton(ui->fahrenheitButton);
	temperatureGroup->addButton(ui->kelvinButton);
	pressureGroup = new QButtonGroup(this);
	pressureGroup->addButton(ui->mmHgButton);
	pressureGroup->addButton(ui->pascalButton);
	directionGroup = new QButtonGroup(this);
	directionGroup->addButton(ui->leftDirButton);
	directionGroup->addButton(ui->centerDirButton);
	directionGroup->addButton(ui->rightDirButton);

	connect(ui->lightToggle, &QPushButton::toggled, this, &CustomMainWindow::turnOnLightMode);
	connect(ui->darkToggle, &QPushButton::toggled, this, &CustomMainWindow::turnOnDarkMode);
	connect(ui->powerButton, &QPushButton::clicked, this, &CustomMainWindow::switchPower);
	connect(ui->increaseButton, &QPushButton::clicked, this, &CustomMainWindow::increaseTemperature);
	connect(ui->decreaseButton, &QPushButton::clicked, this, &CustomMainWindow::decreaseTemperature);
	connect(ui->celsiusButton, &QPushButton::toggled, this, &CustomMainWindow::changeTemperatureModeFromCelsius);
	connect(ui->fahrenheitButton, &QPushButton::toggled, this, &CustomMainWindow::changeTemperatureModeFromFahrenheit);
	connect(ui->kelvinButton, &QPushButton::toggled, this, &CustomMainWindow::changeTemperatureModeFromKelvin);
	connect(ui->mmHgButton, &QPushButton::toggled, this, &CustomMainWindow::changePressureModeFromMmHg);
	connect(ui->pascalButton, &QPushButton::toggled, this, &CustomMainWindow::changePressureModeFromPascal);
	connect(ui->leftDirButton, &QPushButton::toggled, this, &CustomMainWindow::changeDirectionToLeft);
	connect(ui->centerDirButton, &QPushButton::toggled, this, &CustomMainWindow::changeDirectionToCenter);
	connect(ui->rightDirButton, &QPushButton::toggled, this, &CustomMainWindow::changeDirectionToRight);
}

/**
    @brief �����, ���������� �� ������������ ����������� ����,
	���������������� �������� �����.
**/
void CustomMainWindow::turnOnLightMode() {
	if (ui->lightToggle->isChecked()) {
		this->setStyleSheet(lightStyle);
		ui->lightToggle->setIcon(QIcon(sunLightIcon));
		ui->innerBlock->setPixmap(QPixmap(innerBlockLightIcon));
		ui->outerBlock->setPixmap(QPixmap(outerBlockLightIcon));
		ui->increaseButton->setIcon(QIcon(increaseLightIcon));
		ui->decreaseButton->setIcon(QIcon(decreaseLightIcon));
		ui->humidityIcon->setPixmap(QPixmap(dropLightIcon));
		ui->leftDirButton->setIcon(QIcon(leftDirLightIcon));
		ui->centerDirButton->setIcon(QIcon(centerDirLightIcon));
		ui->rightDirButton->setIcon(QIcon(rightDirLightIcon));
	}
}

/**
    @brief �����, ���������� �� ������������ ����������� ����,
	���������������� ������� �����.
**/
void CustomMainWindow::turnOnDarkMode() {
	if (ui->darkToggle->isChecked()) {
		this->setStyleSheet(darkStyle);
		ui->lightToggle->setIcon(QIcon(sunDarkIcon));
		ui->innerBlock->setPixmap(QPixmap(innerBlockDarkIcon));
		ui->outerBlock->setPixmap(QPixmap(outerBlockDarkIcon));
		ui->increaseButton->setIcon(QIcon(increaseDarkIcon));
		ui->decreaseButton->setIcon(QIcon(decreaseDarkIcon));
		ui->humidityIcon->setPixmap(QPixmap(dropDarkIcon));
		ui->leftDirButton->setIcon(QIcon(leftDirDarkIcon));
		ui->centerDirButton->setIcon(QIcon(centerDirDarkIcon));
		ui->rightDirButton->setIcon(QIcon(rightDirDarkIcon));
	}
}

/**
    @brief �����, ���������� �� ��������� � ���������� �������
	�����������������. ��� ��������� ������������ ������ �
	�������� ��������� ���������� �������� � ���������
	���� �� ���������. ��������������, ��� ����������
	��������� ������ � ��������� ���������� � ��������� ����
	���������.
**/
void CustomMainWindow::switchPower() {
	if (!power) {
		power = true;
		ui->controlHide->hide();
		subWindow = new ConditionerImitation(ui->temperatureLabel->text(), ui->humidityLabel->text(), ui->pressureLabel->text(), this);
		subWindow->show();
	}
	else {
		power = false;
		ui->controlHide->show();
		subWindow->close();
		delete subWindow;
	}
}

/**
    @brief �����, �������������� ���������� ������� ����������
	������� ����������������� � ����� ��������.
**/
void CustomMainWindow::saveSettings() {
	QFile settingsFile(settingsPath);
	settingsFile.open(QFile::WriteOnly);
	QXmlStreamWriter writer(&settingsFile);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	writer.writeStartElement("settings");
	writer.writeStartElement(modeGroup->checkedButton()->objectName());
	writer.writeCharacters("\n");
	writer.writeEndElement();
	writer.writeStartElement(ui->temperatureLabel->objectName());
	writer.writeAttribute("text", ui->temperatureLabel->text());
	writer.writeCharacters("\n");
	writer.writeEndElement();
	writer.writeStartElement(temperatureGroup->checkedButton()->objectName());
	writer.writeCharacters("\n");
	writer.writeEndElement();
	writer.writeStartElement(ui->humidityLabel->objectName());
	writer.writeAttribute("text", ui->humidityLabel->text());
	writer.writeCharacters("\n");
	writer.writeEndElement();
	writer.writeStartElement(ui->pressureLabel->objectName());
	writer.writeAttribute("text", ui->pressureLabel->text());
	writer.writeCharacters("\n");
	writer.writeEndElement();
	writer.writeStartElement(pressureGroup->checkedButton()->objectName());
	writer.writeCharacters("\n");
	writer.writeEndElement();
	writer.writeStartElement(directionGroup->checkedButton()->objectName());
	writer.writeCharacters("\n");
	writer.writeEndElement();
	writer.writeEndElement();
	writer.writeEndDocument();
	settingsFile.close();
}

/**
    @brief �����, �������������� �������� ��������� �����������
	���������� ������� ����������������� �� ����� ��������.
**/
void CustomMainWindow::loadSettings() {
	QFile settingsFile(settingsPath);
	if (settingsFile.open(QFile::ReadOnly)) {
		QXmlStreamReader reader(&settingsFile);
		while (!reader.atEnd()) {
			if (reader.isStartElement()) {
				if (reader.name() == "lightToggle" || reader.name() == "darkToggle") {
					ui->centralWidget->findChild<QPushButton*>(reader.name().toString())->setChecked(true);
				}
				else if (reader.name() == "temperatureLabel") {
					ui->temperatureLabel->setText(reader.attributes().at(0).value().toString());
				}
				else if (reader.name() == "celsiusButton" || reader.name() == "fahrenheitButton" || reader.name() == "kelvinButton") {
					ui->centralWidget->findChild<QPushButton*>(reader.name().toString())->setChecked(true);
				}
				else if (reader.name() == "humidityLabel") {
					ui->humidityLabel->setText(reader.attributes().at(0).value().toString());
				}
				else if (reader.name() == "pressureLabel") {
					ui->pressureLabel->setText(reader.attributes().at(0).value().toString());
				}
				else if (reader.name() == "mmHgButton" || reader.name() == "pascalButton") {
					ui->centralWidget->findChild<QPushButton*>(reader.name().toString())->setChecked(true);
				}
				else if (reader.name() == "leftDirButton" || reader.name() == "centerDirButton" || reader.name() == "rightDirButton") {
					ui->centralWidget->findChild<QPushButton*>(reader.name().toString())->setChecked(true);
				}
			}
			reader.readNext();
			if (reader.hasError()) {
				QMessageBox::warning(this, appTitle, loadSettingsWarningMessage.toUtf8());
				setDefaultSettings();
				break;
			}
		}
	}
	else {
		QMessageBox::warning(this, appTitle, loadSettingsWarningMessage.toUtf8());
		setDefaultSettings();
	}
	settingsFile.close();
}

/**
    @brief �����, ��������������� �� ��������� ��������� ���������
	���� ����������.
**/
void CustomMainWindow::setDefaultSettings() {
	turnOnLightMode();
	ui->celsiusButton->setChecked(true);
	ui->mmHgButton->setChecked(true);
	ui->centerDirButton->setChecked(true);
}

/**
    @brief �����, ���������� �������� ����������� ������� �����������������
	�� ���� �����.
**/
void CustomMainWindow::increaseTemperature() {
	double newValue = ui->temperatureLabel->text().toDouble();
	if (ui->celsiusButton->isChecked()) {
		if (validateTemperature(newValue + 1.0)) {
			newValue += 1.0;
			ui->temperatureLabel->setText(QString::number(int(newValue)));
		}
	}
	else if (ui->fahrenheitButton->isChecked()) {
		if (validateTemperature(newValue + 1.8)) {
			newValue += 1.8;
			ui->temperatureLabel->setText(QString::number(newValue));
		}
	}
	else {
		if (validateTemperature(newValue + 1.0)) {
			newValue += 1.0;
			ui->temperatureLabel->setText(QString::number(newValue));
		}
	}
	QString changedValue = QString::number(newValue);
	emit temperatureChanged(changedValue);
}

/**
    @brief �����, ���������� �������� ����������� ������� �����������������
	�� ���� �����.
**/
void CustomMainWindow::decreaseTemperature() {
	double newValue = ui->temperatureLabel->text().toDouble();
	if (ui->celsiusButton->isChecked()) {
		if (validateTemperature(newValue - 1.0)) {
			newValue -= 1.0;
			ui->temperatureLabel->setText(QString::number(int(newValue)));
		}
	}
	else if (ui->fahrenheitButton->isChecked()) {
		if (validateTemperature(newValue - 1.8)) {
			newValue -= 1.8;
			ui->temperatureLabel->setText(QString::number(newValue));
		}
	}
	else {
		if (validateTemperature(newValue - 1.0)) {
			newValue -= 1.0;
			ui->temperatureLabel->setText(QString::number(newValue));
		}
	}
	QString changedValue = QString::number(newValue);
	emit temperatureChanged(changedValue);
}

/**
    @brief  �����, ����������� ���������� ����������� ��������� �����������
	� ���������� ��������.
    @param  newValue - ���������� �������� �����������.
    @retval          - ���������� true, ���� �������� ����������� ���������
	��������� � ���������� ��������, ����� ���������� false.
**/
bool CustomMainWindow::validateTemperature(double newValue) {
	bool res = false;
	if (ui->celsiusButton->isChecked()) {
		if (newValue >= CELSIUS_MIN && newValue <= CELSIUS_MAX) {
			res = true;
		}
	}
	else if (ui->fahrenheitButton->isChecked()) {
		if (newValue >= FAHRENHEIT_MIN && newValue <= FAHRENHEIT_MAX) {
			res = true;
		}
	}
	else {
		if (newValue >= KELVIN_MIN && newValue <= KELVIN_MAX) {
			res = true;
		}
	}
	return res;
}

/**
    @brief �����, �������������� �������� �������� ��������
	����������� �� ����� �������, ��� ����� ��� ���������������
	������, ���������� �������������.
**/
void CustomMainWindow::changeTemperatureModeFromCelsius() {
	if (!ui->celsiusButton->isChecked()) {
		double newValue = ui->temperatureLabel->text().toDouble();
		if (ui->fahrenheitButton->isChecked()) {
			newValue = newValue * 1.8 + 32.0;
		}
		else {
			newValue += 273.15;
		}
		QString changedValue = QString::number(newValue);
		ui->temperatureLabel->setText(changedValue);
		emit temperatureChanged(changedValue);
	}
}

/**
    @brief �����, �������������� �������� �������� ��������
	����������� �� ����� ����������, ��� ����� ��� ���������������
	������, ���������� �������������.
**/
void CustomMainWindow::changeTemperatureModeFromFahrenheit() {
	if (!ui->fahrenheitButton->isChecked()) {
		double newValue = ui->temperatureLabel->text().toDouble();
		if (ui->celsiusButton->isChecked()) {
			newValue = (newValue - 32.0) / 1.8;
		}
		else {
			newValue = (newValue + 459.67) / 1.8;
		}
		QString changedValue = ui->celsiusButton->isChecked() ? QString::number(int(newValue)) : QString::number(newValue);
		ui->temperatureLabel->setText(changedValue);
		emit temperatureChanged(changedValue);
	}
}

/**
    @brief �����, �������������� �������� �������� ��������
	����������� �� ����� ��������, ��� ����� ��� ���������������
	������, ���������� �������������.
**/
void CustomMainWindow::changeTemperatureModeFromKelvin() {
	if (!ui->kelvinButton->isChecked()) {
		double newValue = ui->temperatureLabel->text().toDouble();
		if (ui->celsiusButton->isChecked()) {
			newValue -= 273.15;
		}
		else {
			newValue = newValue * 1.8 - 459.67;
		}
		QString changedValue = ui->celsiusButton->isChecked() ? QString::number(int(newValue)) : QString::number(newValue);
		ui->temperatureLabel->setText(changedValue);
		emit temperatureChanged(changedValue);
	}
}

/**
    @brief  �����, ����������� ���������� ����������� ��������� ���������
	� ���������� ��������.
    @param  newValue - ���������� �������� ���������.
    @retval          - ���������� true, ���� �������� ����������� ���������
	��������� � ���������� ��������, ����� ���������� false.
**/
bool CustomMainWindow::validateHumidity(const QString& newValue) {
	return newValue.toInt() >= HUMIDITY_MIN && newValue.toInt() <= HUMIDITY_MAX;
}

/**
    @brief �����, �������������� �������� �������� ��������
	�������� �� ������� ��������� �� ��. ��., ��� ����� ��� ���������������
	������, ���������� �������������.
**/
void CustomMainWindow::changePressureModeFromMmHg() {
	if (!ui->mmHgButton->isChecked()) {
		double newValue = ui->pressureLabel->text().toDouble();
		newValue *= 133.32;
		QString changedValue = QString::number(newValue);
		ui->pressureLabel->setText(changedValue);
		emit pressureChanged(changedValue);
	}
}

/**
    @brief �����, �������������� �������� �������� ��������
	�������� �� ������� ��������� ��, ��� ����� ��� ���������������
	������, ���������� �������������.
**/
void CustomMainWindow::changePressureModeFromPascal() {
	if (!ui->pascalButton->isChecked()) {
		double newValue = ui->pressureLabel->text().toDouble();
		newValue /= 133.32;
		QString changedValue = QString::number(qRound(newValue));
		ui->pressureLabel->setText(changedValue);
		emit pressureChanged(changedValue);
	}
}

/**
    @brief �����, �������������� ��������� ����������� ������
	������� ����������������� � ����� �������.
**/
void CustomMainWindow::changeDirectionToLeft() {
	if (ui->leftDirButton->isChecked()) {
		QString newDirection = "left";
		emit directionChanged(newDirection);
	}
}

/**
    @brief �����, �������������� ������������� ����������� ������
	������� �����������������.
**/
void CustomMainWindow::changeDirectionToCenter() {
	if (ui->centerDirButton->isChecked()) {
		QString newDirection = "center";
		emit directionChanged(newDirection);
	}
}

/**
    @brief �����, �������������� ��������� ����������� ������
	������� ����������������� � ������ �������.
**/
void CustomMainWindow::changeDirectionToRight() {
	if (ui->rightDirButton->isChecked()) {
		QString newDirection = "right";
		emit directionChanged(newDirection);
	}
}

/**
    @brief ��������������� ������� �������� ��������� ���� ����������,
	��� ����� ����� ���� ����������� ���������� ������� �����������������.
    @param event - ��������, �������������� �� ������-��������, ����������
	�� �������� ����.
**/
void CustomMainWindow::closeEvent(QCloseEvent* event) {
	saveSettings();
	if (power) {
		switchPower();
	}
	event->accept();
}

/**
    @brief �����, �������������� ��������� ��������� �����������, �������������
	� �������� ���� ����������, � ������������ �� ���������, ���������� ��
	��������� ������� �����������������, ��� �������, ��� ���������� ��������
	��������� � ���������� ��������.
    @param newValue - ���������� �������� �����������, ���������� �� ���������.
**/
void CustomMainWindow::getTemperatureEdited(double newValue) {
	if (validateTemperature(newValue)) {
		if (ui->celsiusButton->isChecked()) {
			ui->temperatureLabel->setText(QString::number(int(newValue)));
		}
		else {
			ui->temperatureLabel->setText(QString::number(newValue));
		}
	}
}

/**
    @brief �����, �������������� ��������� ��������� ���������, �������������
	� �������� ���� ����������, � ������������ �� ���������, ���������� ��
	��������� ������� �����������������, ��� �������, ��� ���������� ��������
	��������� � ���������� ��������.
    @param newValue - ���������� �������� ���������, ���������� �� ���������.
**/
void CustomMainWindow::getHumidityEdited(const QString& newValue) {
	if (validateHumidity(newValue)) {
		ui->humidityLabel->setText(newValue + "%");
	}
	if (newValue.isEmpty()) {
		ui->humidityLabel->setText("0%");
	}
}

/**
    @brief �����, �������������� ��������� ��������� ��������, �������������
	� �������� ���� ����������, � ������������ �� ���������, ���������� ��
	��������� ������� �����������������, ��� �������, ��� ���������� ��������
	��������� � ���������� ��������.
    @param newValue - ���������� �������� ��������, ���������� �� ���������.
**/
void CustomMainWindow::getPressureEdited(const QString& newValue) {
	ui->pressureLabel->setText(newValue);
	if (newValue.isEmpty()) {
		ui->pressureLabel->setText("0");
	}
}

/**
    @brief �����, �������������� ��������� �������� � ���������� ������
	������� �����������������, ��������� �� ���������.
**/
void CustomMainWindow::getInnerBlockError() {
	ui->innerBlock->setStyleSheet(innerBlockErrorStyle);
}

/**
    @brief �����, ����������� ��������� �������� � ���������� ������
	������� �����������������, ��������� �� ���������, ��� �������������
	�� �� ����������.
**/
void CustomMainWindow::getInnerBlockOk() {
	ui->innerBlock->setStyleSheet(innerBlockOkStyle);
}

/**
    @brief �����, �������������� ��������� �������� � ������� ������
	������� �����������������, ��������� �� ���������.
**/
void CustomMainWindow::getOuterBlockError() {
	ui->outerBlock->setStyleSheet(outerBlockErrorStyle);
}

/**
    @brief �����, ����������� ��������� �������� � ������� ������
	������� �����������������, ��������� �� ���������, ��� �������������
	�� �� ����������.
**/
void CustomMainWindow::getOuterBlockOk() {
	ui->outerBlock->setStyleSheet(outerBlockOkStyle);
}

/**
    @brief ��������������� ������� ��������� ������� ��������� ���� ����������,
	��� ����� ����������� ������������� ������������� � ����������� ���������
	������� �����������������.
    @param event - ��������, �������������� �� ������-��������, ����������
	���������� � ������� ��������� ������� ����.
**/
void CustomMainWindow::resizeEvent(QResizeEvent* event) {
	if (event->oldSize().width() != -1) {
		int widthDiff = event->size().width() - event->oldSize().width();
		int heightDiff = event->size().height() - event->oldSize().height();
		if (widthDiff % 2 != 0) {
			if (widthDiff > 0) {
				widthDiff--;
				resizeBuffer.setWidth(resizeBuffer.width() + 1);
			}
			else {
				widthDiff++;
				resizeBuffer.setWidth(resizeBuffer.width() - 1);
			}
		}
		if (heightDiff % 2 != 0) {
			if (heightDiff > 0) {
				heightDiff--;
				resizeBuffer.setHeight(resizeBuffer.height() + 1);
			}
			else {
				heightDiff++;
				resizeBuffer.setHeight(resizeBuffer.height() - 1);
			}
		}
		if (resizeBuffer.width() != 0 && resizeBuffer.width() % 2 == 0) {
			widthDiff += resizeBuffer.width();
			resizeBuffer.setWidth(0);
		}
		if (resizeBuffer.height() != 0 && resizeBuffer.height() % 2 == 0) {
			heightDiff += resizeBuffer.height();
			resizeBuffer.setHeight(0);
		}
		QObjectList elements = ui->centralWidget->children();
		QObjectList::const_iterator it = elements.begin();
		QObjectList::const_iterator eIt = elements.end();
		while (it != eIt - 2) {
			QWidget* elem = (QWidget*)(*it++);
			int newX = elem->x() + widthDiff / 2;
			int newY = elem->y() + heightDiff / 2;
			elem->move(newX, newY);
		}
	}
	QMainWindow::resizeEvent(event);
}
