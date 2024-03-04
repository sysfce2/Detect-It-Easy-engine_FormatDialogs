/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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
#include "dumpprocess.h"

DumpProcess::DumpProcess(QObject *pParent) : QObject(pParent)
{
    g_pPdStruct = nullptr;
#ifdef USE_XPROCESS
    g_nProcessID = 0;
    g_nSize = 0;
    g_nAddress = 0;
#ifdef Q_OS_WIN
    g_fixDumpOptions = {};
#endif
#endif
}

void DumpProcess::setData(QIODevice *pDevice, QList<RECORD> listRecords, DT dumpType, QString sJsonFileName, XBinary::PDSTRUCT *pPdStruct)
{
    this->g_pDevice = pDevice;
    this->g_listRecords = listRecords;
    this->g_dumpType = dumpType;
    this->g_sJsonFileName = sJsonFileName;
    this->g_pPdStruct = pPdStruct;
}

void DumpProcess::setData(QIODevice *pDevice, DT dumpType, QString sJsonFileName, XBinary::PDSTRUCT *pPdStruct)
{
    this->g_pDevice = pDevice;
    this->g_dumpType = dumpType;
    this->g_sJsonFileName = sJsonFileName;
    this->g_pPdStruct = pPdStruct;
}
#ifdef USE_XPROCESS
void DumpProcess::setData(X_ID nProcessID, XADDR nAddress, qint64 nSize, DT dumpType, QString sFileName, QString sJsonFileName, XBinary::PDSTRUCT *pPdStruct)
{
    this->g_nProcessID = nProcessID;
    this->g_nAddress = nAddress;
    this->g_nSize = nSize;
    this->g_dumpType = dumpType;
    this->g_sFileName = sFileName;
    this->g_sJsonFileName = sJsonFileName;
    this->g_pPdStruct = pPdStruct;
}
#endif
#ifdef USE_XPROCESS
#ifdef Q_OS_WIN
void DumpProcess::setData(X_ID nProcessID, XADDR nAddress, qint64 nSize, DT dumpType, QString sFileName, QString sJsonFileName,
                          const XPE::FIXDUMP_OPTIONS &fixDumpOptions, const QByteArray &baHeaders, XBinary::PDSTRUCT *pPdStruct)
{
    this->g_nProcessID = nProcessID;
    this->g_nAddress = nAddress;
    this->g_nSize = nSize;
    this->g_dumpType = dumpType;
    this->g_sFileName = sFileName;
    this->g_sJsonFileName = sJsonFileName;
    this->g_fixDumpOptions = fixDumpOptions;
    this->g_baHeaders = baHeaders;
    this->g_pPdStruct = pPdStruct;
}
#endif
#endif
void DumpProcess::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    if (g_dumpType == DT_DUMP_DEVICE_OFFSET) {
        XBinary binary(g_pDevice);

        connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

        qint32 nNumberOfRecords = g_listRecords.count();

        qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
        XBinary::setPdStructInit(g_pPdStruct, _nFreeIndex, nNumberOfRecords);

        QJsonArray jsArray;

        for (qint32 i = 0; (i < nNumberOfRecords) && (!(g_pPdStruct->bIsStop)); i++) {
            QString _sFileName = g_listRecords.at(i).sFileName;
            qint64 _nOffset = g_listRecords.at(i).nOffset;
            qint64 _nSize = g_listRecords.at(i).nSize;

            binary.dumpToFile(_sFileName, _nOffset, _nSize, g_pPdStruct);

            QJsonObject jsObject;
            jsObject.insert("offset", _nOffset);
            jsObject.insert("size", _nSize);
            jsObject.insert("filename", _sFileName);

            jsArray.append(jsObject);

            XBinary::setPdStructCurrentIncrement(g_pPdStruct, _nFreeIndex);
        }

        QJsonDocument saveFormat(jsArray);

        XBinary::writeToFile(g_sJsonFileName, saveFormat.toJson(QJsonDocument::Indented));

        XBinary::setPdStructFinished(g_pPdStruct, _nFreeIndex);
    } else if (g_dumpType == DT_PATCH_DEVICE_OFFSET) {
        XBinary binary(g_pDevice);

        connect(&binary, SIGNAL(errorMessage(QString)), this, SIGNAL(errorMessage(QString)));

        QByteArray baJson = XBinary::readFile(g_sJsonFileName);

        QJsonDocument jsDoc = QJsonDocument::fromJson(baJson);

        if (jsDoc.isArray()) {
            qint32 nNumberOfRecords = jsDoc.array().count();

            qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
            XBinary::setPdStructInit(g_pPdStruct, _nFreeIndex, nNumberOfRecords);

            for (qint32 i = 0; (i < nNumberOfRecords) && (!(g_pPdStruct->bIsStop)); i++) {
                QJsonObject jsObject = jsDoc.array().at(i).toObject();

                QString _sFileName = jsObject.value("filename").toString();
                qint64 _nOffset = jsObject.value("offset").toVariant().toLongLong();
                qint64 _nSize = jsObject.value("size").toVariant().toLongLong();
                qint64 _nFileSize = QFileInfo(_sFileName).size();

                if ((_nOffset > _nFileSize) || (_nOffset <= 0)) {
                    emit errorMessage(tr("Invalid offset"));
                    break;
                }

                if ((_nOffset + _nSize > _nFileSize) || (_nSize <= 0) || (_nFileSize < _nSize)) {
                    emit errorMessage(tr("Invalid size"));
                    break;
                }

                // TODO Check params

                QString sFileMD5 = XBinary::getHash(XBinary::HASH_MD5, _sFileName, g_pPdStruct);
                QString sOriginMD5 = binary.getHash(XBinary::HASH_MD5, _nOffset, _nSize, g_pPdStruct);

                if (sFileMD5 != sOriginMD5) {
                    if (!binary.patchFromFile(_sFileName, _nOffset, _nSize, g_pPdStruct)) {
                        emit errorMessage(QString("%1 :").arg(tr("Cannot read file"), _sFileName));
                        break;
                    }
                }

                XBinary::setPdStructCurrentIncrement(g_pPdStruct, _nFreeIndex);
            }

            XBinary::setPdStructFinished(g_pPdStruct, _nFreeIndex);
        }
    }
#ifdef USE_XPROCESS
#ifdef Q_OS_WIN
    else if ((g_dumpType == DT_DUMP_PROCESS_USER_READPROCESSMEMORY_RAWDUMP) || (g_dumpType == DT_DUMP_PROCESS_USER_READPROCESSMEMORY_REBUILD)) {
        qint32 _nFreeIndex = XBinary::getFreeIndex(g_pPdStruct);
        XBinary::setPdStructInit(g_pPdStruct, _nFreeIndex, 0);

        QString sRawDmpFile;

        if (g_dumpType == DT_DUMP_PROCESS_USER_READPROCESSMEMORY_RAWDUMP) {
            sRawDmpFile = g_sFileName;
        } else if (g_dumpType == DT_DUMP_PROCESS_USER_READPROCESSMEMORY_REBUILD) {
            sRawDmpFile = g_sFileName + ".raw.dmp";  // TODO save in tmp folder
        }

        if (g_nSize) {
            // TODO Open process
            X_HANDLE hProcess = OpenProcess(PROCESS_VM_READ, 0, (DWORD)g_nProcessID);

            if (hProcess != 0) {
                QFile file;
                file.setFileName(sRawDmpFile);

                if (file.open(QIODevice::ReadWrite)) {
                    file.resize(0);
                    file.resize(g_nSize);

                    char buffer[0x1000];

                    for (qint64 i = 0; i < g_nSize; i += 0x1000) {
                        qint64 nBufferSize = qMin(g_nSize - i, (qint64)0x1000);

                        SIZE_T nNumberOfBytes = 0;
                        if (ReadProcessMemory(hProcess, (LPCVOID)(g_nAddress + i), buffer, nBufferSize, &nNumberOfBytes)) {
                            if (nNumberOfBytes == nBufferSize) {
                                file.seek(i);
                                file.write(buffer);
                            }

                            // TODO errors
                        }
                    }

                    file.close();
                }
                // TODO write to dump
                CloseHandle(hProcess);
            }
        }

        if (g_dumpType == DT_DUMP_PROCESS_USER_READPROCESSMEMORY_REBUILD) {
            if (g_nSize) {
                QFile file;
                file.setFileName(sRawDmpFile);

                if (file.open(QIODevice::ReadOnly)) {
                    XPE pe(&file, true, g_nAddress);
                    connect(&pe, SIGNAL(errorMessage(QString)), this, SLOT(errorMessage(QString)));

                    if (pe.isValid(g_pPdStruct)) {
                        if (!pe.fixDump(g_sFileName, g_fixDumpOptions, g_pPdStruct)) {
                            emit errorMessage(QString("%1: %2").arg(tr("Cannot fix dump file"), sRawDmpFile));
                        }
                    }

                    file.close();
                } else {
                    emit errorMessage(QString("%1: %2").arg(tr("Cannot open dump file"), sRawDmpFile));
                }
            }
        }

        XBinary::setPdStructFinished(g_pPdStruct, _nFreeIndex);
    }
#endif
#endif

    emit completed(scanTimer.elapsed());
}
