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

    //串口PLC界面切换
    void ChuanKo_PLCJieMian_QieHuan(void);

    void comGuDingCanShu(void);//com固定参数初始化
    void PLCGuDingCanShu(void);//PLC固定参数初始化
    void XiaoYan_ZiJie(void);//校验切换
    void DuQuShangYiCiZhuangTai(void);//读取上次关闭状态

    void ascii_chushihua(void);//ascii码表初始化
    void ascii_daying(void);//ascii码表打印
    void PLC_XianShiGeShi(void);//表格格式
    void showContextMenu(const QPoint& pos);//右键点击
    void editCell();//右键菜单
    void comod_cotcp(void);//tcpcom切换

    void JiaoCom_JiShi(void);//扫描计时
    //void jiacom(void);//端口扫描
    void com_dakai(void);//打开端口
    void Com_KaiQi(void);//开启串口
    void PLCcom_dakai(void);//打开端口
    void PLC_ShuJuJieSho(void);//PLC数据接收到表格中
    void PLC_ShuJuFaSong(int di, int ding);//接收顶行底行，发送报文
    void ShuJuFaSong(void);//数据定时，持续发送
    void PLC_JieSho_XianShi(QByteArray shuju);//解析接收数据，并打印到表格
    void PLC_DiYiCiXiaoYao(void);//第一次连接确认
    void panduan(void);
    void dakai_gongn(QRadioButton* xinhao, QPushButton* anjian);//打开功能

    void PingMu_FenBianLv(void);//屏幕缩放比例适配1920*1080

    void FaSong_SaoMiao(void);//发送数据扫描计时
    void FaSong_QingKong(void);
    void FaSong_a(void);//一号发送口数据显示
    void FaSong_b(void);//二号发送口数据显示
    void FaSong_c(void);//三号发送口数据显示
    void ComShuJu_FaSong(QLineEdit* dizhi, QCheckBox* zhuangtai);//发送数据整合
    void Com_FaSong(void);

    void CaiDan_TianJia(void);//添加右键菜单选项以及功能
    void CaiDan_TianJia_SUMJiaoYan(QLineEdit* dizhi);//BCC异或校验
    void CaiDan_TianJia_BCCJiaoYan(QLineEdit* dizhi);//BCC异或校验
    void CaiDan_TianJia_Crc8JiaoYan(QLineEdit* dizhi);//
    void CaiDan_TianJia_LrcJiaoYan(QLineEdit* dizhi);//LRC校验
    void CaiDan_TianJia_CrcJiaoYan(QLineEdit* dizhi);//CRC-16/MODBUS
    void CaiDan_TianJia_Crc32JiaoYan(QLineEdit* dizhi);//CRC32

    void FuWuQi_QiDong(void);//服务器启动
    void onNewConnection();
    void socketReadyRead();
    void socketDisconnected();

    void closeEvent(QCloseEvent* event);//记录关闭前数据


private slots:
    void jiacom(void);//点击端口扫描
    void PLCjiacom(void);
    void FuWuQi_DuanKoSaoMiao(void);//点击扫描服务器端口
    void FuWuQi_DuanKoPaiXu(void);//服务器端口排序

    //获取当前表格顶行，底行
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

    //D打印行数
    void toggleTableVisibility() {
        // 清除表格数据
        delete model;
        model = nullptr;
        ui.tableView->setModel(nullptr);
        // 初始化数据
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

    //精确查找
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
    int PLC_BiaoDing = 0, PLC_BiaoDi = 16;//表格第一行，最后一行
    QSerialPort* serialPort;//串口类
    QSerialPort* PLC_serialPort;//串口类
    QVector<QString>ports;//com口容器
    QVector<QString>Plcports;//PLC容器
    QStandardItemModel* model;//PLC数据格式
    QModelIndex clickedIndex;
    QMenu* contextMenu;
    QAction* editAction;
    QTimer* PLC_LianJie;//PLC连接测试
    QTimer* PLC_LJOK_FaSong_BaoWen;//PLC连接OK发送报文延时
    QTcpServer* fuwuqi;//创建服务器
};
