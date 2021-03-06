/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtNetwork module of the Qt Toolkit.
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

#ifndef QNETWORKACCESSCACHE_P_H
#define QNETWORKACCESSCACHE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the Network Access API.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qobject.h"
#include "QtCore/qbasictimer.h"
#include "QtCore/qbytearray.h"
#include "QtCore/qhash.h"
#include "QtCore/qmetatype.h"

QT_BEGIN_NAMESPACE

class QNetworkRequest;
class QUrl;

class QNetworkAccessCache: public QObject
{
    Q_OBJECT
public:
    struct Node;
    typedef QHash<QByteArray, Node> NodeHash;

    class CacheableObject
    {
        friend class QNetworkAccessCache;
        QByteArray key;
        bool expires;
        bool shareable;
    public:
        CacheableObject();
        virtual ~CacheableObject();
        virtual void dispose() = 0;
        inline QByteArray cacheKey() const { return key; }

    protected:
        void setExpires(bool enable);
        void setShareable(bool enable);
    };

    QNetworkAccessCache();
    ~QNetworkAccessCache();

    void clear();

    void addEntry(const QByteArray &key, CacheableObject *entry);
    bool hasEntry(const QByteArray &key) const;
    bool requestEntry(const QByteArray &key, QObject *target, const char *member);
    CacheableObject *requestEntryNow(const QByteArray &key);
    void releaseEntry(const QByteArray &key);
    void removeEntry(const QByteArray &key);

signals:
    void entryReady(QNetworkAccessCache::CacheableObject *);

protected:
    void timerEvent(QTimerEvent *);

private:
    // idea copied from qcache.h
    NodeHash hash;
    Node *oldest;
    Node *newest;

    QBasicTimer timer;

    void linkEntry(const QByteArray &key);
    bool unlinkEntry(const QByteArray &key);
    void updateTimer();
    bool emitEntryReady(Node *node, QObject *target, const char *member);
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QNetworkAccessCache::CacheableObject*)

#endif
