/* ddc_error.cpp
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 * </copyright>
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
   return strdup(repr().toLatin1().data());
}
