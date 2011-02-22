/*---------------------------------------------------
 *
 * libflash bindings for Ruby
 * (C) Copyright 2005 Leon Breedt
 *
 * Licensed under the terms of the MIT license.
 *
 *--------------------------------------------------*/

#include "ruby-flash.h"

VALUE cFlashFile;

typedef struct gobj_holder {
  VALUE self;
  GObject* gobj;
  const void* ptr0;
  gboolean bool0;
} gobj_holder;

VALUE
rflash_new_file (FlashFile *file, gboolean owner)
{
  if (!owner)
    g_object_ref (file);
  return Data_Wrap_Struct (cFlashFile, NULL, g_object_unref, file);
}

FlashFile *
rflash_get_file (VALUE obj)
{
  FlashFile *file;

  file = NULL;
  Data_Get_Struct (obj, FlashFile, file);
  if (file == NULL)
    rb_raise (eFlashError, "Failed to retrieve DATA object for FlashFile");
  return file;
}

static VALUE
execute_callback (VALUE args)
{
  VALUE *argv;

  argv = (VALUE *)args;
  return rb_funcall (argv[0], rb_intern ("call"), 2, argv[1], argv[2]);
}

static void
on_flash_file_event (FlashFile *file, FlashFileEvent event, gpointer user_data)
{
  VALUE cb;
  int exc;
  VALUE args[3];

  cb = (VALUE)user_data;
  args[0] = cb;
  args[1] = FILE_NEW_REFERENCE (file);
  args[2] = INT2FIX (event);

  exc = 0;
  rb_protect (execute_callback, (VALUE)args, &exc);
  if (exc)
  {
    rb_warn(" FlashFile callback raised exception, backtrace:");
		rb_backtrace();
	}
}

/*
 *  call-seq:
 *      new(library, filepath, callback=nil) => self
 *
 *  Creates a new Flash::File, using the plugin at the given path. If
 *  +callback+ is given, it should be a proc accepting a +file+ and +event+
 *  parameter. The +file+ parameter will be the Flash::File instance, and the
 *  +event+ parameter will be one of the following constants:
 *
 *  +Flash::File::EVENT_PLAYBACK_STOPPED+ : the plugin has indicated that it has finished playing the relevant Flash file
 */
static VALUE
cFlashFile_new (int argc, VALUE *argv)
{
  VALUE lib;
  VALUE path;
  VALUE cb;
  FlashFile *file;
  FlashFileEventCallback callback;
  gpointer callback_data;

  lib = Qnil;
  path = Qnil;
  cb = Qnil;
  callback = NULL;
  callback_data = NULL;

  rb_scan_args (argc, argv, "21", &lib, &path, &cb);
  file = NULL;
  if (cb != Qnil)
  {
    callback = on_flash_file_event;
    callback_data = (gpointer)cb;
  }

  RFLASH_TRY (
    file = flash_file_new (
      LIBRARY_GET (lib),
      StringValuePtr (path),
      callback,
      callback_data,
      &error)
  );

  return FILE_NEW (file);
}

/*
 *  call-seq:
 *      play(gtkwindow, loop=true) => true|false
 *
 *  Starts playback of the Flash file in the given Gtk::Window. If +loop+ is set
 *  to +true+, the file will loop until stopped, and will never cause a
 *  PLAYBACK_STOPPED event to be emitted.
 */
static VALUE
cFlashFile_play (int argc, VALUE *argv, VALUE self)
{
  VALUE loop;
  VALUE win;
  GtkWindow *window;
  gobj_holder *holder;
  VALUE ret;

  loop = Qfalse;
  win = Qnil;

  rb_scan_args (argc, argv, "11", &win, &loop);

  holder = NULL;
  Data_Get_Struct (win, gobj_holder, holder);
  if (holder == NULL)
    rb_raise (eFlashError, "Failed to retrieve DATA object from window");

  window = GTK_WINDOW (holder->gobj);
  if (!window)
    rb_raise (eFlashError, "Object is not a Gtk::Window");

  ret = Qfalse;
  RFLASH_TRY (
    ret = flash_file_play (
      FILE_GET (self), 
      window,
      loop == Qtrue ? TRUE : FALSE,
      &error) ? Qtrue : Qfalse
  );
  return ret;
}

/*
 *  call-seq:
 *      playing? => true|false
 *
 *  Returns +true+ if the Flash file is currently playing
 */
static VALUE
cFlashFile_is_playing (VALUE self)
{
  if (flash_file_is_playing (FILE_GET (self)))
    return Qtrue;
  return Qfalse;
}

/*
 *  call-seq:
 *      pause => true|false
 *
 *  Returns +true+ if playback was paused successfully
 */
static VALUE
cFlashFile_pause (VALUE self)
{
  if (flash_file_pause (FILE_GET (self)))
    return Qtrue;
  return Qfalse;
}

/*
 *  call-seq:
 *      resume => true|false
 *
 *  Returns +true+ if playback was resumed successfully
 */
static VALUE
cFlashFile_resume (VALUE self)
{
  if (flash_file_resume (FILE_GET (self)))
    return Qtrue;
  return Qfalse;
}

/*
 *  call-seq:
 *      stop => true|false
 *
 *  Returns +true+ if playback was stopped successfully
 */
static VALUE
cFlashFile_stop (VALUE self)
{
  if (flash_file_stop (FILE_GET (self)))
    return Qtrue;
  return Qfalse;
}

/*
 *  Document-class: Flash::File
 *
 *  The Flash::File class represents a single .SWF file instance. A Flash::File
 *  instance is bound to the Flash::Library it was created with.
 */
void
Init_flash_file ()
{
  cFlashFile = rb_define_class_under (mFlash, "File", rb_cObject);

  rb_define_singleton_method (cFlashFile, "new", cFlashFile_new, -1);

  rb_define_const(cFlashFile, "EVENT_PLAYBACK_STOPPED", INT2FIX(FLASH_FILE_PLAYBACK_STOPPED));

  rb_define_method (cFlashFile, "play", cFlashFile_play, -1);
  rb_define_method (cFlashFile, "playing?", cFlashFile_is_playing, 0);
  rb_define_method (cFlashFile, "pause", cFlashFile_pause, 0);
  rb_define_method (cFlashFile, "resume", cFlashFile_resume, 0);
  rb_define_method (cFlashFile, "stop", cFlashFile_stop, 0);
}
