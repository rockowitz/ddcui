/** hex_number_validator.cpp */

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "hex_number_validator.h"

#include "base/ddcui_globals.h"


HexNumberValidator::HexNumberValidator(QObject * parent)
  : QValidator(parent)
 // , _fieldNumber(fieldNumber)
{
   _cls = strdup(metaObject()->className());
}

static inline bool isHexDigit(QChar ch) {
   bool result =
     ( (ch >= 'a' && ch <='f') ||
        (ch >= 'A' && ch <= 'F' ) ||
        (ch >= '0' && ch <= '9') );
   return result;
}


HexNumberValidator::State HexNumberValidator::validate(QString &input, int &pos) const
{
   bool debug = false;
   QValidator::State result = QValidator::Acceptable;

   for (int ndx = 0; ndx < input.length(); ndx++) {
      const QChar& ch = input.at(ndx);
      if (!isHexDigit(ch)) {
         result = QValidator::Invalid;
         break;
      }
   }

   if (result == QValidator::Acceptable) {
      int sz = input.size();
      if (sz == 0)
         result = QValidator::Intermediate;
      else if (sz > 2)
         result = QValidator::Invalid;
   }

   PRINTFCMF(debug, "String: %s, returning %d", qs2s(input), result);
   return result;
}
