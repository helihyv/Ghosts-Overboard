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





#include "screenlitkeeper.h"

ScreenLitKeeper::ScreenLitKeeper(QObject *parent) :
    QObject(parent)
{
    p_screensaver_ = NULL;
    isKeptLit_ = false;
}

void ScreenLitKeeper::keepScreenLit(bool keepLit)
{

    //If the requested state is the same as the current state do nothing.
    if (keepLit == isKeptLit_)
        return;



    if (keepLit == true )
    {
        //a new screensaver is created, parent is given so that it is automatically destroyed when this object is destroyed
        p_screensaver_ = new QSystemScreenSaver(this);
        //screensaver is disabled, which keeps the screen lit on N900
        p_screensaver_->setScreenSaverInhibit();
        isKeptLit_ = true;

    }

    else if (p_screensaver_ != NULL) //just to be on the safe side, it should never be NULL if this line is reached
    {
      delete p_screensaver_; //The object must be deleted to reverse the effect of setScreenSaverInhibit()
      p_screensaver_ = NULL;
      isKeptLit_ = false;

    }

    return;

}
