/* monitor.cpp */

// Copyright (C) 2018-2026 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <string.h>

#include <QVector>

#include <ddcutil_c_api.h>

#include "base/ddcui_core.h"
#include "c_util/debug_util.h"
#include "c_util/string_util.h"
#include "base/ddcui_rtti.h"
#include "base/global_state.h"
#include "nongui/vcpthread.h"    // includes vcprequest.h
#include "nongui/feature_value.h"

#include "base/monitor.h"

Monitor::Monitor(DDCA_Display_Info2 * display_info, int monitorNumber)
    : _monitorNumber(monitorNumber)
    , _displayInfo(display_info)
    , _baseModel(NULL)
    , _requestQueue(NULL)
    , _cls( strdup(metaObject()->className()) )
{
   bool debug = false;
   TRACECF_STARTING(debug, "monitorNumber=%d, dispno=%d, dref=%s",
                  monitorNumber, display_info->dispno, ddca_dref_repr(display_info->dref));
   _page_moninfo     = _page_capabilities     = NULL;
   _pageno_moninfo   = _pageno_capabilities   = 0;
   _moninfoPlainText = _capabilitiesPlainText = NULL;
   _vcpThread = NULL;

   Parsed_Ddcui_Cmd * parsed_cmd = GlobalState::instance()._parsed_cmd;
   if (parsed_cmd) {
      if (parsed_cmd->estimate_x10_model && streq(display_info->model_name, parsed_cmd->estimate_x10_model))
         estimate_x10 = true;
      if (parsed_cmd->noverify_x10_model && streq(display_info->model_name, parsed_cmd->noverify_x10_model))
         noverify_x10 = true;
   }

   // ddca_report_display_info(_displayInfo, 3);
   if (supportsDdc()) {
      _requestQueue = new VcpRequestQueue();
      _baseModel = new FeatureBaseModel(this);
      // baseModel->setObjectName(QString::asprintf("baseModel-%s",ddca_dref_repr(_displayInfo->dref));

      _vcpThread = new VcpThread(NULL, _displayInfo,  _requestQueue, _baseModel, estimate_x10, noverify_x10);

      // never triggered ??
      QObject::connect(_vcpThread, &VcpThread::finished,
                       this,       &Monitor::vcpThreadFinished);

      _vcpThread->start();
      // _vcp_threads.append(curThread);

      QObject::connect(_baseModel,  SIGNAL(signalVcpRequest(VcpRequest*)),
                       this, SLOT(  putVcpRequest(VcpRequest*)));

#ifdef OUT
      TRACECF_NOPREFIXtrue, "connecting reportDisconnected");
      GlobalState& globals = GlobalState::instance();
      QObject::connect(this,                 &Monitor::reportDisconnected,
                       globals._mainWindow, &MainWindow::removeMonitor);
      TRACECF_NOPREFIXtrue, "connected reportDisconnected");
#endif

   }

   TRACECF_DONE(debug, "_monitorNumber=%d, dref: %s", _monitorNumber, ddca_dref_repr(_displayInfo->dref));
   // if (debug)
   //   ddca_report_display_info(_displayInfo, 3);
}

#ifdef UNUSED
void Monitor::recheck() {
   bool debug = false;
   TRACECF_STARTING(debug, "");
   // get displayinfo for dref
   DDCA_Display_Ref ddca_dref = this->_displayInfo->dref;
   DDCA_Display_Info2 * new_dinfo = nullptr;
   ddca_get_display_info2(ddca_dref, &new_dinfo);
   DDCA_Display_Info2 * old_dinfo = this->_displayInfo;
   this->_displayInfo = new_dinfo;
   ddca_free_display_info2(old_dinfo);
   TRACECF_DONE(debug,"");
}
#endif

Monitor::~Monitor() {
   bool debug = false;
   TRACECF_STARTING(debug, "monitor=%p, _monitor_number=%d, _displayInfo->dispno=%d, _baseModel=%p, _moninfoPlainText=%p, _capabilitiesPlainTex=%p",
         this, _monitorNumber, _displayInfo->dispno, _baseModel,  _moninfoPlainText, _capabilitiesPlainText);

   if (supportsDdc()) {
      _requestQueue->put(new HaltRequest());

      // wait for halt
      while (!_vcpThread->isFinished()) {
         TRACECF_NOPREFIX(debug, "Waiting for _vcpThread to finish");
         QThread::msleep(100);
      }
      TRACECF_NOPREFIX(debug, "_vcpThread finished");

      QObject::disconnect(_baseModel, SIGNAL(signalVcpRequest(VcpRequest*)),
                          this,       SLOT(  putVcpRequest(VcpRequest*)));
      delete _requestQueue;
      delete _vcpThread;
      delete _baseModel;
      delete _moninfoPlainText;
   }
   delete _featuresScrollAreaView;
   ddca_free_display_info2(_displayInfo);
   TRACECF_DONE(debug, "");
   free((void*) _cls);
}

void Monitor::dbgrpt() {
   printf("_monitorNumber:      %d\n",    _monitorNumber);
   printf("_displayInfo->dref:  %p -> %s\n",     _displayInfo->dref, ddca_dref_repr(_displayInfo->dref));
   fflush(stdout);
}

DDCA_Display_Ref Monitor::getDref() {
   return _displayInfo->dref;
}

QString Monitor::dref_repr() {
   if (!_displayInfo->dref)
      return QString("No dref");
   else
      return   QString( ddca_dref_repr(_displayInfo->dref) );
}

DDCA_Feature_List
Monitor::getFeatureList(DDCA_Feature_Subset_Id feature_list_id) {
    bool debugFunc = debugFeatureLists;
    debugFunc = false;
    TRACECF_STARTING(debugFunc,
          "feature_list_id=%d-%s",feature_list_id, ddca_feature_list_id_name(feature_list_id));

    bool include_table_features = false;    // TODO get from feature selection dialog
    DDCA_Status ddcrc = 0;
    DDCA_Feature_List result;
    if (_features.contains(feature_list_id)) {
        result = _features.value(feature_list_id);
    }
    else {
        // ddca_report_display_info(_displayInfo, 3);

        ddcrc = ddca_get_feature_list_by_dref(
                   feature_list_id, _displayInfo->dref, include_table_features, &result);
        TRACECF_NOPREFIX(debugFunc, "ddca_get_feature_list_by_dref() returned %d", ddcrc);
        if (ddcrc == 0) {
           // hack
           if (feature_list_id == DDCA_SUBSET_KNOWN) {
              DDCA_Feature_List mfgFeatureList;
              ddcrc = ddca_get_feature_list_by_dref(
                         DDCA_SUBSET_MFG, _displayInfo->dref, include_table_features, &mfgFeatureList);
              TRACECF_NOPREFIX(debugFunc, "ddca_get_feature_list_by_dref(DDCA_SUBSET_MFG) returned %d", ddcrc);
              if (ddcrc == 0) {
                 result = ddca_feature_list_or(result, mfgFeatureList);
              }
           }
           _features.insert(feature_list_id, result);
        }
    }
    TRACECF_NOPREFIX(debugFunc,
         "Returning: %d features: %s", ddca_feature_list_count(result),
                                       ddca_feature_list_string(result, NULL, (char*)" "));
    TRACECF_DONE(debugFunc, "");
    return result;
}

// consider replacing 2 booleans with an enum with 3 states

bool Monitor::capabilitiesCheckComplete() {
   bool debug = false;
   // considered complete if invalid display
   bool result = (supportsDdc());
   if (result)
      result = (_baseModel->_caps_check_complete);
   TRACECF_EVENT(debug, "dref=%s, returning %s", QS2S(dref_repr()), SBOOL(result));
   return result;
}

bool Monitor::capabilitiesCheckSuccessful() {
   bool debug = false;
   bool result = (_displayInfo->dispno >  0);   // dispno -1 if API found display invalid, -2 if phantom
   if (result)
      result = (_baseModel->_caps_status == 0 && _baseModel->_parsed_caps);  // got capabilities?
   TRACECF_EVENT(debug, "dref=%s, returning %s", QS2S(dref_repr()), SBOOL(result));
   return result;
}

bool Monitor::supportsDdc() {
   bool debug = false;
   bool result = (_displayInfo->dispno >  0);   // dispno -1 if API found display invalid, -2 if phantom
   TRACECF_EVENT(debug, "dref=%s, returning %s", QS2S(dref_repr()), SBOOL(result));
   return result;
}

 // called by initFeaturesScrollAreaView
void Monitor::putVcpRequest(VcpRequest * rqst) {
    bool debug = false;
    TRACECF_STARTING(debug, "-> rqst->type=%d. Adding request to monitor's request queue", rqst->_type);
    _requestQueue->put(rqst);
    TRACECF_DONE(debug, "");
}

// never called!
void Monitor::vcpThreadFinished() {
   bool debug =  true;
   TRACECF_STARTING(debug, "vcp thread finished");
   TRACECF_DONE(debug, "");
}

void Monitor::markDisconnected() {
   bool debug =  true;
   TRACECF_STARTING(debug, "starting");

   QString qstitle("Display Status Change");
   QMessageBox::Icon icon = QMessageBox::Warning;
   QString qstext = QString("Display disconnected on %1, bus /dev/i2c-%2.\n")
              //     .arg(evt.connector_name).arg(evt.io_path.path.i2c_busno);
                     .arg(_displayInfo->drm_card_connector).arg( _displayInfo->path.path.i2c_busno);
   MsgBoxQueueEntry* qe = new MsgBoxQueueEntry(qstitle, qstext, icon);
   GlobalState::instance()._msgBoxQueue->put(qe);

   emit reportDisconnected(this->_displayInfo->dref);
   TRACECF_NOPREFIX(debug, "emitted");
   TRACECF_DONE(debug, "");
}


void init_monitor() {
   bool debug = false;
   DBGF(debug, "Starting");
   RTTI_ADD_METHOD(Monitor::Monitor);
   RTTI_ADD_METHOD(Monitor::~Monitor);
   RTTI_ADD_METHOD(Monitor::getFeatureList);
   RTTI_ADD_METHOD(Monitor::putVcpRequest);
   RTTI_ADD_METHOD(Monitor::vcpThreadFinished);
   RTTI_ADD_METHOD(Monitor::markDisconnected);
   DBGF(debug, "Done");
}
