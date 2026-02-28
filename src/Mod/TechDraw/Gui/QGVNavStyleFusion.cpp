/***************************************************************************
 *   Copyright (c) 2025 FreeCAD contributors                              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,   *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"
#ifndef _PreComp_
# include <QGuiApplication>
# include <QMouseEvent>
#endif

#include "QGVNavStyleFusion.h"
#include "QGVPage.h"


using namespace TechDrawGui;

namespace TechDrawGui {

QGVNavStyleFusion::QGVNavStyleFusion(QGVPage* qgvp) :
    QGVNavStyle(qgvp)
{
}

QGVNavStyleFusion::~QGVNavStyleFusion()
{
}

void QGVNavStyleFusion::handleKeyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
    // Fusion: Shift release does NOT stop panning/zooming while MMB is held
    // All actions stop only when MMB is released
}

void QGVNavStyleFusion::handleMousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void QGVNavStyleFusion::handleMouseMoveEvent(QMouseEvent *event)
{
    if (getViewer()->isBalloonPlacing()) {
        balloonCursorMovement(event);
        return;
    }

    if (QGuiApplication::mouseButtons() & Qt::MiddleButton) {
        if (QGuiApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {
            // Rotate mode: Shift + MMB
            // TechDraw is 2D so we map rotate to zoom (same as SolidWorks Shift+MMB zoom)
            // to give meaningful behavior in TechDraw context
            if (zoomingActive) {
                zoom(mouseZoomFactor(event->pos()));
            } else {
                startZoom(event->pos());
            }
        } else {
            // Pan mode: MMB
            if (panningActive) {
                pan(event->pos());
            } else {
                startPan(event->pos());
            }
        }
        event->accept();
    }
}

void QGVNavStyleFusion::handleMouseReleaseEvent(QMouseEvent *event)
{
    if (getViewer()->isBalloonPlacing()) {
        placeBalloon(event->pos());
    }

    if (event->button() == Qt::MiddleButton) {
        // Stop pan or zoom when MMB is released (Shift state doesn't matter)
        if (panningActive) {
            stopPan();
            event->accept();
        }
        if (zoomingActive) {
            stopZoom();
            event->accept();
        }
    }
}

}  // namespace TechDrawGui
