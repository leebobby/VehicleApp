/*!
 * @file optionwidget.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef OPTIONWIDGET_H
#define OPTIONWIDGET_H

#include <QWidget>

namespace Ui {
class OptionWidget;
}

class OptionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionWidget(QWidget *parent = 0);
    ~OptionWidget();
signals:
    void notifyGalleryAddPic();

    void notifyGalleryClearList();

    void notifyGalleryRefreshList();

private slots:
    void on_addButton_clicked();

    void on_clearButton_clicked();

    void on_refreshButton_clicked();

private:
    Ui::OptionWidget *ui;
};

#endif // OPTIONWIDGET_H
