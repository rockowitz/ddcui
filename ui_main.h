/* ui_main.h */

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QtWidgets>

// #include <QtWidgets/QAction>

#ifdef OLD
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QListView>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTableView>
#include <QTableWidget>
#include <QToolBar>
#include <QWidget>
#include <QMainWindow>
#endif

QT_BEGIN_NAMESPACE

class Ui_MainWindow : public QMainWindow
{
public:
    QAction *actionAbout;
    QAction *actionAboutQt;
    QAction *actionMonitorSummary;
    QAction *actionCapabilities;
    QAction *actionFeaturesListView;
    QAction *actionFeatureSelection;
    QAction *actionShowUnsupportedFeatures;
    QAction *actionKnown;
    QAction *actionScan;
    QAction *actionColor;
    QAction *actionProfile;
    QAction *actionManufacturer;
    QAction *actionFeatureSelectionDialog;
    QAction *actionFeaturesTableView;

    QWidget *centralWidget;
    QStackedWidget *views_stackedWidget;

    QWidget *page_moninfo;
    QPlainTextEdit *moninfoPlainText;

    QWidget *page_capabilities;
    QPlainTextEdit *capabilities_plainText;

    QWidget *page_vcp;
    QListWidget *vcpListWidget;

    QWidget *page_table_item;
    QTableWidget *tableWidget;

    QWidget *page_table_view;
    QHBoxLayout *horizontalLayout;
    QTableView *vcp_tableView;

    QWidget *page_list_view;
    QListView *vcp_listView;

    QLabel *label;

    QMenuBar *menuBar;
    QMenu *menuView;
    QMenu *menuDisplays;
    QMenu *menuHelp;
    QMenu *menuOptions;

    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    Ui_MainWindow();
    ~Ui_MainWindow();

    void setupUi();
    void retranslateFeatureSelectionDialog();

   
// namespace Ui_MainWindow {
//     class Ui_MainWindow public Ui_MainWindow {};
// } // namespace Ui

QT_END_NAMESPACE
};
#endif // UI_MAIN_H
