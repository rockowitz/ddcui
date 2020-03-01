// ddc_error.cpp

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "ddcutil_c_api.h"
#include "ddcutil_status_codes.h"

#include "ddc_error.h"

// *** DdcFeatureError ***

DdcError::DdcError()
   : _ddcErrno(0)
{}


DdcError::DdcError(
      const char *  ddcFunction,
      DDCA_Status   ddcErrno)
    : _ddcErrno(ddcErrno)
    , _ddcFunction(QString(ddcFunction))
{
    // printf("(DdcError::DdcError) Executing\n"); fflush(stdout);
}


DdcError::DdcError(const DdcError& erec)
    : QObject()
    , _ddcErrno(   erec._ddcErrno)
    , _ddcFunction(erec._ddcFunction)
{}


DdcError::~DdcError() {
   // TODO Auto-generated destructor stub
}


QString DdcError::repr() {
   // printf("(DdcError::repr) Executing\n"); fflush(stdout);
   const char * s = ddca_rc_name(_ddcErrno);
   QString msg = QString("[function=%1, ddcrc=%2 - %3]")
                    .arg(_ddcFunction)
                    .arg(_ddcErrno)
                    .arg(s);
   return msg;
}


char *  DdcError::srepr() {
   return strdup(repr().toLatin1().data());
}


QString DdcError::expl() {
   // printf("(DdcError::expl) Executing\n"); fflush(stdout);
   return repr();
}


char *  DdcError::sexpl() {
   return strdup(expl().toLatin1().data());
}


// *** DdcDetailedError ***

DdcDetailedError::DdcDetailedError()
    : DdcError()
    , _detail("")
{}

DdcDetailedError::DdcDetailedError(
      const char * ddcFunction,
      DDCA_Status  ddcErrno,
      const char * detail)
    : DdcError(ddcFunction, ddcErrno)
{
   // printf("(DdcDetailedError::DdcDetailedError) Executing A\n"); fflush(stdout);
   _detail = QString(detail);
}

DdcDetailedError::DdcDetailedError(
      const char * ddcFunction,
      DDCA_Status  ddcErrno,
      QString       detail)
    : DdcError(ddcFunction, ddcErrno)
{
   printf("(DdcDetailedError::DdcDetailedError) Executing B\n"); fflush(stdout);
   _detail = detail;
}


DdcDetailedError::DdcDetailedError(
      const DdcDetailedError& erec)
      : DdcError()
{
    _ddcFunction = erec._ddcFunction;
    _ddcErrno    = erec._ddcErrno;
    _detail      = erec._detail;
}


DdcDetailedError::~DdcDetailedError() {
   // TODO Auto-generated destructor stub
}


QString DdcDetailedError::repr() {
   // printf("(DdcDetailedError::repr) Executing\n"); fflush(stdout);
   const char * s = ddca_rc_name(_ddcErrno);
   QString msg = QString("DdcDetailedError[function=%1, ddcrc=%2 - %3]")
                    .arg(_ddcFunction)
                    .arg(_ddcErrno)
                    .arg(s);
   return msg;
}

QString DdcDetailedError::expl() {
   //  printf("(DdcDetailedError::expl) Starting.\n"); fflush(stdout);
   QString msg = QString("%1").arg(_detail);
   // printf("(DdcDetailedError::expl) msg: %s\n", strdup(msg.toLatin1().data()) ); fflush(stdout);
   // std::cout << "(DdcVerifyError::expl) Returning: " << msg << std::endl;
   return msg;
}



// *** DdcFeatureError ***

DdcFeatureError::DdcFeatureError()
   : DdcError()
   , _featureCode(0)
{}


DdcFeatureError::DdcFeatureError(
      uint8_t       featureCode,
      const char *  ddcFunction,
      DDCA_Status   ddcErrno)
    : DdcError(ddcFunction, ddcErrno)
    , _featureCode(featureCode)
{
    // printf("(DdcFeatureError::DdcFeatureError) Executing\n"); fflush(stdout);
}


DdcFeatureError::DdcFeatureError(
      const DdcFeatureError& erec)
     : DdcError()
{
    _ddcFunction = erec._ddcFunction;
    _ddcErrno    = erec._ddcErrno;
    _featureCode = erec._featureCode;
}


DdcFeatureError::~DdcFeatureError() {
   // TODO Auto-generated destructor stub
}


QString DdcFeatureError::repr() {
   // printf("(DdcFeatureError::repr) Executing\n"); fflush(stdout);
   const char * s = ddca_rc_name(_ddcErrno);
   QString msg = QString("[feature=0x%1, function=%2, ddcrc=%3 - %4]")
                    .arg(_featureCode, 2, 16, QChar('0'))
                    .arg(_ddcFunction)
                    .arg(_ddcErrno)
                    .arg(s);
   return msg;
}


QString DdcFeatureError::expl() {
   // printf("(DdcFeatureError::expl) Executing\n"); fflush(stdout);
   return repr();
}



// *** DdcVerifyError ***

DdcVerifyError::DdcVerifyError(
      uint8_t      featureCode,
      const char * ddcFunction,
      uint16_t      expectedValue,
      uint16_t      observedValue)
    : DdcFeatureError(featureCode, ddcFunction, DDCRC_VERIFY)
    , _expectedValue(expectedValue)
    , _observedValue(observedValue)
{
   // printf("(DdcVerifyError::DdcVerifyError) Executing\n"); fflush(stdout);
}


DdcVerifyError::DdcVerifyError(const DdcVerifyError& erec)
    : DdcFeatureError(erec)
    , _expectedValue(erec._expectedValue)
    , _observedValue(erec._observedValue)
{}


DdcVerifyError::DdcVerifyError(void)
    : DdcFeatureError()
    , _expectedValue(0)
    , _observedValue(0)
{
   // printf("(DdcVerifyError::DdcVerifyError(void)\n"); fflush(stdout);
}


DdcVerifyError::~DdcVerifyError() {
   // TODO
}


QString DdcVerifyError::repr() {
   // printf("(DdcVerifyError::repr)\n"); fflush(stdout);
   const char * s = ddca_rc_name(_ddcErrno);
   QString msg = QString("[feature=0x%1, function=%2, ddcrc=%3 - %4, expected: 0x%5, observed: 0x%6]")
                    .arg(_featureCode, 2, 16)
                    .arg(_ddcFunction)
                    .arg(_ddcErrno)
                    .arg(s)
                    .arg(_expectedValue, 4, 16)
                    .arg(_observedValue, 4, 16)
                    ;
   return msg;
}


QString DdcVerifyError::expl() {
   // printf("(DdcVerifyError::expl) Starting.\n"); fflush(stdout);
   QString msg = QString("Verification failed after value change for feature 0x%1.\n"
                         "Expected value: %2 (0x%3), Reported value: %4 (0x%05)")
                               .arg(_featureCode, 2, 16, QLatin1Char('0'))
                               .arg(_expectedValue)
                               .arg(_expectedValue, 4, 16, QLatin1Char('0'))
                               .arg(_observedValue)
                               .arg(_observedValue, 4, 16, QChar('0'))
                               ;
   // printf("(DdcVerifyError::expl) msg: %s\n", strdup(msg.toLatin1().data()) );
   // std::cout << "(DdcVerifyError::expl) Returning: " << msg << std::endl;
   return msg;
}

