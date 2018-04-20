/*!
 * @file titlewidget.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "titlewidget.h"
#include "ui_titlewidget.h"

TitleWidget::TitleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleWidget)
{
    ui->setupUi(this);
}

TitleWidget::~TitleWidget()
{
    delete ui;
}
void TitleWidget::setCusfixSizeHeight(int height)
{
    this->setFixedHeight(height);
}

int TitleWidget::fixSizeHeight()
{
    return this->height();
}
