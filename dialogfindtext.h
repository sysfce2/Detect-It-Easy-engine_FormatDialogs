/* Copyright (c) 2022 hors<horsicq@gmail.com>
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
#ifndef DIALOGFINDTEXT_H
#define DIALOGFINDTEXT_H

#include <QDialog>

namespace Ui {
class DialogFindText;
}

class DialogFindText : public QDialog {
    Q_OBJECT

public:
    struct DATA {
        QString sText;
        bool bIsMatchCase;
    };

    explicit DialogFindText(QWidget *pParent = nullptr);
    ~DialogFindText();

    void setData(DATA *pData);

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonOK_clicked();

private:
    Ui::DialogFindText *ui;
    DATA *g_pData;
};

#endif  // DIALOGFINDTEXT_H
