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
    @brief Конструктор класса основного окна приложения.
    @param parent - Параметр для обозначения иерархии виджетов, унаследованный
	от класса QWidget, по умолчанию равен nullptr.
**/
CustomMainWindow::CustomMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::CustomMainWindowClass()) {
	ui->setupUi(this);
	initializeStyles();
	initializeButtons();
	loadSettings();
	resizeBuffer = QSize(0, 0);
}

/**
    @brief Деструктор класса основного окна приложения.
**/
CustomMainWindow::~CustomMainWindow() {
	delete modeGroup;
	delete temperatureGroup;
	delete pressureGroup;
	delete directionGroup;
	delete ui;
}

/**
    @brief Метод, отвечающий за инициализацию стилей отображения окна.
	Вызывается в конструкторе класса.
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
    @brief  Метод, обеспечивающий доступ к светлому стилю отображения окна.
    @retval  - Строка, содержащая светлый стиль окна в формате QSS.
**/
QString& CustomMainWindow::getLightStyle() {
	return this->lightStyle;
}

/**
    @brief  Метод, обеспечивающий доступ к темному стилю отображения окна.
    @retval  - Строка, содержащая темный стиль окна в формате QSS.
**/
QString& CustomMainWindow::getDarkStyle() {
	return this->darkStyle;
}

/**
    @brief Метод, отвечающий за инициализацию управляющих элементов класса.
	Вызывается в конструкторе класса.
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
    @brief Метод, отвечающий за переключение отображения окна,
	соответствующего светлому стилю.
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
    @brief Метод, отвечающий за переключение отображения окна,
	соответствующего темному стилю.
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
    @brief Метод, отвечающий за включение и отключение системы
	кондиционирования. При включении обеспечивает доступ к
	основным элементам управления системой и запускает
	окно ее имитатора. Соответственно, при выключении
	запрещает доступ к элементам управления и закрывает окно
	имитатора.
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
    @brief Метод, обеспечивающий сохранение текущих параметров
	системы кондиционирования в файле настроек.
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
    @brief Метод, обеспечивающий загрузку последних сохраненных
	параметров системы кондиционирования из файла настроек.
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
    @brief Метод, устанавливающий по умолчанию параметры основного
	окна приложения.
**/
void CustomMainWindow::setDefaultSettings() {
	turnOnLightMode();
	ui->celsiusButton->setChecked(true);
	ui->mmHgButton->setChecked(true);
	ui->centerDirButton->setChecked(true);
}

/**
    @brief Метод, повышающий параметр температуры системы кондиционирования
	на один пункт.
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
    @brief Метод, понижающий параметр температуры системы кондиционирования
	на один пункт.
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
    @brief  Метод, проверяющий нахождение измененного параметра температуры
	в допустимых пределах.
    @param  newValue - Измененный параметр температуры.
    @retval          - Возвращает true, если значение измененного параметра
	находится в допустимых пределах, иначе возвращает false.
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
    @brief Метод, обеспечивающий пересчет текущего значения
	температуры из шкалы Цельсия, так чтобы оно соответствовало
	режиму, выбранному пользователем.
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
    @brief Метод, обеспечивающий пересчет текущего значения
	температуры из шкалы Фаренгейта, так чтобы оно соответствовало
	режиму, выбранному пользователем.
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
    @brief Метод, обеспечивающий пересчет текущего значения
	температуры из шкалы Кельвина, так чтобы оно соответствовало
	режиму, выбранному пользователем.
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
    @brief  Метод, проверяющий нахождение измененного параметра влажности
	в допустимых пределах.
    @param  newValue - Измененный параметр влажности.
    @retval          - Возвращает true, если значение измененного параметра
	находится в допустимых пределах, иначе возвращает false.
**/
bool CustomMainWindow::validateHumidity(const QString& newValue) {
	return newValue.toInt() >= HUMIDITY_MIN && newValue.toInt() <= HUMIDITY_MAX;
}

/**
    @brief Метод, обеспечивающий пересчет текущего значения
	давления из единицы измерения мм рт. ст., так чтобы оно соответствовало
	режиму, выбранному пользователем.
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
    @brief Метод, обеспечивающий пересчет текущего значения
	давления из единицы измерения Па, так чтобы оно соответствовало
	режиму, выбранному пользователем.
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
    @brief Метод, обеспечивающий изменение направления выдува
	системы кондиционирования в левую сторону.
**/
void CustomMainWindow::changeDirectionToLeft() {
	if (ui->leftDirButton->isChecked()) {
		QString newDirection = "left";
		emit directionChanged(newDirection);
	}
}

/**
    @brief Метод, обеспечивающий центрирование направления выдува
	системы кондиционирования.
**/
void CustomMainWindow::changeDirectionToCenter() {
	if (ui->centerDirButton->isChecked()) {
		QString newDirection = "center";
		emit directionChanged(newDirection);
	}
}

/**
    @brief Метод, обеспечивающий изменение направления выдува
	системы кондиционирования в правую сторону.
**/
void CustomMainWindow::changeDirectionToRight() {
	if (ui->rightDirButton->isChecked()) {
		QString newDirection = "right";
		emit directionChanged(newDirection);
	}
}

/**
    @brief Переопределение события закрытия основного окна приложения,
	так чтобы перед этим происходило выключение системы кондиционирования.
    @param event - Параметр, унаследованный от класса-родителя, отвечающий
	за закрытие окна.
**/
void CustomMainWindow::closeEvent(QCloseEvent* event) {
	saveSettings();
	if (power) {
		switchPower();
	}
	event->accept();
}

/**
    @brief Метод, обеспечивающий изменение параметра температуры, отображаемого
	в основном окне приложения, в соответствии со значением, полученным от
	имитатора системы кондиционирования, при условии, что полученное значение
	находится в допустимых пределах.
    @param newValue - Измененный параметр температуры, полученный от имитатора.
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
    @brief Метод, обеспечивающий изменение параметра влажности, отображаемого
	в основном окне приложения, в соответствии со значением, полученным от
	имитатора системы кондиционирования, при условии, что полученное значение
	находится в допустимых пределах.
    @param newValue - Измененный параметр влажности, полученный от имитатора.
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
    @brief Метод, обеспечивающий изменение параметра давления, отображаемого
	в основном окне приложения, в соответствии со значением, полученным от
	имитатора системы кондиционирования, при условии, что полученное значение
	находится в допустимых пределах.
    @param newValue - Измененный параметр давления, полученный от имитатора.
**/
void CustomMainWindow::getPressureEdited(const QString& newValue) {
	ui->pressureLabel->setText(newValue);
	if (newValue.isEmpty()) {
		ui->pressureLabel->setText("0");
	}
}

/**
    @brief Метод, обеспечивающий индикацию проблемы с внутренним блоком
	системы кондиционирования, исходящей от имитатора.
**/
void CustomMainWindow::getInnerBlockError() {
	ui->innerBlock->setStyleSheet(innerBlockErrorStyle);
}

/**
    @brief Метод, отключающий индикацию проблемы с внутренним блоком
	системы кондиционирования, исходящей от имитатора, что сигнализирует
	об ее устранении.
**/
void CustomMainWindow::getInnerBlockOk() {
	ui->innerBlock->setStyleSheet(innerBlockOkStyle);
}

/**
    @brief Метод, обеспечивающий индикацию проблемы с внешним блоком
	системы кондиционирования, исходящей от имитатора.
**/
void CustomMainWindow::getOuterBlockError() {
	ui->outerBlock->setStyleSheet(outerBlockErrorStyle);
}

/**
    @brief Метод, отключающий индикацию проблемы с внешним блоком
	системы кондиционирования, исходящей от имитатора, что сигнализирует
	об ее устранении.
**/
void CustomMainWindow::getOuterBlockOk() {
	ui->outerBlock->setStyleSheet(outerBlockOkStyle);
}

/**
    @brief Переопределение события изменения размера основного окна приложения,
	так чтобы происходило центрирование информирующих и управляющих элементов
	системы кондиционирования.
    @param event - Параметр, унаследованный от класса-родителя, содержащий
	информацию о событии изменения размера окна.
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
