#pragma once

#include "ui_ZLCom.h"
#include "gongn.h"
#include "xialadianji.h"
#include <QtWidgets/QMainWindow>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTableView>
#include <QTimer>
#include <QCloseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QButtonGroup>
#include <QDebug>
#include <QScrollBar>
#include <QStandardItem>
#include <QInputDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>



class ZLCom : public QMainWindow
{
    Q_OBJECT

public:
    ZLCom(QWidget *parent = nullptr);
    ~ZLCom();

    //����PLC�����л�
    void ChuanKo_PLCJieMian_QieHuan(void);

    void comGuDingCanShu(void);//com�̶�������ʼ��
    void PLCGuDingCanShu(void);//PLC�̶�������ʼ��
    void XiaoYan_ZiJie(void);//У���л�
    void DuQuShangYiCiZhuangTai(void);//��ȡ�ϴιر�״̬

    void ascii_chushihua(void);//ascii����ʼ��
    void ascii_daying(void);//ascii����ӡ
    void PLC_XianShiGeShi(void);//����ʽ
    void showContextMenu(const QPoint& pos);//�Ҽ����
    void editCell();//�Ҽ��˵�
    void comod_cotcp(void);//tcpcom�л�

    void JiaoCom_JiShi(void);//ɨ���ʱ
    //void jiacom(void);//�˿�ɨ��
    void com_dakai(void);//�򿪶˿�
    void Com_KaiQi(void);//��������
    void PLCcom_dakai(void);//�򿪶˿�
    void PLC_ShuJuJieSho(void);//PLC���ݽ��յ������
    void PLC_ShuJuFaSong(int di, int ding);//���ն��е��У����ͱ���
    void ShuJuFaSong(void);//���ݶ�ʱ����������
    void PLC_JieSho_XianShi(QByteArray shuju);//�����������ݣ�����ӡ�����
    void PLC_DiYiCiXiaoYao(void);//��һ������ȷ��
    void panduan(void);
    void dakai_gongn(QRadioButton* xinhao, QPushButton* anjian);//�򿪹���

    void PingMu_FenBianLv(void);//��Ļ���ű�������1920*1080

    void FaSong_SaoMiao(void);//��������ɨ���ʱ
    void FaSong_QingKong(void);
    void FaSong_a(void);//һ�ŷ��Ϳ�������ʾ
    void FaSong_b(void);//���ŷ��Ϳ�������ʾ
    void FaSong_c(void);//���ŷ��Ϳ�������ʾ
    void ComShuJu_FaSong(QLineEdit* dizhi, QCheckBox* zhuangtai);//������������
    void Com_FaSong(void);

    void CaiDan_TianJia(void);//����Ҽ��˵�ѡ���Լ�����
    void CaiDan_TianJia_SUMJiaoYan(QLineEdit* dizhi);//BCC���У��
    void CaiDan_TianJia_BCCJiaoYan(QLineEdit* dizhi);//BCC���У��
    void CaiDan_TianJia_Crc8JiaoYan(QLineEdit* dizhi);//
    void CaiDan_TianJia_LrcJiaoYan(QLineEdit* dizhi);//LRCУ��
    void CaiDan_TianJia_CrcJiaoYan(QLineEdit* dizhi);//CRC-16/MODBUS
    void CaiDan_TianJia_Crc32JiaoYan(QLineEdit* dizhi);//CRC32

    void FuWuQi_QiDong(void);//����������
    void onNewConnection();
    void socketReadyRead();
    void socketDisconnected();

    void closeEvent(QCloseEvent* event);//��¼�ر�ǰ����


private slots:
    void jiacom(void);//����˿�ɨ��
    void PLCjiacom(void);
    void FuWuQi_DuanKoSaoMiao(void);//���ɨ��������˿�
    void FuWuQi_DuanKoPaiXu(void);//�������˿�����

    //��ȡ��ǰ����У�����
    void updateVisibleRows() {
        int topRow = -1;
        int bottomRow = -1;
        for (int row = 0; row < model->rowCount(); ++row) {
            QRect rect = ui.tableView->visualRect(model->index(row, 0));
            if (ui.tableView->viewport()->rect().intersects(rect)) {
                if (topRow == -1) {
                    topRow = row;
                }
                bottomRow = row;
            }
        }
        PLC_BiaoDing = topRow;
        PLC_BiaoDi = bottomRow;
        //qDebug("Top visible row: %d, Bottom visible row: %d", topRow, bottomRow);
    }

    //D��ӡ����
    void toggleTableVisibility() {
        // ����������
        delete model;
        model = nullptr;
        ui.tableView->setModel(nullptr);
        // ��ʼ������
        model = new QStandardItemModel(this);
        int totalItems = 10000;
        for (int i = 0; i < totalItems; i += 3) {
            QList<QStandardItem*> items;
            items.append(new QStandardItem(QString("D%1").arg(i)));
            items.append(new QStandardItem(QString("").arg(i)));
            if (i + 1 < totalItems) {
                items.append(new QStandardItem(QString("D%1").arg(i + 1)));
                items.append(new QStandardItem(QString("").arg(i + 1)));
            }
            else {
                items.append(new QStandardItem(""));
                items.append(new QStandardItem(""));
            }
            if (i + 2 < totalItems) {
                items.append(new QStandardItem(QString("D%1").arg(i + 2)));
                items.append(new QStandardItem(QString("").arg(i + 2)));
            }
            else {
                items.append(new QStandardItem(""));
                items.append(new QStandardItem(""));
            }

            model->appendRow(items);
        }  
        ui.tableView->setModel(model);
    }

    //��ȷ����
    void searchText() {
        QString searchText = ui.JiCunQi->text();
        if (searchText.isEmpty()) {
            return;
        }

        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount(); ++col) {
                QStandardItem* item = model->item(row, col);
                if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                    ui.tableView->scrollTo(model->index(row, col));
                    ui.tableView->setCurrentIndex(model->index(row, col));
                    return;
                }
            }
        }
    }
    
private:
    Ui::ZLComClass ui;
    int PLC_BiaoDing = 0, PLC_BiaoDi = 16;//����һ�У����һ��
    QSerialPort* serialPort;//������
    QSerialPort* PLC_serialPort;//������
    QVector<QString>ports;//com������
    QVector<QString>Plcports;//PLC����
    QStandardItemModel* model;//PLC���ݸ�ʽ
    QModelIndex clickedIndex;
    QMenu* contextMenu;
    QAction* editAction;
    QTimer* PLC_LianJie;//PLC���Ӳ���
    QTimer* PLC_LJOK_FaSong_BaoWen;//PLC����OK���ͱ�����ʱ
    QTcpServer* fuwuqi;//����������
};
