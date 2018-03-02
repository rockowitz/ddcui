
#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
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

QT_BEGIN_NAMESPACE

class Ui_MainWindow : public QMainWindow
{
public:
    QAction *actionAbout;
    QAction *actionAbout_Qt;
    QAction *actionMonitor_Summary;
    QAction *actionCapabilities;
    QAction *actionFeaturesListView;
    QAction *actionFeature_Selection;
    QAction *actionShow_Unsupported_Features;
    QAction *actionKnown;
    QAction *actionScan;
    QAction *actionColor;
    QAction *actionProfile;
    QAction *actionManufacturer;
    QAction *actionFeature_Selection_Dialog;
    QAction *actionFeatures_TableView;

    QWidget *centralWidget;
    QStackedWidget *views_stackedWidget;

    QWidget *page_moninfo;
    QPlainTextEdit *moninfoPlainText;

    QWidget *page_capabilities;
    QPlainTextEdit *capabilities_plainText;

    QWidget *page_vcp;
    QListWidget *vcp_listWidget;

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
    void retranslateUi();

   
// namespace Ui_MainWindow {
//     class Ui_MainWindow public Ui_MainWindow {};
// } // namespace Ui

QT_END_NAMESPACE
};
#endif // UI_MAIN_H
