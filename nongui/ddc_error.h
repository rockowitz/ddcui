/* ddc_error.h
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#ifndef DDC_ERROR_H_
#define DDC_ERROR_H_

#include "ddcutil_types.h"

#include <QtCore/QString>

class DdcError {
public:
   DdcError();

   DdcError(
         uint8_t      featureCode,
         const char * ddcFunction,
         DDCA_Status  ddcErrno);

   DdcError(const DdcError& erec);

   virtual ~DdcError();

   DDCA_Status _ddcErrno;
   uint8_t     _featureCode;
   QString     _ddcFunction;

   QString repr();
   char *  srepr();

};

#endif /* DDC_ERROR_H_ */
