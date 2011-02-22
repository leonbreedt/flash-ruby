/*---------------------------------------------------
 *
 * libflash bindings for Ruby
 * (C) Copyright 2005 Leon Breedt
 *
 * Licensed under the terms of the MIT license.
 *
 *--------------------------------------------------*/

#ifndef RUBY_FLASH_H
#define RUBY_FLASH_H

#include <flash/flash.h>
#include "ruby.h"

#ifdef DEBUG
#undef DEBUG
#endif

#ifdef RFLASH_DEBUG
#define DEBUG(fmt, x...) fprintf (stderr, "[RFLASH] " fmt "\n", ##x)
#else
#define DEBUG(fmt, x...) 
#endif

#define RFLASH_TRY_ERR(expr, klass)   \
  do {                                \
    GError *error;                    \
    error = NULL;                     \
    (expr);                           \
    if (error)                        \
      rb_raise (klass, error->message);\
  } while (0);

#define RFLASH_TRY(expr) \
  RFLASH_TRY_ERR((expr), eFlashError)

#define RFLASH_RAISE_IF(cond, message) \
  if (cond) rb_raise (eFlashError, message)

#define LIBRARY_GET(obj) \
  rflash_get_library (obj)

#define LIBRARY_NEW(obj) \
  rflash_new_library (obj)

#define FILE_GET(obj) \
  rflash_get_file (obj)

#define FILE_NEW(obj) \
  rflash_new_file (obj, TRUE)

#define FILE_NEW_REFERENCE(obj) \
  rflash_new_file (obj, FALSE)

extern VALUE mFlash;
extern VALUE cFlashLibrary;
extern VALUE cFlashFile;
extern VALUE eFlashError;

void Init_flash_library ();
void Init_flash_file    ();

VALUE rflash_new_library   (FlashLibrary *library);
VALUE rflash_new_file      (FlashFile *file, gboolean owner);

FlashLibrary  *rflash_get_library   (VALUE obj);
FlashFile     *rflash_get_file      (VALUE obj);

#endif
