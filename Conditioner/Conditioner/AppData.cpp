#include "AppData.h"

extern const QString appTitle{ "Conditioner" };
extern const QString appIcon{ "./icons/conditioner.png" };
extern const QString loadSettingsWarningMessage{
	u8"Ошибка при попытке загрузки предыдущих настроек.\n"
	"Установлены натройки по умолчанию"
};

extern const QString settingsPath{ "./settings.xml" };

extern const QString lightStyleSheetPath{ "./LightStyleSheet.css" };
extern const QString darkStyleSheetPath{ "./DarkStyleSheet.css" };

extern const QString sunLightIcon{ "./icons/big_light_Sun.png" };
extern const QString moonLightIcon{ "./icons/big_light_Moon.png" };
extern const QString innerBlockLightIcon{ "./icons/big_light_InnerBlockModel.png" };
extern const QString outerBlockLightIcon{ "./icons/big_light_OuterBlockModel.png" };
extern const QString powerIcon{ "./icons/big_Power.png" };
extern const QString increaseLightIcon{ "./icons/big_light_ChevronUp.png" };
extern const QString decreaseLightIcon{ "./icons/big_light_ChevronDown.png" };
extern const QString dropLightIcon{ "./icons/big_light_Drop.png" };
extern const QString leftDirLightIcon{ "./icons/big_light_LeftWind.png" };
extern const QString centerDirLightIcon{ "./icons/big_light_CenterWind.png" };
extern const QString rightDirLightIcon{ "./icons/big_light_RightWind.png" };

extern const QString sunDarkIcon{ "./icons/big_light_Sun.png" };
extern const QString innerBlockDarkIcon{ "./icons/big_dark_InnerBlockModel.png" };
extern const QString outerBlockDarkIcon{ "./icons/big_dark_OuterBlockModel.png" };
extern const QString increaseDarkIcon{ "./icons/big_dark_ChevronUp.png" };
extern const QString decreaseDarkIcon{ "./icons/big_dark_ChevronDown.png" };
extern const QString dropDarkIcon{ "./icons/big_dark_Drop.png" };
extern const QString leftDirDarkIcon{ "./icons/big_dark_LeftWind.png" };
extern const QString centerDirDarkIcon{ "./icons/big_dark_CenterWind.png" };
extern const QString rightDirDarkIcon{ "./icons/big_dark_RightWind.png" };

extern const int CELSIUS_MIN{ -10 };
extern const int CELSIUS_MAX{ 42 };
extern const double FAHRENHEIT_MIN{ 14.0 };
extern const double FAHRENHEIT_MAX{ 107.6 };
extern const double KELVIN_MIN{ 263.15 };
extern const double KELVIN_MAX{ 315.15 };
extern const int HUMIDITY_MIN{ 0 };
extern const int HUMIDITY_MAX{ 100 };

extern const QString innerBlockErrorStyle{ "QLabel#innerBlock { background-color: #B85757; }" };
extern const QString innerBlockOkStyle{ "QLabel#innerBlock { background-color: transparent; }" };
extern const QString outerBlockErrorStyle{ "QLabel#outerBlock { background-color: #B85757; }" };
extern const QString outerBlockOkStyle{ "QLabel#outerBlock { background-color: transparent; }" };
