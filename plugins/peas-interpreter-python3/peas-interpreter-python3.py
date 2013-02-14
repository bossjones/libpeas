# -*- coding: utf-8 -*-
#
# peas-interpreter-python3.py -- A Peas Interpreter for Python 3
#
# Copyright (C) 2006 - Steve Frécinaux
# Copyright (C) 2011 - Garrett Regier
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# Parts from "Interactive Python-GTK Console" (stolen from epiphany's console.py)
#     Copyright (C), 1998 James Henstridge <james@daa.com.au>
#     Copyright (C), 2005 Adam Hooper <adamh@densi.com>
# Bits from gedit Python Console Plugin
#     Copyrignt (C), 2005 Raphaël Slinckx

import copy
import os
import rlcompleter
import sys
import traceback

from gi.repository import GObject, Peas

class PeasInterpreterPython3(GObject.Object, Peas.Interpreter):
    __gtype_name__ = "PeasInterpreterPython3"

    signals = GObject.property(type=Peas.InterpreterSignals)

    def __init__(self):
        GObject.Object.__init__(self)

        self.signals = Peas.InterpreterSignals.new()
        self.signals.connect("reset", self.reset)

        self._stdout = FakeFile(self.write, sys.stdout.fileno())
        self._stderr = FakeFile(self.write_error, sys.stderr.fileno())

        self.do_set_namespace(None)

    def do_prompt(self):
        if not self._in_block and not self._in_multiline:
            if hasattr(sys, "ps1"):
                return sys.ps1

        else:
            if hasattr(sys, "ps2"):
                return sys.ps2

        return ""

    def do_execute(self, code):
        success = False

        # We only get passed an exact statment
        self._code += code + "\n"

        if self._code.rstrip().endswith(":") or \
              (self._in_block and not self._code.endswith("\n\n")):
            self._in_block = True
            return

        elif self._code.endswith("\\"):
            self._in_multiline = True
            return

        sys.stdout, self._stdout = self._stdout, sys.stdout
        sys.stderr, self._stderr = self._stderr, sys.stderr

        try:
            try:
                retval = eval(self._code, self._namespace, self._namespace)

                # This also sets '_'. Bug #607963
                sys.displayhook(retval)

            except SyntaxError:
                exec(self._code, self._namespace)

        except SystemExit:
            self.signals.emit("reset")

        except:
            # Cause only one write-error to be emitted,
            # using traceback.print_exc() would cause multiple.
            self.write_error(traceback.format_exc())

        else:
            success = True

        sys.stdout, self._stdout = self._stdout, sys.stdout
        sys.stderr, self._stderr = self._stderr, sys.stderr

        self._code = ""
        self._in_block = False
        self._in_multiline = False

        return success

    def do_complete(self, code):
        completions = []

        try:
            words = code.split()

            if len(words) == 0:
                word = ""
                text_prefix = code
            else:
                word = words[-1]
                text_prefix = code[:-len(word)]

            for match in self._completer.global_matches(word):
                # The "C" messes with GTK+'s text wrapping
                if match.endswith("("):
                    match = match[:-1]
                else:
                    match += " "

                text = text_prefix + match
                completion = Peas.InterpreterCompletion.new(match, text)
                completions.append(completion)

        except:
            pass

        return completions

    def do_get_namespace(self):
        return self._original_namespace

    def do_set_namespace(self, namespace):
        if namespace is None:
            self._original_namespace = {}

        else:
            self._original_namespace = namespace

        self.reset()

    def reset(self, unused=None):
        self._code = ""
        self._in_block = False
        self._in_multiline = False

        sys.ps1 = ">>> "
        sys.ps2 = "... "

        self._namespace = copy.copy(self._original_namespace)

        if os.getenv("PEAS_DEBUG") is not None:
            if not self._namespace.has_key("self"):
                self._namespace["self"] = self

        try:
            self._completer = rlcompleter.Completer(self._namespace)

        except:
            pass

    def write(self, text):
        self.signals.emit("write", text)

    def write_error(self, text):
        self.signals.emit("write-error", text)


class FakeFile:
    """A fake output file object."""

    def __init__(self, func, fn):
        self._func = func
        self._fn = fn

    def close(self):         pass
    def flush(self):         pass
    def fileno(self):        return self._fn
    def isatty(self):        return 0
    def read(self, a):       return ''
    def readline(self):      return ''
    def readlines(self):     return []
    def write(self, s):      self._func(s)
    def writelines(self, l): self._func(l)
    def seek(self, a):       raise IOError((29, "Illegal seek"))
    def tell(self):          raise IOError((29, "Illegal seek"))
    truncate = tell

# ex:ts=4:et:
