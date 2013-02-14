/*
 * peas-interpreter-signals.c
 * This file is part of libpeas
 *
 * Copyright (C) 2011 - Garrett Regier
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "peas-interpreter-signals.h"

/**
 * SECTION:peas-interpreter-signals
 * @short_description: The signals used by #PeasInterpreter.
 * @see: #PeasInterpreter.
 *
 * #PeasInterpreterSignals is an object for signals
 * related to #PeasInterpreter.
 **/

/* This only exists because peas wrapped objects do not support signals */

/* Signals */
enum {
  WRITE,
  WRITE_ERROR,
  RESET,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

G_DEFINE_TYPE (PeasInterpreterSignals, peas_interpreter_signals, G_TYPE_OBJECT);

static void
peas_interpreter_signals_class_init (PeasInterpreterSignalsClass *klass)
{
  GType the_type = G_TYPE_FROM_CLASS (klass);

  /**
   * PeasInterpreterSignal::write:
   * @signals: A #PeasInterpreterSignals.
   * @text: the text to write.
   *
   * The write signal is used by a console to write
   * @text to its buffer.
   */
  signals[WRITE] =
    g_signal_new_class_handler ("write",
                                the_type,
                                G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                NULL,
                                NULL, NULL,
                                g_cclosure_marshal_VOID__STRING,
                                G_TYPE_NONE,
                                1, G_TYPE_STRING);

  /**
   * PeasInterpreterSignal::write-error:
   * @signals: A #PeasInterpreterSignals.
   * @text: the text to write as an error.
   *
   * The error signal is used by a console to write
   * @text as an error message to its buffer.
   */
  signals[WRITE_ERROR] =
    g_signal_new_class_handler ("write-error",
                                the_type,
                                G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                NULL,
                                NULL, NULL,
                                g_cclosure_marshal_VOID__STRING,
                                G_TYPE_NONE,
                                1, G_TYPE_STRING);

  /**
   * PeasInterpreterSignal::reset:
   * @signals: A #PeasInterpreterSignals.
   *
   * The reset signal is used by a console to reset its internal state.
   *
   * The interpreter should also connect to this signal to
   * properly reset when a reset keybinding is activated.
   */
  signals[RESET] =
    g_signal_new_class_handler ("reset",
                                the_type,
                                G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                                NULL,
                                NULL, NULL,
                                g_cclosure_marshal_VOID__VOID,
                                G_TYPE_NONE,
                                0);
}

static void
peas_interpreter_signals_init (PeasInterpreterSignals *signals)
{
}

/**
 * peas_interpreter_signals_new:
 *
 * Creates a new #PeasInterpreterSignals.
 *
 * Return: (transfer full): the new #PeasInterpreterSignals.
 */
PeasInterpreterSignals *
peas_interpreter_signals_new (void)
{
  return PEAS_INTERPRETER_SIGNALS (g_object_new (PEAS_TYPE_INTERPRETER_SIGNALS,
                                                 NULL));
}
