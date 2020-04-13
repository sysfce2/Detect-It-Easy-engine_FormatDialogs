// copyright (c) 2019-2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "searchprocess.h"

SearchProcess::SearchProcess(QObject *parent) : QObject(parent)
{
    connect(&binary,SIGNAL(findProgressValueChanged(qint32)),this,SIGNAL(progressValueChanged(qint32)));
    connect(&binary,SIGNAL(findProgressMinimumChanged(qint32)),this,SIGNAL(progressValueMinimum(qint32)));
    connect(&binary,SIGNAL(findProgressMaximumChanged(qint32)),this,SIGNAL(progressValueMaximum(qint32)));
}

void SearchProcess::setData(QIODevice *pDevice, SearchProcess::SEARCHDATA *pSearchData)
{
    this->pDevice=pDevice;
    this->pSearchData=pSearchData;
}

void SearchProcess::stop()
{
    binary.setDumpProcessEnable(false);
}

void SearchProcess::process()
{
    QElapsedTimer scanTimer;
    scanTimer.start();

    binary.setData(pDevice);

    qint64 nStartOffset=0;

    if(pSearchData->startFrom==SF_CURRENTOFFSET)
    {
        nStartOffset=pSearchData->nCurrentOffset;
    }

    if(pSearchData->type==TYPE_ANSISTRING)
    {
        pSearchData->nResult=binary.find_ansiString(nStartOffset,-1,pSearchData->variant.toString());
    }
    else if(pSearchData->type==TYPE_ANSISTRING_I)
    {
        pSearchData->nResult=binary.find_ansiStringI(nStartOffset,-1,pSearchData->variant.toString());
    }
    else if(pSearchData->type==TYPE_UNICODESTRING)
    {
        pSearchData->nResult=binary.find_unicodeString(nStartOffset,-1,pSearchData->variant.toString());
    }
    else if(pSearchData->type==TYPE_UNICODESTRING_I)
    {
        pSearchData->nResult=binary.find_unicodeStringI(nStartOffset,-1,pSearchData->variant.toString());
    }
    else if(pSearchData->type==TYPE_SIGNATURE)
    {
        pSearchData->nResult=binary.find_signature(nStartOffset,-1,pSearchData->variant.toString());
    }

    if(pSearchData->nResult!=-1)
    {
        pSearchData->bInit=true;
    }

    emit completed(scanTimer.elapsed());
}
