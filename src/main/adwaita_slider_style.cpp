#include "main/adwaita_slider_style.h"

#include <algorithm>

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPalette>

AdwaitaSliderStyle::AdwaitaSliderStyle(QStyle *baseStyle)
   : QProxyStyle(baseStyle) {}

int AdwaitaSliderStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const {
   switch (metric) {
   case PM_SliderThickness:
      return 22;
   case PM_SliderLength:
      return 22;
   default:
      break;
   }

   return QProxyStyle::pixelMetric(metric, option, widget);
}

QRectF AdwaitaSliderStyle::sliderGrooveRect(const QStyleOptionSlider *option, const QWidget *widget) const {
   QRect groove = QProxyStyle::subControlRect(CC_Slider, option, SC_SliderGroove, widget);
   if (groove.isNull())
      groove = option->rect;

   constexpr qreal grooveThickness = 6.0;
   if (option->orientation == Qt::Horizontal) {
      qreal centerY = option->rect.center().y();
      return QRectF(groove.left(), centerY - grooveThickness / 2.0, groove.width(), grooveThickness);
   }

   qreal centerX = option->rect.center().x();
   return QRectF(centerX - grooveThickness / 2.0, groove.top(), grooveThickness, groove.height());
}

QRectF AdwaitaSliderStyle::sliderHandleRect(const QStyleOptionSlider *option, const QWidget *widget) const {
   QRect baseHandle = QProxyStyle::subControlRect(CC_Slider, option, SC_SliderHandle, widget);
   if (baseHandle.isNull())
      baseHandle = option->rect;

   constexpr qreal diameter = 18.0;
   if (option->orientation == Qt::Horizontal) {
      qreal centerX = baseHandle.center().x();
      qreal centerY = option->rect.center().y();
      return QRectF(centerX - diameter / 2.0, centerY - diameter / 2.0, diameter, diameter);
   }

   qreal centerX = option->rect.center().x();
   qreal centerY = baseHandle.center().y();
   return QRectF(centerX - diameter / 2.0, centerY - diameter / 2.0, diameter, diameter);
}

QColor AdwaitaSliderStyle::blendColors(const QColor &a, const QColor &b, qreal ratio) {
   ratio = std::clamp(ratio, 0.0, 1.0);
   QColor result;
   result.setRedF(a.redF() * (1.0 - ratio) + b.redF() * ratio);
   result.setGreenF(a.greenF() * (1.0 - ratio) + b.greenF() * ratio);
   result.setBlueF(a.blueF() * (1.0 - ratio) + b.blueF() * ratio);
   result.setAlphaF(a.alphaF() * (1.0 - ratio) + b.alphaF() * ratio);
   return result;
}

void AdwaitaSliderStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const {
   if (control != CC_Slider) {
      QProxyStyle::drawComplexControl(control, option, painter, widget);
      return;
   }

   const auto *slider = qstyleoption_cast<const QStyleOptionSlider *>(option);
   if (!slider) {
      QProxyStyle::drawComplexControl(control, option, painter, widget);
      return;
   }

   painter->save();
   painter->setRenderHint(QPainter::Antialiasing, true);

   const QRectF grooveRect = sliderGrooveRect(slider, widget);
   const QRectF handleRect = sliderHandleRect(slider, widget);
   const qreal radius = grooveRect.height() / 2.0;

   QColor windowColor = slider->palette.color(QPalette::Window);
   QColor baseColor = slider->palette.color(QPalette::Base);
   QColor highlightColor = slider->palette.color(QPalette::Highlight);
   QColor disabledColor = slider->palette.color(QPalette::Mid);

   QColor grooveColor = blendColors(baseColor, windowColor, 0.6);
   grooveColor.setAlphaF(slider->state & State_Enabled ? 0.55 : 0.35);

   QColor activeColor = highlightColor;
   if (!(slider->state & State_Enabled))
      activeColor = blendColors(highlightColor, disabledColor, 0.65);

   QColor handleColor;
   if (windowColor.lightnessF() < 0.4)
      handleColor = blendColors(baseColor, QColor(Qt::white), 0.35);
   else
      handleColor = blendColors(baseColor, QColor(Qt::black), 0.08);

   QColor handleOutline = blendColors(handleColor, slider->palette.color(QPalette::Shadow), 0.35);

   const bool handleActive = (slider->state & (State_Sunken | State_On)) || (slider->activeSubControls & SC_SliderHandle);
   const bool handleHovered = slider->state & State_MouseOver;
   if (handleActive) {
      handleColor = blendColors(handleColor, highlightColor, 0.65);
      handleOutline = highlightColor.darker(120);
   } else if (handleHovered) {
      handleColor = blendColors(handleColor, highlightColor, 0.35);
      handleOutline = blendColors(handleOutline, highlightColor, 0.40);
   }

   QPainterPath groovePath;
   groovePath.addRoundedRect(grooveRect, radius, radius);
   painter->setPen(Qt::NoPen);
   painter->setBrush(grooveColor);
   painter->drawPath(groovePath);

   QRectF activeRect = grooveRect;
   if (slider->orientation == Qt::Horizontal) {
      if (slider->upsideDown)
         activeRect.setLeft(handleRect.center().x());
      else
         activeRect.setRight(handleRect.center().x());
   } else {
      if (slider->upsideDown)
         activeRect.setTop(handleRect.center().y());
      else
         activeRect.setBottom(handleRect.center().y());
   }

   if (activeRect.isValid() && !qFuzzyIsNull(activeRect.width()) && !qFuzzyIsNull(activeRect.height())) {
      QPainterPath activePath;
      const qreal activeRadius = std::min(activeRect.width(), activeRect.height()) / 2.0;
      activePath.addRoundedRect(activeRect, activeRadius, activeRadius);
      painter->setBrush(activeColor);
      painter->drawPath(activePath);
   }

   painter->setBrush(handleColor);
   painter->setPen(QPen(handleOutline, 1.2));
   painter->drawEllipse(handleRect);

   painter->restore();

   if (slider->subControls & SC_SliderTickmarks) {
      QStyleOptionSlider tickOption(*slider);
      tickOption.subControls = SC_SliderTickmarks;
      QProxyStyle::drawComplexControl(control, &tickOption, painter, widget);
   }
}

