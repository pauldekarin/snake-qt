#pragma once

#include <QtWidgets/QProxyStyle>
#include <QtGui/QPalette>
#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <iostream>

class ProxyStyle : public QProxyStyle{
public:
    void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, 
                      bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override {
        
        // painter->setBrush(Qt::BrushStyle::Dense6Pattern);
        
        QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
    }
};