/** @file main.cpp
 *
 *  @brief This is the main file
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

#include <QtGui/QApplication>
#include "calculator.h"

int main(int argc, char *argv[])
{
    /* Give control to Qt */
    QApplication a(argc, argv);

    /* Create our widget, the calculator */
    Calculator w;

    /* Color it */
    w.setStyleSheet("background-color: white");

    /* Show it */
    w.show();

    /* Execute it */
    return a.exec();
}
