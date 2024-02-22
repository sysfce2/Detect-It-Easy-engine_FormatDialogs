﻿/* Copyright (c) 2019-2023 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef DUMPPROCESS_H
#define DUMPPROCESS_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "xbinary.h"
#ifdef USE_XPROCESS
#include "xprocess.h"
#endif

class DumpProcess : public QObject {
    Q_OBJECT

public:
    enum DT {
        DT_DUMP_DEVICE_OFFSET = 0,
        DT_PATCH_DEVICE_OFFSET,
    #ifdef USE_XPROCESS
        DT_DUMP_PROCESS_USER_ID_RAW
    #endif
    };

    struct RECORD {
        qint64 nOffset;
        qint64 nSize;
        QString sFileName;
    };

    explicit DumpProcess(QObject *pParent = nullptr);

    void setData(QIODevice *pDevice, QList<RECORD> listRecords, DT dumpType, QString sJsonFileName, XBinary::PDSTRUCT *pPdStruct);
    void setData(QIODevice *pDevice, DT dumpType, QString sJsonFileName, XBinary::PDSTRUCT *pPdStruct);
#ifdef USE_XPROCESS
    void setData(X_ID nProcessID, DT dumpType, QString sFileName, QString sJsonFileName, XBinary::PDSTRUCT *pPdStruct);
#endif
signals:
    void completed(qint64 nElapsed);
    void errorMessage(const QString &sText);

public slots:
    void process();

private:
    QIODevice *g_pDevice;
    QList<RECORD> g_listRecords;
    DT g_dumpType;
    QString g_sFileName;
    QString g_sJsonFileName;
    XBinary::PDSTRUCT *g_pPdStruct;
#ifdef USE_XPROCESS
    X_ID g_nProcessID;
#endif
};

#endif  // DUMPPROCESS_H
