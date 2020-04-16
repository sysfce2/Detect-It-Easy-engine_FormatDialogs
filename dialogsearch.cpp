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
#include "dialogsearch.h"
#include "ui_dialogsearch.h"

DialogSearch::DialogSearch(QWidget *parent, QIODevice *pDevice, SearchProcess::SEARCHDATA *pSearchData) :
    QDialog(parent),
    ui(new Ui::DialogSearch)
{
    ui->setupUi(this);

    this->pDevice=pDevice;
    this->pSearchData=pSearchData;

    ui->comboBoxSearchFrom->addItem(tr("Begin"));
    ui->comboBoxSearchFrom->addItem(tr("Cursor"));

    ui->comboBoxType->addItem(tr("ANSI"));
    ui->comboBoxType->addItem(tr("Unicode"));

    ui->comboBoxEndianness->addItem(tr("Big endian"));
    ui->comboBoxEndianness->addItem(tr("Little endian"));

    ui->plainTextEditString->setFocus();

    ui->tabWidgetSearch->setCurrentIndex(0);

    ui->lineEditValue->setText("0");

    ui->radioButtonUint->setChecked(true);

    ajustValue();
}

DialogSearch::~DialogSearch()
{
    delete ui;
}

void DialogSearch::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogSearch::on_pushButtonOK_clicked()
{
    if(ui->tabWidgetSearch->currentIndex()==0) // Strings
    {
        // TODO
        bool bMatchCase=ui->checkBoxMatchCase->isChecked();

        if(ui->comboBoxType->currentIndex()==0) // ANSI
        {
            if(bMatchCase)
            {
                pSearchData->type=SearchProcess::TYPE_ANSISTRING;
            }
            else
            {
                pSearchData->type=SearchProcess::TYPE_ANSISTRING_I;
            }
        }
        else if(ui->comboBoxType->currentIndex()==1) // UNICODE
        {
            if(bMatchCase)
            {
                pSearchData->type=SearchProcess::TYPE_UNICODESTRING;
            }
            else
            {
                pSearchData->type=SearchProcess::TYPE_UNICODESTRING_I;
            }
        }

        QString sText=ui->plainTextEditString->toPlainText();

        if(sText.length()>256) // TODO const
        {
            sText.resize(256);
        }

        pSearchData->variant=sText;
    }
    else if(ui->tabWidgetSearch->currentIndex()==1) // Signature
    {
        QString sText=ui->plainTextEditSignature->toPlainText();

        if(sText.length()>256) // TODO const
        {
            sText.resize(256);
        }

        pSearchData->type=SearchProcess::TYPE_SIGNATURE;
        pSearchData->variant=sText;
    }
    else if(ui->tabWidgetSearch->currentIndex()==2) // Value
    {
        // TODO
    }

    DialogSearchProcess dsp(this,pDevice,pSearchData);

    done(dsp.exec());
}

void DialogSearch::on_tabWidgetSearch_currentChanged(int index)
{
    if(ui->tabWidgetSearch->currentIndex()==0) // Strings
    {
        ui->plainTextEditString->setFocus();
    }
    else if(ui->tabWidgetSearch->currentIndex()==1) // Signatures
    {
        ui->plainTextEditSignature->setFocus();
    }
}

void DialogSearch::on_lineEditValue_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

    ajustValue();
}

void DialogSearch::on_comboBoxEndianness_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    ajustValue();
}

void DialogSearch::on_radioButtonChar_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonUchar_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonShort_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonUshort_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonInt_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonUint_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonInt64_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonUint64_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonFloat_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::on_radioButtonDouble_toggled(bool checked)
{
    Q_UNUSED(checked)

    ajustValue();
}

void DialogSearch::ajustValue()
{
    QString sValue=ui->lineEditValue->text();

    bool bSuccess=false;

    if(ui->radioButtonChar->isChecked())
    {

    }
    else if(ui->radioButtonUchar->isChecked())
    {

    }
    else if(ui->radioButtonInt->isChecked())
    {

    }
    else if(ui->radioButtonUint->isChecked())
    {

    }
    else if(ui->radioButtonInt64->isChecked())
    {

    }
    else if(ui->radioButtonUint64->isChecked())
    {

    }
    else if(ui->radioButtonFloat->isChecked())
    {

    }
    else if(ui->radioButtonDouble->isChecked())
    {

    }

    checkValid();
}

void DialogSearch::checkValid()
{
    bool bIsValid=false;

    if(ui->tabWidgetSearch->currentIndex()==0) // Strings
    {
        bIsValid=!(ui->plainTextEditString->toPlainText().isEmpty());
    }
    else if(ui->tabWidgetSearch->currentIndex()==1) // Signature
    {
        bIsValid=!(ui->plainTextEditSignature->toPlainText().isEmpty());
    }
    else if(ui->tabWidgetSearch->currentIndex()==2) // Value
    {
        bIsValid=!(ui->lineEditHex->text().isEmpty());
    }

    ui->pushButtonOK->setEnabled(bIsValid);
}

void DialogSearch::on_plainTextEditString_textChanged()
{
    checkValid();
}

void DialogSearch::on_plainTextEditSignature_textChanged()
{
    checkValid();
}
