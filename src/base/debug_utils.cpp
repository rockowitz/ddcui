// debug_utils.cpp

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

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


void reportPolicy(   QSizePolicy::Policy  hp,
      const char * msg = nullptr)
{
    if (msg)
       printf("%s\n", msg);

    // Two ways of looking at the policy field, as bits or as enum

    printf("policy:  %s %s %s %s\n",
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

    printf("     %s\n", s);
}


void reportQSizePolicy(
      QSizePolicy p ,
    //        const char * className,
      //      const char * funcName,
            const char * msg = nullptr)
{
    if (msg)
       printf("%s\n", msg);

    int x;
    char buf[100];
    Qt::Orientations orientations = p.expandingDirections();
    snprintf(buf, 100, "expandingDirections:  %s  %s\n",
             (orientations & Qt::Horizontal) ? "Horizontal" : "",
             (orientations & Qt::Vertical)   ? "Vertical"   : "");
    puts(buf);
    printf("horizontalStretch:  %d\n", p.horizontalStretch());
    printf("verticalStretch:    %d\n", p.verticalStretch());

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
    printf("controlType:   %s\n", s);

    QSizePolicy::Policy  hp = p.horizontalPolicy();
    reportPolicy(hp, "Horizontal Policy:");
    reportPolicy(p.verticalPolicy(), "Vertical Policy");

    // TODO: additional
    //    ->ExpandingDirections();
    //     more?
}



void reportLayout(
      QLayout * lay)
{
   QMargins layoutContentsMargins = lay->contentsMargins();
        printf("layoutContentsMargins: left: %d, top: %d, right: %d, bottom: %d\n",
               layoutContentsMargins.left(), layoutContentsMargins.top(),
               layoutContentsMargins.right(), layoutContentsMargins.bottom() );
#ifdef NOT_USEFUL
        QRect    layoutContentsRect  = lay->contentsRect();
        printf("layoutContentsRect: x: %d, y: %d, width: %d, height: %d\n",
               layoutContentsRect.x(), layoutContentsRect.y(), layoutContentsRect.width(), layoutContentsRect.height());
#endif
        // spacing between the widgets inside the layout
        // int layoutSpacing = lay->spacing();
        printf("layout spacing:        %d\n", lay->spacing()); // layoutSpacing);

        char buf[100];
        Qt::Orientations orientations = lay->expandingDirections();
        snprintf(buf, 100, "expandingDirections:  %s  %s\n",
                 (orientations & Qt::Horizontal) ? "Horizontal" : "",
                 (orientations & Qt::Vertical)   ? "Vertical"   : "");
        puts(buf);

        QSize sz = lay->minimumSize();
        printf("minimumSize:          %d,%d\n",  sz.width(), sz.height() );
        sz = lay->maximumSize();
        printf("maximumSize:          %d,%d\n",  sz.width(), sz.height() );

        // and more
}


void reportWidgetDimensions(
      QWidget *    w,
      const char * className,
      const char * funcName,
      const char * msg = nullptr)
{
    if (msg)
       printf("%s\n", msg);
    printf("(%s::%s) Dimensions:\n", className, funcName);

    QSize sz = w->baseSize();
    printf("baseSize:             %d,%d\n",  sz.width(), sz.height());

    sz = w->sizeIncrement();
    printf("sizeIncrement:        %d,%d\n", sz.width(), sz.height());

    // QSize minimumSize   = w->minimumSize();
    //    printf("minimumSize:    %d,%d\n",  minimumSize.width(), minimumSize.height());
    printf("minimumSize:          %d,%d\n",  w->minimumWidth(), w->minimumHeight());

    sz  = w->minimumSizeHint();
    printf("minimumSizeHint:      %d,%d\n",  sz.width(), sz.height());

    // QSize maximumSize   = w->maximumSize();
    printf("maximumSize:          %d,%d\n",  w->maximumWidth(), w->maximumHeight());

    // sz = w->size();
    // size of widget excluding any window frame
    printf("size:                 %d,%d\n",  w->width(), w->height());

    // recommended widget size
    sz = w->sizeHint();
    printf("sizeHint:             %d,%d\n",  sz.width(), sz.height());

    // size of the widget including any window frame
    QSize frameSize     = w->frameSize();
    printf("frameSize:             %d,%d\n",  frameSize.width(), frameSize.height());


    int m_left, m_right, m_top, m_bottom;
    w->getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
    printf("(margins:              left=%d, top=%d, right=%d, bottom=%d)\n",
           m_left, m_top, m_right, m_bottom);

    QMargins contentsMargins = w->contentsMargins();
    printf("contentsMargins: left: %d, top: %d, right: %d, bottom: %d\n",
           contentsMargins.left(), contentsMargins.top(), contentsMargins.right(), contentsMargins.bottom() );

    printf("hasHeightForWidth:     %s\n", (w->hasHeightForWidth()) ? "True" : "False");
    printf("updatesEnabled:        %s\n", (w->updatesEnabled()) ? "True" : "False");
    printf("visible:               %s\n", (w->isVisible()) ? "True" : "False");
    printf("hidden:                %s\n", (w->isHidden()) ? "True" : "False");

    Qt::LayoutDirection direction = w->layoutDirection();
    char * s = NULL;
    switch(direction) {
    case Qt::LeftToRight:   s = (char *) "LeftToRight";        break;
    case Qt::RightToLeft:   s = (char *) "RightToLeft";        break;
    case Qt::LayoutDirectionAuto:  s = (char*) "LayoutDirectionAuto";        break;
    }
    printf("layoutDirection:      %d - %s\n", direction, s);

#ifdef USELESS
    QRect childrenRect = w->childrenRect();
    printf("childrenRect:          x: %d, y: %d, width: %d, height: %d\n",
           childrenRect.x(), childrenRect.y(), childrenRect.width(), childrenRect.height());

    // specifies a clip region for a painter
    // QRegion childrenRegion = w->childrenRegion();

    QRect frameGeometry = w->frameGeometry();
    printf("frameGeometry:   x: %d, y: %d, width: %d, height: %d\n",
           frameGeometry.x(), frameGeometry.y(), frameGeometry.width(), frameGeometry.height());

     QRect normalGeometry = w->normalGeometry();
     printf("normalGeometry: x: %d, y: %d, width: %d, height: %d\n",
            normalGeometry.x(), normalGeometry.y(), normalGeometry.width(), normalGeometry.height());

     QRect rect = w->rect();
     printf("rect:           x: %d, y: %d, width: %d, height: %d\n",
            rect.x(), rect.y(), rect.width(), rect.height());

     QRect    contentsRect    = w->contentsRect();
     printf("contentsRect:   x: %d, y: %d, width: %d, height: %d\n",
            contentsRect.x(), contentsRect.y(), contentsRect.width(), contentsRect.height());
#endif

     QSizePolicy policy = w->sizePolicy();
     reportQSizePolicy(policy, "widget sizePolicy:");

     QLayout * lay = w->layout();
     if (lay)
        reportLayout(lay);
     else
        printf("No layout set\n");

     QSizePolicy sizePolicy = w->sizePolicy();
     reportQSizePolicy(sizePolicy);

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

   printf("lineWidth:   %d\n", f->lineWidth() );


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
   printf("margin:  %d\n", label->margin());

   reportFrameDimensions(label, className, funcName, NULL);
}

