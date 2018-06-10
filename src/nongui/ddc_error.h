/* ddc_error.h
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 * </copyright>
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

   DDCA_Status _ddcErrno = 0;
   uint8_t     _featureCode = 0;
   QString     _ddcFunction;

   QString repr();
   char *  srepr();
};

#endif /* DDC_ERROR_H_ */
