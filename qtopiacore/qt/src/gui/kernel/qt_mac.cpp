/****************************************************************************
 **
 ** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
 **
 ** This file is part of the QtGui module of the Qt Toolkit.
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
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

#include <private/qt_mac_p.h>
#include <private/qpixmap_mac_p.h>
#include <private/qnativeimage_p.h>
#include <qdebug.h>

QT_BEGIN_NAMESPACE
#ifdef QT_MAC_USE_COCOA
static CTFontRef CopyCTThemeFont(ThemeFontID themeID)
{
    CTFontUIFontType ctID = HIThemeGetUIFontType(themeID);
    return CTFontCreateUIFontForLanguage(ctID, 0, 0);
}
#endif

QFont qfontForThemeFont(ThemeFontID themeID)
{
#ifndef QT_MAC_USE_COCOA
    static const ScriptCode Script = smRoman;
    Str255 f_name;
    SInt16 f_size;
    Style f_style;
    GetThemeFont(themeID, Script, f_name, &f_size, &f_style);
    extern QString qt_mac_from_pascal_string(const Str255); //qglobal.cpp
    return QFont(qt_mac_from_pascal_string(f_name), f_size,
                 (f_style & ::bold) ? QFont::Bold : QFont::Normal,
                 (bool)(f_style & ::italic));
#else
    QCFType<CTFontRef> ctfont = CopyCTThemeFont(themeID);
    QString familyName = QCFString(CTFontCopyFamilyName(ctfont));
    QCFType<CFDictionaryRef> dict = CTFontCopyTraits(ctfont);
    CFNumberRef num = static_cast<CFNumberRef>(CFDictionaryGetValue(dict, kCTFontWeightTrait));
    float fW;
    CFNumberGetValue(num, kCFNumberFloat32Type, &fW);
    QFont::Weight wght = fW > 0. ? QFont::Bold : QFont::Normal;
    num = static_cast<CFNumberRef>(CFDictionaryGetValue(dict, kCTFontSlantTrait));
    CFNumberGetValue(num, kCFNumberFloatType, &fW);
    bool italic = (fW != 0.0);
    return QFont(familyName, CTFontGetSize(ctfont), wght, italic);
#endif
}

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
static QColor qcolorFromCGColor(CGColorRef cgcolor)
{
    QColor pc;
    CGColorSpaceModel model = CGColorSpaceGetModel(CGColorGetColorSpace(cgcolor));
    const CGFloat *components = CGColorGetComponents(cgcolor);
    if (model == kCGColorSpaceModelRGB) {
        pc.setRgbF(components[0], components[1], components[2], components[3]);
    } else if (model == kCGColorSpaceModelCMYK) {
        pc.setCmykF(components[0], components[1], components[2], components[3]);
    } else {
        // Colorspace we can't deal with.
        qWarning("Qt: qcolorFromCGColor: cannot convert from colorspace model: %d", model);
        Q_ASSERT(false);
    }
    return pc;
}

static inline QColor leopardBrush(ThemeBrush brush)
{
    QCFType<CGColorRef> cgClr = 0;
    HIThemeBrushCreateCGColor(brush, &cgClr);
    return qcolorFromCGColor(cgClr);
}
#endif

QColor qcolorForTheme(ThemeBrush brush)
{
#ifndef QT_MAC_USE_COCOA
#  if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
    if (QSysInfo::MacintoshVersion >= QSysInfo::MV_10_5) {
        return leopardBrush(brush);
    } else
#  endif
    {
        RGBColor rgbcolor;
        GetThemeBrushAsColor(brush, 32, true, &rgbcolor);
        return QColor(rgbcolor.red / 256, rgbcolor.green / 256, rgbcolor.blue / 256);
    }
#else
    return leopardBrush(brush);
#endif
}

QColor qcolorForThemeTextColor(ThemeTextColor themeColor)
{
#ifndef QT_MAC_USE_COCOA
    RGBColor c;
    GetThemeTextColor(themeColor, 32, true, &c);
    return QColor(c.red / 265, c.green / 256, c.blue / 256);
#else
    QNativeImage nativeImage(16,16, QNativeImage::systemFormat());
    CGRect cgrect = CGRectMake(0, 0, 16, 16);
    HIThemeSetTextFill(themeColor, 0, nativeImage.cg, kHIThemeOrientationNormal);
    CGContextFillRect(nativeImage.cg, cgrect);
    return QColor(nativeImage.image.pixel(0 , 0));
#endif
}
QT_END_NAMESPACE