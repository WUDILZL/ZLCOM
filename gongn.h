#pragma once
#include "ZLCom.h"

//PLC��������У��
int PLC_JieShuXiaoYan(QByteArray shuju);

//PLC��������У��
QString PLC_XiaoYan(QString shuju);

//32У��ߵ�
QString XiaoYan_GaoDi32(uint32_t shuju, int geshi);

//16У��ߵ�
QString XiaoYan_GaoDi(uint16_t shuju,int geshi);

//�滻Сд
QString DaXiaoTiHuan(QString tihuanshuju);

//QPushButton����
void SuoFang_qpushbutton(QPushButton* dizhi,float suofangbilie);

//QStackedWidget����
void SuoFang_qstackedwidget(QStackedWidget* dizhi, float suofangbilie);

//QLabel����
void SuoFang_qlabel(QLabel* dizhi, float suofangbilie);

//QComboBox����
void SuoFang_qcombobox(QComboBox* dizhi, float suofangbilie);

//QRadioButton����
void SuoFang_qradiobutton(QRadioButton* dizhi, float suofangbilie);

//QToolButton����
void SuoFang_qtoolbutton(QToolButton* dizhi, float suofangbilie);

//QCheckBox����
void SuoFang_qcheckbox(QCheckBox* dizhi, float suofangbilie);

//QLineEdit����
void SuoFang_qlineedit(QLineEdit* dizhi, float suofangbilie);

//QPlainTextEdit����
void SuoFang_qplaintextedit(QPlainTextEdit* dizhi, float suofangbilie);

//QTableView����
void SuoFang_qtableView(QTableView* dizhi, float suofangbilie);
