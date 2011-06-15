/**************************************************************************
        ScreenLitKeeper

        Copyright (C) 2010  Heli Hyvättinen
        
        This file is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************/





#ifndef SCREENLITKEEPER_H
#define SCREENLITKEEPER_H

#include <QObject>
#include <QSystemScreenSaver>
using namespace QtMobility;


/*! Allows keeping the sreen lit by disabling the screensaver

Works at least on N900.
The sreen can be set to be kept lit or not.
To use this class, you must have the following lines  in your .pro file:
CONFIG += mobility
MOBILITY += systeminfo

@author Heli Hyvättinen
@date 2010-09-07
@version 1

  */

class ScreenLitKeeper : public QObject
{
    Q_OBJECT
public:
    explicit ScreenLitKeeper(QObject *parent = 0);

signals:

public slots:
 /*!
Sets whether the screen is to be kept lit
@param keepLit true for keeping lit, false for allowing blanking
*/

    void keepScreenLit(bool keepLit);

private:
     QSystemScreenSaver * p_screensaver_;
     bool isKeptLit_;

};

#endif // SCREENLITKEEPER_H
