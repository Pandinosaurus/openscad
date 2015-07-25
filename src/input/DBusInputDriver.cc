/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2015 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include "DBusInputDriver.h"
#include "InputDriverManager.h"

#include "openscad_adaptor.h"
#include "openscad_interface.h"

void DBusInputDriver::run()
{

}

DBusInputDriver::DBusInputDriver() : is_open(false)
{

}

DBusInputDriver::~DBusInputDriver()
{

}

bool DBusInputDriver::openOnce()
{
    return true;
}

bool DBusInputDriver::isOpen()
{
    return is_open;
}

bool DBusInputDriver::open()
{
    if (is_open) {
        return true;
    }

    if (!QDBusConnection::sessionBus().isConnected()) {
        return false;
    }

    new OpenSCADAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/org/openscad/OpenSCAD/Application", this);

    org::openscad::OpenSCAD *iface;
    iface = new org::openscad::OpenSCAD(QString(), QString(), QDBusConnection::sessionBus(), this);

    connect(iface, SIGNAL(zoom(double)), this, SLOT(zoom(double)));
    connect(iface, SIGNAL(zoomTo(double)), this, SLOT(zoomTo(double)));
    connect(iface, SIGNAL(rotate(double, double, double)), this, SLOT(rotate(double, double, double)));
    connect(iface, SIGNAL(rotateTo(double, double, double)), this, SLOT(rotateTo(double, double, double)));
    connect(iface, SIGNAL(translate(double, double, double)), this, SLOT(translate(double, double, double)));
    connect(iface, SIGNAL(translateTo(double, double, double)), this, SLOT(translateTo(double, double, double)));
    is_open = true;
    return true;
}

void DBusInputDriver::close()
{

}

void DBusInputDriver::zoom(double zoom)
{
    InputDriverManager::instance()->postEvent(new InputEventZoom(zoom), false);
}

void DBusInputDriver::zoomTo(double zoom)
{
    InputDriverManager::instance()->postEvent(new InputEventZoom(zoom, false), false);
}

void DBusInputDriver::rotate(double x, double y, double z)
{
    InputDriverManager::instance()->postEvent(new InputEventRotate(x, y, z), false);
}

void DBusInputDriver::rotateTo(double x, double y, double z)
{
    InputDriverManager::instance()->postEvent(new InputEventRotate(x, y, z, false), false);
}

void DBusInputDriver::translate(double x, double y, double z)
{
    InputDriverManager::instance()->postEvent(new InputEventTranslate(x, y, z), false);
}

void DBusInputDriver::translateTo(double x, double y, double z)
{
    InputDriverManager::instance()->postEvent(new InputEventTranslate(x, y, z, false), false);
}

const std::string & DBusInputDriver::get_name() const
{
    static std::string name = "DBusInputDriver";
    return name;
}