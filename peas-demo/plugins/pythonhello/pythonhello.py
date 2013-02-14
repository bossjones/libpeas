# -*- coding: utf-8 -*-
# ex:set ts=4 et sw=4 ai:

##
# pythonhello.py
# This file is part of libpeas
#
# Copyright (C) 2009-2010 Steve Frécinaux
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
##

from gi.repository import GObject
from gi.repository import Peas
from gi.repository import PeasGtk
from gi.repository import Gtk

LABEL_STRING="Python Says Hello!"

class PythonHelloPlugin(GObject.Object, Peas.Activatable):
    __gtype_name__ = 'PythonHelloPlugin'

    object = GObject.property(type=GObject.Object)

    def do_activate(self):
        window = self.object
        print "PythonHelloPlugin.do_activate", repr(window)

        engine = Peas.Engine.get_default()
        plugin = engine.get_plugin_info("peas-interpreter-python3")

        self.interpreter = engine.create_extension(plugin,
                                                   Peas.Interpreter.__gtype__,
                                                   None)
        #self.interpreter.set_namespace({ "window": window })

        self.console = PeasGtk.Console.new(self.interpreter)

        # How do we write that: "You can access the window through 'window':\n " + repr(window)

        window.get_child().pack_start(self.console, True, True, 0)
        self.console.show()

    def do_deactivate(self):
        window = self.object
        print "PythonHelloPlugin.do_deactivate", repr(window)

        window.get_child().remove(self.console)
        self.console.destroy()

    def do_update_state(self):
        print("PythonHelloPlugin.do_update_state", repr(self.object))

class PythonHelloConfigurable(GObject.Object, PeasGtk.Configurable):
    __gtype_name__ = 'PythonHelloConfigurable'

    def do_create_configure_widget(self):
        return Gtk.Label.new("Python Hello configure widget")
