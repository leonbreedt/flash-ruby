/*---------------------------------------------------
 *
 * libflash bindings for Ruby
 * (C) Copyright 2005 Leon Breedt
 *
 * Licensed under the terms of the MIT license.
 *
 *--------------------------------------------------*/

#include "ruby-flash.h"

VALUE mFlash;
VALUE eFlashError;

/*
 *  The Flash module contains all the classes and modules related to the +libflash+
 *  bindings.
 */
void
Init_flash ()
{
  mFlash = rb_define_module ("Flash");
  eFlashError = rb_define_class_under (mFlash, "Error", rb_eStandardError);

  Init_flash_library ();
  Init_flash_file ();

  g_type_init ();
}
