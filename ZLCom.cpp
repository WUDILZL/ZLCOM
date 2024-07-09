#include "ZLCom.h"

//ASCII码表表头
const char* SPBT[] = {
"ASCII值",
"控制字符",
"十六进制",
"八进制",
"二进制"
};

//ASCII表格控制字符
const char* KZZF[] = {
    "nul","soh","stx","etx","eot",
    "enq","ack","bel","bs","ht",
    "nl","vt","ff","er","so",
    "si","dle","dc1","dc2","dc3",
    "dc4","nak","syn","etb","can",
    "em","sub","esc","fs","gs",
    "re","us","sp","del"
};

//发送窗口数据状态
int  fa_a = 0, fa_b = 0, fa_c = 0, Fasong_Guangbiao = 0, xiaoyanbiaoji = 0, CRC_USB = 0, duankozhuangtai = 0, QuanJuSuoFang = 0, XiaoYan_ZhuangTai = 0, PLC_duankozhuangtai = 0, PLC_lianjiezhuangtai = 0;
int fasongjishu = 0;//PLC接收发送计数
float PingMuBiLie = 0;//缩放比例
int BiaoGe_ShuJuLiang = 0;//PLC读取数据长度
QByteArray PLC_CeShiJieShoShuJu = NULL;//接收数据
QByteArray SuoYouShuJu = NULL;//发送PLC数据
QString FaSong_ShuJu1 = NULL;//一号数据
QString FaSong_ShuJu2 = NULL;//二号数据
QString FaSong_ShuJu3 = NULL;//三号数据
QModelIndex ZuoBiao;

ZLCom::ZLCom(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ChuanKo_PLCJieMian_QieHuan();//PLC串口界面切换
    comGuDingCanShu();//固定参数初始化
    PLCGuDingCanShu();//PLC固定参数初始化
    PLC_XianShiGeShi();//PLC表格格式 表格相关跳转按钮
    XiaoYan_ZiJie();//校验切换
    DuQuShangYiCiZhuangTai();//配置上一次参数

    ascii_chushihua();//初始化工具菜单 并获取菜单按键属性进行处理
    CaiDan_TianJia();//窗口右键功能实现
    comod_cotcp();//com界面TCP界面切换
    com_dakai();//按键打开com
    PLCcom_dakai();

    FaSong_SaoMiao();//发送数据扫描
    FaSong_QingKong();//发送数据清空
    Com_FaSong();//数据发送
    FuWuQi_DuanKoSaoMiao();//点击扫描IP端口
    

    PingMu_FenBianLv();//屏幕分辨率自适应1920*1080

    JiaoCom_JiShi();//点击扫描端口

}

ZLCom::~ZLCom()
{}

//串口PLC界面切换
void ZLCom::ChuanKo_PLCJieMian_QieHuan(void)
{
    ui.QieHuanChuangKo->setCurrentIndex(0);
    ui.ChuanKo_GongJu->setDisabled(true);
    ui.PLC_JianKo->setDisabled(false);
    connect(ui.PLC_JianKo, &QAction::triggered, [&]() {
        ui.QieHuanChuangKo->setCurrentIndex(1);
        ui.ChuanKo_GongJu->setDisabled(false);
        ui.PLC_JianKo->setDisabled(true);
        });

    connect(ui.ChuanKo_GongJu, &QAction::triggered, [&]() {
        ui.QieHuanChuangKo->setCurrentIndex(0);
        ui.PLC_JianKo->setDisabled(false);
        ui.ChuanKo_GongJu->setDisabled(true);
        });

}

//串口固定参数初始化
void ZLCom::comGuDingCanShu(void)
{
    //波特率
    ui.combotelv->addItem("1200");
    ui.combotelv->addItem("2400");
    ui.combotelv->addItem("4800");
    ui.combotelv->addItem("9600");
    ui.combotelv->addItem("19200");
    ui.combotelv->addItem("38400");
    ui.combotelv->addItem("57600");
    ui.combotelv->addItem("115200");

    //数据位
    ui.comshujuwei->addItem("7");
    ui.comshujuwei->addItem("8");

    //校验位
    ui.comxiaoyan->addItem("None");
    ui.comxiaoyan->addItem("Even");
    ui.comxiaoyan->addItem("Odd");
    ui.comxiaoyan->addItem("Space");
    ui.comxiaoyan->addItem("Mark");

    //停止位
    ui.comtingzhi->addItem("1");
    ui.comtingzhi->addItem("1.5");
    ui.comtingzhi->addItem("2");

    //显示数据的格式
    ui.BianMa->addItem("ASCII");
    ui.BianMa->addItem("HEX");
    ui.BianMa->addItem("DEC");
    ui.BianMa->addItem("Bin");

    //服务器选择
    ui.fuwuql->addItem("TCP Server");
    ui.fuwuql_bendi->addItem("8888");

}

//PLC固定参数初始化
void ZLCom::PLCGuDingCanShu(void)
{
    //PLC型号
    ui.Plc_XingHao_XuanZe->addItem(QString::fromLocal8Bit("台达DVP"));

    //通讯格式
    ui.PLC_TongXun_GeShi->addItem("ASCII");
    ui.PLC_TongXun_GeShi->addItem("RTU");

    //通讯波特率
    ui.PLC_BoTeLv_XuanZe->addItem("1200");
    ui.PLC_BoTeLv_XuanZe->addItem("2400");
    ui.PLC_BoTeLv_XuanZe->addItem("4800");
    ui.PLC_BoTeLv_XuanZe->addItem("9600");
    ui.PLC_BoTeLv_XuanZe->addItem("19200");
    ui.PLC_BoTeLv_XuanZe->addItem("38400");
    ui.PLC_BoTeLv_XuanZe->addItem("57600");
    ui.PLC_BoTeLv_XuanZe->addItem("115200");

    //数据位
    ui.PLC_ShuJuWei_XuanZe->addItem("7");
    ui.PLC_ShuJuWei_XuanZe->addItem("8");

    //校验位
    ui.PLC_XiaoYan_XuanZe->addItem("None");
    ui.PLC_XiaoYan_XuanZe->addItem("Even");
    ui.PLC_XiaoYan_XuanZe->addItem("Odd");

    //停止位
    ui.PLC_TingZhiXuanZe->addItem("1");
    ui.PLC_TingZhiXuanZe->addItem("1.5");
    ui.PLC_TingZhiXuanZe->addItem("2");
}

//校验切换
void ZLCom::XiaoYan_ZiJie(void)
{
    QButtonGroup* buttonGroup = new QButtonGroup;

    ui.xiaoyan_3->setChecked(true);

    buttonGroup->addButton(ui.xiaoyan_1,1);
    buttonGroup->addButton(ui.xiaoyan_2,2);
    buttonGroup->addButton(ui.xiaoyan_3,3);
    // 将按钮组的 exclusive 属性设置为 true，确保只能选中一个选项
    buttonGroup->setExclusive(true);

}

//读取上一次界面配置
void ZLCom::DuQuShangYiCiZhuangTai(void)
{
    QFile file("test.txt");
    file.open(QIODevice::ReadOnly);//只读打开
    QByteArray data = file.readAll();//读取全部数据
    file.close();
    if (data != "")
    {
        for (int i = 0; i < data.size(); i++)
        {
            QByteArray dandu_zifu = data.mid(i, 1);
            QString shuju = QString(dandu_zifu);
            int shuju_biaohao = shuju.toInt(nullptr, 10);
            if (i == 0)ui.combotelv->setCurrentIndex(shuju_biaohao);
            if (i == 1)ui.comshujuwei->setCurrentIndex(shuju_biaohao);
            if (i == 2)ui.comxiaoyan->setCurrentIndex(shuju_biaohao);
            if (i == 3)ui.comtingzhi->setCurrentIndex(shuju_biaohao);
            if (i == 4) {
                if (shuju_biaohao == 0) {
                    ui.Ding_a->setChecked(false);
                }
                else {
                    ui.Ding_a->setChecked(true);
                }
            }
            if (i == 5) {
                if (shuju_biaohao == 0) {
                    ui.Ding_b->setChecked(false);
                }
                else {
                    ui.Ding_b->setChecked(true);
                }
            }
            if (i == 6) {
                if (shuju_biaohao == 0) {
                    ui.Ding_c->setChecked(false);
                }
                else {
                    ui.Ding_c->setChecked(true);
                }
            }
            if (i == 7) {
                if (shuju_biaohao == 0) {
                    ui.Fa_a->setChecked(false);
                }
                else {
                    ui.Fa_a->setChecked(true);
                    fa_a = 1;
                }
            }
            if (i == 8) {
                if (shuju_biaohao == 0) {
                    ui.Fa_b->setChecked(false);
                }
                else {
                    ui.Fa_b->setChecked(true);
                    fa_b = 1;
                }
            }
            if (i == 9) {
                if (shuju_biaohao == 0) {
                    ui.Fa_c->setChecked(false);
                }
                else {
                    ui.Fa_c->setChecked(true);
                    fa_c = 1;
                }
            }

            if (i == 10)ui.BianMa->setCurrentIndex(shuju_biaohao);

            if (i == 11)
            {
                if (shuju_biaohao == 1)
                {
                    ui.xiaoyan_1->setChecked(true); 
                    XiaoYan_ZhuangTai = 1;
                }
                else if (shuju_biaohao == 2)
                {
                    ui.xiaoyan_2->setChecked(true);
                    XiaoYan_ZhuangTai = 2;
                }
                else if (shuju_biaohao == 3)
                {
                    ui.xiaoyan_3->setChecked(true);
                    XiaoYan_ZhuangTai = 3;
                }
            }

            if (i == 12)ui.Plc_XingHao_XuanZe->setCurrentIndex(shuju_biaohao);
            if (i == 13)ui.PLC_TongXun_GeShi->setCurrentIndex(shuju_biaohao);
            if (i == 14)ui.PLC_BoTeLv_XuanZe->setCurrentIndex(shuju_biaohao);
            if (i == 15)ui.PLC_ShuJuWei_XuanZe->setCurrentIndex(shuju_biaohao);
            if (i == 16)ui.PLC_XiaoYan_XuanZe->setCurrentIndex(shuju_biaohao);
            if (i == 17)ui.PLC_TingZhiXuanZe->setCurrentIndex(shuju_biaohao);

        }
    }


    QFile canshu("FSSJ.txt");
    canshu.open(QIODevice::ReadOnly);//只读打开
    data = canshu.readAll();//读取全部数据
    canshu.close();
    if (data != "")
    {
        int weizhi = 0, changdu = 0;//下标位置 weizhi ， 数据长度 
        QByteArray fasongko_yi = data.mid(weizhi, 1);//提取数据
        QString shuju = QString(fasongko_yi);
        int shuju_biaohao = shuju.toInt(nullptr, 10);
        if (shuju_biaohao == 1)//一号数据解析
        {
            ++weizhi;
            fasongko_yi = data.mid(weizhi, 3);
            weizhi = weizhi + 3;
            shuju = QString(fasongko_yi);
            changdu = shuju.toInt(nullptr, 10);
            fasongko_yi = data.mid(weizhi, changdu);
            weizhi = weizhi + changdu;
            shuju = QString(fasongko_yi);
            ui.fasong->setText(shuju);
        }

        fasongko_yi = data.mid(weizhi, 1);
        shuju = QString(fasongko_yi);
        shuju_biaohao = shuju.toInt(nullptr, 10);
        if (shuju_biaohao == 2)//二号数据解析
        {
            ++weizhi;
            fasongko_yi = data.mid(weizhi, 3);
            weizhi = weizhi + 3;
            shuju = QString(fasongko_yi);
            changdu = shuju.toInt(nullptr, 10);
            fasongko_yi = data.mid(weizhi, changdu);
            weizhi = weizhi + changdu;
            shuju = QString(fasongko_yi);
            ui.fasong2->setText(shuju);
        }

        fasongko_yi = data.mid(weizhi, 1);
        shuju = QString(fasongko_yi);
        shuju_biaohao = shuju.toInt(nullptr, 10);
        if (shuju_biaohao == 3)//三号数据解析
        {
            ++weizhi;
            fasongko_yi = data.mid(weizhi, 3);
            weizhi = weizhi + 3;
            shuju = QString(fasongko_yi);
            changdu = shuju.toInt(nullptr, 10);
            fasongko_yi = data.mid(weizhi, changdu);
            weizhi = weizhi + changdu;
            shuju = QString(fasongko_yi);
            ui.fasong3->setText(shuju);
        }

    }

}

//初始化工具菜单
void ZLCom::ascii_chushihua(void)
{
   
    ui.gongJu->setText(QString::fromLocal8Bit("工具"));//按键命名
    ui.gongJu->setPopupMode(QToolButton::MenuButtonPopup);//添加下拉菜单属性

    QMenu* menul = new QMenu;//下拉菜单类
    //创建一个按键并命名
    QAction* cao = new QAction();
    cao->setText(QString::fromLocal8Bit("ASCII码表"));
    menul->addAction(cao);
    ui.gongJu->setMenu(menul);//添加菜单到按键中
    //槽获取按键信息
    connect(cao, &QAction::triggered, [&]() {
        ascii_daying();
        });
    
}

//创建并打印码表
void ZLCom::ascii_daying(void)
{
    QWidget* ASCIIMB = new QWidget;//创建窗口
    QTableView* biaoge = new QTableView(ASCIIMB);//创建表格
    ASCIIMB->setAttribute(Qt::WA_QuitOnClose, false);//绑定置父窗口
    //biaoge->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ASCIIMB->resize(500, 600);//窗口大小
    biaoge->resize(500, 600);//表格大小
    ASCIIMB->setWindowTitle(QString::fromLocal8Bit("ASCII码表"));//窗口名字
    ASCIIMB->show();//显示窗口

    QStandardItemModel* m_standard_data_model = new QStandardItemModel(ASCIIMB);//创建表格数据类
    //setHorizontalHeaderLabels 设置水平表头
    //打印码表名称列
    biaoge->setModel(m_standard_data_model);//设置 tableView 数据类型
    QStringList table_h_headers;
    for (int i = 0; i < sizeof(SPBT) / sizeof(SPBT[0]); i++)
    {
        QString mstring = QString::fromLocal8Bit(SPBT[i]);
        QStringList mdec = mstring.split(",");
        table_h_headers.append(mdec);
    }
    m_standard_data_model->setHorizontalHeaderLabels(table_h_headers);
    //biaoge->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//自适应所有列
    //打印128行
    QStringList table_v_headers;
    for (int i = 0; i < 128; i++)
    {
        QString sdec = QString::number(i, 10);
        QStringList mdec = sdec.split(",");
        table_v_headers.append(" ");
    }
    m_standard_data_model->setVerticalHeaderLabels(table_v_headers);

    //ASCII码表打印
    for (int i = 0; i < sizeof(SPBT) / sizeof(SPBT[0]); i++)
    {
        QStandardItem* can;
        if (i == 0)//10
        {
            for (int j = 0; j < 128; j++)
            {
                QString shuju = NULL;
                shuju = QString::number(j, 10);
                can = new QStandardItem(shuju);
                m_standard_data_model->setItem(j, i, can);
            }
        }
        else if (i == 1)//ASCII
        {
            for (int j = 0; j < 128; j++)
            {
                if (0 <= j && j <= 33 || j == 127)
                {
                    if (0 <= j && j <= 33)
                    {
                        QString zf = KZZF[j];
                        can = new QStandardItem(zf);
                        m_standard_data_model->setItem(j, i, can);
                    }
                    else if (j == 127)
                    {
                        QString zf = KZZF[33];
                        can = new QStandardItem(zf);
                        m_standard_data_model->setItem(j, i, can);
                    }

                }
                else
                {
                    QString zf = NULL;
                    zf.append(j);
                    can = new QStandardItem(zf);
                    m_standard_data_model->setItem(j, i, can);
                }
            }
        }
        else if (i == 2)//16
        {
            for (int j = 0; j < 128; j++)
            {
                QString shuju = NULL;
                QString shex = QString::number(j, 16);
                if (shex.size() < 2)
                {
                    int chang = 2 - shex.size();
                    for (int m = 0; m < chang; m++)
                    {
                        shex.prepend('0');
                    }
                }
                shuju.append(shex);//H
                can = new QStandardItem(shuju);
                m_standard_data_model->setItem(j, i, can);
            }
        }
        else if (i == 3)//8
        {
            for (int j = 0; j < 128; j++)
            {
                QString shuju = NULL;
                QString soct = QString::number(j, 8);
                if (soct.size() < 3)
                {
                    int chang = 3 - soct.size();
                    for (int m = 0; m < chang; m++)
                    {
                        soct.prepend('0');
                    }
                }
                shuju.append(soct);
                can = new QStandardItem(shuju);
                m_standard_data_model->setItem(j, i, can);
            }
        }
        else if (i == 4)//2
        {
            for (int j = 0; j < 128; j++)
            {
                QString shuju = NULL;
                QString sbin = QString::number(j, 2);
                if (sbin.size() < 8)
                {
                    int chang = 8 - sbin.size();
                    for (int m = 0; m < chang; m++)
                    {
                        sbin.prepend('0');
                    }
                }
                shuju.append(sbin);
                can = new QStandardItem(shuju);
                m_standard_data_model->setItem(j, i, can);
            }
        }
    }
    biaoge->setEditTriggers(QAbstractItemView::NoEditTriggers);// NoEditTriggers 不可编辑
    ASCIIMB->setAttribute(Qt::WA_DeleteOnClose);//关闭窗口释放内存
}

//PLC表格格式 表格相关跳转按钮
void ZLCom::PLC_XianShiGeShi(void)
{
    
    
    ui.tableView->setColumnWidth(0, 150); // 设置第1列宽度
    ui.tableView->setColumnWidth(1, 200); // 设置第2列宽度
    ui.tableView->setColumnWidth(2, 150); // 设置第3列宽度
    ui.tableView->setColumnWidth(3, 200); // 设置第4列宽度
    ui.tableView->setColumnWidth(4, 150); // 设置第3列宽度
    ui.tableView->setColumnWidth(5, 200); // 设置第4列宽度

    ui.tableView->verticalHeader()->setDefaultSectionSize(30); // 设置默认行高为30

    ui.tableView->verticalHeader()->hide();
    ui.tableView->horizontalHeader()->hide();
    connect(ui.TaiDa_D, &QPushButton::clicked, [&]() {toggleTableVisibility(); });//表格打印

    // 连接右键点击事件
    ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableView, &QTableView::customContextMenuRequested, this, &ZLCom::showContextMenu);//右键点击编辑，并返回当前选择行坐标

    //添加菜单动作
    contextMenu = new QMenu(this);
    editAction = new QAction(this);
    editAction->setText(QString::fromLocal8Bit("修改参数"));
    contextMenu->addAction(editAction);
    connect(editAction, &QAction::triggered, this, &ZLCom::editCell);//右键菜单

    connect(ui.DiZhiChaZhao, &QPushButton::clicked, [&]() {searchText(); });//精确查找
    connect(ui.tableView->verticalScrollBar(), &QScrollBar::valueChanged, [&]() {updateVisibleRows(); });//返回当前表格顶行，底行
}

//右键点击
void ZLCom::showContextMenu(const QPoint& pos)
{
    clickedIndex = ui.tableView->indexAt(pos);
    //返回坐标
    int row = clickedIndex.row();
    int col = clickedIndex.column();
    //不可编辑
    if (clickedIndex.isValid() && clickedIndex.column() != 0 && clickedIndex.column() != 2 && clickedIndex.column() != 4) {
        contextMenu->exec(ui.tableView->viewport()->mapToGlobal(pos));
    }
}

//右键菜单
void ZLCom::editCell()
{
    if (clickedIndex.isValid()) {
        bool ok;
        QString text = QInputDialog::getText(this, QString::fromLocal8Bit("数据修改"),
            QString::fromLocal8Bit("请输入："), QLineEdit::Normal,
            model->data(clickedIndex).toString(), &ok);
        if (ok && !text.isEmpty()) {             //确认并且有数据执行
            model->setData(clickedIndex, text);
        }
    }
}

//串口与服务器界面切换
void ZLCom::comod_cotcp(void)
{
    ui.stackedWidget->setCurrentIndex(0);
    ui.COMxuanz->setDisabled(true);
    ui.fuwuqi->setDisabled(false);

    connect(ui.fuwuqi, &QPushButton::clicked, [&]() {
        ui.stackedWidget->setCurrentIndex(1);
        ui.COMxuanz->setDisabled(false);
        ui.fuwuqi->setDisabled(true);
        });

    connect(ui.COMxuanz, &QPushButton::clicked, [&]() {
        ui.stackedWidget->setCurrentIndex(0);
        ui.COMxuanz->setDisabled(true);
        ui.fuwuqi->setDisabled(false);
        });
}

//点击扫描端口
void ZLCom::JiaoCom_JiShi(void)
{
    connect(ui.comko, SIGNAL(clicked()), this, SLOT(jiacom())); 
    connect(ui.PLC_ComKoXuanZe, SIGNAL(clicked()), this, SLOT(PLCjiacom()));
}

//扫描com端口
void ZLCom::jiacom(void)
{
    QVector<QString>temp;
    for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts())
    {
        temp.push_back(info.portName());//添加元素
    }

    std::sort(temp.begin(), temp.end());//去重排序

    if (temp != ports)//如果与上次数据不同
    {
        ui.comko->clear();
        this->ports = temp;
        for (auto& a : ports) ui.comko->addItem(a);
    }
    
    if (ui.dakaicom->text() == (QString::fromLocal8Bit("断开")))
    {
        serialPort->close();
        dakai_gongn(ui.comzhuangtai, ui.dakaicom);
    }

}

//扫描plccom端口
void ZLCom::PLCjiacom(void)
{
    QVector<QString>temp;
    for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts())
    {
        temp.push_back(info.portName());//添加元素
    }

    std::sort(temp.begin(), temp.end());//去重排序

    if (temp != Plcports)//如果与上次数据不同
    {
        ui.PLC_ComKoXuanZe->clear();
        this->Plcports = temp;
        for (auto& a : Plcports) ui.PLC_ComKoXuanZe->addItem(a);
    }

    if (ui.dakaicom->text() == (QString::fromLocal8Bit("断开")))
    {
        serialPort->close();
        dakai_gongn(ui.PLC_TongXunZhuangTaiDeng, ui.PLC_Com_KaiQi);
    }
}

//按键 打开com
void ZLCom::com_dakai(void)
{
    ui.comzhuangtai->setStyleSheet("QRadioButton::indicator:unchecked {background-color:red;}");//设置初始状态
    ui.dakaicom->setText(QString::fromLocal8Bit("打开"));
    connect(ui.dakaicom, &QPushButton::clicked, [&]() {
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("打开")))Com_KaiQi();
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("断开"))) serialPort->close();
        if (duankozhuangtai == 1 || ui.dakaicom->text() == (QString::fromLocal8Bit("断开")))dakai_gongn(ui.comzhuangtai, ui.dakaicom);
        });
}

//数据接收
void ZLCom::Com_KaiQi(void)
{
    QSerialPort::BaudRate Baud;//波特率
    QSerialPort::DataBits Data;//数据位
    QSerialPort::Parity Check;//校验位
    QSerialPort::StopBits Stop;//停止位
    
    QString ChuanKoMing = ui.comko->currentText();
    QString ChuankoBoTeLvWei = ui.combotelv->currentText();
    QString ChuanKoShuJuWei = ui.comshujuwei->currentText();
    QString ChuanKoXiaoYanWei = ui.comxiaoyan->currentText();
    QString ChuanKoTingZhiWei = ui.comtingzhi->currentText();

    if (ChuankoBoTeLvWei == "1200") Baud = QSerialPort::Baud1200;
    else if (ChuankoBoTeLvWei == "2400") Baud = QSerialPort::Baud2400;
    else if (ChuankoBoTeLvWei == "4800") Baud = QSerialPort::Baud4800;
    else if (ChuankoBoTeLvWei == "9600") Baud = QSerialPort::Baud9600;
    else if (ChuankoBoTeLvWei == "19200") Baud = QSerialPort::Baud19200;
    else if (ChuankoBoTeLvWei == "38400") Baud = QSerialPort::Baud38400;
    else if (ChuankoBoTeLvWei == "57600") Baud = QSerialPort::Baud57600;
    else if (ChuankoBoTeLvWei == "115200") Baud = QSerialPort::Baud115200;

    if (ChuanKoShuJuWei == "8") Data = QSerialPort::Data8;
    else if (ChuanKoShuJuWei == "7") Data = QSerialPort::Data7;

    if (ChuanKoXiaoYanWei == "None") Check = QSerialPort::NoParity;
    else if (ChuanKoXiaoYanWei == "Even") Check = QSerialPort::EvenParity;
    else if (ChuanKoXiaoYanWei == "Odd") Check = QSerialPort::OddParity;
    else if (ChuanKoXiaoYanWei == "Space") Check = QSerialPort::SpaceParity;
    else if (ChuanKoXiaoYanWei == "Mark") Check = QSerialPort::MarkParity;

    if (ChuanKoTingZhiWei == "1") Stop = QSerialPort::OneStop;
    else if (ChuanKoTingZhiWei == "1.5") Stop = QSerialPort::OneAndHalfStop;
    else if (ChuanKoTingZhiWei == "2") Stop = QSerialPort::TwoStop;

    serialPort = new QSerialPort(this);
    serialPort->setBaudRate(Baud);
    serialPort->setDataBits(Data);
    serialPort->setParity(Check);
    serialPort->setStopBits(Stop);
    serialPort->setPortName(ChuanKoMing);

    if (serialPort->open(QSerialPort::ReadWrite))//以读写的方式打开
    {
        connect(serialPort, &QSerialPort::readyRead, [&]() {
            auto data = serialPort->readAll();//readAll 解析数据
            if (ui.BianMa->currentText() == "HEX")//如果是16进制
            {
                QString hex = data.toHex(' ');

                ui.wenbenjiesho->appendPlainText(hex);
            }
            /*else if (receiveMode->currentText() == "文本")
            {
                QString str = QString(data);
                receiveAera->appendPlainText(str);
            }*/
            else if (ui.BianMa->currentText() == "DEC")//10进制
            {
                QString DEC = NULL;
                for (int i = 0; i < data.size(); i++)
                {
                    QByteArray dan = data.mid(i, 1);
                    QString shex = dan.toHex();
                    int idec = shex.toInt(nullptr, 16);
                    QString sdec = QString::number(idec);
                    sdec.append(' ');
                    DEC.append(sdec);
                }
                ui.wenbenjiesho->appendPlainText(DEC);
            }
            else if (ui.BianMa->currentText() == "Bin")//2进制
            {
                QString DEC = NULL;
                for (int i = 0; i < data.size(); i++)
                {
                    QByteArray dan = data.mid(i, 1);
                    QString shex = dan.toHex();
                    int idec = shex.toInt(nullptr, 16);
                    QString sdec = QString::number(idec, 2);
                    if (sdec.size() < 8)
                    {
                        int chang = 8 - sdec.size();
                        for (int j = 0; j < chang; j++)
                        {
                            sdec.prepend('0');
                        }
                    }
                    sdec.append(' ');
                    DEC.append(sdec);
                }
                ui.wenbenjiesho->appendPlainText(DEC);
            }
            else if (ui.BianMa->currentText() == "ASCII")//ASCII码
            {
                QString DEC = NULL;
                for (int i = 0; i < data.size(); i++)
                {
                    QByteArray dan = data.mid(i, 1);
                    QString shex = dan.toHex();
                    int idec = shex.toInt(nullptr, 16);
                    DEC.append(idec);
                }
                ui.wenbenjiesho->appendPlainText(DEC);
            }

            });//readyRead数据刷新用
        duankozhuangtai = 1;
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("串口打开失败"), QString::fromLocal8Bit("请确认串口是否连接正确"));
    }

}

//PLC数据接收
void ZLCom::PLC_ShuJuJieSho(void)
{
    QSerialPort::BaudRate Baud;//波特率
    QSerialPort::DataBits Data;//数据位
    QSerialPort::Parity Check;//校验位
    QSerialPort::StopBits Stop;//停止位

    QString ChuanKoMing = ui.PLC_ComKoXuanZe->currentText();
    QString ChuankoBoTeLvWei = ui.PLC_BoTeLv_XuanZe->currentText();
    QString ChuanKoShuJuWei = ui.PLC_ShuJuWei_XuanZe->currentText();
    QString ChuanKoXiaoYanWei = ui.PLC_XiaoYan_XuanZe->currentText();
    QString ChuanKoTingZhiWei = ui.PLC_TingZhiXuanZe->currentText();

    if (ChuankoBoTeLvWei == "1200") Baud = QSerialPort::Baud1200;
    else if (ChuankoBoTeLvWei == "2400") Baud = QSerialPort::Baud2400;
    else if (ChuankoBoTeLvWei == "4800") Baud = QSerialPort::Baud4800;
    else if (ChuankoBoTeLvWei == "9600") Baud = QSerialPort::Baud9600;
    else if (ChuankoBoTeLvWei == "19200") Baud = QSerialPort::Baud19200;
    else if (ChuankoBoTeLvWei == "38400") Baud = QSerialPort::Baud38400;
    else if (ChuankoBoTeLvWei == "57600") Baud = QSerialPort::Baud57600;
    else if (ChuankoBoTeLvWei == "115200") Baud = QSerialPort::Baud115200;

    if (ChuanKoShuJuWei == "8") Data = QSerialPort::Data8;
    else if (ChuanKoShuJuWei == "7") Data = QSerialPort::Data7;

    if (ChuanKoXiaoYanWei == "None") Check = QSerialPort::NoParity;
    else if (ChuanKoXiaoYanWei == "Even") Check = QSerialPort::EvenParity;
    else if (ChuanKoXiaoYanWei == "Odd") Check = QSerialPort::OddParity;

    if (ChuanKoTingZhiWei == "1") Stop = QSerialPort::OneStop;
    else if (ChuanKoTingZhiWei == "1.5") Stop = QSerialPort::OneAndHalfStop;
    else if (ChuanKoTingZhiWei == "2") Stop = QSerialPort::TwoStop;

    PLC_serialPort = new QSerialPort(this);
    PLC_serialPort->setBaudRate(Baud);
    PLC_serialPort->setDataBits(Data);
    PLC_serialPort->setParity(Check);
    PLC_serialPort->setStopBits(Stop);
    PLC_serialPort->setPortName(ChuanKoMing);
    if (PLC_serialPort->open(QSerialPort::ReadWrite))//以读写的方式打开
    {
        if (PLC_lianjiezhuangtai == 0)PLC_DiYiCiXiaoYao();//第一次打开测试是否正确连接PLC
        //功能测试
        /*connect(ui.pushButton, &QPushButton::clicked, [&]() {
            PLC_ShuJuFaSong(PLC_BiaoDing, PLC_BiaoDi);
            });*/
        

        connect(PLC_serialPort, &QSerialPort::readyRead, [&]() {
            auto data = PLC_serialPort->readAll();//readAll 解析数据
            if (PLC_lianjiezhuangtai == 0) PLC_CeShiJieShoShuJu = data;
            else {
                QByteArray JieSho_ShuJu = data;
                PLC_JieSho_XianShi(JieSho_ShuJu);
            }
            });//readyRead数据刷新用
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("串口打开失败"), QString::fromLocal8Bit("请确认串口是否连接正确"));
    }
}

//获取表顶，表底并计算长度，发送报文
void ZLCom::PLC_ShuJuFaSong(int di, int ding )
{
    //固定报文
    QString FS_ShuJu = "0103";
    //D区起始地址
    int ShuJuQiShiDiZhi = 0x1000;
    //计算数据起始地址，确认报文
    int ShuJu_QiShidiZhi = ShuJuQiShiDiZhi + (di * 3);

    FS_ShuJu.append(QString::number(ShuJu_QiShidiZhi, 16));

    //计算数据长度，确认报文
    BiaoGe_ShuJuLiang = ((ding - di) + 1) * 3;
    
    QString FS_ChangDu = QString::number(BiaoGe_ShuJuLiang, 16);
    if (FS_ChangDu.size() < 4)
    {
        int chang = 4 - FS_ChangDu.size();
        for (int j = 0; j < chang; j++)
        {
            FS_ChangDu.prepend('0');
        }
    }
    FS_ShuJu.append(FS_ChangDu);
    FS_ChangDu = PLC_XiaoYan(FS_ShuJu);//计算校验
    FS_ShuJu.append(FS_ChangDu);

    //数据格式转换
    QByteArray FA = NULL;
    for (int i = 0; i < FS_ShuJu.size(); i++)
    {
        int num = FS_ShuJu.mid(i, 2).toUInt(nullptr, 16);
        FA.append(num);
        ++i;
    }
    //发送报文
    PLC_serialPort->write(FA);
}

//数据定时，持续发送
void ZLCom::ShuJuFaSong(void)
{
    PLC_LJOK_FaSong_BaoWen = new QTimer(this);
    PLC_LJOK_FaSong_BaoWen->start(100);
    connect(PLC_LJOK_FaSong_BaoWen, &QTimer::timeout, [&]() {PLC_ShuJuFaSong(PLC_BiaoDing, PLC_BiaoDi); });
}

//解析接收数据，并打印到表格
void ZLCom::PLC_JieSho_XianShi(QByteArray shuju)
{
    //接收数据
    QByteArray JS_Shu_Ju = shuju;
    //判断数据是否标准格式
    QByteArray PanDuan = JS_Shu_Ju.mid(0, 1);
    QString shex = PanDuan.toHex();
    int idec = shex.toInt(nullptr, 16);
    int SJ_chang = 0;
    int Lie = 0;
    int hang = PLC_BiaoDing;
    if (idec == 1)//判断站号
    {
        PanDuan = JS_Shu_Ju.mid(1, 1);
        shex = PanDuan.toHex();
        idec = shex.toInt(nullptr, 16);
        if (idec == 3)//判断功能码
        {
            PanDuan = JS_Shu_Ju.mid(2, 1);
            shex = PanDuan.toHex();
            idec = shex.toInt(nullptr, 16);

            if (idec == BiaoGe_ShuJuLiang * 2)//判断数据长度
            {
                SJ_chang = BiaoGe_ShuJuLiang * 2;
                //判断校验是否正确
                if (PLC_JieShuXiaoYan(JS_Shu_Ju) == 1)
                {
                    for (int i = 0; i < SJ_chang; i++)
                    {
                        PanDuan = JS_Shu_Ju.mid(i+3, 2);
                        shex = PanDuan.toHex();
                        idec = shex.toInt(nullptr, 16);
                        shex = QString::number(idec);
                        ++i;
                        ++Lie;
                        ZuoBiao = model->index(hang, Lie);
                        model->setData(ZuoBiao, shex, Qt::EditRole);
                        ++Lie;
                        if (Lie > 5)
                        {
                            ++hang;
                            Lie = 0;
                        }
                    }
                }
            }
        }

    }

}

//第一次打开测试是否正确连接PLC
void ZLCom::PLC_DiYiCiXiaoYao(void)
{
    QByteArray PLC_shuju = NULL;
    QString PLC_FaSongShuJu = "01031000000180CA";

    for (int i = 0; i < PLC_FaSongShuJu.size(); i++)
    {
        int num = PLC_FaSongShuJu.mid(i, 2).toUInt(nullptr, 16);
        PLC_shuju.append(num);
        ++i;
    }
    SuoYouShuJu = PLC_shuju;
    if (fasongjishu == 0)PLC_serialPort->write(PLC_shuju);
    PLC_LianJie = new QTimer(this);
    connect(PLC_LianJie, &QTimer::timeout, [&]() {panduan();
    if (fasongjishu > 3)
    {
        PLC_LianJie->stop();
        fasongjishu = 0;
        PLC_serialPort->close();
        PLC_CeShiJieShoShuJu = NULL;
        SuoYouShuJu = NULL;
        QMessageBox::critical(this, QString::fromLocal8Bit("PLC数据回复错误"), QString::fromLocal8Bit("请确认参数设置或PLC通讯参数"));
    }      
    if (PLC_lianjiezhuangtai == 1)
    {
        fasongjishu = 0;
        SuoYouShuJu = NULL;
        PLC_LianJie->stop();
    }
        });
    PLC_LianJie->start(80);

    
}

//判断是否打开
void ZLCom::panduan(void)
{
    QByteArray fasong_plcshuju = SuoYouShuJu;
    if (PLC_CeShiJieShoShuJu != "")
    {
        QString hex = PLC_CeShiJieShoShuJu.toHex();
        if (hex[1] == "1")
        {
            if (hex[3] == "3")
            {
                if (hex[5] == "2")
                {
                    int shuju_qba = PLC_CeShiJieShoShuJu.size();
                    int shu = 0;
                    QString xiaoyan = NULL;
                    uint16_t wcrc = 0XFFFF;
                    uint8_t temp = 0;
                    uint16_t jie = 0;
                    uint8_t xy = 0;
                    for (int i = 0; i < shuju_qba-2; i++)
                    {
                        temp = PLC_CeShiJieShoShuJu.at(i);
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

                    xy = jie;
                    shu = int(xy);
                    xiaoyan = QString::number(shu, 16);
                    QByteArray jiesho = PLC_CeShiJieShoShuJu.mid(5, 1);
                    QString XiaoYan = jiesho.toHex();
                    if (xiaoyan == XiaoYan )
                    {
                        xy = jie>>8;
                        shu = int(xy);
                        xiaoyan = QString::number(shu, 16);
                        jiesho = PLC_CeShiJieShoShuJu.mid(6, 1);
                        XiaoYan = jiesho.toHex();
                        if (xiaoyan == XiaoYan )
                        {
                            PLC_lianjiezhuangtai = 1;
                            PLC_duankozhuangtai = 1;
                            PLC_CeShiJieShoShuJu = NULL;
                            dakai_gongn(ui.PLC_TongXunZhuangTaiDeng, ui.PLC_Com_KaiQi);
                            toggleTableVisibility();
                            ShuJuFaSong();
                        }
                    }
                }
            }
        }
        else return;
    }
    else PLC_serialPort->write(fasong_plcshuju);
    ++fasongjishu;
}

//按键 打开com
void ZLCom::PLCcom_dakai(void)
{
    ui.PLC_TongXunZhuangTaiDeng->setStyleSheet("QRadioButton::indicator:unchecked {background-color:red;}");//设置初始状态
    ui.PLC_Com_KaiQi->setText(QString::fromLocal8Bit("打开"));
    connect(ui.PLC_Com_KaiQi, &QPushButton::clicked, [&]() {
        if (ui.PLC_Com_KaiQi->text() == (QString::fromLocal8Bit("打开")))PLC_ShuJuJieSho();
        if (ui.PLC_Com_KaiQi->text() == (QString::fromLocal8Bit("断开")))
        {
            PLC_serialPort->close();
            PLC_lianjiezhuangtai = 0;//
            PLC_duankozhuangtai = 0;//
            PLC_LJOK_FaSong_BaoWen->stop();
        } 
        if (PLC_duankozhuangtai == 1 || ui.PLC_Com_KaiQi->text() == (QString::fromLocal8Bit("断开")))dakai_gongn(ui.PLC_TongXunZhuangTaiDeng, ui.PLC_Com_KaiQi);
        });
}

//打开显示 功能
void ZLCom::dakai_gongn(QRadioButton* xinhao, QPushButton* anjian)
{
    xinhao->setStyleSheet( "QRadioButton::indicator:checked {background-color:green;}"
        "QRadioButton::indicator:unchecked {background-color:red;}"
    );
    QString content = anjian->text();
    if (QString::compare(content, QString::fromLocal8Bit("打开"), Qt::CaseInsensitive) == 0)
    {
        xinhao->setChecked(true);
        anjian->setText(QString::fromLocal8Bit("断开"));
    }
    else
    {
        xinhao->setChecked(false);
        anjian->setText(QString::fromLocal8Bit("打开"));
    }

}

//屏幕缩放比例适配1920*1080
void ZLCom::PingMu_FenBianLv(void)
{
    QuanJuSuoFang = QGuiApplication::primaryScreen()->logicalDotsPerInch();//获取全局缩放比例
    if (QuanJuSuoFang == 96) PingMuBiLie = 1.0;
    else if (QuanJuSuoFang == 120) PingMuBiLie = 1.25;
    else if (QuanJuSuoFang == 144) PingMuBiLie = 1.5;
    else if (QuanJuSuoFang == 192) PingMuBiLie = 2.0;

    //获取当前窗口宽高，进行等比例缩放
    int chuangko_x = 0, chuangko_y = 0;
    chuangko_x = geometry().width();
    chuangko_y = geometry().height();
    chuangko_x = float(chuangko_x) * PingMuBiLie;
    chuangko_y = float(chuangko_y) * PingMuBiLie;
    this->setFixedSize(chuangko_x, chuangko_y);

    //切换窗口
    SuoFang_qstackedwidget(ui.QieHuanChuangKo, PingMuBiLie);
    SuoFang_qstackedwidget(ui.stackedWidget, PingMuBiLie);
    SuoFang_qstackedwidget(ui.PLC_JiCunQiDiShi, PingMuBiLie);

    //文字标签
    SuoFang_qlabel(ui.label, PingMuBiLie);
    SuoFang_qlabel(ui.label_2, PingMuBiLie);
    SuoFang_qlabel(ui.label_3, PingMuBiLie);
    SuoFang_qlabel(ui.label_4, PingMuBiLie);
    SuoFang_qlabel(ui.label_5, PingMuBiLie);
    SuoFang_qlabel(ui.label_6, PingMuBiLie);
    SuoFang_qlabel(ui.label_7, PingMuBiLie);
    SuoFang_qlabel(ui.label_8, PingMuBiLie);
    SuoFang_qlabel(ui.label_9, PingMuBiLie);
    SuoFang_qlabel(ui.XianShiGeShi, PingMuBiLie);
    SuoFang_qlabel(ui.PLC_XingHao, PingMuBiLie);
    SuoFang_qlabel(ui.PLC_TongXun, PingMuBiLie);
    SuoFang_qlabel(ui.PLC_ComKo, PingMuBiLie);
    SuoFang_qlabel(ui.PLC_BoTeLv, PingMuBiLie);
    SuoFang_qlabel(ui.PLC_ShuJuWei, PingMuBiLie);
    SuoFang_qlabel(ui.PLC_XiaoYanWei, PingMuBiLie);
    SuoFang_qlabel(ui.PLC_TingZhiWei, PingMuBiLie);


    //下拉选项
    SuoFang_qcombobox(ui.comko, PingMuBiLie);
    SuoFang_qcombobox(ui.combotelv, PingMuBiLie);
    SuoFang_qcombobox(ui.comshujuwei, PingMuBiLie);
    SuoFang_qcombobox(ui.comxiaoyan, PingMuBiLie);
    SuoFang_qcombobox(ui.comtingzhi, PingMuBiLie);
    SuoFang_qcombobox(ui.BianMa, PingMuBiLie);
    SuoFang_qcombobox(ui.fuwuql, PingMuBiLie);
    SuoFang_qcombobox(ui.fuwuql_IP, PingMuBiLie);
    SuoFang_qcombobox(ui.fuwuql_bendi, PingMuBiLie);
    SuoFang_qcombobox(ui.Plc_XingHao_XuanZe, PingMuBiLie);
    SuoFang_qcombobox(ui.PLC_TongXun_GeShi, PingMuBiLie);
    SuoFang_qcombobox(ui.PLC_ComKoXuanZe, PingMuBiLie);
    SuoFang_qcombobox(ui.PLC_BoTeLv_XuanZe, PingMuBiLie);
    SuoFang_qcombobox(ui.PLC_ShuJuWei_XuanZe, PingMuBiLie);
    SuoFang_qcombobox(ui.PLC_XiaoYan_XuanZe, PingMuBiLie);
    SuoFang_qcombobox(ui.PLC_TingZhiXuanZe, PingMuBiLie);



    //勾选
    SuoFang_qradiobutton(ui.comzhuangtai, PingMuBiLie);
    SuoFang_qradiobutton(ui.xiaoyan_1, PingMuBiLie);
    SuoFang_qradiobutton(ui.xiaoyan_2, PingMuBiLie);
    SuoFang_qradiobutton(ui.xiaoyan_3, PingMuBiLie);
    SuoFang_qradiobutton(ui.comzhuangtai_2, PingMuBiLie);
    SuoFang_qradiobutton(ui.PLC_TongXunZhuangTaiDeng, PingMuBiLie);


    //按键下拉
    SuoFang_qtoolbutton(ui.gongJu, PingMuBiLie);


    //按钮
    SuoFang_qpushbutton(ui.COMxuanz, PingMuBiLie);
    SuoFang_qpushbutton(ui.fuwuqi, PingMuBiLie);
    SuoFang_qpushbutton(ui.shujuqk, PingMuBiLie);
    SuoFang_qpushbutton(ui.TXTzanting, PingMuBiLie);
    SuoFang_qpushbutton(ui.fasong_qingk1_1, PingMuBiLie);
    SuoFang_qpushbutton(ui.fasong_qingk1_2, PingMuBiLie);
    SuoFang_qpushbutton(ui.fasong_qingk1_3, PingMuBiLie);
    SuoFang_qpushbutton(ui.fa1, PingMuBiLie);
    SuoFang_qpushbutton(ui.fa2, PingMuBiLie);
    SuoFang_qpushbutton(ui.fa3, PingMuBiLie);
    SuoFang_qpushbutton(ui.set_stop, PingMuBiLie);
    SuoFang_qpushbutton(ui.dakaicom, PingMuBiLie);
    SuoFang_qpushbutton(ui.fuwuqi_chuangj, PingMuBiLie);
    SuoFang_qpushbutton(ui.PLC_Com_KaiQi, PingMuBiLie);
    SuoFang_qpushbutton(ui.TaiDa_D, PingMuBiLie);
    SuoFang_qpushbutton(ui.DiZhiChaZhao, PingMuBiLie);

    //勾选框
    SuoFang_qcheckbox(ui.Ding_a, PingMuBiLie);
    SuoFang_qcheckbox(ui.Ding_b, PingMuBiLie);
    SuoFang_qcheckbox(ui.Ding_c, PingMuBiLie);
    SuoFang_qcheckbox(ui.Fa_a, PingMuBiLie);
    SuoFang_qcheckbox(ui.Fa_b, PingMuBiLie);
    SuoFang_qcheckbox(ui.Fa_c, PingMuBiLie);

    //单行文本框
    SuoFang_qlineedit(ui.jishi, PingMuBiLie);
    SuoFang_qlineedit(ui.jishi_2, PingMuBiLie);
    SuoFang_qlineedit(ui.jishi_3, PingMuBiLie);
    SuoFang_qlineedit(ui.fasong, PingMuBiLie);
    SuoFang_qlineedit(ui.fasong2, PingMuBiLie);
    SuoFang_qlineedit(ui.fasong3, PingMuBiLie);
    SuoFang_qlineedit(ui.JiCunQi, PingMuBiLie);


    //TEXT文本接收框
    SuoFang_qplaintextedit(ui.wenbenjiesho, PingMuBiLie);

    //表格类缩放
    SuoFang_qtableView(ui.tableView, PingMuBiLie);

}

//发送数据扫描计时
void ZLCom::FaSong_SaoMiao(void)    
{
    
    QTimer* shuju_t = new QTimer(this);
    connect(shuju_t, &QTimer::timeout, [&]() {
        if (fa_a == 1) FaSong_a();
        });//一号发送口

    connect(shuju_t, &QTimer::timeout, [&]() {
        if (fa_b == 1) FaSong_b();
        });//二号发送口

    connect(shuju_t, &QTimer::timeout, [&]() {
        if (fa_c == 1) FaSong_c();
        });//三号发送口

    shuju_t->start(5);

    //发送数据状态切换
    connect(ui.Fa_a, &QCheckBox::clicked, [&](bool checked) {
        if (checked) { fa_a = 1;  ui.fasong->clear();
        }
        else { //timer->stop();
            fa_a = 0;
        }
        });

    connect(ui.Fa_b, &QCheckBox::clicked, [&](bool checked) {
        if (checked) { fa_b = 1; ui.fasong2->clear();
        }
        else { //timer->stop();
            fa_b = 0;
        }
        });

    connect(ui.Fa_c, &QCheckBox::clicked, [&](bool checked) {
        if (checked) { fa_c = 1; ui.fasong3->clear();
        }
        else { //timer->stop();
            fa_c = 0;
        }
        });

    connect(ui.xiaoyan_1, &QRadioButton::clicked, [&]() {
        XiaoYan_ZhuangTai = 1;
        });

    connect(ui.xiaoyan_2, &QRadioButton::clicked, [&]() {
        XiaoYan_ZhuangTai = 2;
        });

    connect(ui.xiaoyan_3, &QRadioButton::clicked, [&]() {
        XiaoYan_ZhuangTai = 3;
        });


}

//数据清空
void ZLCom::FaSong_QingKong(void)
{
    //发送数据清空
    connect(ui.shujuqk, &QPushButton::clicked, [&](bool checked) {
        ui.wenbenjiesho->clear();
        });

    //1
    connect(ui.fasong_qingk1_1, &QPushButton::clicked, [&](bool checked) {
         ui.fasong->clear();
        });

    //2
    connect(ui.fasong_qingk1_2, &QPushButton::clicked, [&](bool checked) {
        ui.fasong2->clear();
        });

    //3
    connect(ui.fasong_qingk1_3, &QPushButton::clicked, [&](bool checked) {
        ui.fasong3->clear();
        });    
}

//一号数据显示内容
void ZLCom::FaSong_a(void)
{
    QString str = ui.fasong->text();
    if (str != "" && FaSong_ShuJu1 != NULL)
    {
        if (QString::compare(str, FaSong_ShuJu1, Qt::CaseInsensitive) != 0)
        {
            QString neirong = str;
            int jin = 0, chu = 0;
            Fasong_Guangbiao = ui.fasong->cursorPosition();
            neirong = neirong.replace(" ", "");    // 去除所有的空格
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // 计算需插入空格的数量
            for (int i = 1; i < insertCount; i++) {                             // 遍历插入空格
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong->clear();
            ui.fasong->setText(neirong);
            QString nei = ui.fasong->text();
            jin = neirong.size();
            chu = FaSong_ShuJu1.size();
            if (xiaoyanbiaoji == 1) 
            {
                ui.fasong->setCursorPosition(ui.fasong->text().size());
                xiaoyanbiaoji = 0;
            }
            else if (chu < jin && Fasong_Guangbiao != neirong.size())
            {
                if (Fasong_Guangbiao % 3 == 0)ui.fasong->setCursorPosition(Fasong_Guangbiao + 1);
                else ui.fasong->setCursorPosition(Fasong_Guangbiao);
            }
            else if (chu > jin && Fasong_Guangbiao != neirong.size())
            {
                if (Fasong_Guangbiao % 3 == 0)ui.fasong->setCursorPosition(Fasong_Guangbiao - 1);
                else ui.fasong->setCursorPosition(Fasong_Guangbiao);
                
            }

            FaSong_ShuJu1 = nei;
        }
    }
    else
    {
        if (str != "")
        {
            Fasong_Guangbiao = ui.fasong->cursorPosition();
            QString neirong = str;
            neirong = neirong.replace(" ", "");    // 去除所有的空格
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // 计算需插入空格的数量
            for (int i = 1; i < insertCount; i++) {                             // 遍历插入空格
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong->clear();
            ui.fasong->setText(neirong);

            QString nei = ui.fasong->text();
            FaSong_ShuJu1 = nei;
        }
    }
}

//二号数据显示内容
void ZLCom::FaSong_b(void)
{
    QString str = ui.fasong2->text();
    if (str != "" && FaSong_ShuJu2 != NULL)
    {
        if (QString::compare(str, FaSong_ShuJu2, Qt::CaseInsensitive) != 0)
        {
            QString neirong = str;
            Fasong_Guangbiao = ui.fasong2->cursorPosition();
            int jin = 0, chu = 0;
            neirong = neirong.replace(" ", "");    // 去除所有的空格
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // 计算需插入空格的数量
            for (int i = 1; i < insertCount; i++) {                             // 遍历插入空格
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong2->clear();
            ui.fasong2->setText(neirong);
            QString nei = ui.fasong2->text();
            jin = neirong.size();
            chu = FaSong_ShuJu2.size();
            if (xiaoyanbiaoji == 1)
            {
                ui.fasong->setCursorPosition(ui.fasong->text().size());
                xiaoyanbiaoji = 0;
            }
            else if (chu < jin && Fasong_Guangbiao != neirong.size())
            {
                if (Fasong_Guangbiao % 3 == 0)ui.fasong->setCursorPosition(Fasong_Guangbiao + 1);
                else ui.fasong->setCursorPosition(Fasong_Guangbiao);
            }
            else if (chu > jin && Fasong_Guangbiao != neirong.size())
            {
                if (Fasong_Guangbiao % 3 == 0)ui.fasong->setCursorPosition(Fasong_Guangbiao - 1);
                else ui.fasong->setCursorPosition(Fasong_Guangbiao);

            }
            FaSong_ShuJu2 = nei;
        }
    }
    else
    {
        if (str != "")
        {
            Fasong_Guangbiao = ui.fasong2->cursorPosition();
            QString neirong = str;
            neirong = neirong.replace(" ", "");    // 去除所有的空格
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // 计算需插入空格的数量
            for (int i = 1; i < insertCount; i++) {                             // 遍历插入空格
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong2->clear();
            ui.fasong2->setText(neirong);

            QString nei = ui.fasong2->text();
            FaSong_ShuJu2 = nei;
        }
    }
}

//三号数据显示内容
void ZLCom::FaSong_c(void)
{
    QString str = ui.fasong3->text();
    if (str != "" && FaSong_ShuJu3 != NULL)
    {
        if (QString::compare(str, FaSong_ShuJu3, Qt::CaseInsensitive) != 0)
        {
            QString neirong = str;
            Fasong_Guangbiao = ui.fasong3->cursorPosition();
            int jin = 0, chu = 0;
            
            neirong = neirong.replace(" ", "");    // 去除所有的空格
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // 计算需插入空格的数量
            for (int i = 1; i < insertCount; i++) {                             // 遍历插入空格
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong3->clear();
            ui.fasong3->setText(neirong);
            QString nei = ui.fasong3->text();
            jin = neirong.size();
            chu = FaSong_ShuJu3.size();
            if (xiaoyanbiaoji == 1)
            {
                ui.fasong->setCursorPosition(ui.fasong->text().size());
                xiaoyanbiaoji = 0;
            }
            else if (chu < jin && Fasong_Guangbiao != neirong.size())
            {
                if (Fasong_Guangbiao % 3 == 0)ui.fasong->setCursorPosition(Fasong_Guangbiao + 1);
                else ui.fasong->setCursorPosition(Fasong_Guangbiao);
            }
            else if (chu > jin && Fasong_Guangbiao != neirong.size())
            {
                if (Fasong_Guangbiao % 3 == 0)ui.fasong->setCursorPosition(Fasong_Guangbiao - 1);
                else ui.fasong->setCursorPosition(Fasong_Guangbiao);

            }
            FaSong_ShuJu3 = nei;
        }
    }
    else
    {
        if (str != "")
        {
            Fasong_Guangbiao = ui.fasong3->cursorPosition();
            QString neirong = str;
            neirong = neirong.replace(" ", "");    // 去除所有的空格
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // 计算需插入空格的数量
            for (int i = 1; i < insertCount; i++) {                             // 遍历插入空格
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong3->clear();
            ui.fasong3->setText(neirong);

            QString nei = ui.fasong3->text();
            FaSong_ShuJu3 = nei;
        }
    }
}

//发送数据整合
void ZLCom::ComShuJu_FaSong(QLineEdit* dizhi, QCheckBox* zhuangtai)
{
    QString data = dizhi->text();
    data = data.replace(" ", "");
    if (zhuangtai)
    {
        QByteArray arr;
        for (int i = 0; i < data.size(); i++)
        {
            int num = data.mid(i, 2).toUInt(nullptr, 16);
            ++i;
            arr.append(num);
        }
        serialPort->write(arr);
    }
    else
    {
        serialPort->write(data.toLocal8Bit().data());
    }
       
}

//发送数据
void ZLCom::Com_FaSong(void)
{
    connect(ui.fa1, &QPushButton::clicked, [&]() {
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("断开")))ComShuJu_FaSong(ui.fasong, ui.Fa_a);
        else QMessageBox::critical(this, QString::fromLocal8Bit("串口打开失败"), QString::fromLocal8Bit("请确认串口是否连接正确"));
        });

    connect(ui.fa2, &QPushButton::clicked, [&]() {
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("断开")))ComShuJu_FaSong(ui.fasong2, ui.Fa_b);
        else QMessageBox::critical(this, QString::fromLocal8Bit("串口打开失败"), QString::fromLocal8Bit("请确认串口是否连接正确"));
        });

    connect(ui.fa3, &QPushButton::clicked, [&]() {
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("断开")))ComShuJu_FaSong(ui.fasong3, ui.Fa_c);
        else QMessageBox::critical(this, QString::fromLocal8Bit("串口打开失败"), QString::fromLocal8Bit("请确认串口是否连接正确"));
        });
}

//添加菜单选择
void ZLCom::CaiDan_TianJia(void)
{
    //ui.wenbenjiesho->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(ui.wenbenjiesho, &QTextEdit::customContextMenuRequested, [&]() {
    //    QMenu* stdMenu = ui.wenbenjiesho->createStandardContextMenu();//添加菜单选择
    //    QAction* clearAction = new QAction(QString::fromLocal8Bit("无效"), ui.wenbenjiesho);//按键名称
    //    stdMenu->addAction(clearAction);//添加
    //    QObject::connect(clearAction, &QAction::triggered, [&]() {

    //        });//菜单触发操作
    //    stdMenu->move(cursor().pos());//当前鼠标位置
    //    stdMenu->show();//显示菜单
    //    });

    //一号发送窗口添加功能
    ui.fasong->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.fasong, &QTextEdit::customContextMenuRequested, [&]() {
        QMenu* stdMenu = ui.fasong->createStandardContextMenu();//添加菜单选择

        QAction* SUMXiaoYan = new QAction(QString::fromLocal8Bit("SUM校验"), ui.fasong);//按键名称
        QAction* BccXiaoYan = new QAction(QString::fromLocal8Bit("BCC(异或校验)"), ui.fasong);//按键名称
        QAction* Crc8XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC8)"), ui.fasong);//按键名称
        QAction* clearAction = new QAction(QString::fromLocal8Bit("LRC(纵向沉余校验)"), ui.fasong);//按键名称
        QAction* CrcXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-MODBUS)"), ui.fasong);//按键名称
        QAction* CrcUsbXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-USB)"), ui.fasong);//按键名称
        QAction* Crc32XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC32)"), ui.fasong);//按键名称

        stdMenu->addAction(SUMXiaoYan);
        stdMenu->addAction(BccXiaoYan);
        stdMenu->addAction(Crc8XiaoYan);
        stdMenu->addAction(clearAction);//添加
        stdMenu->addAction(CrcXiaoYan);
        stdMenu->addAction(CrcUsbXiaoYan);
        stdMenu->addAction(Crc32XiaoYan);

        QObject::connect(SUMXiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_SUMJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
            }

            });

        //BCC
        QObject::connect(BccXiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_BCCJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
            }

            });//菜单触发操作

        //CRC8
        QObject::connect(Crc8XiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_Crc8JiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
            }

            });//菜单触发操作

        //LRC
        QObject::connect(clearAction, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_LrcJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
            }

            });//菜单触发操作

        //CRC16-MODBUS
        QObject::connect(CrcXiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_CrcJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
            }

            });//菜单触发操作

        QObject::connect(CrcUsbXiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CRC_USB = 1;
                CaiDan_TianJia_CrcJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
            }

            });//菜单触发操作

        QObject::connect(Crc32XiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_Crc32JiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
            }

            });

        stdMenu->move(cursor().pos());//当前鼠标位置
        stdMenu->show();//显示菜单

     });

     //2号发送窗口添加功能
     ui.fasong2->setContextMenuPolicy(Qt::CustomContextMenu);
     connect(ui.fasong2, &QTextEdit::customContextMenuRequested, [&]() {
         QMenu* stdMenu = ui.fasong2->createStandardContextMenu();//添加菜单选择

         QAction* SUMXiaoYan = new QAction(QString::fromLocal8Bit("SUM校验"), ui.fasong2);//按键名称
         QAction* BccXiaoYan = new QAction(QString::fromLocal8Bit("BCC(异或校验)"), ui.fasong2);//按键名称
         QAction* Crc8XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC8)"), ui.fasong2);//按键名称
         QAction* clearAction = new QAction(QString::fromLocal8Bit("LRC(纵向沉余校验)"), ui.fasong2);//按键名称
         QAction* CrcXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-MODBUS)"), ui.fasong2);//按键名称
         QAction* CrcUsbXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-USB)"), ui.fasong2);//按键名称
         QAction* Crc32XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC32)"), ui.fasong2);//按键名称

         stdMenu->addAction(SUMXiaoYan);
         stdMenu->addAction(BccXiaoYan);
         stdMenu->addAction(Crc8XiaoYan);
         stdMenu->addAction(clearAction);//添加
         stdMenu->addAction(CrcXiaoYan);
         stdMenu->addAction(CrcUsbXiaoYan);
         stdMenu->addAction(Crc32XiaoYan);

         QObject::connect(SUMXiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_SUMJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
             }

             });

         //BCC
         QObject::connect(BccXiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_BCCJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
             }

             });//菜单触发操作

         //CRC8
         QObject::connect(Crc8XiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_Crc8JiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
             }

             });//菜单触发操作

         //LRC
         QObject::connect(clearAction, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_LrcJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
             }

             });//菜单触发操作

         //CRC16-MODBUS
         QObject::connect(CrcXiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_CrcJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
             }

             });//菜单触发操作

         QObject::connect(CrcUsbXiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CRC_USB = 1;
                 CaiDan_TianJia_CrcJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
             }

             });//菜单触发操作

         QObject::connect(Crc32XiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_Crc32JiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
             }

             });

         stdMenu->move(cursor().pos());//当前鼠标位置
         stdMenu->show();//显示菜单

      });

      //3号发送窗口添加功能
      ui.fasong3->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(ui.fasong3, &QTextEdit::customContextMenuRequested, [&]() {
          QMenu* stdMenu = ui.fasong3->createStandardContextMenu();//添加菜单选择

          QAction* SUMXiaoYan = new QAction(QString::fromLocal8Bit("SUM校验"), ui.fasong3);//按键名称
          QAction* BccXiaoYan = new QAction(QString::fromLocal8Bit("BCC(异或校验)"), ui.fasong3);//按键名称
          QAction* Crc8XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC8)"), ui.fasong3);//按键名称
          QAction* clearAction = new QAction(QString::fromLocal8Bit("LRC(纵向沉余校验)"), ui.fasong3);//按键名称
          QAction* CrcXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-MODBUS)"), ui.fasong3);//按键名称
          QAction* CrcUsbXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-USB)"), ui.fasong3);//按键名称
          QAction* Crc32XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC32)"), ui.fasong3);//按键名称

          stdMenu->addAction(SUMXiaoYan);
          stdMenu->addAction(BccXiaoYan);
          stdMenu->addAction(Crc8XiaoYan);
          stdMenu->addAction(clearAction);//添加
          stdMenu->addAction(CrcXiaoYan);
          stdMenu->addAction(CrcUsbXiaoYan);
          stdMenu->addAction(Crc32XiaoYan);

          QObject::connect(SUMXiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_SUMJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
              }

              });

          //BCC
          QObject::connect(BccXiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_BCCJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
              }

              });//菜单触发操作

          //CRC8
          QObject::connect(Crc8XiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_Crc8JiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
              }

              });//菜单触发操作

          //LRC
          QObject::connect(clearAction, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_LrcJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
              }

              });//菜单触发操作

          //CRC16-MODBUS
          QObject::connect(CrcXiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_CrcJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
              }

              });//菜单触发操作

          QObject::connect(CrcUsbXiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CRC_USB = 1;
                  CaiDan_TianJia_CrcJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
              }

              });//菜单触发操作

          QObject::connect(Crc32XiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_Crc32JiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("仅在勾选HEX下可使用"));
              }

              });

          stdMenu->move(cursor().pos());//当前鼠标位置
          stdMenu->show();//显示菜单

       });

}

//SUM校验
void ZLCom::CaiDan_TianJia_SUMJiaoYan(QLineEdit* dizhi)
{
    QString FaSong_ShuJu = dizhi->text();
    FaSong_ShuJu = FaSong_ShuJu.replace(" ", "");
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
        uint16_t wcrc = 0;
        uint8_t temp = 0;
        uint16_t jie = 0;
        for (int i = 0; i < shuju_qba; i++)
        {
            temp = ShuJu.at(i);
            wcrc += temp;
        }
        jie = wcrc;

        FaSong_ShuJu.append(XiaoYan_GaoDi(jie, XiaoYan_ZhuangTai));

        dizhi->setText(FaSong_ShuJu);
        xiaoyanbiaoji = 1;
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("HEX数据错误"));
    }
}

//BCC异或校验
void ZLCom::CaiDan_TianJia_BCCJiaoYan(QLineEdit* dizhi)
{
    QString FaSong_ShuJu = dizhi->text();
    FaSong_ShuJu = FaSong_ShuJu.replace(" ", "");
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
        uint8_t wcrc = 0X00;
        uint8_t temp = 0;
        uint8_t jie = 0;
        for (int i = 0; i < shuju_qba; i++)
        {
            temp = ShuJu.at(i);
            wcrc ^= temp;
        }
        jie = wcrc;

        uint8_t di = jie;
        int didi = int(di);
        QString XiaoYan_ShuJu = QString::number(didi, 16);
        XiaoYan_ShuJu = DaXiaoTiHuan(XiaoYan_ShuJu);
        FaSong_ShuJu.append(XiaoYan_ShuJu);

        dizhi->setText(FaSong_ShuJu);
        xiaoyanbiaoji = 1;
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("HEX数据错误"));
    }
}

//CRC8
void ZLCom::CaiDan_TianJia_Crc8JiaoYan(QLineEdit* dizhi)
{
    QString FaSong_ShuJu = dizhi->text();
    FaSong_ShuJu = FaSong_ShuJu.replace(" ", "");
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
        uint8_t wcrc = 0X00;
        uint8_t temp = 0;
        uint8_t jie = 0;
        for (int i = 0; i < shuju_qba; i++)
        {
            temp = ShuJu.at(i);
            wcrc ^= temp;

            for (int j = 0; j < 8; j++) {

                if (wcrc & 0X80) {
                    wcrc <<= 1;
                    wcrc ^= 0X07;
                }
                else
                    wcrc <<= 1;
            }
        }
        jie = wcrc;


        uint8_t di = jie;
        int didi = int(di);
        QString XiaoYan_ShuJu = QString::number(didi, 16);
        XiaoYan_ShuJu = DaXiaoTiHuan(XiaoYan_ShuJu);
        FaSong_ShuJu.append(XiaoYan_ShuJu);

        dizhi->setText(FaSong_ShuJu);
        xiaoyanbiaoji = 1;
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("HEX数据错误"));
    }
}

//LRC纵向沉余校验
void ZLCom::CaiDan_TianJia_LrcJiaoYan(QLineEdit* dizhi)
{
    QString YiHao_ShuJu = dizhi->text();
    QString Hex_zifu;
    YiHao_ShuJu = YiHao_ShuJu.replace(" ", "");
    int chang = YiHao_ShuJu.size();
    int zongshu = 0, mo = 0;
    if (chang % 2 == 0 )
    {
        for (int i = 0; i < chang; i++)
        {
            int mm = 0;
            Hex_zifu = YiHao_ShuJu.mid(i, 2);
            mm = Hex_zifu.toInt(nullptr, 16);
            zongshu = zongshu + mm;
            ++i;
        }
        mo = zongshu % 256;
        mo = 256 - mo;
        QString num = QString::number(mo, 16);
        num = DaXiaoTiHuan(num);
        YiHao_ShuJu.append(num);
        dizhi->setText(YiHao_ShuJu);
        xiaoyanbiaoji = 1;
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("HEX数据错误"));
    }

}

//CRC-16/MODBUD校验   CRC16-USB
void ZLCom::CaiDan_TianJia_CrcJiaoYan(QLineEdit* dizhi)
{
    QString FaSong_ShuJu = dizhi->text();
    FaSong_ShuJu = FaSong_ShuJu.replace(" ", "");
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
        if (CRC_USB)
        {
            jie ^= 0xFFFF;
            CRC_USB = 0;
        }
        
        FaSong_ShuJu.append(XiaoYan_GaoDi(jie, XiaoYan_ZhuangTai));

        dizhi->setText(FaSong_ShuJu);
        xiaoyanbiaoji = 1;
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("HEX数据错误"));
    }

}

//CRC32
void ZLCom::CaiDan_TianJia_Crc32JiaoYan(QLineEdit* dizhi)
{
    QString FaSong_ShuJu = dizhi->text();
    FaSong_ShuJu = FaSong_ShuJu.replace(" ", "");
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
        uint32_t wcrc = 0XFFFFFFFF;
        uint8_t temp = 0;
        uint32_t jie = 0;
        for (int i = 0; i < shuju_qba; i++)
        {
            temp = ShuJu.at(i);
            wcrc ^= temp;

            for (int j = 0; j < 8; j++) {

                if (wcrc & 0X00000001) {
                    wcrc >>= 1;
                    wcrc ^= 0XEDB88320;
                }
                else
                    wcrc >>= 1;
            }
        }
        jie = wcrc;
        jie ^= 0xFFFFFFFF;
        FaSong_ShuJu.append(XiaoYan_GaoDi32(jie, XiaoYan_ZhuangTai));
        dizhi->setText(FaSong_ShuJu);
        xiaoyanbiaoji = 1;
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("HEX数据错误"));
    }
}

//服务器启动
void ZLCom::FuWuQi_QiDong(void)
{
    fuwuqi = new QTcpServer(this);
    QHostAddress serverAddress(ui.fuwuql_IP->currentText());
    QString duanko = ui.fuwuql_bendi->currentText();
    quint16 serverPort = duanko.toInt();
    if (!fuwuqi->listen(serverAddress, serverPort)) {
        QMessageBox::critical(this, QString::fromLocal8Bit("无法启动服务器！"), QString::fromLocal8Bit("请确认IP、端口是否正确"));
    }
    else {
        ui.wenbenjiesho->appendPlainText(QString::fromLocal8Bit("服务器正在监听地址:"));
        ui.wenbenjiesho->appendPlainText(serverAddress.toString());
        ui.wenbenjiesho->appendPlainText(QString::fromLocal8Bit("端口:"));
        ui.wenbenjiesho->appendPlainText(QString(serverPort));
        //qDebug() << "服务器正在监听地址" << serverAddress.toString() << "端口" << serverPort << "...";
    }
    connect(fuwuqi, &QTcpServer::newConnection, this, &ZLCom::onNewConnection);
}

void ZLCom::onNewConnection()
{
    QTcpSocket* socket = fuwuqi->nextPendingConnection();
    if (!socket)
        return;
    int FuWuQi_DuanKo = socket->socketDescriptor();
    //qDebug() << "收到新连接：" << socket->socketDescriptor();
    ui.wenbenjiesho->appendPlainText(QString::number(FuWuQi_DuanKo));
    connect(socket, &QTcpSocket::readyRead, this, &ZLCom::socketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ZLCom::socketDisconnected);
}

void ZLCom::socketReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;

    //qDebug() << "准备读取数据：" << socket->socketDescriptor();

    QByteArray data = socket->readAll();
    ui.wenbenjiesho->appendPlainText(QString(data));
    //qDebug() << "客户端发来的数据：" << data;

    //socket->write(data);
}

void ZLCom::socketDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;
    int FuWuQi_DuanKo = socket->socketDescriptor();
    ui.wenbenjiesho->appendPlainText(QString::number(FuWuQi_DuanKo));
    //qDebug() << "连接断开：" << socket->socketDescriptor();
    socket->deleteLater();
}


//服务器IP扫描+启动服务器
void ZLCom::FuWuQi_DuanKoSaoMiao(void)
{
    connect(ui.fuwuql_IP, SIGNAL(clicked()), this, SLOT(FuWuQi_DuanKoPaiXu()));
    connect(ui.fuwuqi_chuangj, &QPushButton::clicked, [&]() {FuWuQi_QiDong(); });//按键启动服务器
}

//服务器端口排序
void ZLCom::FuWuQi_DuanKoPaiXu(void)
{
    // 获取所有网络接口列表
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    ui.fuwuql_IP->clear();
    // 遍历每个网络接口
    foreach(QNetworkInterface interface, interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning))
        {
           
            //interface.name() 网络接口
            //interface.hardwareAddress()硬件地址
            /*qDebug() << "网络接口:" << interface.name();
            qDebug() << "  硬件地址:" << interface.hardwareAddress();*/

            // 获取该接口的所有 IP 地址条目
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            foreach(QNetworkAddressEntry entry, entries) {
                // 只打印 IPv4 地址
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ui.fuwuql_IP->addItem(entry.ip().toString());
                    /*qDebug() << "  IP 地址:" << entry.ip().toString();
                    qDebug() << "  子网掩码:" << entry.netmask().toString();*/
                }
            }
        }

    }
}

//关闭软件，读取当前配置
void ZLCom::closeEvent(QCloseEvent* event)
{
    //波特率
    int bianhao = ui.combotelv->currentIndex();
    QString shuju = QString::number(bianhao);
    QString zoshu;
    zoshu.append(shuju);

    //数据位
    bianhao = ui.comshujuwei->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //校验位
    bianhao = ui.comxiaoyan->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //停止位
    bianhao = ui.comtingzhi->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //定时1
    if (ui.Ding_a->isChecked() == false)
    {
        bianhao = 0;
    }
    else
    {
        bianhao = 1;
    }
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //定时2
    if (ui.Ding_b->isChecked() == false)
    {
        bianhao = 0;
    }
    else
    {
        bianhao = 1;
    }
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //定时3
    if (ui.Ding_c->isChecked() == false)
    {
        bianhao = 0;
    }
    else
    {
        bianhao = 1;
    }
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //HEX1
    if (ui.Fa_a->isChecked() == false)
    {
        bianhao = 0;
    }
    else
    {
        bianhao = 1;
    }
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //HEX2
    if (ui.Fa_b->isChecked() == false)
    {
        bianhao = 0;
    }
    else
    {
        bianhao = 1;
    }
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //HEX3
    if (ui.Fa_c->isChecked() == false)
    {
        bianhao = 0;
    }
    else
    {
        bianhao = 1;
    }
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //数据显示格式
    bianhao = ui.BianMa->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //校验格式
    bool zhuangtai = ui.xiaoyan_1->isChecked();
    if (zhuangtai)
    {
        bianhao = 1;
        shuju = QString::number(bianhao);
        zoshu.append(shuju);
    }
    else if (zhuangtai = ui.xiaoyan_2->isChecked())
    {
        bianhao = 2;
        shuju = QString::number(bianhao);
        zoshu.append(shuju);
    }
    else if (zhuangtai = ui.xiaoyan_3->isChecked())
    {
        bianhao = 3;
        shuju = QString::number(bianhao);
        zoshu.append(shuju);
    }

    //PLC界面、型号
    bianhao = ui.Plc_XingHao_XuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC界面、通讯格式
    bianhao = ui.PLC_TongXun_GeShi->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC界面、波特率
    bianhao = ui.PLC_BoTeLv_XuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC界面、数据位
    bianhao = ui.PLC_ShuJuWei_XuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC界面、校验位
    bianhao = ui.PLC_XiaoYan_XuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC界面、停止位
    bianhao = ui.PLC_TingZhiXuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //QFile file(QApplication::applicationDirPath() + "/test.txt");//获取当前执行目录
    //打开文件，不存在则创建
    QFile file("test.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(zoshu.toUtf8());
    file.close();

    //一号发送窗口数据
    QString neirong;
    zoshu = "";
    shuju = ui.fasong->text();
    if (shuju != "")
    {
        bianhao = 1;
        neirong = QString::number(bianhao);
        zoshu.append(neirong);
        int changdu = 0;
        shuju = shuju.replace(" ", "");
        if (shuju.size() < 999)
        {
            changdu = shuju.size();
            neirong = QString::number(changdu);
            if (neirong.size() < 3)
            {
                for (int j = 0; j < 3 - neirong.size(); j++)
                {
                    neirong.prepend('0');
                }
            }
            zoshu.append(neirong);
        }
        zoshu.append(shuju);
    }

    shuju = ui.fasong2->text();
    if (shuju != "")
    {
        bianhao = 2;
        zoshu.append(QString::number(bianhao));

        int changdu = 0;
        shuju = shuju.replace(" ", "");
        if (shuju.size() < 999)
        {
            changdu = shuju.size();
            neirong = QString::number(changdu);
            if (neirong.size() < 3)
            {
                for (int j = 0; j < 3 - neirong.size(); j++)
                {
                    neirong.prepend('0');
                }
            }
            zoshu.append(neirong);
        }
        zoshu.append(shuju);
    }

    shuju = ui.fasong3->text();
    if (shuju != "")
    {
        bianhao = 3;
        zoshu.append(QString::number(bianhao));

        int changdu = 0;
        shuju = shuju.replace(" ", "");
        if (shuju.size() < 999)
        {
            changdu = shuju.size();
            neirong = QString::number(changdu);
            if (neirong.size() < 3)
            {
                for (int j = 0; j < 3 - neirong.size(); j++)
                {
                    neirong.prepend('0');
                }
            }
            zoshu.append(neirong);
        }
        zoshu.append(shuju);
    }

    QFile fasong("FSSJ.txt");
    fasong.open(QIODevice::WriteOnly | QIODevice::Text);
    fasong.write(zoshu.toUtf8());
    fasong.close();

}




