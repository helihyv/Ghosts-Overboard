/**************************************************************************
        Ghosts Overboard - a game 'Meego 1.2 Harmattan'

        Copyright (C) 2011  Heli Hyvättinen

        This file is part of Ghosts Overboard

        Ghosts Overboard is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************************/

#include <QtGui/QApplication>
#include "seaview.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Ghosts Overboard");
    a.setOrganizationName("Ghosts Overboard");
    a.setApplicationVersion("0.4.0");
    a.setWindowIcon(QIcon(":/pix/laiva_3aave.png"));
    SeaView w;
#if defined(Q_WS_S60)
    w.showMaximized();
#else
    w.show();
#endif

    return a.exec();
}
