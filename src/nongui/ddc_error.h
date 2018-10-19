// ddc_error.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDC_ERROR_H_
#define DDC_ERROR_H_

#include "ddcutil_types.h"

#include <QtCore/QString>
#include <QtCore/QObject>


class DdcFeatureError: public QObject {
    Q_OBJECT

public:
   DdcFeatureError();

   DdcFeatureError(
         uint8_t      featureCode,
         const char * ddcFunction,
         DDCA_Status  ddcErrno);

   // n. public copy constructor and destructor needed for qRegisterMetaType()
   DdcFeatureError(const DdcFeatureError& erec);

   virtual ~DdcFeatureError();

   uint8_t     _featureCode;
   DDCA_Status _ddcErrno;
   QString     _ddcFunction;

   virtual QString repr();
   virtual char *  srepr();
   virtual QString expl();
   virtual char *  sexpl();
};






class DdcVerifyError : public DdcFeatureError {
   Q_OBJECT

public:
   DdcVerifyError(
         uint8_t      featureCode,
         const char * ddcFunction,
         uint8_t      expectedValue,
         uint8_t      observedValue);

   DdcVerifyError(const DdcVerifyError& erec);

   DdcVerifyError(void);

   virtual ~DdcVerifyError();

   virtual QString repr();
   virtual QString expl();

   uint8_t  _expectedValue;
   uint8_t  _observedValue;
};

 // Q_DECLARE_METATYPE(DdcFeatureError*);
 // Q_DECLARE_METATYPE(DdcVerifyError);

#endif /* DDC_ERROR_H_ */
