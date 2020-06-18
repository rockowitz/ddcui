// widget_debug.cpp

// Functions for debugging Qt layouts

// Copyright (C) 2018-2020 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <glib-2.0/glib.h>
#include <string.h>
#include <stdio.h>

#include <QtCore/QMargins>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QString>
#include <QtGui/QRegion>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSizePolicy>

#include <QtWidgets/QApplication>
#include <QtGui/QScreen>
#include <QtWidgets/QDesktopWidget>
#include <QtGui/QWindow>

#include "core.h"
#include "widget_debug.h"


void reportBasicWidgetDimensions(
      QWidget *    w,
      const char * caller_class,
      const char * caller_func,
      const int    caller_lineno,
      const char * caller_filename)
{
    const char * cls = w->metaObject()->className();

    QSize sizeHint = w->sizeHint();
    QSize minSize = w->minimumSize();
    QSize minSizeHint = w->minimumSizeHint();
    QSize maxSize = w->maximumSize();

    QSizePolicy sp = w->sizePolicy();
    QSizePolicy::Policy hp = sp.horizontalPolicy();
    QSizePolicy::Policy vp = sp.verticalPolicy();

    printftcmf(
       true,
       caller_class,
       NULL,
       caller_func,
       caller_lineno,
       caller_filename,
       "BASIC WIDGET DIMENSIONS for class %s:, sizePolicy(%s,%s)",
            cls, sizePolicyName(hp), sizePolicyName(vp) );

    printf("   size (%d,%d) sizeHint(%d,%d) minimumSize(%d,%d) minimumSizeHint(%d,%d) maximumSize(%d,%d) \n",  // maximumSize(%d,%d)\n",
        w->width(),          w->height(),
        sizeHint.width(),     sizeHint.height(),
        minSize.width(),      minSize.height(),
        minSizeHint.width(),  minSizeHint.height(),
        maxSize.width(),      maxSize.height() );
        //   maxSize.width(),      maxSize.height()  );

    QMargins margins = w->contentsMargins();
    QRect    rect    = w->contentsRect();
    printf("   contents margins (%d,%d,%d,%d) rectangle (%d,%d) \n",
        margins.top(), margins.right(), margins.bottom(), margins.left(),
        rect.width(), rect.height() );
}


#ifdef NOT_NEEDED
void reportBasicWindowDimensions2(
      const char * classname,
      const char * func,
      const int    line,
      const char * filename,
      const char * format,
      const char * format, ...)
{
}
#endif



void reportWidgetChildren(QWidget * w, const char * msg) {
    if (msg)
        puts(msg);
    QObjectList  childs = w->children();
    for (int ndx = 0; ndx < childs.size(); ndx++) {
        QObject* curobj = childs.at(ndx);
        QString name   = curobj->objectName();
        const char *  clsName = curobj->metaObject()->className();
        printf("   Child: %s, type:%s\n", name.toLatin1().data(), clsName);
    }
    fflush(stdout);
}


const char * sizePolicyName(QSizePolicy::Policy policy) {
   char * s = NULL;
   switch(policy) {
   case QSizePolicy::Fixed:             s = (char*) "Fixed"; break;
   case QSizePolicy::Minimum:           s = (char*) "Minimum"; break;
   case QSizePolicy::Maximum:           s = (char*) "Maximum"; break;
   case QSizePolicy::Preferred:         s = (char*) "Preferred"; break;
   case QSizePolicy::Expanding:         s = (char*) "Expanding"; break;
   case QSizePolicy::MinimumExpanding:  s = (char*) "MinimumExpanding"; break;
   case QSizePolicy::Ignored:           s = (char*) "Ignored"; break;
   default: s = (char*) "other";
   }
   return s;
}




void reportPolicy(   QSizePolicy::Policy  hp,
      int depth,
      const char * msg = nullptr)
{
   int d1 = depth+1;
   char indent[100];
   g_snprintf(indent, 100, "%*s", depth*3, "");

    if (msg)
       printf("%s%s\n", indent, msg);

    g_snprintf(indent, 100, "%*s", d1*3, "");

    // Two ways of looking at the policy field, as bits or as enum

    printf("%spolicy:  %s %s %s %s\n",
           indent,
           (hp & QSizePolicy::GrowFlag)   ? "Grow"   : "",
           (hp & QSizePolicy::ExpandFlag) ? "Expand" :"",
           (hp & QSizePolicy::ShrinkFlag) ? "Shrink" : "",
           (hp & QSizePolicy::IgnoreFlag) ? "Ignore" : "");

    char * s = NULL;
    switch(hp) {
    case QSizePolicy::Fixed:             s = (char*) "Fixed"; break;
    case QSizePolicy::Minimum:           s = (char*) "Minimum"; break;
    case QSizePolicy::Maximum:           s = (char*) "Maximum"; break;
    case QSizePolicy::Preferred:         s = (char*) "Preferred"; break;
    case QSizePolicy::Expanding:         s = (char*) "Expanding"; break;
    case QSizePolicy::MinimumExpanding:  s = (char*) "MinimumExpanding"; break;
    case QSizePolicy::Ignored:           s = (char*) "Ignored"; break;
    default: s = (char*) "other";
    }

    printf("%s     %s\n", indent, s);
}


void reportQSizePolicy(
      QSizePolicy p ,
      //        const char * className,
      //      const char * funcName,
      int         depth,
      const char * msg = nullptr)
{
    int d1 = depth+1;
    char indent[100];
    g_snprintf(indent, 100, "%*s", depth*3, "");

    if (msg)
       printf("%s%s\n", indent, msg);

    g_snprintf(indent, 100, "%*s", d1*3, "");

    char buf[100];
    Qt::Orientations orientations = p.expandingDirections();
    g_snprintf(buf, 100, "%sexpandingDirections:  %s  %s\n",
             indent,
             (orientations & Qt::Horizontal) ? "Horizontal" : "",
             (orientations & Qt::Vertical)   ? "Vertical"   : "");
    printf("%s%s\n", indent, buf);
    printf("%shorizontalStretch:  %d\n", indent, p.horizontalStretch());
    printf("%sverticalStretch:    %d\n", indent, p.verticalStretch());

    char * s;
    QSizePolicy::ControlTypes ct = p.controlType();
    switch(ct) {
    case QSizePolicy::DefaultType: s = (char*) "DefaultType"; break;
    case QSizePolicy::ButtonBox:   s = (char*) "ButtonBox";   break;
    case QSizePolicy::CheckBox:    s = (char*) "CheckBox";    break;
    case QSizePolicy::ComboBox:    s = (char*) "ComboBox";    break;
    case QSizePolicy::Frame:       s = (char*) "Frame";       break;
    case QSizePolicy::GroupBox:    s = (char*) "GroupBox";    break;
    case QSizePolicy::Label:       s = (char*) "Label";       break;
    case QSizePolicy::Line:        s = (char*) "Line";        break;
    case QSizePolicy::LineEdit:    s = (char*) "LineEdit";    break;
    case QSizePolicy::PushButton:  s = (char*) "PushButton";  break;
    case QSizePolicy::RadioButton: s = (char*) "RadioButton"; break;
    case QSizePolicy::Slider:      s = (char*) "Slider";      break;
    case QSizePolicy::SpinBox:     s = (char*) "SpinBox";     break;
    case QSizePolicy::TabWidget:   s = (char*) "TabWidget";   break;
    case QSizePolicy::ToolButton:  s = (char*) "ToolButton";  break;
    default:                       s = (char*) "other";
    }
    printf("%scontrolType:   %s\n", indent, s);

    QSizePolicy::Policy  hp = p.horizontalPolicy();
    reportPolicy(hp, depth+1, "Horizontal Policy:");
    reportPolicy(p.verticalPolicy(), depth+1, "Vertical Policy");

    // TODO: additional
    //    ->ExpandingDirections();
    //     more?
}



void reportLayout(
      QLayout * lay, int depth)
{
   int d1 = depth+1;
   char indent[100];
   g_snprintf(indent, 100, "%*s", d1*3, "");

   QMargins layoutContentsMargins = lay->contentsMargins();
        printf("%slayoutContentsMargins: left: %d, top: %d, right: %d, bottom: %d\n",
               indent,
               layoutContentsMargins.left(), layoutContentsMargins.top(),
               layoutContentsMargins.right(), layoutContentsMargins.bottom() );
#ifdef NOT_USEFUL
        QRect    layoutContentsRect  = lay->contentsRect();
        printf("layoutContentsRect: x: %d, y: %d, width: %d, height: %d\n",
               layoutContentsRect.x(), layoutContentsRect.y(), layoutContentsRect.width(), layoutContentsRect.height());
#endif
        // spacing between the widgets inside the layout
        // int layoutSpacing = lay->spacing();
        printf("%slayout spacing:        %d\n", indent, lay->spacing()); // layoutSpacing);

        char buf[100];
        Qt::Orientations orientations = lay->expandingDirections();
        g_snprintf(buf, 100, "%sexpandingDirections:  %s  %s\n",
              indent,
                 (orientations & Qt::Horizontal) ? "Horizontal" : "",
                 (orientations & Qt::Vertical)   ? "Vertical"   : "");
        puts(buf);

        QSize sz = lay->minimumSize();
        printf("%sminimumSize:          %d,%d\n",  indent, sz.width(), sz.height() );
        sz = lay->maximumSize();
        printf("%smaximumSize:          %d,%d\n",  indent, sz.width(), sz.height() );

        // and more
}


void reportWidgetDimensions(
      QWidget *    w,
      const char * className,
      const char * funcName,
      const char * msg)
{
   int depth = 0;
   int d1 = depth+1;

   char indent[100];
   g_snprintf(indent, 100, "%*s", d1*3, "");

    if (msg)
       printf("%s\n", msg);
    // printf("(%s::%s) Dimensions:\n", className, funcName);


    QSize sz = w->baseSize();
    printf("%sbaseSize:              %d,%d\n", indent,  sz.width(), sz.height());

    sz = w->sizeIncrement();
    printf("%ssizeIncrement:         %d,%d\n", indent, sz.width(), sz.height());

    // QSize minimumSize   = w->minimumSize();
    //    printf("%sminimumSize:    %d,%d\n", indent,  minimumSize.width(), minimumSize.height());
    printf("%sminimumSize:           %d,%d\n", indent,  w->minimumWidth(), w->minimumHeight());

    sz  = w->minimumSizeHint();
    printf("%sminimumSizeHint:       %d,%d\n", indent,  sz.width(), sz.height());

    // QSize maximumSize   = w->maximumSize();
    printf("%smaximumSize:           %d,%d\n", indent,  w->maximumWidth(), w->maximumHeight());

    // sz = w->size();
    // size of widget excluding any window frame
    printf("%ssize:                  %d,%d\n", indent,  w->width(), w->height());

    // recommended widget size
    sz = w->sizeHint();
    printf("%ssizeHint:              %d,%d\n", indent,  sz.width(), sz.height());

    // size of the widget including any window frame
    QSize frameSize     = w->frameSize();
    printf("%sframeSize:             %d,%d\n", indent,  frameSize.width(), frameSize.height());

   // both work
   // int m_left, m_right, m_top, m_bottom;
   // w->getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
   // printf("%scontents margins:      left=%d, top=%d, right=%d, bottom=%d\n", indent,
   //         m_left, m_top, m_right, m_bottom);

    QMargins contentsMargins = w->contentsMargins();
    printf("%scontentsMargins:       left=%d, top=%d, right=%d, bottom=%d\n", indent,
           contentsMargins.left(), contentsMargins.top(), contentsMargins.right(), contentsMargins.bottom() );

    printf("%shasHeightForWidth:     %s\n", indent, (w->hasHeightForWidth()) ? "True" : "False");
    printf("%supdatesEnabled:        %s\n", indent, (w->updatesEnabled()) ? "True" : "False");
    printf("%svisible:               %s\n", indent, (w->isVisible()) ? "True" : "False");
    printf("%shidden:                %s\n", indent, (w->isHidden()) ? "True" : "False");

    Qt::LayoutDirection direction = w->layoutDirection();
    char * s = NULL;
    switch(direction) {
    case Qt::LeftToRight:   s = (char *) "LeftToRight";        break;
    case Qt::RightToLeft:   s = (char *) "RightToLeft";        break;
    case Qt::LayoutDirectionAuto:  s = (char*) "LayoutDirectionAuto";        break;
    }
    printf("%slayoutDirection:       %d - %s\n", indent, direction, s);

#ifdef USELESS
    QRect childrenRect = w->childrenRect();
    printf("%schildrenRect:          x: %d, y: %d, width: %d, height: %d\n", indent,
           childrenRect.x(), childrenRect.y(), childrenRect.width(), childrenRect.height());

    // specifies a clip region for a painter
    // QRegion childrenRegion = w->childrenRegion();

    QRect frameGeometry = w->frameGeometry();
    printf("%sframeGeometry:   x: %d, y: %d, width: %d, height: %d\n", indent,
           frameGeometry.x(), frameGeometry.y(), frameGeometry.width(), frameGeometry.height());

     QRect normalGeometry = w->normalGeometry();
     printf("%snormalGeometry: x: %d, y: %d, width: %d, height: %d\n", indent,
            normalGeometry.x(), normalGeometry.y(), normalGeometry.width(), normalGeometry.height());

     QRect rect = w->rect();
     printf("%srect:                 x: %d, y: %d, width: %d, height: %d\n", indent,
            rect.x(), rect.y(), rect.width(), rect.height());

     QRect    contentsRect    = w->contentsRect();
     printf("%scontentsRect:         x: %d, y: %d, width: %d, height: %d\n", indent,
            contentsRect.x(), contentsRect.y(), contentsRect.width(), contentsRect.height());
#endif

     QSizePolicy sizePolicy = w->sizePolicy();
     reportQSizePolicy(sizePolicy, d1, "widget sizePolicy:");


     QLayout * lay = w->layout();
     if (lay) {
        printf("%sLayout:\n", indent);
        reportLayout(lay, d1+1);
     }
     else
        printf("%sNo layout set\n", indent);

     printf("\n");  fflush(stdout);
}


void reportFrameDimensions(
      QFrame * f,
      const char * className,
      const char * funcName,
      const char * msg)
{
   if (msg)
      printf("%s\n", msg);
   printf("(%s::%s) Frame Dimension Information: \n", className, funcName); fflush(stdout);

   QRect frameRect = f->frameRect();
   printf("frameRect: x: %d, y: %d, width: %d, height: %d\n",
          frameRect.x(), frameRect.y(), frameRect.width(), frameRect.height());

   QFrame::Shape shape = f->frameShape();
   char * s = NULL;
   switch(shape) {
   case QFrame::NoFrame:   s = (char*) "NoFrame";      break;
   case QFrame::Box:       s = (char*) "Box";          break;
   case QFrame::Panel:     s = (char*) "Panel";        break;
   case QFrame::StyledPanel:  s = (char*) "StyledPanel";  break;
   case QFrame::HLine:        s = (char*) "HLIine";       break;
   case QFrame::VLine:        s = (char*) "VLine";        break;
   case QFrame::WinPanel:    s = (char*) "WinPanel";     break;
   }
   printf("shape:        %d - %s\n", shape, s);

   // int style = f->frameStyle()

   printf("frameWidth:   %d\n",  f->frameWidth() );
   printf("lineWidth:    %d\n", f->lineWidth() );

   reportWidgetDimensions(f, className, funcName);
}


void reportLabelDimensions(
      QLabel * label,
      const char * className,
      const char * funcName,
      const char * msg)
{
   if (msg)
      printf("%s\n", msg);
   printf("(%s::%s) Label Dimension Information: \n", className, funcName); fflush(stdout);

   printf("indent:  %d\n", label->indent());

   reportFrameDimensions(label, className, funcName, NULL);
}


void whereIsApplication() {
   printf("Application screens\n");
   QApplication* qGuiApplication = qApp;

   QList<QScreen*> screens = qGuiApplication->screens();
   for(int ndx=0; ndx < screens.count();ndx++) {
        QScreen* screen = screens.at(ndx);
        QRect geometry  = screen->availableGeometry();
        QSize availableSize = screen->availableSize();
        QSize size = screen->size();
        QSize virtualSize = screen->virtualSize();
        printf("screen[%d]\n", ndx);
        printf("  geometry: left=%d, top=%d, right=%d, bottom=%d\n",
               geometry.left(), geometry.top(), geometry.right(), geometry.bottom() );
        printf("  availableSize: %d,%d\n", availableSize.width(), availableSize.height());
        printf("  Size: %d,%d\n", size.width(), size.height());
        printf("  virtualSize: %d,%d\n", virtualSize.width(), virtualSize.height());
   }

   QDesktopWidget * desktopWidget = QApplication::desktop();
   printf("Desktop widget width, height: %d,%d\n", desktopWidget->width(),  desktopWidget->height() );
#ifdef FOR_TESTING
   printf("screen count: %d\n", desktopWidget->screenCount() );
   printf("is virtual desktop: %s\n", SBOOL( desktopWidget->isVirtualDesktop() ) );
   QRect geometry  = desktopWidget->availableGeometry();
   printf("  geometry: left=%d, top=%d, right=%d, bottom=%d\n",
          geometry.left(), geometry.top(), geometry.right(), geometry.bottom() );
#endif
}


void whereAmI(QWidget * w, const char * msg) {
   if (msg)
      printf("%s\n", msg);
   else
      printf("This widget:\n");

   printf("   x,y (upper left == pos) =%d,%d\n", w->x(), w->y());
   printf("   width,height = %d,%d\n", w->width(), w->height() );
   // QScreen * screen = w->screen();   // bit in 5.12
   printf("   isWindow(): %s\n", SBOOL(w->isWindow() )) ;
   if (w->isWindow() ) {
#ifdef OUT
      QWindow* wind = dynamic_cast<QWindow*>(w);
      QScreen * screen = wind->screen();  // this is the line that causes crash
      QRect geometry  = screen->availableGeometry();
      printf("  screen geometry: left=%d, top=%d, right=%d, bottom=%d\n",
             geometry.left(), geometry.top(), geometry.right(), geometry.bottom() );
#endif
   }
}

