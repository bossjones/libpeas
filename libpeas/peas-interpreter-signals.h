/*
 * peas-interpreter-signals.h
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

#ifndef __PEAS_INTERPRETER_SIGNALS_H__
#define __PEAS_INTERPRETER_SIGNALS_H__

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define PEAS_TYPE_INTERPRETER_SIGNALS            (peas_interpreter_signals_get_type())
#define PEAS_INTERPRETER_SIGNALS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), PEAS_TYPE_INTERPRETER_SIGNALS, PeasInterpreterSignals))
#define PEAS_INTERPRETER_SIGNALS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), PEAS_TYPE_INTERPRETER_SIGNALS, PeasInterpreterSignalsClass))
#define PEAS_IS_INTERPRETER_SIGNALS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), PEAS_TYPE_INTERPRETER_SIGNALS))
#define PEAS_IS_INTERPRETER_SIGNALS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_TYPE_INTERPRETER_SIGNALS))
#define PEAS_INTERPRETER_SIGNALS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), PEAS_TYPE_INTERPRETER_SIGNALS, PeasInterpreterSignalsClass))

typedef struct _PeasInterpreterSignals       PeasInterpreterSignals;
typedef struct _PeasInterpreterSignalsClass  PeasInterpreterSignalsClass;

/**
 * PeasInterpreterSignals:
 *
 * The #PeasInterpreterSignals structure contains only private
 * data and should only be accessed using the provided API.
 */
struct _PeasInterpreterSignals {
  GObject parent;
};

/**
 * PeasInterpreterSignalsClass:
 * @parent_class: The parent class.
 * @write: Signal class handler for the #PeasInterpreterSignals::write signal.
 * @write_error: Signal class handler for the
 *               #PeasInterpreterSignals::write_error signal.
 * @reset: Signal class handler for the #PeasInterpreterSignals::reset signal.
 *
 * The class structure for #PeasInterpreterSignals.
 */
struct _PeasInterpreterSignalsClass {
  GObjectClass parent_class;

  /* Signals */
  void    (*write)       (PeasInterpreterSignals *signals,
                          const gchar            *text);
  void    (*write_error) (PeasInterpreterSignals *signals,
                          const gchar            *text);
  void    (*reset)       (PeasInterpreterSignals *signals);

  /*< private >*/
  gpointer padding[8];
};

/*
 * Public methods
 */
GType                   peas_interpreter_signals_get_type (void) G_GNUC_CONST;

PeasInterpreterSignals *peas_interpreter_signals_new      (void);

G_END_DECLS

#endif /* __PEAS_INTERPRETER_SIGNALS_H__ */
