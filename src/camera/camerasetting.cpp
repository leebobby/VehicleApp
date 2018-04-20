/*!
 * @file camerasetting.cpp
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#include "camerasetting.h"
#include "ui_camerasetting.h"
#include "src/switchbutton.h"
#include "src/frmmessagebox.h"

#include <QButtonGroup>
#include <QDebug>
#include <QMutex>


/*!
 * @pGlobalMainDesk
 * this global var is defined in maindesk, for widget return
 */
#include "src/maindesk.h"
extern MainDesk* pGlobalMainDesk;

CameraSetting *CameraSetting::self = NULL;
CameraSetting *CameraSetting::Instance(QWidget *parent)
{
    if (self == NULL) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self == NULL) {
            self = new CameraSetting(parent);
        }
    }
    return self;
}

CameraSetting::CameraSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraSetting)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);//the ~CameraSetting will be call when func close() run

    QButtonGroup* btnGroup = new QButtonGroup(this);
    btnGroup->addButton(ui->DvrSettingButton);
    btnGroup->addButton(ui->GeneralSettingButton);
    btnGroup->addButton(ui->ReserveLineSettingButton);
    btnGroup->setExclusive(true);

    ui->stackedWidget->setCurrentWidget(ui->DvrsettingPage);

    SwitchButton* recordAutoButton = new SwitchButton(ui->recordSettingWidget);
    SwitchButton* watermarkSettingButton = new SwitchButton(ui->watermarkSettingWidget);
    SwitchButton* recordAudioSettingButton = new SwitchButton(ui->recordAudioSettingWidget);
    SwitchButton* loopRecordSettingButton = new SwitchButton(ui->loopRecordSettingWidget);
    SwitchButton* reverseLineOpenButton = new SwitchButton(ui->reverseLineSettingWidget);

    recordAutoButton->setToggled(false);
    connect(recordAutoButton, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));

    watermarkSettingButton->setToggled(false);
    connect(watermarkSettingButton, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));

    recordAudioSettingButton->setToggled(false);
    connect(recordAudioSettingButton, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));

    loopRecordSettingButton->setToggled(false);
    connect(loopRecordSettingButton, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));

    reverseLineOpenButton->setToggled(false);
    connect(reverseLineOpenButton, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));

    ui->spinBox->setRange(1,5);
    ui->spinBox->setSingleStep(2);
    ui->spinBox->setValue(1);
    ui->spinBox->setSuffix(tr("分钟"));

    ui->balanceSlider->setRange(0,100);
    ui->balanceValueLabel->setText(tr("0"));

    ui->colorSlider->setRange(0,100);
    ui->colorValueLabel->setText(tr("0"));

    ui->explosureSlider->setRange(0,100);
    ui->explosureValueLabel->setText(tr("0"));

    ui->lightSlider->setRange(0,100);
    ui->lightValueLabel->setText(tr("0"));

    qDebug()<<"CameraSetting constructiong done";
}

CameraSetting::~CameraSetting()
{
    delete ui;
    self =NULL;
}

//first ui
void CameraSetting::on_DvrSettingButton_clicked()
{
    qDebug()<<"on_DvrSettingButton_clicked";
    ui->stackedWidget->setCurrentWidget(ui->DvrsettingPage);
}

void CameraSetting::on_GeneralSettingButton_clicked()
{
    qDebug()<<"on_GeneralSettingButton_clicked";
    ui->stackedWidget->setCurrentWidget(ui->GeneralSettingPage);
}

void CameraSetting::on_ReserveLineSettingButton_clicked()
{
    qDebug()<<"on_ReserveLineSettingButton_clicked";
    ui->stackedWidget->setCurrentWidget(ui->ReverseLineSettingPage);
}

void CameraSetting::on_cancelButton_clicked()
{
    qDebug()<<"CameraSetting::on_cancelButton_clicked";
#if 0
    if(this->parentWidget() == NULL){
        qDebug()<<"Oops!!I don't have any parent widget!!";
        return;
    }
    this->parentWidget()->show();
#endif
    this->hide();
    frmMessageBox* msg = new frmMessageBox();
    msg->SetMessage(QString(tr("确认放弃修改？")), 1);
    if(1 == msg->exec())
    {
        qDebug()<<"save the setttings";
        //save the setttings
        //...

        //then go to the parent widget
        pGlobalMainDesk->setHidden(false);
        this->close();
    }else{
        qDebug()<<"give up the setting changes";
        //give up all changes
        //...
        //then go to camerasetting ui
        this->show();
    }
}

void CameraSetting::on_saveSettingButton_clicked()
{
    this->hide();
    frmMessageBox* msg = new frmMessageBox();
    msg->SetMessage(QString(tr("确认保存？")), 1);
    if(1 == msg->exec())
    {
        qDebug()<<"save the setttings";
        //save the setttings
        //...

        //then go to the parent widget
        pGlobalMainDesk->show();
        this->close();
    }else{
        qDebug()<<"give up the setting changes";
        //give up all changes
        //...
        //then go to camerasetting ui
        this->show();
    }
}

//Dvrsetting
void CameraSetting::onToggled(bool ischecked)
{
    qDebug()<< "State : " << ischecked;
}

//general Setting
void CameraSetting::on_lightSlider_sliderMoved(int position)
{
    ui->lightValueLabel->setText(QString::number(position,10));
}

void CameraSetting::on_colorSlider_sliderMoved(int position)
{
    ui->colorValueLabel->setText(QString::number(position,10));
}

void CameraSetting::on_explosureSlider_sliderMoved(int position)
{
    ui->explosureValueLabel->setText(QString::number(position,10));
}

void CameraSetting::on_balanceSlider_sliderMoved(int position)
{
    ui->balanceValueLabel->setText(QString::number(position,10));
}



