/*!
 * @file titlewidget.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>

namespace Ui {
class TitleWidget;
}

class TitleWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int fixSizeHeight READ fixSizeHeight WRITE setCusfixSizeHeight)

public:
    explicit TitleWidget(QWidget *parent = 0);
    ~TitleWidget();

    void setCusfixSizeHeight(int height);
    int  fixSizeHeight();
private:
    Ui::TitleWidget *ui;
};

#endif // TITLEWIDGET_H
