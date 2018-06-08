/* ddc_error.cpp
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

#include "ddcutil_c_api.h"

#include "ddc_error.h"

DdcError::DdcError()
   //  : _featureCode(0)
   //  , _ddcErrno(0)
{}

DdcError::DdcError(
      uint8_t       featureCode,
      const char *  ddcFunction,
      DDCA_Status   ddcErrno)
{
   _featureCode = featureCode;
   _ddcErrno    = ddcErrno;
   _ddcFunction = QString(ddcFunction);

}

DdcError::DdcError(const DdcError& erec) {
   _featureCode = erec._featureCode;
   _ddcFunction = erec._ddcFunction;
   _ddcErrno    = erec._ddcErrno;
}


DdcError::~DdcError() {
   // TODO Auto-generated destructor stub
}


QString DdcError::repr() {
   char * s = ddca_rc_name(_ddcErrno);
   QString msg = QString("[feature=0x%1, function=%2, ddcrc=%3 - %4]")
                    .arg(_featureCode, 2, 16)
                    .arg(_ddcFunction)
                    .arg(_ddcErrno)
                    .arg(s);
   return msg;

}

char *  DdcError::srepr() {
   return repr().toLatin1().data();
}
