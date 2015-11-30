/** @file calculator.h
 *
 *  @brief This file contains the declarations of the classes
 *
 *  Copyright (C) 2009, Romit Chatterjee
 *
 *  This file is part of QCalc.
 *
 *  QCalc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CALCULATOR_H
#define CALCULATOR_H

/** Enable or disabled debugging */
#define DEBUG   0

/** Enable or disable hex input */
#define HEX     0

/* Includes */
#include <QtGui/QWidget>
#include <QString>

/* Forward declarations */
class QLCDNumber;
class QGridLayout;
class QButtonGroup;
class QVBoxLayout;
#if DEBUG
class QLabel;
#endif
class Control;

/* Defines */

/** Number of rows of buttons */
#define BUTTONS_ROW     6
/** Number of columns of buttons */
#define BUTTONS_COL     5
/** Total number of buttons except hex buttons */
#define NUM_BUTTONS     30
#if HEX
/** Total number of hex buttons */
#define NUM_HEX_BUTTONS 6
#endif
/** Number of digits supported in LCD */
#define LCD_LENGTH      20

/** Operator : None */
#define OPERATOR_NONE   0
/** Operator : '+' */
#define OPERATOR_PLUS   1
/** Operator : '-' */
#define OPERATOR_MINUS  2
/** Operator : '*' */
#define OPERATOR_MUL    3
/** Operator : '/' */
#define OPERATOR_DIV    4
/** Operator : 'sqrt' */
#define OPERATOR_SQRT   5
/** Operator : '!' */
#define OPERATOR_FACT   6

/** Last button clicked: Init */
#define TYPE_INIT       0
/** Last button clicked: Number */
#define TYPE_NUM        1
/** Last button clicked: Operator */
#define TYPE_OP         2
/** Last button clicked: Equal-to */
#define TYPE_EQ         3
/** Last button clicked: Dot */
#define TYPE_DOT        4
/** Last button clicked: Others */
#define TYPE_OTHER      5

/** Button names */
const QString buttonLabels[NUM_BUTTONS] = {
        "7",    "8",   "9",   "/",   "C",
        "4",    "5",   "6",   "*",   "Sq",
        "1",    "2",   "3",   "-",   "1/x",
        "0",    "+/-", ".",   "+",   "=",
        "MC",   "MR",  "MS",  "M+",  "Bksp",
        "Sqrt", "!x",  "x^3", "Bin", "Hex" };

#if HEX
/** Hex button names */
const QString hexButtonLabels[NUM_HEX_BUTTONS] = {
        "A", "B", "C", "D", "E", "F" };
#endif

/** Button shortcuts */
const QString buttonShotcuts[NUM_BUTTONS] = {
        "7",    "8",   "9",  "/",  "esc",
        "4",    "5",   "6",  "*",  "",
        "1",    "2",   "3",  "-",  "",
        "0",    "", ".",  "+",  "=",
        "",   "",  "", "", "backspace",
        "", "",  "" };

#if HEX
/* Hex button shortcuts */
const QString hexButtonShortcuts[NUM_HEX_BUTTONS] = {
        "a", "b", "c", "d", "e", "f" };
#endif

/** Button : '7' */
#define BUTTON_7    0
/** Button : '8' */
#define BUTTON_8    1
/** Button : '9' */
#define BUTTON_9    2
/** Button : '/' */
#define BUTTON_DIV  3
/** Button : 'C' */
#define BUTTON_CLR  4
/** Button : '4' */
#define BUTTON_4    5
/** Button : '5' */
#define BUTTON_5    6
/** Button : '6' */
#define BUTTON_6    7
/** Button : '*' */
#define BUTTON_MUL  8
/** Button : 'Sqrt' */
#define BUTTON_SQ   9
/** Button : '1' */
#define BUTTON_1    10
/** Button : '2' */
#define BUTTON_2    11
/** Button : '3' */
#define BUTTON_3    12
/** Button : '4' */
#define BUTTON_NEG  13
/** Button : '1/x' */
#define BUTTON_INV  14
/** Button : '0' */
#define BUTTON_0    15
/** Button : '+/-' */
#define BUTTON_SIGN 16
/** Button : '.' */
#define BUTTON_DOT  17
/** Button : '+' */
#define BUTTON_PLUS 18
/** Button : '=' */
#define BUTTON_EQ   19
/** Button : 'MC' */
#define BUTTON_MC   20
/** Button : 'MR' */
#define BUTTON_MR   21
/** Button : 'MS' */
#define BUTTON_MS   22
/** Button : 'MP' */
#define BUTTON_MP   23
/** Button : 'Bksp' */
#define BUTTON_BS   24
/** Button : 'Sqrt' */
#define BUTTON_SQRT 25
/** Button : '!x' */
#define BUTTON_FACT 26
/** Button : 'x^3' */
#define BUTTON_CUBE 27
/** Button : 'Bin' */
#define BUTTON_BIN  28
/** Button : 'Hex' */
#define BUTTON_HEX  29

#if HEX
/** Hex button : 'A' */
#define HEX_BUTTON_A    0
/** Hex button : 'B' */
#define HEX_BUTTON_B    1
/** Hex button : 'C' */
#define HEX_BUTTON_C    2
/** Hex button : 'D' */
#define HEX_BUTTON_D    3
/** Hex button : 'E' */
#define HEX_BUTTON_E    4
/** Hex button : 'F' */
#define HEX_BUTTON_F    5
#endif

/** Mode status : Decimal */
#define MODE_DEC    1
/** Mode status : Binary */
#define MODE_BIN    2
/** Mode status : Hexadecimal */
#define MODE_HEX    3

/** Our main object */
class Calculator : public QWidget
{
    Q_OBJECT

public:
    /** Constructor */
    Calculator(QWidget *parent = 0);
    /** Destructor */
    ~Calculator();

public slots:
    /** Handle button change */
    void buttonChanged(int button, QString text, int mode);

private:
    /** Control unit */
    class Control *control;
    /** LCD Number */
    QLCDNumber *lcd;
#if DEBUG
    /** Label */
    QLabel *label;
#endif
    /** Button group */
    QButtonGroup *buttonGroup;
    /** Button layout */
    QGridLayout *buttonLayout;
#if HEX
    /** Hex button group */
    QButtonGroup *hexButtonGroup;
    /** Hex button layout */
    QGridLayout *hexButtonLayout;
#endif
    /** Main layout */
    QVBoxLayout *mainLayout;
};

/** Our controller unit object */
class Control : public QWidget
{
    Q_OBJECT

public:
    /** Constructor */
    Control(QWidget *parent = 0);
    /** Destructor */
    ~Control();
    /** Get the current set text */
    QString getText(void);
    /** Set the current text */
    void setText(QString);
    /** Get the text set in memory */
    QString getMemoryText(void);
    /** Set the text in memory */
    void setMemoryText(QString);
    /** Get the operand text */
    QString getOperand1(void);
    /** Set the operand text */
    void setOperand1(QString);
    /** Get the last set operator */
    int getOperator(void);
    /** Set the operator */
    void setOperator(int);
    /** Get the decimal status */
    bool getDecimalStatus(void);
    /** Set the decimal status */
    void setDecimalStatus(bool);
    /** Get the negative sign status */
    bool getNegativeStatus(void);
    /** Set the negative sign status */
    void setNegativeStatus(bool);
    /** Get the last clicked button type */
    int getLastClicked(void);
    /** Set the last clicked button type */
    void setLastClicked(int);
    /** Get 'Bin' button status */
    int getBinButtonStatus(void);
    /** Set 'Bin' button status */
    void setBinButtonStatus(int, int);
    /** Get 'Hex' button status */
    int getHexButtonStatus(void);
    /** Set 'Hex' button status */
    void setHexButtonStatus(int, int);
    /** Get number of digits in LCD */
    int getNumDigits(void);
    /** Save number of digits in LCD */
    void setNumDigits(int);
    /** Update LCD */
    void updateLCD(void);
    /** Make calculation */
    QString calculate(QString, QString, int);

public slots:
    /** Capture button press */
    void buttonPressed(int index);
#if HEX
    /** Capture hex button press */
    void hexButtonPressed(int index);
#endif

signals:
    /** Signal LCD change */
    void setLCD(QString text);
    /** Signal button name change */
    void setButton(int button, QString text, int oldStatus);

private:
    /** LCD text */
    QString lcdText;
    /** Operand */
    QString operand1;
    /** Memory text */
    QString memoryText;
    /** Last operator */
    int lastOperator;
    /** Last clicked button type */
    int lastClicked;
    /** Decimal status */
    bool isDecimalEnabled;
    /** Negative sign status */
    bool isNegativeEnabled;
    /** 'Bin' button status */
    int binButtonStatus;
    /** 'Hex' button status */
    int hexButtonStatus;
    /** Number of digits in LCD */
    int numLCDDigits;
    /** Show error function */
    void showError(void);
};

#endif // CALCULATOR_H
