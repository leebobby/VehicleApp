/*!
 * @file camerasetting.h
 * Copyright (c) 2018
 * @brief des
 * detailed des
 *
 * @date 2018
 * @author lee
 */
#ifndef CAMERASETTING_H
#define CAMERASETTING_H

#include <QWidget>

namespace Ui {
class CameraSetting;
}

class CameraSetting : public QWidget
{
    Q_OBJECT

public:

    ~CameraSetting();
    static CameraSetting *Instance(QWidget *parent = 0);//create window by this interface

private slots:
    void onToggled(bool);//for switch button;

    void on_DvrSettingButton_clicked();

    void on_GeneralSettingButton_clicked();

    void on_ReserveLineSettingButton_clicked();

    void on_cancelButton_clicked();

    void on_saveSettingButton_clicked();

    void on_lightSlider_sliderMoved(int position);

    void on_colorSlider_sliderMoved(int position);

    void on_explosureSlider_sliderMoved(int position);

    void on_balanceSlider_sliderMoved(int position);

private:
    static CameraSetting* self;
    Ui::CameraSetting *ui;
    explicit CameraSetting(QWidget *parent = 0);
};

#endif // CAMERASETTING_H
