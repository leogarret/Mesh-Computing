/*
 * Copyright (C) 1997-1998 Janne Löf <jlof@mail.student.oulu.fi>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "config.h"

#include <stdarg.h>

#include "gdkgl.h"
#include "gtkglarea.h"

static void gtk_gl_area_destroy       (GtkObject      *object); /* change to finalize? */

G_DEFINE_TYPE (GtkGLArea, gtk_gl_area, GTK_TYPE_DRAWING_AREA);

static void
gtk_gl_area_class_init (GtkGLAreaClass *klass)
{
  GtkObjectClass *object_class;

  object_class = (GtkObjectClass*) klass;

  object_class->destroy = gtk_gl_area_destroy;
}


static void
gtk_gl_area_init (GtkGLArea *gl_area)
{
  gl_area->glcontext = NULL;
  gtk_widget_set_double_buffered(GTK_WIDGET(gl_area), FALSE);
}



GtkWidget*
gtk_gl_area_new_vargs(GtkGLArea *share, ...)
{
  GtkWidget *glarea;
  va_list ap;
  int i;
  gint *attrlist;

  va_start(ap, share);
  i=1;
  while (va_arg(ap, int) != GDK_GL_NONE) /* get number of arguments */
    i++;
  va_end(ap);

  attrlist = g_new(int,i);

  va_start(ap,share);
  i=0;
  while ( (attrlist[i] = va_arg(ap, int)) != GDK_GL_NONE) /* copy args to list */
    i++;
  va_end(ap);

  glarea = gtk_gl_area_share_new(attrlist, share);

  g_free(attrlist);

  return glarea;
}

GtkWidget*
gtk_gl_area_new (int *attrlist)
{
  return gtk_gl_area_share_new(attrlist, NULL);
}

GtkWidget*
gtk_gl_area_share_new (int *attrlist, GtkGLArea *share)
{
  GdkGLContext *glcontext;
  GtkGLArea *gl_area;
#if defined GDK_WINDOWING_X11
  GdkVisual *visual;
#endif

  g_return_val_if_fail(share == NULL || GTK_IS_GL_AREA(share), NULL);

#if defined GDK_WINDOWING_X11
  visual = gdk_gl_choose_visual(attrlist);
  if (visual == NULL)
    return NULL;

  glcontext = gdk_gl_context_share_new(visual, share ? share->glcontext : NULL, TRUE);
#else
  glcontext = gdk_gl_context_attrlist_share_new(attrlist, share ? share->glcontext : NULL, TRUE);
#endif
  if (glcontext == NULL)
    return NULL;

#if defined GDK_WINDOWING_X11
  /* use colormap and visual suitable for OpenGL rendering */
  gtk_widget_push_colormap(gdk_colormap_new(visual,TRUE));
#endif

  gl_area = g_object_new(GTK_TYPE_GL_AREA, NULL);
  gl_area->glcontext = glcontext;

#if defined GDK_WINDOWING_X11
  /* pop back defaults */
  gtk_widget_pop_colormap();
#endif

  return GTK_WIDGET(gl_area);
}


static void
gtk_gl_area_destroy(GtkObject *object)
{
  GtkGLArea *gl_area;

  g_return_if_fail (object != NULL);
  g_return_if_fail (GTK_IS_GL_AREA(object));

  gl_area = GTK_GL_AREA(object);

  if (gl_area->glcontext)
    g_object_unref(gl_area->glcontext);
  gl_area->glcontext = NULL;

  GTK_OBJECT_CLASS (gtk_gl_area_parent_class)->destroy;
}


gint gtk_gl_area_make_current(GtkGLArea *gl_area)
{
  g_return_val_if_fail(gl_area != NULL, FALSE);
  g_return_val_if_fail(GTK_IS_GL_AREA (gl_area), FALSE);
  GtkWidget *widget = GTK_WIDGET (gl_area);
  g_return_val_if_fail(gtk_widget_get_realized(widget), FALSE);

  return gdk_gl_make_current(gtk_widget_get_window (widget),
                             gl_area->glcontext);
}

void gtk_gl_area_swap_buffers(GtkGLArea *gl_area)
{
  g_return_if_fail(gl_area != NULL);
  g_return_if_fail(GTK_IS_GL_AREA(gl_area));
  GtkWidget *widget = GTK_WIDGET (gl_area);
  g_return_if_fail(gtk_widget_get_realized(widget));

  gdk_gl_swap_buffers(gtk_widget_get_window (widget));
}
