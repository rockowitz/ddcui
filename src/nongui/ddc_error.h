// ddc_error.h

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DDC_ERROR_H_
#define DDC_ERROR_H_

#include "ddcutil_types.h"

#include <QtCore/QString>
#include <QtCore/QObject>



class DdcError: public QObject {
    Q_OBJECT

public:
   DdcError();

   DdcError(
         const char * ddcFunction,
         DDCA_Status  ddcErrno);

   // n. public copy constructor and destructor needed for qRegisterMetaType()
   DdcError(const DdcError& erec);

   virtual ~DdcError();

   DDCA_Status _ddcErrno;
   QString     _ddcFunction;

   virtual QString repr();
   virtual char *  srepr();
   virtual QString expl();
   virtual char *  sexpl();
};


class DdcDetailedError: public DdcError {
   Q_OBJECT

public:
   DdcDetailedError();

   DdcDetailedError(
         const char * ddcFunction,
         DDCA_Status  ddcErrno,
         const char * detail);

   DdcDetailedError(
         const char * ddcFunction,
         DDCA_Status  ddcErrno,
         QString      detail);

   // n. public copy constructor and destructor needed for qRegisterMetaType()
   DdcDetailedError(const DdcDetailedError& erec);

   virtual ~DdcDetailedError();

   QString  _detail;

   virtual QString repr();
   virtual QString expl();
};

class DdcFeatureError: public DdcError {
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

   virtual QString repr();
   virtual QString expl();
};



class DdcVerifyError : public DdcFeatureError {
   Q_OBJECT

public:
   DdcVerifyError(
         uint8_t      featureCode,
         const char * ddcFunction,
         uint16_t     expectedValue,
         uint16_t     observedValue);

   DdcVerifyError(const DdcVerifyError& erec);

   DdcVerifyError(void);

   virtual ~DdcVerifyError();

   virtual QString repr();
   virtual QString expl();

   uint16_t  _expectedValue;
   uint16_t  _observedValue;
};

 // Q_DECLARE_METATYPE(DdcFeatureError*);
 // Q_DECLARE_METATYPE(DdcVerifyError);

#endif /* DDC_ERROR_H_ */
