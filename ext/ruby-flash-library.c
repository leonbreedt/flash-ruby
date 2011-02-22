/*---------------------------------------------------
 *
 * libflash bindings for Ruby
 * (C) Copyright 2005 Leon Breedt
 *
 * Licensed under the terms of the MIT license.
 *
 *--------------------------------------------------*/

#include "ruby-flash.h"

VALUE cFlashLibrary;

VALUE
rflash_new_library (FlashLibrary *library)
{
  return Data_Wrap_Struct (cFlashLibrary, NULL, g_object_unref, library);
}

FlashLibrary *
rflash_get_library (VALUE obj)
{
  FlashLibrary *library;

  library = NULL;
  Data_Get_Struct (obj, FlashLibrary, library);
  if (library == NULL)
    rb_raise (eFlashError, "Failed to retrieve DATA object for FlashLibrary");
  return library;
}

/*
 *  call-seq:
 *      new(pluginpath) => self
 *
 *  Creates a new FlashLibrary, using the plugin at the given path.
 */
static VALUE
cFlashLibrary_new (VALUE klass, VALUE path)
{
  FlashLibrary *library;

  library = NULL;
  RFLASH_TRY (library = flash_library_new (StringValuePtr (path), &error));
  return LIBRARY_NEW (library);
}

/*
 *  call-seq:
 *      description => string
 *
 *  Returns the description of the associated Macromedia(R) Flash plugin. Usually
 *  contains the plugin version.
 */
static VALUE
cFlashLibrary_description (VALUE self)
{
  const gchar *description;

  description = NULL;
  g_object_get (LIBRARY_GET (self), "description", &description, NULL);
  if (description == NULL)
    return Qnil;
  return rb_str_new2 (description);
}

/*
 *  Document-class: Flash::Library
 *
 *  The Flash::Library class is associated with a single +libflashplayer.so+
 *  plugin. All Flash::File instances created with a specific library will
 *  be played using the plugin associated with the library.
 */
void
Init_flash_library ()
{
  cFlashLibrary = rb_define_class_under (mFlash, "Library", rb_cObject);

  rb_define_singleton_method (cFlashLibrary, "new", cFlashLibrary_new, 1);

  rb_define_method (cFlashLibrary, "description", cFlashLibrary_description, 0);
}
