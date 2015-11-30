/** @file calculator.cpp
 *
 *  @brief This file contains the definitions of the classes
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

/* Includes */
#include "calculator.h"
#include <QtGui/QLCDNumber>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QButtonGroup>
#include <QtGui/QLabel>
#if DEBUG
#include <QtGui/QMessageBox>
#endif

#include <math.h>

#if DEBUG
void alert(QString text)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}
#endif

/**
 *  @brief  Main object constructor
 *
 *  @param  parent  pointer to parent widget
 *
 *  @return N/A
 */
Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    /* Initilize the components */

    lcd = new QLCDNumber(LCD_LENGTH + 1);
    buttonLayout = new QGridLayout;
    buttonGroup = new QButtonGroup;
#if HEX
    hexButtonLayout = new QGridLayout;
    hexButtonGroup = new QButtonGroup;
#endif
    control = new Control;
    mainLayout = new QVBoxLayout;
#if DEBUG
    label = new QLabel;
#endif

    /* Configure the LCD */
    lcd->setMode(QLCDNumber::Dec);
    lcd->setSegmentStyle(QLCDNumber::Filled);
    lcd->setSmallDecimalPoint(true);
    lcd->setFixedHeight(50);
    lcd->setStyleSheet("border-color: black; color: white; background-color: rgb(90, 90, 150)");

    /* Create the buttons, add them to button group and button layout */
    for (int row = 0, index = 0; row < BUTTONS_ROW; row++) {
        for (int col = 0; col < BUTTONS_COL; col++, index++) {
            if (index < NUM_BUTTONS) {
                QPushButton *button = new QPushButton(buttonLabels[index]);
                button->setStyleSheet("color: black; background-color: rgb(215, 215, 215)");
                button->setShortcut(buttonShotcuts[index]);
                buttonGroup->addButton(button, index);
                buttonLayout->addWidget(button, row, col);
            }
        }
    }

#if HEX
    /* Create the hex buttons, add them to button group and button layout */
    for (int index = 0; index < NUM_HEX_BUTTONS; index++) {
        QPushButton *button = new QPushButton(hexButtonLabels[index]);
        button->setStyleSheet("color: black; background-color: rgb(215, 215, 215)");
        button->setShortcut(hexButtonShortcuts[index]);
        hexButtonGroup->addButton(button, index);
        hexButtonLayout->addWidget(button, 1, index);
    }
#endif

    /* Configure the controller object to init status */
    control->setDecimalStatus(false);
    control->setNegativeStatus(false);
    control->setText("0");
    control->setOperator(OPERATOR_NONE);
    control->setOperand1("0");
    control->setLastClicked(TYPE_INIT);
    control->setMemoryText("0");
    control->setBinButtonStatus(MODE_BIN, MODE_DEC);
    control->setHexButtonStatus(MODE_HEX, MODE_DEC);
    lcd->setMode(QLCDNumber::Dec);
    control->setFocus();
#if HEX
    hexButtonGroup->blockSignals(true);
    hexButtonLayout->setEnabled(false);
#endif

    /* Connect buttons with controller */
    connect(buttonGroup, SIGNAL(buttonClicked(int)), control, SLOT(buttonPressed(int)));
#if HEX
    /* Connect hex buttons with controller */
    connect(hexButtonGroup, SIGNAL(buttonClicked(int)), control, SLOT(hexButtonPressed(int)));
#endif
    /* Connect controller with LCD */
    connect(control, SIGNAL(setLCD(QString)), lcd, SLOT(display(QString)));
    /* Connect controller with main */
    connect(control, SIGNAL(setButton(int, QString, int)), this, SLOT(buttonChanged(int, QString, int)));
#if DEBUG
    /* Connect controller with debug label */
    connect(control, SIGNAL(setLCD(QString)), label, SLOT(setText(QString)));
#endif
    /* Add the components to the main layout */
    mainLayout->addWidget(lcd);
#if DEBUG
    mainLayout->addWidget(label);
#endif
    mainLayout->addLayout(buttonLayout, 1);
#if HEX
    mainLayout->addLayout(hexButtonLayout, 1);
#endif
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    /* Set the layout to main layout */
    setLayout(mainLayout);
}

/**
 *  @brief  Main object destructor
 *
 *  @return N/A
 */
Calculator::~Calculator()
{
    /* Release the keyboard */
    control->releaseKeyboard();

    /* Free the allocated components */
    delete lcd;
    delete buttonLayout;
    delete buttonGroup;
#if HEX
    delete hexButtonLayout;
    delete hexButtonGroup;
#endif
    delete control;
    delete mainLayout;
#if DEBUG
    delete label;
#endif
    return;
}

/**
 *  @brief  Controller object slot : Handle button change
 *
 *  @param  button  Index of button to change
 *  @param  text    New button text
 *  @param  mode    Mode to set
 *
 *  @return N/A
 */
void Calculator::buttonChanged(int buttonIdx, QString text, int mode)
{
    /* Get the button */
    QAbstractButton *tempButton = buttonGroup->button(buttonIdx);

    /* Change the text */
    tempButton->setText(text);

    /* Update the LCD mode accordingly */
    if (mode == MODE_DEC) {
        lcd->setMode(QLCDNumber::Dec);
#if HEX
        hexButtonGroup->blockSignals(true);
#endif
    } else if (mode == MODE_HEX) {
        lcd->setMode(QLCDNumber::Hex);
#if HEX
        hexButtonGroup->blockSignals(false);
#endif
    } else if (mode == MODE_BIN) {
        lcd->setMode(QLCDNumber::Bin);
#if HEX
        hexButtonGroup->blockSignals(true);
#endif
    }

#if HEX
    /* Enable/disable the Hex buttons */
    if (mode == MODE_HEX) {
        hexButtonLayout->setEnabled(true);
    } else {
        hexButtonLayout->setEnabled(false);
    }
#endif
    return;
}

/**
 *  @brief  Controller object constructor
 *
 *  @param  parent  pointer to parent widget
 *
 *  @return N/A
 */
Control::Control(QWidget *parent)
    : QWidget(parent)
{
    return;
}

/**
 *  @brief  Controller object destructor
 *
 *  @return N/A
 */
Control::~Control()
{
    return;
}

/**
 *  @brief  Controller object method : Get the current set text
 *
 *  @return Current set text
 */
QString Control::getText(void)
{
    /* Return set text */
    return lcdText;
}

/**
 *  @brief  Controller object method : Set the current text
 *
 *  @param  newText     The text to set
 *
 *  @return N/A
 */
void Control::setText(QString newText)
{
    /* Set text */
    lcdText = newText;
    return;
}

/**
 *  @brief  Controller object method : Get the text set in memory
 *
 *  @return Current set text in memory
 */
QString Control::getMemoryText(void)
{
    /* Return memory text */
    return memoryText;
}

/**
 *  @brief  Controller object method : Set the text in memory
 *
 *  @param  newText     The text to set in memory
 *
 *  @return N/A
 */
void Control::setMemoryText(QString newText)
{
    /* Set memory text */
    memoryText = newText;
    return;
}

/**
 *  @brief  Controller object method : Get the operand text
 *
 *  @return Current operand text
 */
QString Control::getOperand1(void)
{
    /* Return operand text */
    return operand1;
}

/**
 *  @brief  Controller object method : Set the operand text
 *
 *  @param  operand     The text to set in operand
 *
 *  @return N/A
 */
void Control::setOperand1(QString operand)
{
    /* Set operand text */
    operand1 = operand;
    return;
}

/**
 *  @brief  Controller object method : Get the last set operator
 *
 *  @return Current set operator
 */
int Control::getOperator(void)
{
    /* Return set operator */
    return lastOperator;
}

/**
 *  @brief  Controller object method : Set the operator
 *
 *  @param  op  The operator to set
 *
 *  @return N/A
 */
void Control::setOperator(int op)
{
    /* Set operator */
    lastOperator = op;
    return;
}

/**
 *  @brief  Controller object method : Get the decimal status
 *
 *  @return Current decimal status
 */
bool Control::getDecimalStatus(void)
{
    /* Return decimal status */
    return isDecimalEnabled;
}

/**
 *  @brief  Controller object method : Set the decimal status
 *
 *  @param  status  The decimal status to set
 *
 *  @return N/A
 */
void Control::setDecimalStatus(bool status)
{
    /* Set decimal status */
    isDecimalEnabled = status;
    return;
}

/**
 *  @brief  Controller object method : Get the negative sign status
 *
 *  @return Current negative sign status
 */
bool Control::getNegativeStatus(void)
{
    /* Return negative sign status */
    return isNegativeEnabled;
}

/**
 *  @brief  Controller object method : Set the negative sign status
 *
 *  @param  status  The negative sign status to set
 *
 *  @return N/A
 */
void Control::setNegativeStatus(bool status)
{
    /* Set negative sign status */
    isNegativeEnabled = status;
    return;
}

/**
 *  @brief  Controller object method : Get the last clicked button type
 *
 *  @return Last clicked button type
 */
int Control::getLastClicked(void)
{
    /* Return last clicked button type */
    return lastClicked;
}

/**
 *  @brief  Controller object method : Set the last clicked button type
 *
 *  @param  lc      Last clicked button type to set
 *
 *  @return N/A
 */
void Control::setLastClicked(int lc)
{
    /* Set last clicked button type */
    lastClicked = lc;
    return;
}

/**
 *  @brief  Controller object method : Get 'Bin' button status
 *
 *  @return 'Bin' button status
 */
int Control::getBinButtonStatus(void)
{
    /* Return 'Bin' button status */
    return binButtonStatus;
}

/**
 *  @brief  Controller object method : Set 'Bin' button status
 *
 *  @param  status      'Bin' button status to set
 *  @param  oldStatus   Old status to override
 *
 *  @return N/A
 */
void Control::setBinButtonStatus(int status, int oldStatus)
{
    /* Set 'Bin' button status */
    binButtonStatus = status;

    if (status == MODE_BIN) { emit setButton(BUTTON_BIN, "Bin", oldStatus); }
    if (status == MODE_HEX) { emit setButton(BUTTON_BIN, "Hex", oldStatus); }
    if (status == MODE_DEC) { emit setButton(BUTTON_BIN, "Dec", oldStatus); }
    return;
}

/**
 *  @brief  Controller object method : Get 'Hex' button status
 *
 *  @return 'Hex' button status
 */
int Control::getHexButtonStatus(void)
{
    /* Return 'Hex' button status */
    return hexButtonStatus;
}

/**
 *  @brief  Controller object method : Set 'Hex' button status
 *
 *  @param  status      'Hex' button status to set
 *  @param  oldStatus   Old status to override
 *
 *  @return N/A
 */
void Control::setHexButtonStatus(int status, int oldStatus)
{
    /* Set 'Hex' button status */
    hexButtonStatus = status;

    if (status == MODE_BIN) { emit setButton(BUTTON_HEX, "Bin", oldStatus); }
    if (status == MODE_HEX) { emit setButton(BUTTON_HEX, "Hex", oldStatus); }
    if (status == MODE_DEC) { emit setButton(BUTTON_HEX, "Dec", oldStatus); }
    return;
}

/**
 *  @brief  Controller object method : Get number of digits in LCD
 *
 *  @return Number of digits shown in LCD
 */
int Control::getNumDigits(void)
{
    /* Return number of digits in LCD */
    return numLCDDigits;
}

/**
 *  @brief  Controller object method : Save number of digits in LCD
 *
 *  @param  num     Number of digits to save
 *
 *  @return N/A
 */
void Control::setNumDigits(int num)
{
    /* Save number of digits in LCD */
    numLCDDigits = num;
    return;
}

/**
 *  @brief  Controller object method :  Update LCD
 *
 *  @return N/A
 */
void Control::updateLCD(void)
{
    /* Get the current set text */
    QString text = getText();

    /* Disable dot button action if already present */
    setDecimalStatus(text.contains("."));
    /* Disable sign button action if already present */
    setNegativeStatus(text.contains("-"));

    /* Signal the LCD component to show the text */
    emit setLCD(text);

    /* Save the number of digits shown */
    setNumDigits(text.length());

    return;
}

/**
 *  @brief  Controller object method :  Make calculation
 *
 *  @param  opString1   Operand 1 as string
 *  @param  opString2   Operand 2 as string
 *  @param  op          Operator
 *
 *  @return Calculated result as string
 */
QString Control::calculate(QString opString1, QString opString2, int op)
{
    double op1 = 0, op2 = 0, result = 0;
    QString ret = "0";

    /* Check if operand 1 exists, 0 value is allowed */
    if (opString1.isEmpty()) {
        return ret;
    }

    /* Check if operand 2 exists, 0 value is allowed */
    if (opString2.isEmpty()) {
        return ret;
    }

    /* Convert strings to doubles */
    op1 = opString1.toDouble();
    op2 = opString2.toDouble();

    /* Perform the calculation */
    switch (op) {
        case OPERATOR_PLUS:
            /* Addition */
            result = op1 + op2;
            ret.setNum(result);
            break;
        case OPERATOR_MINUS:
            /* Substraction */
            result = op1 - op2;
            ret.setNum(result);
            break;
        case OPERATOR_MUL:
            /* Multiplication */
            result = op1 * op2;
            ret.setNum(result);
            break;
        case OPERATOR_DIV:
            /* Division */
            if (op2 == 0) {
                /* Check for divide-by-zero error */
                showError();
                break;
            }
            result = op1 / op2;
            ret.setNum(result);
            break;
        case OPERATOR_SQRT:
            /* Square root */
            result = sqrt(op1);
            ret.setNum(result);
            break;
        case OPERATOR_FACT:
            /* Factorial */
            if (op1 == 0) {
                op1 = 1;
            } else if (op1 < 0) {
                op1 = -op1;
            } else {
                op1 = round(op1);
            }
            result = 1;
            for (int i = 1; i <= op1; i++) {
                result = result * i;
            }
            ret.setNum(result);
            break;
        default:
            break;
    }
    return ret;
}

/**
 *  @brief  Controller object method :  Show error function
 *
 *  @return N/A
 */
void Control::showError(void)
{
    /* Show error */
    setText("-- error --");
    updateLCD();

    /* Reset the components */
    setDecimalStatus(false);
    setNegativeStatus(false);
    setText("0");
    setOperator(OPERATOR_NONE);
    setOperand1("0");
    setLastClicked(TYPE_INIT);
}

/**
 *  @brief  Controller object slot :  Capture button press
 *
 *  @param  index   Index of button pressed
 *
 *  @return N/A
 */
void Control::buttonPressed(int index)
{
    /* Get the current set text */
    QString text = getText();
    /* Allocate a temporary text buffer */
    QString tempText;

    /* Initialize in use operators */
    int op = OPERATOR_NONE, newOp = OPERATOR_NONE;

    /* Get the last clicked button type */
    int lc = getLastClicked();

    /** Allocate operand text buffer */
    QString operand1;

    /* Actual working logic */
    switch(index) {
        case BUTTON_1:  /* Button 1: Fall through */
        case BUTTON_2:  /* Button 2: Fall through */
        case BUTTON_3:  /* Button 3: Fall through */
        case BUTTON_4:  /* Button 4: Fall through */
        case BUTTON_5:  /* Button 5: Fall through */
        case BUTTON_6:  /* Button 6: Fall through */
        case BUTTON_7:  /* Button 7: Fall through */
        case BUTTON_8:  /* Button 8: Fall through */
        case BUTTON_9:  /* Button 9: Fall through */
        case BUTTON_0:  /* Button 0 */
            if (text.toDouble() == 0) {
            /* Value is zero */
                if ((lc == TYPE_OP) || (lc == TYPE_EQ) || (lc == TYPE_INIT) || (lc == TYPE_OTHER) || (lc == TYPE_NUM)) {
                    /* Take a new value */
                    text = buttonLabels[index];
                } else if (lc == TYPE_DOT) {
                    /* User pressed '.' before */
                    text = "0.";
                    text.append(buttonLabels[index]);
                }
                /* Update LCD */
                setText(text);
                updateLCD();
            } else if ((lc == TYPE_OP) || (lc == TYPE_EQ)) {
                /* Value is not zero, but need to take a new value */
                text = buttonLabels[index];
                /* Update LCD */
                setText(text);
                updateLCD();
            } else if (text.length() < LCD_LENGTH) {
                /* Value is not zero, append to existing text */
                text.append(buttonLabels[index]);
                /* Update LCD */
                setText(text);
                updateLCD();
            }
            /* Set the last clicked button type to number */
            setLastClicked(TYPE_NUM);
            break;
        case BUTTON_SQ: /* Button square */
            if (text.toDouble() != 0) {
                /* We need to work only is value is non-zero */
                /* Square the current value */
                text = calculate(text, text, OPERATOR_MUL);
                /* Update LCD */
                setText(text);
                updateLCD();
                /* Set the last clicked button type to operator */
                setLastClicked(TYPE_OP);
            }
            break;
        case BUTTON_SQRT:    /* Button sqaure root */
            if (text.toDouble() != 0) {
                /* We need to work only is value is non-zero */
                /* Square root the current value */
                text = calculate(text, text, OPERATOR_SQRT);
                /* Update LCD */
                setText(text);
                updateLCD();
                /* Set the last clicked button type to operator */
                setLastClicked(TYPE_OP);
            }
            break;
        case BUTTON_FACT:    /* Button sqaure root */
            /* Factorial the current value */
            text = calculate(text, text, OPERATOR_FACT);
            /* Update LCD */
            setText(text);
            updateLCD();
            /* Set the last clicked button type to operator */
            setLastClicked(TYPE_OP);
            break;
        case BUTTON_INV:    /* Button inverse */
            if (text.toDouble() == 0) {
                /* Value is zero, this makes divide-by-zero error */
                showError();
            } else {
                /* Value is noon-zero, inverse the current value */
                text = calculate("1", text, OPERATOR_DIV);
                /* Update LCD */
                setText(text);
                updateLCD();
                /* Set the last clicked button type to operator */
                setLastClicked(TYPE_OP);
            }
            break;
        case BUTTON_SIGN:
            if (getNegativeStatus() == false) {
                /* Negative sign not present, need to add it */
                setNegativeStatus(true);
                if (text.length() <= LCD_LENGTH) {
                    /* This does not affect the LCD precision */
                    text.prepend("-");
                }
            } else {
                /* Negative sign is already present, need to remove it */
                setNegativeStatus(false);
                text.remove(0, 1);
            }
            /* Update LCD */
            setText(text);
            updateLCD();
            break;
        case BUTTON_DOT:    /* Button dot */
            if (getDecimalStatus() == false) {
                /* Only do this if a dot is not already shown */
                if ((text.toDouble() == 0) || (lc == TYPE_OP)) {
                    text = "0.";
                } else {
                    text.append(buttonLabels[index]);
                }
                /* Make sure dots are disabled for future */
                setDecimalStatus(true);
                /* Update LCD */
                setText(text);
                updateLCD();
                /* Set the last clicked button type to dot */
                setLastClicked(TYPE_DOT);
            }
            break;
        case BUTTON_PLUS:   /* Button plus : Fall through */
            /* Save the operator */
            if (newOp == OPERATOR_NONE) { newOp = OPERATOR_PLUS; }
        case BUTTON_NEG:    /* Button minus : Fall through */
            /* Save the operator */
            if (newOp == OPERATOR_NONE) { newOp = OPERATOR_MINUS; }
        case BUTTON_MUL:    /* Button multiply : Fall through */
            /* Save the operator */
            if (newOp == OPERATOR_NONE) { newOp = OPERATOR_MUL; }
        case BUTTON_DIV:    /* Button divide : Fall through */
            /* Save the operator */
            if (newOp == OPERATOR_NONE) { newOp = OPERATOR_DIV; }
            if (((text.toDouble()) == 0) || (lc == TYPE_OP)) {
                /* No need to do anything, except updating the operator ... */
                setOperator(newOp);
                /* ... and last button clicked type */
                setLastClicked(TYPE_OP);
                break;
            }

            /* Get the last operator saved */
            op = getOperator();
            /* Get the last operand saved */
            operand1 = getOperand1();

            if ((operand1.toDouble() == 0) || (lc == TYPE_EQ) || (op == OPERATOR_NONE)) {
                /* No last operand, save current value for next calculation */
                setOperand1(text);
            } else {
                /* We already have an operand saved, need to perform calculation */
                operand1 = calculate(operand1, text, op);
                /* Save the result for next calculation */
                setOperand1(operand1);
                /* Update LCD */
                setText(operand1);
                updateLCD();
            }
            /* Save the operator */
            setOperator(newOp);
            /* Set the last clicked button type to operator */
            setLastClicked(TYPE_OP);
            /* Enable decimal */
            setDecimalStatus(false);
            break;
        case BUTTON_EQ: /* Button equal to */
            if ((text.toDouble() == 0) || (lc == TYPE_OP) || (lc == TYPE_EQ)) {
                /* Nothing to do */
                setLastClicked(TYPE_EQ);
                break;
            }

            /* Get the last operator saved */
            op = getOperator();
            /* Get the last operand saved */
            operand1 = getOperand1();

            /* Calculate the result ... */
            if (operand1.toDouble() != 0) {
                /* ... only if we have an operand to operate on */
                operand1 = calculate(operand1, text, op);
            }
            /* Check the result */
            if (operand1.toDouble() != 0) {
                /* Update the operator saved */
                setOperator(newOp);
                /* Update LCD */
                setText(operand1);
                updateLCD();
            }
            /* Set the last clicked button type to equal to */
            setLastClicked(TYPE_EQ);
            /* Enable decimal */
            setDecimalStatus(false);
            break;
        case BUTTON_MC: /* Button memory clear */
            /* Reset the memory string */
            setMemoryText("0");
            break;
        case BUTTON_MR: /* Button memory recall */
            /* Get the current memory text */
            tempText = getMemoryText();
            /* Update LCD */
            if (tempText.toDouble() == 0)
                setText("0");
            else
                setText(tempText);
            updateLCD();
            break;
        case BUTTON_MS: /* Button memory set */
            /* Save current value to memory */
            if (text.toDouble() == 0)
                setMemoryText("0");
            else
                setMemoryText(text);
            /* Make sure the next value is started anew */
            setText("0");
            setLastClicked(TYPE_INIT);
            break;
        case BUTTON_MP: /* Button memory plus */
            /* Get the current memory text */
            tempText = getMemoryText();
            if (text.toDouble() == 0) {
                /* Current value is zero, do nothing */
                break;
            }
            if (tempText.toDouble() == 0) {
                /* Memory value is zero, save the current one */
                setMemoryText(text);
            } else {
                /* Update memory after addition */
                setMemoryText(calculate(text, tempText, OPERATOR_PLUS));
            }
            break;
        case BUTTON_BS: /* Button backspace */
            if (text.length() > 1) {
                /* If length is more than one, just cut one from end */
                text.remove((text.length() - 1), 1);
                /* Update LCD */
                setText(text);
                updateLCD();
            } else if (text.length() == 1) {
                /* If length is 1, set the value to zero and update LCD */
                setText("0");
                updateLCD();
            }
            /* Set the last clicked button type to others */
            setLastClicked(TYPE_OTHER);
            break;
        case BUTTON_CLR:    /* Button clear */
            /* Reset everything, except memory text */
            setDecimalStatus(false);
            setNegativeStatus(false);
            setText("0");
            setOperator(OPERATOR_NONE);
            setOperand1("0");
            setLastClicked(TYPE_INIT);
            updateLCD();
            break;
        case BUTTON_CUBE:   /* Button cube */
            if (text.toDouble() != 0) {
                /* We need to work only is value is non-zero */
                /* Cube the current value */
                tempText = calculate(text, text, OPERATOR_MUL);
                tempText = calculate(tempText, text, OPERATOR_MUL);
                /* Update LCD */
                setText(tempText);
                updateLCD();
                /* Set the last clicked button type to operator */
                setLastClicked(TYPE_OP);
            }
            break;
        case BUTTON_BIN:    /* Button binary */
            switch (getBinButtonStatus()) {
                case MODE_DEC:
                    /* Currently set to 'Dec', change to 'Bin' */
                    setBinButtonStatus(MODE_BIN, MODE_DEC);
                    break;
                case MODE_BIN:
                    /* We already changed to 'Bin' before, change back to 'Dec' */
                    setBinButtonStatus(MODE_DEC, MODE_BIN);
                    break;
                case MODE_HEX:  /* This should never come */
                default:
                    break;
            }
            break;
        case BUTTON_HEX:    /* Button hexadecimal */
            switch (getHexButtonStatus()) {
                case MODE_DEC:
                    /* Currently set to 'Dec', change to 'Hex' */
                    setHexButtonStatus(MODE_HEX, MODE_DEC);
                    break;
                case MODE_HEX:
                    /* We already changed to 'Hex' before, change back to 'Dec' */
                    setHexButtonStatus(MODE_DEC, MODE_HEX);
                    break;
                case MODE_BIN:  /* This should never come */
                default:
                    break;
            }
            break;
        default:
            break;
        }

    return;
}

#if HEX
/**
 *  @brief  Controller object slot :  Capture hex button press
 *
 *  @param  index   Index of hex button pressed
 *
 *  @return N/A
 */
void Control::hexButtonPressed(int index)
{
    /* Get the current set text */
    QString text = getText();

    /* Get the last clicked button type */
    int lc = getLastClicked();

    /* Get the number of digits displayed */
    int numDigits = getNumDigits();

    if (getHexButtonStatus() == MODE_DEC) {
        switch(index) {
            case HEX_BUTTON_A:  /* Button A: Fall through */
            case HEX_BUTTON_B:  /* Button B: Fall through */
            case HEX_BUTTON_C:  /* Button C: Fall through */
            case HEX_BUTTON_D:  /* Button D: Fall through */
            case HEX_BUTTON_E:  /* Button E: Fall through */
            case HEX_BUTTON_F:  /* Button F: */
                if (numDigits == 0) {
#if DEBUG
                alert("Here 1");
#endif
                /* Value is zero */
                    if ((lc == TYPE_OP) || (lc == TYPE_EQ) || (lc == TYPE_INIT) || (lc == TYPE_OTHER) || (lc == TYPE_DOT)) {
                        /* Take a new value */
                        text = hexButtonLabels[index];
                    }
                    /* Update LCD */
                    setText(text.toLower());
                    updateLCD();
                } else if ((lc == TYPE_OP) || (lc == TYPE_EQ)) {
#if DEBUG
                alert("Here 2");
#endif
                    /* Value is not zero, but need to take a new value */
                    text = hexButtonLabels[index];
                    /* Update LCD */
                    setText(text.toLower());
                    updateLCD();
                } else if (text.length() < LCD_LENGTH) {
#if DEBUG
                alert("Here 3");
#endif
                    /* Value is not zero, append to existing text */
                    text.append(hexButtonLabels[index]);
                    /* Update LCD */
                    setText(text.toLower());
                    updateLCD();
                }
#if DEBUG
                alert("Here 4");
#endif
                /* Set the last clicked button type to number */
                setLastClicked(TYPE_NUM);
                break;
            default:
                break;
        }
    }
    return;
}
#endif
