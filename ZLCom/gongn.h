#pragma once
#include "ZLCom.h"

//PLC接收数据校验
int PLC_JieShuXiaoYan(QByteArray shuju);

//PLC发送数据校验
QString PLC_XiaoYan(QString shuju);

//32校验高低
QString XiaoYan_GaoDi32(uint32_t shuju, int geshi);

//16校验高低
QString XiaoYan_GaoDi(uint16_t shuju,int geshi);

//替换小写
QString DaXiaoTiHuan(QString tihuanshuju);

//QPushButton缩放
void SuoFang_qpushbutton(QPushButton* dizhi,float suofangbilie);

//QStackedWidget缩放
void SuoFang_qstackedwidget(QStackedWidget* dizhi, float suofangbilie);

//QLabel缩放
void SuoFang_qlabel(QLabel* dizhi, float suofangbilie);

//QComboBox缩放
void SuoFang_qcombobox(QComboBox* dizhi, float suofangbilie);

//QRadioButton缩放
void SuoFang_qradiobutton(QRadioButton* dizhi, float suofangbilie);

//QToolButton缩放
void SuoFang_qtoolbutton(QToolButton* dizhi, float suofangbilie);

//QCheckBox缩放
void SuoFang_qcheckbox(QCheckBox* dizhi, float suofangbilie);

//QLineEdit缩放
void SuoFang_qlineedit(QLineEdit* dizhi, float suofangbilie);

//QPlainTextEdit缩放
void SuoFang_qplaintextedit(QPlainTextEdit* dizhi, float suofangbilie);

//QTableView缩放
void SuoFang_qtableView(QTableView* dizhi, float suofangbilie);
