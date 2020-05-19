// feature_selector.h
//
// Controls which features are shown, as specified by the
// Options->Feature Selection dialog.

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef FEATURE_SELECTOR_H
#define FEATURE_SELECTOR_H

#include <QtCore/QObject>

#include "ddcutil_types.h"

#include "cmdline/parsed_cmd.h"


class FeatureSelector    // : public QObject
{
    // Q_OBJECT

public:
   FeatureSelector();
   FeatureSelector(Parsed_Cmd * parsedCmd);

    FeatureSelector(const FeatureSelector &other);

    DDCA_Feature_Subset_Id _featureSubsetId         = DDCA_SUBSET_KNOWN;
    bool                   _showUnsupportedFeatures = false;
    bool                   _includeTableFeatures    = false;
    bool                   _includeOnlyCapabilities = true;
    bool                   _includeAllCapabilities  = false;
    DDCA_Feature_List      _customFeatureList       = DDCA_EMPTY_FEATURE_LIST;


    void applyParsedOptions(Parsed_Cmd * parsed_cmd);

    FeatureSelector& operator=(const FeatureSelector& sel2) = default;
    bool operator==(const FeatureSelector& sel2) const;
    bool operator!=(const FeatureSelector& sel2) const;

    void dbgrpt();

private:
    const char * _cls = "FeatureSelector";
   //  void applyParsedOptions(Parsed_Cmd * parsed_cmd);
};

#endif // FEATURE_SELECTOR_H
