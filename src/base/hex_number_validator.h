/** hex_number_validator.h */

// Copyright (C) 2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef HEX_NUMBER_VALIDATOR_H_
#define HEX_NUMBER_VALIDATOR_H_


#include <QtGui/QValidator>

class HexNumberValidator : public QValidator
{
   Q_OBJECT

public:
   HexNumberValidator(QObject * parent = nullptr);

   QValidator::State validate(QString &input, int &pos) const;

private:
   const char *  _cls;
};

#endif /* HEX_NUMBER_VALIDATOR_H_ */
