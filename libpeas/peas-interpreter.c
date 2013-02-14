/*
 * peas-interpreter.c
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

#include "peas-interpreter.h"

/**
 * SECTION:peas-interpreter
 * @short_description: Interface for interpreter plugins.
 *
 * #PeasInterpreter is an interface which should be implemented by plugins
 * that should be interpreters.
 **/

G_DEFINE_INTERFACE(PeasInterpreter, peas_interpreter, G_TYPE_OBJECT)

void
peas_interpreter_default_init (PeasInterpreterInterface *iface)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  /**
   * PeasInterpreter:signals:
   *
   * The signals of the interpreter.
   *
   * Note: this is only needed because libpeas' cannot proxy signals.
   */
  g_object_interface_install_property (iface,
                                       g_param_spec_object ("signals",
                                                            "Signals",
                                                            "Signals of the interpreter",
                                                            PEAS_TYPE_INTERPRETER_SIGNALS,
                                                            G_PARAM_READWRITE |
                                                            G_PARAM_CONSTRUCT_ONLY |
                                                            G_PARAM_STATIC_STRINGS));

  initialized = TRUE;
}

/**
 * peas_interpreter_complete:
 * @interpreter: A #PeasInterpreter.
 * @code: The code to complete.
 *
 * Gets a list of possible completions for @code.
 *
 * Note that @code must not have leading whitespace stripped and
 * the completions are to be sorted alphabetically.
 *
 * Returns: (transfer full) (element-type Peas.InterpreterCompletion):
 * a newly-allocated %NULL-terminated list of completions.
 */
GList *
peas_interpreter_complete (PeasInterpreter *interpreter,
                           const gchar     *code)
{
  PeasInterpreterInterface *iface;

  /* Hmm should this be async? */

  g_return_val_if_fail (PEAS_IS_INTERPRETER (interpreter), NULL);
  g_return_val_if_fail (code != NULL, NULL);

  iface = PEAS_INTERPRETER_GET_IFACE (interpreter);
  if (iface->complete != NULL)
    return iface->complete (interpreter, code);

  return NULL;
}

/**
 * peas_interpreter_execute:
 * @interpreter: A #PeasInterpreter.
 * @code: statement to execute.
 *
 * Executes @code and returns if it did not
 * cause an error, for instance if it was invalid.
 *
 * Returns: if an error did not occurred.
 */
gboolean
peas_interpreter_execute (PeasInterpreter *interpreter,
                          const gchar     *code)
{
  PeasInterpreterInterface *iface;

  g_return_val_if_fail (PEAS_IS_INTERPRETER (interpreter), FALSE);
  g_return_val_if_fail (code != NULL, FALSE);

  iface = PEAS_INTERPRETER_GET_IFACE (interpreter);
  if (iface->execute != NULL)
    return iface->execute (interpreter, code);

  return FALSE;
}

/**
 * peas_interpreter_prompt:
 * @interpreter: A #PeasInterpreter.
 *
 * Gets @interpreter's prompt.
 *
 * Returns: the interpreter's prompt.
 */
gchar *
peas_interpreter_prompt (PeasInterpreter *interpreter)
{
  PeasInterpreterInterface *iface;

  g_return_val_if_fail (PEAS_IS_INTERPRETER (interpreter), NULL);

  iface = PEAS_INTERPRETER_GET_IFACE (interpreter);
  if (iface->prompt != NULL)
    return iface->prompt (interpreter);

  return NULL;
}

/**
 * peas_interpreter_write:
 * @interpreter: A #PeasInterpreter.
 * @text: text to write.
 *
 * Emits the "write" signal for @text.
 */
void
peas_interpreter_write (PeasInterpreter *interpreter,
                        const gchar     *text)
{
  g_return_if_fail (PEAS_IS_INTERPRETER (interpreter));
  g_return_if_fail (text != NULL);

  g_signal_emit_by_name (peas_interpreter_get_signals (interpreter),
                         "write", text);
}

/**
 * peas_interpreter_write_error:
 * @interpreter: A #PeasInterpreter.
 * @text: text to write as an error.
 *
 * Emits the "write-error" signal for @text.
 */
void
peas_interpreter_write_error (PeasInterpreter *interpreter,
                              const gchar     *text)
{
  g_return_if_fail (PEAS_IS_INTERPRETER (interpreter));
  g_return_if_fail (text != NULL);

  g_signal_emit_by_name (peas_interpreter_get_signals (interpreter),
                         "write-error", text);
}

/**
 * peas_interpreter_reset:
 * @interpreter: A #PeasInterpreter.
 *
 * Emits the "reset" signal.
 */
void
peas_interpreter_reset (PeasInterpreter *interpreter)
{
  g_return_if_fail (PEAS_IS_INTERPRETER (interpreter));

  g_signal_emit_by_name (peas_interpreter_get_signals (interpreter), "reset");
}

/**
 * peas_interpreter_get_namespace:
 * @interpreter: A #PeasInterpeter.
 *
 * Gets the #GHashTable representing the namespace of @interpreter.
 *
 * Returns: (element-type utf8 GObject.Value) (transfer none) (allow-none):
 *  A #GHashTable representing the interpreter's namespace, or %NULL.
 */
const GHashTable *
peas_interpreter_get_namespace (PeasInterpreter *interpreter)
{
  PeasInterpreterInterface *iface;

  g_return_val_if_fail (PEAS_IS_INTERPRETER (interpreter), NULL);

  iface = PEAS_INTERPRETER_GET_IFACE (interpreter);
  if (iface->get_namespace != NULL)
    return iface->get_namespace (interpreter);

  return NULL;
}


/**
 * peas_interpreter_set_namespace:
 * @interpreter: A #PeasInterpeter.
 * @namespace_: (element-type utf8 GObject.Value) (transfer none):
 *  The namespace for the interpreter.
 *
 * Sets the namespace of @interpreter.
 *
 * Note: this must be set before adding the interpreter to a console.
 */
void
peas_interpreter_set_namespace (PeasInterpreter  *interpreter,
                                const GHashTable *namespace_)
{
  PeasInterpreterInterface *iface;

  g_return_if_fail (PEAS_IS_INTERPRETER (interpreter));
  g_return_if_fail (namespace_ != NULL);

  iface = PEAS_INTERPRETER_GET_IFACE (interpreter);
  if (iface->set_namespace != NULL)
    iface->set_namespace (interpreter, namespace_);
}

/**
 * peas_interpreter_get_signals:
 * @interpreter: A #PeasInterpreter.
 *
 * Gets the #PeasInterpreterSignals that @interpreter's
 * signals are emitted on.
 *
 * This is done because libpeas cannot proxy signals.
 *
 * Returns: (transfer none): the interpreter's signals object.
 */
PeasInterpreterSignals *
peas_interpreter_get_signals (PeasInterpreter *interpreter)
{
  PeasInterpreterSignals *signals;

  g_return_val_if_fail (PEAS_IS_INTERPRETER (interpreter), NULL);

  g_object_get (G_OBJECT (interpreter),
                "signals", &signals,
                NULL);

  g_object_unref (signals);

  return signals;
}
