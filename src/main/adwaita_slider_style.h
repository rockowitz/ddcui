#pragma once

#include <QtCore/QRectF>
#include <QtGui/QColor>
#include <QtWidgets/QProxyStyle>
#include <QtWidgets/QStyleOptionSlider>

class AdwaitaSliderStyle : public QProxyStyle {
public:
   explicit AdwaitaSliderStyle(QStyle *baseStyle = nullptr);

   int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override;
   void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const override;

private:
   QRectF sliderGrooveRect(const QStyleOptionSlider *option, const QWidget *widget) const;
   QRectF sliderHandleRect(const QStyleOptionSlider *option, const QWidget *widget) const;
   static QColor blendColors(const QColor &a, const QColor &b, qreal ratio);
};

