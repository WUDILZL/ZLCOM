#include "gongn.h"

//32校验高低
QString XiaoYan_GaoDi32(uint32_t shuju, int geshi)
{
    QString xiaoyan = NULL;
    uint8_t xy = 0;
    int shu = 0;
    if (geshi == 1)
    {
        xy = shuju;
        shu = int(xy);
        xiaoyan = QString::number(shu, 16);
        xiaoyan = DaXiaoTiHuan(xiaoyan);

        //xy = shuju >> 8;
        //shu = int(xy);
        //QString di = QString::number(shu, 16);
        //di = DaXiaoTiHuan(di);
        //xiaoyan.append(di);
    }
    else if (geshi == 2)
    {
        xy = shuju >> 24;
        shu = int(xy);
        xiaoyan = QString::number(shu, 16);
        xiaoyan = DaXiaoTiHuan(xiaoyan);

        xy = shuju >> 16;
        shu = int(xy);
        QString di = QString::number(shu, 16);
        di = DaXiaoTiHuan(di);
        xiaoyan.append(di);

        xy = shuju >> 8;
        shu = int(xy);
        di = QString::number(shu, 16);
        di = DaXiaoTiHuan(di);
        xiaoyan.append(di);

        xy = shuju;
        shu = int(xy);
        di = QString::number(shu, 16);
        di = DaXiaoTiHuan(di);
        xiaoyan.append(di);
    }
    else if (geshi == 3)
    {
        xy = shuju;
        shu = int(xy);
        xiaoyan = QString::number(shu, 16);
        xiaoyan = DaXiaoTiHuan(xiaoyan);

        xy = shuju >> 8;
        shu = int(xy);
        QString di = QString::number(shu, 16);
        di = DaXiaoTiHuan(di);
        xiaoyan.append(di);

        xy = shuju >> 16;
        shu = int(xy);
        di = QString::number(shu, 16);
        di = DaXiaoTiHuan(di);
        xiaoyan.append(di);

        xy = shuju >> 24;
        shu = int(xy);
        di = QString::number(shu, 16);
        di = DaXiaoTiHuan(di);
        xiaoyan.append(di);
    }
    return xiaoyan;
}

//16 校验高低
QString XiaoYan_GaoDi(uint16_t shuju, int geshi)
{
    QString xiaoyan = NULL;
    uint8_t xy = 0;
    int shu = 0;
    if (geshi == 1)
    {
        xy = shuju;
        shu = int(xy);
        xiaoyan = QString::number(shu, 16);
        xiaoyan = DaXiaoTiHuan(xiaoyan);
    }
    else if (geshi == 2)
    {
        xy = shuju >> 8;
        shu = int(xy);
        xiaoyan = QString::number(shu, 16);
        xiaoyan = DaXiaoTiHuan(xiaoyan);

        xy = shuju;
        shu = int(xy);
        QString di = QString::number(shu, 16);
        di = DaXiaoTiHuan(di);
        xiaoyan.append(di);
    }
    else if (geshi == 3)
    {
        xy = shuju;
        shu = int(xy);
        xiaoyan = QString::number(shu, 16);
        xiaoyan = DaXiaoTiHuan(xiaoyan);

        xy = shuju >> 8;
        shu = int(xy);
        QString di = QString::number(shu, 16);
        di = DaXiaoTiHuan(di);
        xiaoyan.append(di);
    }

    return xiaoyan;
}

//替换小写
QString DaXiaoTiHuan(QString tihuanshuju)
{
    if (tihuanshuju.size() < 2) tihuanshuju.prepend('0');
    if (tihuanshuju.mid(0, 1) != "0" && tihuanshuju.mid(0, 1) != "1" && tihuanshuju.mid(0, 1) != "2" && tihuanshuju.mid(0, 1) != "3" && tihuanshuju.mid(0, 1) != "4" && tihuanshuju.mid(0, 1) != "5" && tihuanshuju.mid(0, 1) != "6" &&
        tihuanshuju.mid(0, 1) != "7" && tihuanshuju.mid(0, 1) != "8" && tihuanshuju.mid(0, 1) != "9")
    {
        if (tihuanshuju.mid(0, 1) == "a") tihuanshuju.prepend("A"); else if (tihuanshuju.mid(0, 1) == "b") tihuanshuju.prepend("B"); else if (tihuanshuju.mid(0, 1) == "c") tihuanshuju.prepend("C"); else if (tihuanshuju.mid(0, 1) == "d") tihuanshuju.prepend("D");
        else if (tihuanshuju.mid(0, 1) == "e") tihuanshuju.prepend("E"); else if (tihuanshuju.mid(0, 1) == "f") tihuanshuju.prepend("F");
        tihuanshuju.remove(1, 1);
    }

    if (tihuanshuju.mid(1, 1) != "0" && tihuanshuju.mid(1, 1) != "1" && tihuanshuju.mid(1, 1) != "2" && tihuanshuju.mid(1, 1) != "3" && tihuanshuju.mid(1, 1) != "4" && tihuanshuju.mid(1, 1) != "5" && tihuanshuju.mid(1, 1) != "6" &&
        tihuanshuju.mid(1, 1) != "7" && tihuanshuju.mid(1, 1) != "8" && tihuanshuju.mid(1, 1) != "9")
    {
        if (tihuanshuju.mid(1, 1) == "a") tihuanshuju.append("A"); else if (tihuanshuju.mid(1, 1) == "b") tihuanshuju.append("B"); else if (tihuanshuju.mid(1, 1) == "c") tihuanshuju.append("C");
        else if (tihuanshuju.mid(1, 1) == "d") tihuanshuju.append("D"); else if (tihuanshuju.mid(1, 1) == "e") tihuanshuju.append("E"); else if (tihuanshuju.mid(1, 1) == "f") tihuanshuju.append("F");
        tihuanshuju.remove(1, 1);
    }

    return tihuanshuju;
}

//QPushButton缩放
void SuoFang_qpushbutton(QPushButton* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);

}

//QStackedWidget缩放
void SuoFang_qstackedwidget(QStackedWidget* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//QLabel缩放
void SuoFang_qlabel(QLabel* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//QComboBox缩放
void SuoFang_qcombobox(QComboBox* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//QRadioButton缩放
void SuoFang_qradiobutton(QRadioButton* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//QToolButton缩放
void SuoFang_qtoolbutton(QToolButton* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//QCheckBox缩放
void SuoFang_qcheckbox(QCheckBox* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//QLineEdit缩放
void SuoFang_qlineedit(QLineEdit* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//QPlainTextEdit缩放
void SuoFang_qplaintextedit(QPlainTextEdit* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//QTableView缩放
void SuoFang_qtableView(QTableView* dizhi, float suofangbilie)
{
    int kojian_weizhi_x = 0, kojian_weizhi_y = 0, kojian_kuan = 0, kojian_gao = 0;
    kojian_kuan = dizhi->width();
    kojian_gao = dizhi->height();
    kojian_kuan = float(kojian_kuan) * suofangbilie;
    kojian_gao = float(kojian_gao) * suofangbilie;
    dizhi->setFixedSize(kojian_kuan, kojian_gao);

    kojian_weizhi_x = dizhi->pos().x();
    kojian_weizhi_y = dizhi->pos().y();
    kojian_weizhi_x = float(kojian_weizhi_x) * suofangbilie;
    kojian_weizhi_y = float(kojian_weizhi_y) * suofangbilie;
    dizhi->move(kojian_weizhi_x, kojian_weizhi_y);
}

//接收数据校验
int PLC_JieShuXiaoYan(QByteArray shuju)
{
    QByteArray Jie = shuju;
    int shuju_qba = Jie.size();
    uint16_t wcrc = 0XFFFF;
    uint8_t temp = 0;
    uint16_t jie = 0;
    int di = 0, gao = 0, jie_di = 0, jie_gao = 0;
    for (int i = 0; i < shuju_qba -2; i++)
    {
        temp = Jie.at(i);
        wcrc ^= temp;

        for (int j = 0; j < 8; j++) {

            if (wcrc & 0X0001) {
                wcrc >>= 1;
                wcrc ^= 0XA001;
            }
            else
                wcrc >>= 1;
        }
    }
    jie = wcrc;
    temp = jie;
    di = int(temp);

    temp = jie >> 8;
    gao = int(temp);

    QByteArray PanDuan = Jie.mid((Jie.size() - 2), 1);
    QString shex = PanDuan.toHex();
    jie_di = shex.toInt(nullptr, 16);

    PanDuan = Jie.mid((Jie.size() - 1), 1);
    shex = PanDuan.toHex();
    jie_gao = shex.toInt(nullptr, 16);

    if (di == jie_di && gao == jie_gao)return 1;
    else return 0;

    
}

//PLC发送数据校验
QString PLC_XiaoYan(QString shuju)
{
    QString FaSong_ShuJu = shuju;
    int chang = FaSong_ShuJu.size();

    if (chang % 2 == 0)
    {
        QByteArray ShuJu = NULL;
        for (int i = 0; i < chang; i++)
        {
            int num = FaSong_ShuJu.mid(i, 2).toUInt(nullptr, 16);
            ShuJu.append(num);
            ++i;
        }

        int shuju_qba = ShuJu.size();
        uint16_t wcrc = 0XFFFF;
        uint8_t temp = 0;
        uint16_t jie = 0;
        for (int i = 0; i < shuju_qba; i++)
        {
            temp = ShuJu.at(i);
            wcrc ^= temp;

            for (int j = 0; j < 8; j++) {

                if (wcrc & 0X0001) {
                    wcrc >>= 1;
                    wcrc ^= 0XA001;
                }
                else
                    wcrc >>= 1;
            }
        }
        jie = wcrc;
        temp = jie;
        chang = int(temp);
        FaSong_ShuJu = DaXiaoTiHuan(QString::number(chang, 16));
        temp = jie >> 8;
        chang = int(temp);
        FaSong_ShuJu.append(DaXiaoTiHuan(QString::number(chang, 16)));
        return FaSong_ShuJu;
    }

}
