/* Copyright (c) 2019-2025 hors<horsicq@gmail.com>
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
#include "dialogresize.h"

#include "ui_dialogresize.h"

DialogResize::DialogResize(QWidget *pParent, DATA *pData) : XShortcutsDialog(pParent, false), ui(new Ui::DialogResize)
{
    ui->setupUi(this);

    g_pData = pData;

    ui->checkBoxHex->setChecked(true);
    ui->lineEditValue->setValue_uint64(pData->nNewSize);
}

DialogResize::~DialogResize()
{
    delete ui;
}

void DialogResize::adjustView()
{
}

void DialogResize::on_pushButtonCancel_clicked()
{
    reject();
}

void DialogResize::on_pushButtonOK_clicked()
{
    g_pData->nNewSize = ui->lineEditValue->getValue_uint64();

    accept();
}

void DialogResize::on_checkBoxHex_toggled(bool bChecked)
{
    if (bChecked) {
        ui->lineEditValue->setMode(XLineEditHEX::_MODE_HEX);
    } else {
        ui->lineEditValue->setMode(XLineEditHEX::_MODE_DEC);
    }
}

void DialogResize::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
