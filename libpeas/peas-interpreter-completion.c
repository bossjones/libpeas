/*
 * peas-interpreter-completion.c
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

#include "peas-interpreter-completion.h"

/**
 * SECTION:peas-interpreter-completion
 * @short_description: A completion for use with #PeasInterpreter.
 * @see: #PeasInterpreter.
 *
 * #PeasInterpreterCompletion is an object for completions
 * related to #PeasInterpreter.
 **/

/* Properties */
enum {
  PROP_0,
  PROP_LABEL,
  PROP_TEXT,
  N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES] = { NULL };

G_DEFINE_TYPE (PeasInterpreterCompletion, peas_interpreter_completion, G_TYPE_OBJECT);

struct _PeasInterpreterCompletionPrivate {
  gchar *label;
  gchar *text;
};

static void
peas_interpreter_completion_set_property (GObject      *object,
                                          guint         prop_id,
                                          const GValue *value,
                                          GParamSpec   *pspec)
{
  PeasInterpreterCompletion *completion = PEAS_INTERPRETER_COMPLETION (object);

  switch (prop_id)
    {
    case PROP_LABEL:
      completion->priv->label = g_value_dup_string (value);
      break;
    case PROP_TEXT:
      completion->priv->text = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
peas_interpreter_completion_get_property (GObject    *object,
                                          guint       prop_id,
                                          GValue     *value,
                                          GParamSpec *pspec)
{
  PeasInterpreterCompletion *completion = PEAS_INTERPRETER_COMPLETION (object);

  switch (prop_id)
    {
    case PROP_LABEL:
      g_value_set_string (value, completion->priv->label);
      break;
    case PROP_TEXT:
      g_value_set_string (value, completion->priv->text);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
peas_interpreter_completion_finalize (GObject *object)
{
  PeasInterpreterCompletion *completion = PEAS_INTERPRETER_COMPLETION (object);

  g_free (completion->priv->label);
  g_free (completion->priv->text);

  G_OBJECT_CLASS (peas_interpreter_completion_parent_class)->finalize (object);
}

static void
peas_interpreter_completion_class_init (PeasInterpreterCompletionClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = peas_interpreter_completion_set_property;
  object_class->get_property = peas_interpreter_completion_get_property;
  object_class->finalize = peas_interpreter_completion_finalize;

  properties[PROP_LABEL] =
    g_param_spec_string ("label",
                         "Label",
                         "The label to be shown",
                         NULL,
                         G_PARAM_READWRITE |
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_STATIC_STRINGS);

  properties[PROP_TEXT] =
    g_param_spec_string ("text",
                         "Text",
                         "The replacement text",
                         NULL,
                         G_PARAM_READWRITE |
                         G_PARAM_CONSTRUCT_ONLY |
                         G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties (object_class, N_PROPERTIES, properties);
  g_type_class_add_private (klass, sizeof (PeasInterpreterCompletionPrivate));
}

static void
peas_interpreter_completion_init (PeasInterpreterCompletion *completion)
{
  completion->priv = G_TYPE_INSTANCE_GET_PRIVATE (completion,
                                                  PEAS_TYPE_INTERPRETER_COMPLETION,
                                                  PeasInterpreterCompletionPrivate);
}

/**
 * peas_interpreter_completion_new:
 * @label: The label for the completion.
 * @text: The text for the completion.
 *
 * Creates a new #PeasInterpreterCompletion with @label and @text.
 *
 * Return: (transfer full): the new #PeasInterpreterCompletion.
 */
PeasInterpreterCompletion *
peas_interpreter_completion_new (const gchar *label,
                                 const gchar *text)
{
  g_return_val_if_fail (label != NULL && *label != '\0', NULL);
  g_return_val_if_fail (text != NULL && *text != '\0', NULL);

  return PEAS_INTERPRETER_COMPLETION (g_object_new (PEAS_TYPE_INTERPRETER_COMPLETION,
                                                    "label", label,
                                                    "text", text,
                                                    NULL));
}

/**
 * peas_interpreter_completion_get_label:
 * @completion: A #PeasInterpreterCompletion.
 *
 * Gets the label.
 *
 * Return: (transfer none): the label.
 */
const gchar *
peas_interpreter_completion_get_label  (PeasInterpreterCompletion *completion)
{
  g_return_val_if_fail (PEAS_IS_INTERPRETER_COMPLETION (completion), NULL);

  return completion->priv->label;
}

/**
 * peas_interpreter_completion_get_text:
 * @completion: A #PeasInterpreterCompletion.
 *
 * Gets the text.
 *
 * Return: (transfer none): the text.
 */
const gchar *
peas_interpreter_completion_get_text  (PeasInterpreterCompletion *completion)
{
  g_return_val_if_fail (PEAS_IS_INTERPRETER_COMPLETION (completion), NULL);

  return completion->priv->text;
}
