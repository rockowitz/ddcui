#ifndef DDCUI_QCONFIG_H
#define DDCUI_QCONFIG_H

// config.h template file, QMake version

#define ALT_FEATURES_FLAG 0
#define APPLY_CANCEL_FLAG 0

#if defined(ALT_FEATURES_FLAG) && (ALT_FEATURES_FLAG == 1)
#define ALT_FEATURES
#endif

#if defined(APPLY_CANCEL_FLAG) && (APPLY_CANCEL_FLAG == 1)
#define APPLY_CANCEL
#endif

#ifdef VERBOSE

#ifdef APPLY_CANCEL
#warning APPLY_CANCEL IS defined
#else
#warning APPLY_CANCEL is NOT defined
#endif

#ifdef ALT_FEATURES
#warning ALT_FEATURES IS defined
#else
#warning ALT_FEATURES is NOT defined
#endif

#endif

#endif