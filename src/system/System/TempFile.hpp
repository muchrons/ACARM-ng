/*
 * TempFile.hpp
 *
 * temporary file (stored in /tmp/) to be created and
 * removed from disk when released.
 * this file wil be inaccessible by its file name after
 * creation.
 *
 */
#ifndef INCLUDE_SYSTEM_TEMPFILE_HPP
#define INCLUDE_SYSTEM_TEMPFILE_HPP

/* public header */

#include <cassert>

#include "System/DiskFile.hpp"
#include "System/Exception.hpp"


namespace System
{

/** \brief class represenintg temporary file.
 */
class TempFile: public DiskFile
{
public:
  /** \brief creates temporary file.
   */
  TempFile(void);
}; // class TempFile

} // namespace System

#endif

