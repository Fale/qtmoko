/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT_NO_DIRECTFB

#include "qdirectfbscreen.h"
#include "qdirectfbpaintdevice.h"

QDirectFBPaintDevice::~QDirectFBPaintDevice()
{
    delete lockedImage;
}


IDirectFBSurface *QDirectFBPaintDevice::directFBSurface() const
{
    return dfbSurface;
}


// Locks the dfb surface and creates a QImage (lockedImage) from the pointer
void QDirectFBPaintDevice::lockDirectFB() {

    if (lockedImage)
        return; // Already locked

    void *mem;
    int w, h;
    DFBResult result = dfbSurface->Lock(dfbSurface, DSLF_WRITE, &mem, &bpl);
    if (result != DFB_OK || !mem) {
        DirectFBError("QDirectFBPixmapData::buffer()", result);
        return;
    }

    dfbSurface->GetSize(dfbSurface, &w, &h);
    lockedImage = new QImage(static_cast<uchar*>(mem), w, h, bpl,
                             QDirectFBScreen::getImageFormat(dfbSurface));
}


void QDirectFBPaintDevice::unlockDirectFB()
{
    if (!lockedImage || !QDirectFBScreen::instance())
        return;

    dfbSurface->Unlock(dfbSurface);
    delete lockedImage;
    lockedImage = 0;
}


void* QDirectFBPaintDevice::memory() const
{
    QDirectFBPaintDevice* that = const_cast<QDirectFBPaintDevice*>(this);
    that->lockDirectFB();
    Q_ASSERT(that->lockedImage);
    return that->lockedImage->bits();
}


QImage::Format QDirectFBPaintDevice::format() const
{
    return QDirectFBScreen::getImageFormat(dfbSurface);
}


int QDirectFBPaintDevice::bytesPerLine() const
{
    if (bpl == -1) {
        // Can only get the stride when we lock the surface
        Q_ASSERT(!lockedImage);
        QDirectFBPaintDevice* that = const_cast<QDirectFBPaintDevice*>(this);
        that->lockDirectFB();
        Q_ASSERT(bpl != -1);
        that->unlockDirectFB();
    }
    return bpl;
}


QSize QDirectFBPaintDevice::size() const
{
    int w, h;
    dfbSurface->GetSize(dfbSurface, &w, &h);
    return QSize(w, h);
}

int QDirectFBPaintDevice::metric(QPaintDevice::PaintDeviceMetric metric) const
{
    if (!dfbSurface)
        return 0;

    int w, h;
    dfbSurface->GetSize(dfbSurface, &w, &h);

    switch (metric) {
    case QPaintDevice::PdmWidth:
        return w;
    case QPaintDevice::PdmHeight:
        return h;
    case QPaintDevice::PdmWidthMM:
        return (w * 1000) / dotsPerMeterX();
    case QPaintDevice::PdmHeightMM:
        return (h * 1000) / dotsPerMeterY();
    case QPaintDevice::PdmPhysicalDpiX:
    case QPaintDevice::PdmDpiX:
        return (dotsPerMeterX() * 254) / 10000; // 0.0254 meters-per-inch
    case QPaintDevice::PdmPhysicalDpiY:
    case QPaintDevice::PdmDpiY:
        return (dotsPerMeterY() * 254) / 10000; // 0.0254 meters-per-inch
    case QPaintDevice::PdmDepth:
        DFBSurfacePixelFormat format;
        dfbSurface->GetPixelFormat(dfbSurface, &format);
        return QDirectFBScreen::depth(format);
    case QPaintDevice::PdmNumColors: {
       if (lockedImage)
            return lockedImage->numColors();

        DFBResult result;
        IDirectFBPalette *palette = 0;
        unsigned int numColors = 0;

        result = dfbSurface->GetPalette(dfbSurface, &palette);
        if ((result != DFB_OK) || !palette)
            return 0;

        result = palette->GetSize(palette, &numColors);
        palette->Release(palette);
        if (result != DFB_OK)
            return 0;

        return numColors;
    }
    default:
        qCritical("QDirectFBPaintDevice::metric(): Unhandled metric!");
        return 0;
    }
}

#endif
