/*!
 * @file optionwidget.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "optionwidget.h"
#include "ui_optionwidget.h"

OptionWidget::OptionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionWidget)
{
    ui->setupUi(this);
}

OptionWidget::~OptionWidget()
{
    delete ui;
}

void OptionWidget::on_addButton_clicked()
{
    emit notifyGalleryAddPic();
}

void OptionWidget::on_clearButton_clicked()
{
    emit notifyGalleryClearList();
}

void OptionWidget::on_refreshButton_clicked()
{
    emit notifyGalleryRefreshList();
}
