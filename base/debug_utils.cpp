/* debug_utils.cpp
 *
 * <copyright>
 * Copyright (C) 2014-2015 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#include <string.h>
#include <stdio.h>

#include <QtWidgets/QWidget>
#include <QtWidgets/QLayout>
#include <QtWidgets/QFrame>
#include <QtCore/QSize>
#include <QtCore/QRect>
#include <QtGui/QRegion>
#include <QtCore/QMargins>
#include <QtWidgets/QLabel>


void reportWidgetChildren(QWidget * w) {
    printf("============> Children of widget: \n");  fflush(stdout);
    QObjectList  childs = w->children();
    for (int ndx = 0; ndx < childs.size(); ndx++) {
        QObject* curobj = childs.at(ndx);
        QString name   = curobj->objectName();
       // cout << "Child: " << QString("xxx") <<  endl;   // no match for operator<<
        QByteArray ba = name.toLatin1();
        const char * s = ba.data();
        printf("   Child: %s\n", s); fflush(stdout);
    }
}





void reportWidgetDimensions(QWidget * w, const char * className, const char * funcName) {
    // printf("(%s) ========================================> \n", __func__); fflush(stdout);
    int m_left, m_right, m_top, m_bottom;
    w->getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
    printf("(%s::%s) margins: left=%d, top=%d, right=%d, bottom=%d)\n",
           className, funcName, m_left, m_top, m_right, m_bottom);

    QSize baseSize = w->baseSize();
    printf("baseSize:        %d,%d\n",  baseSize.width(), baseSize.height());

    QRect childrenRect = w->childrenRect();
    printf("childrenRect:    x: %d, y: %d, width: %d, height: %d\n",
           childrenRect.x(), childrenRect.y(), childrenRect.width(), childrenRect.height());

    // specifies a clip region for a painter
    // QRegion childrenRegion = w->childrenRegion();

    QRect frameGeometry = w->frameGeometry();
    printf("frameGeometry:   x: %d, y: %d, width: %d, height: %d\n",
           frameGeometry.x(), frameGeometry.y(), frameGeometry.width(), frameGeometry.height());

    // size of the widget including any window frame
    QSize frameSize     = w->frameSize();
    printf("frameSize:       %d,%d\n",  frameSize.width(), frameSize.height());


    QSize maximumSize   = w->maximumSize();
    printf("maximumSize:     %d,%d\n",  maximumSize.width(), maximumSize.height());
    // int   maximumWidth = w->maximumWidth();
    // int   maximumHeight = w->maximumHeight();

     QSize minimumSize   = w->minimumSize();
     printf("minimumSize:    %d,%d\n",  minimumSize.width(), minimumSize.height());
     // int   minimumWidth = w->minimumWidth();

     QSize minimumSizeHint   = w->minimumSizeHint();
     printf("minimumSizeHint:%d,%d\n",  minimumSizeHint.width(), minimumSizeHint.height());

     QRect normalGeometry = w->normalGeometry();
     printf("normalGeometry: x: %d, y: %d, width: %d, height: %d\n",
            normalGeometry.x(), normalGeometry.y(), normalGeometry.width(), normalGeometry.height());

     QRect rect = w->rect();
     printf("rect:           x: %d, y: %d, width: %d, height: %d\n",
            rect.x(), rect.y(), rect.width(), rect.height());

     QSize size = w->size();
     printf("size:           %d,%d\n",  size.width(), size.height());
     // int width = w->width();
     // int height = w->height();

     QSize sizeHint = w->sizeHint();
     printf("sizeHint:       %d,%d\n",  sizeHint.width(), sizeHint.height());

     QMargins contentsMargins = w->contentsMargins();
     printf("contentsMargins: left: %d, top: %d, right: %d, bottom: %d\n",
            contentsMargins.left(), contentsMargins.top(), contentsMargins.right(), contentsMargins.bottom() );

     QRect    contentsRect    = w->contentsRect();
     printf("contentsRect:   x: %d, y: %d, width: %d, height: %d\n",
            contentsRect.x(), contentsRect.y(), contentsRect.width(), contentsRect.height());

     bool     hasHeightForWidth = w->hasHeightForWidth();
     printf("hasHeightForWidth: %s\n", (hasHeightForWidth) ? "True" : "False");

     QLayout * lay = w->layout();
     if (lay) {

        QMargins layoutContentsMargins = lay->contentsMargins();
        printf("layoutContentsMargins: left: %d, top: %d, right: %d, bottom: %d\n",
               layoutContentsMargins.left(), layoutContentsMargins.top(),
               layoutContentsMargins.right(), layoutContentsMargins.bottom() );

        QRect    layoutContentsRect  = lay->contentsRect();
        printf("layoutContentsRect: x: %d, y: %d, width: %d, height: %d\n",
               layoutContentsRect.x(), layoutContentsRect.y(), layoutContentsRect.width(), layoutContentsRect.height());

        // spacing between the widgets inside the layout
        int layoutSpacing = lay->spacing();
        printf("layout spacing: %d\n", layoutSpacing);
     }
     else {
        printf("No layout set\n");
     }
     fflush(stdout);

}

void reportFrameDimensions(QFrame * f, const char * className, const char * funcName) {
   printf("(%s::%s) Frame Dimension Information: \n", className, funcName); fflush(stdout);

   QRect frameRect = f->frameRect();
   printf("frameRect: x: %d, y: %d, width: %d, height: %d\n",
          frameRect.x(), frameRect.y(), frameRect.width(), frameRect.height());

   reportWidgetDimensions(f, className, funcName);
}

void reportLabelDimensions(QLabel * label, const char * className, const char * funcName) {
   printf("(%s::%s) Label Dimension Information: \n", className, funcName); fflush(stdout);

   printf("indent:  %d\n", label->indent());
   printf("margin:  %d\n", label->margin());

   reportFrameDimensions(label, className, funcName);
}


