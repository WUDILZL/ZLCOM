#include "ZLCom.h"

//ASCII����ͷ
const char* SPBT[] = {
"ASCIIֵ",
"�����ַ�",
"ʮ������",
"�˽���",
"������"
};

//ASCII�������ַ�
const char* KZZF[] = {
    "nul","soh","stx","etx","eot",
    "enq","ack","bel","bs","ht",
    "nl","vt","ff","er","so",
    "si","dle","dc1","dc2","dc3",
    "dc4","nak","syn","etb","can",
    "em","sub","esc","fs","gs",
    "re","us","sp","del"
};

//���ʹ�������״̬
int  fa_a = 0, fa_b = 0, fa_c = 0, Fasong_Guangbiao = 0, xiaoyanbiaoji = 0, CRC_USB = 0, duankozhuangtai = 0, QuanJuSuoFang = 0, XiaoYan_ZhuangTai = 0, PLC_duankozhuangtai = 0, PLC_lianjiezhuangtai = 0;
int fasongjishu = 0;//PLC���շ��ͼ���
float PingMuBiLie = 0;//���ű���
int BiaoGe_ShuJuLiang = 0;//PLC��ȡ���ݳ���
QByteArray PLC_CeShiJieShoShuJu = NULL;//��������
QByteArray SuoYouShuJu = NULL;//����PLC����
QString FaSong_ShuJu1 = NULL;//һ������
QString FaSong_ShuJu2 = NULL;//��������
QString FaSong_ShuJu3 = NULL;//��������
QModelIndex ZuoBiao;

ZLCom::ZLCom(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ChuanKo_PLCJieMian_QieHuan();//PLC���ڽ����л�
    comGuDingCanShu();//�̶�������ʼ��
    PLCGuDingCanShu();//PLC�̶�������ʼ��
    PLC_XianShiGeShi();//PLC����ʽ ��������ת��ť
    XiaoYan_ZiJie();//У���л�
    DuQuShangYiCiZhuangTai();//������һ�β���

    ascii_chushihua();//��ʼ�����߲˵� ����ȡ�˵��������Խ��д���
    CaiDan_TianJia();//�����Ҽ�����ʵ��
    comod_cotcp();//com����TCP�����л�
    com_dakai();//������com
    PLCcom_dakai();

    FaSong_SaoMiao();//��������ɨ��
    FaSong_QingKong();//�����������
    Com_FaSong();//���ݷ���
    FuWuQi_DuanKoSaoMiao();//���ɨ��IP�˿�
    

    PingMu_FenBianLv();//��Ļ�ֱ�������Ӧ1920*1080

    JiaoCom_JiShi();//���ɨ��˿�

}

ZLCom::~ZLCom()
{}

//����PLC�����л�
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

//���ڹ̶�������ʼ��
void ZLCom::comGuDingCanShu(void)
{
    //������
    ui.combotelv->addItem("1200");
    ui.combotelv->addItem("2400");
    ui.combotelv->addItem("4800");
    ui.combotelv->addItem("9600");
    ui.combotelv->addItem("19200");
    ui.combotelv->addItem("38400");
    ui.combotelv->addItem("57600");
    ui.combotelv->addItem("115200");

    //����λ
    ui.comshujuwei->addItem("7");
    ui.comshujuwei->addItem("8");

    //У��λ
    ui.comxiaoyan->addItem("None");
    ui.comxiaoyan->addItem("Even");
    ui.comxiaoyan->addItem("Odd");
    ui.comxiaoyan->addItem("Space");
    ui.comxiaoyan->addItem("Mark");

    //ֹͣλ
    ui.comtingzhi->addItem("1");
    ui.comtingzhi->addItem("1.5");
    ui.comtingzhi->addItem("2");

    //��ʾ���ݵĸ�ʽ
    ui.BianMa->addItem("ASCII");
    ui.BianMa->addItem("HEX");
    ui.BianMa->addItem("DEC");
    ui.BianMa->addItem("Bin");

    //������ѡ��
    ui.fuwuql->addItem("TCP Server");
    ui.fuwuql_bendi->addItem("8888");

}

//PLC�̶�������ʼ��
void ZLCom::PLCGuDingCanShu(void)
{
    //PLC�ͺ�
    ui.Plc_XingHao_XuanZe->addItem(QString::fromLocal8Bit("̨��DVP"));

    //ͨѶ��ʽ
    ui.PLC_TongXun_GeShi->addItem("ASCII");
    ui.PLC_TongXun_GeShi->addItem("RTU");

    //ͨѶ������
    ui.PLC_BoTeLv_XuanZe->addItem("1200");
    ui.PLC_BoTeLv_XuanZe->addItem("2400");
    ui.PLC_BoTeLv_XuanZe->addItem("4800");
    ui.PLC_BoTeLv_XuanZe->addItem("9600");
    ui.PLC_BoTeLv_XuanZe->addItem("19200");
    ui.PLC_BoTeLv_XuanZe->addItem("38400");
    ui.PLC_BoTeLv_XuanZe->addItem("57600");
    ui.PLC_BoTeLv_XuanZe->addItem("115200");

    //����λ
    ui.PLC_ShuJuWei_XuanZe->addItem("7");
    ui.PLC_ShuJuWei_XuanZe->addItem("8");

    //У��λ
    ui.PLC_XiaoYan_XuanZe->addItem("None");
    ui.PLC_XiaoYan_XuanZe->addItem("Even");
    ui.PLC_XiaoYan_XuanZe->addItem("Odd");

    //ֹͣλ
    ui.PLC_TingZhiXuanZe->addItem("1");
    ui.PLC_TingZhiXuanZe->addItem("1.5");
    ui.PLC_TingZhiXuanZe->addItem("2");
}

//У���л�
void ZLCom::XiaoYan_ZiJie(void)
{
    QButtonGroup* buttonGroup = new QButtonGroup;

    ui.xiaoyan_3->setChecked(true);

    buttonGroup->addButton(ui.xiaoyan_1,1);
    buttonGroup->addButton(ui.xiaoyan_2,2);
    buttonGroup->addButton(ui.xiaoyan_3,3);
    // ����ť��� exclusive ��������Ϊ true��ȷ��ֻ��ѡ��һ��ѡ��
    buttonGroup->setExclusive(true);

}

//��ȡ��һ�ν�������
void ZLCom::DuQuShangYiCiZhuangTai(void)
{
    QFile file("test.txt");
    file.open(QIODevice::ReadOnly);//ֻ����
    QByteArray data = file.readAll();//��ȡȫ������
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
    canshu.open(QIODevice::ReadOnly);//ֻ����
    data = canshu.readAll();//��ȡȫ������
    canshu.close();
    if (data != "")
    {
        int weizhi = 0, changdu = 0;//�±�λ�� weizhi �� ���ݳ��� 
        QByteArray fasongko_yi = data.mid(weizhi, 1);//��ȡ����
        QString shuju = QString(fasongko_yi);
        int shuju_biaohao = shuju.toInt(nullptr, 10);
        if (shuju_biaohao == 1)//һ�����ݽ���
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
        if (shuju_biaohao == 2)//�������ݽ���
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
        if (shuju_biaohao == 3)//�������ݽ���
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

//��ʼ�����߲˵�
void ZLCom::ascii_chushihua(void)
{
   
    ui.gongJu->setText(QString::fromLocal8Bit("����"));//��������
    ui.gongJu->setPopupMode(QToolButton::MenuButtonPopup);//��������˵�����

    QMenu* menul = new QMenu;//�����˵���
    //����һ������������
    QAction* cao = new QAction();
    cao->setText(QString::fromLocal8Bit("ASCII���"));
    menul->addAction(cao);
    ui.gongJu->setMenu(menul);//��Ӳ˵���������
    //�ۻ�ȡ������Ϣ
    connect(cao, &QAction::triggered, [&]() {
        ascii_daying();
        });
    
}

//��������ӡ���
void ZLCom::ascii_daying(void)
{
    QWidget* ASCIIMB = new QWidget;//��������
    QTableView* biaoge = new QTableView(ASCIIMB);//�������
    ASCIIMB->setAttribute(Qt::WA_QuitOnClose, false);//���ø�����
    //biaoge->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ASCIIMB->resize(500, 600);//���ڴ�С
    biaoge->resize(500, 600);//����С
    ASCIIMB->setWindowTitle(QString::fromLocal8Bit("ASCII���"));//��������
    ASCIIMB->show();//��ʾ����

    QStandardItemModel* m_standard_data_model = new QStandardItemModel(ASCIIMB);//�������������
    //setHorizontalHeaderLabels ����ˮƽ��ͷ
    //��ӡ���������
    biaoge->setModel(m_standard_data_model);//���� tableView ��������
    QStringList table_h_headers;
    for (int i = 0; i < sizeof(SPBT) / sizeof(SPBT[0]); i++)
    {
        QString mstring = QString::fromLocal8Bit(SPBT[i]);
        QStringList mdec = mstring.split(",");
        table_h_headers.append(mdec);
    }
    m_standard_data_model->setHorizontalHeaderLabels(table_h_headers);
    //biaoge->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//����Ӧ������
    //��ӡ128��
    QStringList table_v_headers;
    for (int i = 0; i < 128; i++)
    {
        QString sdec = QString::number(i, 10);
        QStringList mdec = sdec.split(",");
        table_v_headers.append(" ");
    }
    m_standard_data_model->setVerticalHeaderLabels(table_v_headers);

    //ASCII����ӡ
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
    biaoge->setEditTriggers(QAbstractItemView::NoEditTriggers);// NoEditTriggers ���ɱ༭
    ASCIIMB->setAttribute(Qt::WA_DeleteOnClose);//�رմ����ͷ��ڴ�
}

//PLC����ʽ ��������ת��ť
void ZLCom::PLC_XianShiGeShi(void)
{
    
    
    ui.tableView->setColumnWidth(0, 150); // ���õ�1�п��
    ui.tableView->setColumnWidth(1, 200); // ���õ�2�п��
    ui.tableView->setColumnWidth(2, 150); // ���õ�3�п��
    ui.tableView->setColumnWidth(3, 200); // ���õ�4�п��
    ui.tableView->setColumnWidth(4, 150); // ���õ�3�п��
    ui.tableView->setColumnWidth(5, 200); // ���õ�4�п��

    ui.tableView->verticalHeader()->setDefaultSectionSize(30); // ����Ĭ���и�Ϊ30

    ui.tableView->verticalHeader()->hide();
    ui.tableView->horizontalHeader()->hide();
    connect(ui.TaiDa_D, &QPushButton::clicked, [&]() {toggleTableVisibility(); });//����ӡ

    // �����Ҽ�����¼�
    ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableView, &QTableView::customContextMenuRequested, this, &ZLCom::showContextMenu);//�Ҽ�����༭�������ص�ǰѡ��������

    //��Ӳ˵�����
    contextMenu = new QMenu(this);
    editAction = new QAction(this);
    editAction->setText(QString::fromLocal8Bit("�޸Ĳ���"));
    contextMenu->addAction(editAction);
    connect(editAction, &QAction::triggered, this, &ZLCom::editCell);//�Ҽ��˵�

    connect(ui.DiZhiChaZhao, &QPushButton::clicked, [&]() {searchText(); });//��ȷ����
    connect(ui.tableView->verticalScrollBar(), &QScrollBar::valueChanged, [&]() {updateVisibleRows(); });//���ص�ǰ����У�����
}

//�Ҽ����
void ZLCom::showContextMenu(const QPoint& pos)
{
    clickedIndex = ui.tableView->indexAt(pos);
    //��������
    int row = clickedIndex.row();
    int col = clickedIndex.column();
    //���ɱ༭
    if (clickedIndex.isValid() && clickedIndex.column() != 0 && clickedIndex.column() != 2 && clickedIndex.column() != 4) {
        contextMenu->exec(ui.tableView->viewport()->mapToGlobal(pos));
    }
}

//�Ҽ��˵�
void ZLCom::editCell()
{
    if (clickedIndex.isValid()) {
        bool ok;
        QString text = QInputDialog::getText(this, QString::fromLocal8Bit("�����޸�"),
            QString::fromLocal8Bit("�����룺"), QLineEdit::Normal,
            model->data(clickedIndex).toString(), &ok);
        if (ok && !text.isEmpty()) {             //ȷ�ϲ���������ִ��
            model->setData(clickedIndex, text);
        }
    }
}

//����������������л�
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

//���ɨ��˿�
void ZLCom::JiaoCom_JiShi(void)
{
    connect(ui.comko, SIGNAL(clicked()), this, SLOT(jiacom())); 
    connect(ui.PLC_ComKoXuanZe, SIGNAL(clicked()), this, SLOT(PLCjiacom()));
}

//ɨ��com�˿�
void ZLCom::jiacom(void)
{
    QVector<QString>temp;
    for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts())
    {
        temp.push_back(info.portName());//���Ԫ��
    }

    std::sort(temp.begin(), temp.end());//ȥ������

    if (temp != ports)//������ϴ����ݲ�ͬ
    {
        ui.comko->clear();
        this->ports = temp;
        for (auto& a : ports) ui.comko->addItem(a);
    }
    
    if (ui.dakaicom->text() == (QString::fromLocal8Bit("�Ͽ�")))
    {
        serialPort->close();
        dakai_gongn(ui.comzhuangtai, ui.dakaicom);
    }

}

//ɨ��plccom�˿�
void ZLCom::PLCjiacom(void)
{
    QVector<QString>temp;
    for (const QSerialPortInfo& info : QSerialPortInfo::availablePorts())
    {
        temp.push_back(info.portName());//���Ԫ��
    }

    std::sort(temp.begin(), temp.end());//ȥ������

    if (temp != Plcports)//������ϴ����ݲ�ͬ
    {
        ui.PLC_ComKoXuanZe->clear();
        this->Plcports = temp;
        for (auto& a : Plcports) ui.PLC_ComKoXuanZe->addItem(a);
    }

    if (ui.dakaicom->text() == (QString::fromLocal8Bit("�Ͽ�")))
    {
        serialPort->close();
        dakai_gongn(ui.PLC_TongXunZhuangTaiDeng, ui.PLC_Com_KaiQi);
    }
}

//���� ��com
void ZLCom::com_dakai(void)
{
    ui.comzhuangtai->setStyleSheet("QRadioButton::indicator:unchecked {background-color:red;}");//���ó�ʼ״̬
    ui.dakaicom->setText(QString::fromLocal8Bit("��"));
    connect(ui.dakaicom, &QPushButton::clicked, [&]() {
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("��")))Com_KaiQi();
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("�Ͽ�"))) serialPort->close();
        if (duankozhuangtai == 1 || ui.dakaicom->text() == (QString::fromLocal8Bit("�Ͽ�")))dakai_gongn(ui.comzhuangtai, ui.dakaicom);
        });
}

//���ݽ���
void ZLCom::Com_KaiQi(void)
{
    QSerialPort::BaudRate Baud;//������
    QSerialPort::DataBits Data;//����λ
    QSerialPort::Parity Check;//У��λ
    QSerialPort::StopBits Stop;//ֹͣλ
    
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

    if (serialPort->open(QSerialPort::ReadWrite))//�Զ�д�ķ�ʽ��
    {
        connect(serialPort, &QSerialPort::readyRead, [&]() {
            auto data = serialPort->readAll();//readAll ��������
            if (ui.BianMa->currentText() == "HEX")//�����16����
            {
                QString hex = data.toHex(' ');

                ui.wenbenjiesho->appendPlainText(hex);
            }
            /*else if (receiveMode->currentText() == "�ı�")
            {
                QString str = QString(data);
                receiveAera->appendPlainText(str);
            }*/
            else if (ui.BianMa->currentText() == "DEC")//10����
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
            else if (ui.BianMa->currentText() == "Bin")//2����
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
            else if (ui.BianMa->currentText() == "ASCII")//ASCII��
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

            });//readyRead����ˢ����
        duankozhuangtai = 1;
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("���ڴ�ʧ��"), QString::fromLocal8Bit("��ȷ�ϴ����Ƿ�������ȷ"));
    }

}

//PLC���ݽ���
void ZLCom::PLC_ShuJuJieSho(void)
{
    QSerialPort::BaudRate Baud;//������
    QSerialPort::DataBits Data;//����λ
    QSerialPort::Parity Check;//У��λ
    QSerialPort::StopBits Stop;//ֹͣλ

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
    if (PLC_serialPort->open(QSerialPort::ReadWrite))//�Զ�д�ķ�ʽ��
    {
        if (PLC_lianjiezhuangtai == 0)PLC_DiYiCiXiaoYao();//��һ�δ򿪲����Ƿ���ȷ����PLC
        //���ܲ���
        /*connect(ui.pushButton, &QPushButton::clicked, [&]() {
            PLC_ShuJuFaSong(PLC_BiaoDing, PLC_BiaoDi);
            });*/
        

        connect(PLC_serialPort, &QSerialPort::readyRead, [&]() {
            auto data = PLC_serialPort->readAll();//readAll ��������
            if (PLC_lianjiezhuangtai == 0) PLC_CeShiJieShoShuJu = data;
            else {
                QByteArray JieSho_ShuJu = data;
                PLC_JieSho_XianShi(JieSho_ShuJu);
            }
            });//readyRead����ˢ����
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("���ڴ�ʧ��"), QString::fromLocal8Bit("��ȷ�ϴ����Ƿ�������ȷ"));
    }
}

//��ȡ������ײ����㳤�ȣ����ͱ���
void ZLCom::PLC_ShuJuFaSong(int di, int ding )
{
    //�̶�����
    QString FS_ShuJu = "0103";
    //D����ʼ��ַ
    int ShuJuQiShiDiZhi = 0x1000;
    //����������ʼ��ַ��ȷ�ϱ���
    int ShuJu_QiShidiZhi = ShuJuQiShiDiZhi + (di * 3);

    FS_ShuJu.append(QString::number(ShuJu_QiShidiZhi, 16));

    //�������ݳ��ȣ�ȷ�ϱ���
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
    FS_ChangDu = PLC_XiaoYan(FS_ShuJu);//����У��
    FS_ShuJu.append(FS_ChangDu);

    //���ݸ�ʽת��
    QByteArray FA = NULL;
    for (int i = 0; i < FS_ShuJu.size(); i++)
    {
        int num = FS_ShuJu.mid(i, 2).toUInt(nullptr, 16);
        FA.append(num);
        ++i;
    }
    //���ͱ���
    PLC_serialPort->write(FA);
}

//���ݶ�ʱ����������
void ZLCom::ShuJuFaSong(void)
{
    PLC_LJOK_FaSong_BaoWen = new QTimer(this);
    PLC_LJOK_FaSong_BaoWen->start(100);
    connect(PLC_LJOK_FaSong_BaoWen, &QTimer::timeout, [&]() {PLC_ShuJuFaSong(PLC_BiaoDing, PLC_BiaoDi); });
}

//�����������ݣ�����ӡ�����
void ZLCom::PLC_JieSho_XianShi(QByteArray shuju)
{
    //��������
    QByteArray JS_Shu_Ju = shuju;
    //�ж������Ƿ��׼��ʽ
    QByteArray PanDuan = JS_Shu_Ju.mid(0, 1);
    QString shex = PanDuan.toHex();
    int idec = shex.toInt(nullptr, 16);
    int SJ_chang = 0;
    int Lie = 0;
    int hang = PLC_BiaoDing;
    if (idec == 1)//�ж�վ��
    {
        PanDuan = JS_Shu_Ju.mid(1, 1);
        shex = PanDuan.toHex();
        idec = shex.toInt(nullptr, 16);
        if (idec == 3)//�жϹ�����
        {
            PanDuan = JS_Shu_Ju.mid(2, 1);
            shex = PanDuan.toHex();
            idec = shex.toInt(nullptr, 16);

            if (idec == BiaoGe_ShuJuLiang * 2)//�ж����ݳ���
            {
                SJ_chang = BiaoGe_ShuJuLiang * 2;
                //�ж�У���Ƿ���ȷ
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

//��һ�δ򿪲����Ƿ���ȷ����PLC
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
        QMessageBox::critical(this, QString::fromLocal8Bit("PLC���ݻظ�����"), QString::fromLocal8Bit("��ȷ�ϲ������û�PLCͨѶ����"));
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

//�ж��Ƿ��
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

//���� ��com
void ZLCom::PLCcom_dakai(void)
{
    ui.PLC_TongXunZhuangTaiDeng->setStyleSheet("QRadioButton::indicator:unchecked {background-color:red;}");//���ó�ʼ״̬
    ui.PLC_Com_KaiQi->setText(QString::fromLocal8Bit("��"));
    connect(ui.PLC_Com_KaiQi, &QPushButton::clicked, [&]() {
        if (ui.PLC_Com_KaiQi->text() == (QString::fromLocal8Bit("��")))PLC_ShuJuJieSho();
        if (ui.PLC_Com_KaiQi->text() == (QString::fromLocal8Bit("�Ͽ�")))
        {
            PLC_serialPort->close();
            PLC_lianjiezhuangtai = 0;//
            PLC_duankozhuangtai = 0;//
            PLC_LJOK_FaSong_BaoWen->stop();
        } 
        if (PLC_duankozhuangtai == 1 || ui.PLC_Com_KaiQi->text() == (QString::fromLocal8Bit("�Ͽ�")))dakai_gongn(ui.PLC_TongXunZhuangTaiDeng, ui.PLC_Com_KaiQi);
        });
}

//����ʾ ����
void ZLCom::dakai_gongn(QRadioButton* xinhao, QPushButton* anjian)
{
    xinhao->setStyleSheet( "QRadioButton::indicator:checked {background-color:green;}"
        "QRadioButton::indicator:unchecked {background-color:red;}"
    );
    QString content = anjian->text();
    if (QString::compare(content, QString::fromLocal8Bit("��"), Qt::CaseInsensitive) == 0)
    {
        xinhao->setChecked(true);
        anjian->setText(QString::fromLocal8Bit("�Ͽ�"));
    }
    else
    {
        xinhao->setChecked(false);
        anjian->setText(QString::fromLocal8Bit("��"));
    }

}

//��Ļ���ű�������1920*1080
void ZLCom::PingMu_FenBianLv(void)
{
    QuanJuSuoFang = QGuiApplication::primaryScreen()->logicalDotsPerInch();//��ȡȫ�����ű���
    if (QuanJuSuoFang == 96) PingMuBiLie = 1.0;
    else if (QuanJuSuoFang == 120) PingMuBiLie = 1.25;
    else if (QuanJuSuoFang == 144) PingMuBiLie = 1.5;
    else if (QuanJuSuoFang == 192) PingMuBiLie = 2.0;

    //��ȡ��ǰ���ڿ�ߣ����еȱ�������
    int chuangko_x = 0, chuangko_y = 0;
    chuangko_x = geometry().width();
    chuangko_y = geometry().height();
    chuangko_x = float(chuangko_x) * PingMuBiLie;
    chuangko_y = float(chuangko_y) * PingMuBiLie;
    this->setFixedSize(chuangko_x, chuangko_y);

    //�л�����
    SuoFang_qstackedwidget(ui.QieHuanChuangKo, PingMuBiLie);
    SuoFang_qstackedwidget(ui.stackedWidget, PingMuBiLie);
    SuoFang_qstackedwidget(ui.PLC_JiCunQiDiShi, PingMuBiLie);

    //���ֱ�ǩ
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


    //����ѡ��
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



    //��ѡ
    SuoFang_qradiobutton(ui.comzhuangtai, PingMuBiLie);
    SuoFang_qradiobutton(ui.xiaoyan_1, PingMuBiLie);
    SuoFang_qradiobutton(ui.xiaoyan_2, PingMuBiLie);
    SuoFang_qradiobutton(ui.xiaoyan_3, PingMuBiLie);
    SuoFang_qradiobutton(ui.comzhuangtai_2, PingMuBiLie);
    SuoFang_qradiobutton(ui.PLC_TongXunZhuangTaiDeng, PingMuBiLie);


    //��������
    SuoFang_qtoolbutton(ui.gongJu, PingMuBiLie);


    //��ť
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

    //��ѡ��
    SuoFang_qcheckbox(ui.Ding_a, PingMuBiLie);
    SuoFang_qcheckbox(ui.Ding_b, PingMuBiLie);
    SuoFang_qcheckbox(ui.Ding_c, PingMuBiLie);
    SuoFang_qcheckbox(ui.Fa_a, PingMuBiLie);
    SuoFang_qcheckbox(ui.Fa_b, PingMuBiLie);
    SuoFang_qcheckbox(ui.Fa_c, PingMuBiLie);

    //�����ı���
    SuoFang_qlineedit(ui.jishi, PingMuBiLie);
    SuoFang_qlineedit(ui.jishi_2, PingMuBiLie);
    SuoFang_qlineedit(ui.jishi_3, PingMuBiLie);
    SuoFang_qlineedit(ui.fasong, PingMuBiLie);
    SuoFang_qlineedit(ui.fasong2, PingMuBiLie);
    SuoFang_qlineedit(ui.fasong3, PingMuBiLie);
    SuoFang_qlineedit(ui.JiCunQi, PingMuBiLie);


    //TEXT�ı����տ�
    SuoFang_qplaintextedit(ui.wenbenjiesho, PingMuBiLie);

    //���������
    SuoFang_qtableView(ui.tableView, PingMuBiLie);

}

//��������ɨ���ʱ
void ZLCom::FaSong_SaoMiao(void)    
{
    
    QTimer* shuju_t = new QTimer(this);
    connect(shuju_t, &QTimer::timeout, [&]() {
        if (fa_a == 1) FaSong_a();
        });//һ�ŷ��Ϳ�

    connect(shuju_t, &QTimer::timeout, [&]() {
        if (fa_b == 1) FaSong_b();
        });//���ŷ��Ϳ�

    connect(shuju_t, &QTimer::timeout, [&]() {
        if (fa_c == 1) FaSong_c();
        });//���ŷ��Ϳ�

    shuju_t->start(5);

    //��������״̬�л�
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

//�������
void ZLCom::FaSong_QingKong(void)
{
    //�����������
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

//һ��������ʾ����
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
            neirong = neirong.replace(" ", "");    // ȥ�����еĿո�
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // ���������ո������
            for (int i = 1; i < insertCount; i++) {                             // ��������ո�
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
            neirong = neirong.replace(" ", "");    // ȥ�����еĿո�
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // ���������ո������
            for (int i = 1; i < insertCount; i++) {                             // ��������ո�
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong->clear();
            ui.fasong->setText(neirong);

            QString nei = ui.fasong->text();
            FaSong_ShuJu1 = nei;
        }
    }
}

//����������ʾ����
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
            neirong = neirong.replace(" ", "");    // ȥ�����еĿո�
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // ���������ո������
            for (int i = 1; i < insertCount; i++) {                             // ��������ո�
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
            neirong = neirong.replace(" ", "");    // ȥ�����еĿո�
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // ���������ո������
            for (int i = 1; i < insertCount; i++) {                             // ��������ո�
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong2->clear();
            ui.fasong2->setText(neirong);

            QString nei = ui.fasong2->text();
            FaSong_ShuJu2 = nei;
        }
    }
}

//����������ʾ����
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
            
            neirong = neirong.replace(" ", "");    // ȥ�����еĿո�
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // ���������ո������
            for (int i = 1; i < insertCount; i++) {                             // ��������ո�
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
            neirong = neirong.replace(" ", "");    // ȥ�����еĿո�
            for (int i = 0; i < neirong.size(); i++)
            {
                if (neirong[i] != "0" && neirong[i] != "1" && neirong[i] != "2" && neirong[i] != "3" && neirong[i] != "4" && neirong[i] != "5" && neirong[i] != "6" && neirong[i] != "7" && neirong[i] != "8" && neirong[i] != "9" &&
                    neirong[i] != "A" && neirong[i] != "B" && neirong[i] != "C" && neirong[i] != "D" && neirong[i] != "E" && neirong[i] != "F") neirong.remove(i, 1);
            }
            int insertCount = (neirong.count() / 2) + ((neirong.count() % 2) ? 1 : 0);  // ���������ո������
            for (int i = 1; i < insertCount; i++) {                             // ��������ո�
                neirong.insert(3 * i - 1, " ");
            }
            ui.fasong3->clear();
            ui.fasong3->setText(neirong);

            QString nei = ui.fasong3->text();
            FaSong_ShuJu3 = nei;
        }
    }
}

//������������
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

//��������
void ZLCom::Com_FaSong(void)
{
    connect(ui.fa1, &QPushButton::clicked, [&]() {
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("�Ͽ�")))ComShuJu_FaSong(ui.fasong, ui.Fa_a);
        else QMessageBox::critical(this, QString::fromLocal8Bit("���ڴ�ʧ��"), QString::fromLocal8Bit("��ȷ�ϴ����Ƿ�������ȷ"));
        });

    connect(ui.fa2, &QPushButton::clicked, [&]() {
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("�Ͽ�")))ComShuJu_FaSong(ui.fasong2, ui.Fa_b);
        else QMessageBox::critical(this, QString::fromLocal8Bit("���ڴ�ʧ��"), QString::fromLocal8Bit("��ȷ�ϴ����Ƿ�������ȷ"));
        });

    connect(ui.fa3, &QPushButton::clicked, [&]() {
        if (ui.dakaicom->text() == (QString::fromLocal8Bit("�Ͽ�")))ComShuJu_FaSong(ui.fasong3, ui.Fa_c);
        else QMessageBox::critical(this, QString::fromLocal8Bit("���ڴ�ʧ��"), QString::fromLocal8Bit("��ȷ�ϴ����Ƿ�������ȷ"));
        });
}

//��Ӳ˵�ѡ��
void ZLCom::CaiDan_TianJia(void)
{
    //ui.wenbenjiesho->setContextMenuPolicy(Qt::CustomContextMenu);
    //connect(ui.wenbenjiesho, &QTextEdit::customContextMenuRequested, [&]() {
    //    QMenu* stdMenu = ui.wenbenjiesho->createStandardContextMenu();//��Ӳ˵�ѡ��
    //    QAction* clearAction = new QAction(QString::fromLocal8Bit("��Ч"), ui.wenbenjiesho);//��������
    //    stdMenu->addAction(clearAction);//���
    //    QObject::connect(clearAction, &QAction::triggered, [&]() {

    //        });//�˵���������
    //    stdMenu->move(cursor().pos());//��ǰ���λ��
    //    stdMenu->show();//��ʾ�˵�
    //    });

    //һ�ŷ��ʹ�����ӹ���
    ui.fasong->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.fasong, &QTextEdit::customContextMenuRequested, [&]() {
        QMenu* stdMenu = ui.fasong->createStandardContextMenu();//��Ӳ˵�ѡ��

        QAction* SUMXiaoYan = new QAction(QString::fromLocal8Bit("SUMУ��"), ui.fasong);//��������
        QAction* BccXiaoYan = new QAction(QString::fromLocal8Bit("BCC(���У��)"), ui.fasong);//��������
        QAction* Crc8XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC8)"), ui.fasong);//��������
        QAction* clearAction = new QAction(QString::fromLocal8Bit("LRC(�������У��)"), ui.fasong);//��������
        QAction* CrcXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-MODBUS)"), ui.fasong);//��������
        QAction* CrcUsbXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-USB)"), ui.fasong);//��������
        QAction* Crc32XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC32)"), ui.fasong);//��������

        stdMenu->addAction(SUMXiaoYan);
        stdMenu->addAction(BccXiaoYan);
        stdMenu->addAction(Crc8XiaoYan);
        stdMenu->addAction(clearAction);//���
        stdMenu->addAction(CrcXiaoYan);
        stdMenu->addAction(CrcUsbXiaoYan);
        stdMenu->addAction(Crc32XiaoYan);

        QObject::connect(SUMXiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_SUMJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
            }

            });

        //BCC
        QObject::connect(BccXiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_BCCJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
            }

            });//�˵���������

        //CRC8
        QObject::connect(Crc8XiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_Crc8JiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
            }

            });//�˵���������

        //LRC
        QObject::connect(clearAction, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_LrcJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
            }

            });//�˵���������

        //CRC16-MODBUS
        QObject::connect(CrcXiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_CrcJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
            }

            });//�˵���������

        QObject::connect(CrcUsbXiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CRC_USB = 1;
                CaiDan_TianJia_CrcJiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
            }

            });//�˵���������

        QObject::connect(Crc32XiaoYan, &QAction::triggered, [&]() {
            if (fa_a == 1) {
                CaiDan_TianJia_Crc32JiaoYan(ui.fasong);
            }
            else
            {
                QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
            }

            });

        stdMenu->move(cursor().pos());//��ǰ���λ��
        stdMenu->show();//��ʾ�˵�

     });

     //2�ŷ��ʹ�����ӹ���
     ui.fasong2->setContextMenuPolicy(Qt::CustomContextMenu);
     connect(ui.fasong2, &QTextEdit::customContextMenuRequested, [&]() {
         QMenu* stdMenu = ui.fasong2->createStandardContextMenu();//��Ӳ˵�ѡ��

         QAction* SUMXiaoYan = new QAction(QString::fromLocal8Bit("SUMУ��"), ui.fasong2);//��������
         QAction* BccXiaoYan = new QAction(QString::fromLocal8Bit("BCC(���У��)"), ui.fasong2);//��������
         QAction* Crc8XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC8)"), ui.fasong2);//��������
         QAction* clearAction = new QAction(QString::fromLocal8Bit("LRC(�������У��)"), ui.fasong2);//��������
         QAction* CrcXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-MODBUS)"), ui.fasong2);//��������
         QAction* CrcUsbXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-USB)"), ui.fasong2);//��������
         QAction* Crc32XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC32)"), ui.fasong2);//��������

         stdMenu->addAction(SUMXiaoYan);
         stdMenu->addAction(BccXiaoYan);
         stdMenu->addAction(Crc8XiaoYan);
         stdMenu->addAction(clearAction);//���
         stdMenu->addAction(CrcXiaoYan);
         stdMenu->addAction(CrcUsbXiaoYan);
         stdMenu->addAction(Crc32XiaoYan);

         QObject::connect(SUMXiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_SUMJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
             }

             });

         //BCC
         QObject::connect(BccXiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_BCCJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
             }

             });//�˵���������

         //CRC8
         QObject::connect(Crc8XiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_Crc8JiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
             }

             });//�˵���������

         //LRC
         QObject::connect(clearAction, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_LrcJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
             }

             });//�˵���������

         //CRC16-MODBUS
         QObject::connect(CrcXiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_CrcJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
             }

             });//�˵���������

         QObject::connect(CrcUsbXiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CRC_USB = 1;
                 CaiDan_TianJia_CrcJiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
             }

             });//�˵���������

         QObject::connect(Crc32XiaoYan, &QAction::triggered, [&]() {
             if (fa_b == 1) {
                 CaiDan_TianJia_Crc32JiaoYan(ui.fasong2);
             }
             else
             {
                 QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
             }

             });

         stdMenu->move(cursor().pos());//��ǰ���λ��
         stdMenu->show();//��ʾ�˵�

      });

      //3�ŷ��ʹ�����ӹ���
      ui.fasong3->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(ui.fasong3, &QTextEdit::customContextMenuRequested, [&]() {
          QMenu* stdMenu = ui.fasong3->createStandardContextMenu();//��Ӳ˵�ѡ��

          QAction* SUMXiaoYan = new QAction(QString::fromLocal8Bit("SUMУ��"), ui.fasong3);//��������
          QAction* BccXiaoYan = new QAction(QString::fromLocal8Bit("BCC(���У��)"), ui.fasong3);//��������
          QAction* Crc8XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC8)"), ui.fasong3);//��������
          QAction* clearAction = new QAction(QString::fromLocal8Bit("LRC(�������У��)"), ui.fasong3);//��������
          QAction* CrcXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-MODBUS)"), ui.fasong3);//��������
          QAction* CrcUsbXiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC16-USB)"), ui.fasong3);//��������
          QAction* Crc32XiaoYan = new QAction(QString::fromLocal8Bit("CRC(CRC32)"), ui.fasong3);//��������

          stdMenu->addAction(SUMXiaoYan);
          stdMenu->addAction(BccXiaoYan);
          stdMenu->addAction(Crc8XiaoYan);
          stdMenu->addAction(clearAction);//���
          stdMenu->addAction(CrcXiaoYan);
          stdMenu->addAction(CrcUsbXiaoYan);
          stdMenu->addAction(Crc32XiaoYan);

          QObject::connect(SUMXiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_SUMJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
              }

              });

          //BCC
          QObject::connect(BccXiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_BCCJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
              }

              });//�˵���������

          //CRC8
          QObject::connect(Crc8XiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_Crc8JiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
              }

              });//�˵���������

          //LRC
          QObject::connect(clearAction, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_LrcJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
              }

              });//�˵���������

          //CRC16-MODBUS
          QObject::connect(CrcXiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_CrcJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
              }

              });//�˵���������

          QObject::connect(CrcUsbXiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CRC_USB = 1;
                  CaiDan_TianJia_CrcJiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
              }

              });//�˵���������

          QObject::connect(Crc32XiaoYan, &QAction::triggered, [&]() {
              if (fa_c == 1) {
                  CaiDan_TianJia_Crc32JiaoYan(ui.fasong3);
              }
              else
              {
                  QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���ڹ�ѡHEX�¿�ʹ��"));
              }

              });

          stdMenu->move(cursor().pos());//��ǰ���λ��
          stdMenu->show();//��ʾ�˵�

       });

}

//SUMУ��
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
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("HEX���ݴ���"));
    }
}

//BCC���У��
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
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("HEX���ݴ���"));
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
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("HEX���ݴ���"));
    }
}

//LRC�������У��
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
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("HEX���ݴ���"));
    }

}

//CRC-16/MODBUDУ��   CRC16-USB
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
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("HEX���ݴ���"));
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
        QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("HEX���ݴ���"));
    }
}

//����������
void ZLCom::FuWuQi_QiDong(void)
{
    fuwuqi = new QTcpServer(this);
    QHostAddress serverAddress(ui.fuwuql_IP->currentText());
    QString duanko = ui.fuwuql_bendi->currentText();
    quint16 serverPort = duanko.toInt();
    if (!fuwuqi->listen(serverAddress, serverPort)) {
        QMessageBox::critical(this, QString::fromLocal8Bit("�޷�������������"), QString::fromLocal8Bit("��ȷ��IP���˿��Ƿ���ȷ"));
    }
    else {
        ui.wenbenjiesho->appendPlainText(QString::fromLocal8Bit("���������ڼ�����ַ:"));
        ui.wenbenjiesho->appendPlainText(serverAddress.toString());
        ui.wenbenjiesho->appendPlainText(QString::fromLocal8Bit("�˿�:"));
        ui.wenbenjiesho->appendPlainText(QString(serverPort));
        //qDebug() << "���������ڼ�����ַ" << serverAddress.toString() << "�˿�" << serverPort << "...";
    }
    connect(fuwuqi, &QTcpServer::newConnection, this, &ZLCom::onNewConnection);
}

void ZLCom::onNewConnection()
{
    QTcpSocket* socket = fuwuqi->nextPendingConnection();
    if (!socket)
        return;
    int FuWuQi_DuanKo = socket->socketDescriptor();
    //qDebug() << "�յ������ӣ�" << socket->socketDescriptor();
    ui.wenbenjiesho->appendPlainText(QString::number(FuWuQi_DuanKo));
    connect(socket, &QTcpSocket::readyRead, this, &ZLCom::socketReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ZLCom::socketDisconnected);
}

void ZLCom::socketReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;

    //qDebug() << "׼����ȡ���ݣ�" << socket->socketDescriptor();

    QByteArray data = socket->readAll();
    ui.wenbenjiesho->appendPlainText(QString(data));
    //qDebug() << "�ͻ��˷��������ݣ�" << data;

    //socket->write(data);
}

void ZLCom::socketDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket)
        return;
    int FuWuQi_DuanKo = socket->socketDescriptor();
    ui.wenbenjiesho->appendPlainText(QString::number(FuWuQi_DuanKo));
    //qDebug() << "���ӶϿ���" << socket->socketDescriptor();
    socket->deleteLater();
}


//������IPɨ��+����������
void ZLCom::FuWuQi_DuanKoSaoMiao(void)
{
    connect(ui.fuwuql_IP, SIGNAL(clicked()), this, SLOT(FuWuQi_DuanKoPaiXu()));
    connect(ui.fuwuqi_chuangj, &QPushButton::clicked, [&]() {FuWuQi_QiDong(); });//��������������
}

//�������˿�����
void ZLCom::FuWuQi_DuanKoPaiXu(void)
{
    // ��ȡ��������ӿ��б�
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    ui.fuwuql_IP->clear();
    // ����ÿ������ӿ�
    foreach(QNetworkInterface interface, interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning))
        {
           
            //interface.name() ����ӿ�
            //interface.hardwareAddress()Ӳ����ַ
            /*qDebug() << "����ӿ�:" << interface.name();
            qDebug() << "  Ӳ����ַ:" << interface.hardwareAddress();*/

            // ��ȡ�ýӿڵ����� IP ��ַ��Ŀ
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            foreach(QNetworkAddressEntry entry, entries) {
                // ֻ��ӡ IPv4 ��ַ
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ui.fuwuql_IP->addItem(entry.ip().toString());
                    /*qDebug() << "  IP ��ַ:" << entry.ip().toString();
                    qDebug() << "  ��������:" << entry.netmask().toString();*/
                }
            }
        }

    }
}

//�ر��������ȡ��ǰ����
void ZLCom::closeEvent(QCloseEvent* event)
{
    //������
    int bianhao = ui.combotelv->currentIndex();
    QString shuju = QString::number(bianhao);
    QString zoshu;
    zoshu.append(shuju);

    //����λ
    bianhao = ui.comshujuwei->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //У��λ
    bianhao = ui.comxiaoyan->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //ֹͣλ
    bianhao = ui.comtingzhi->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //��ʱ1
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

    //��ʱ2
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

    //��ʱ3
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

    //������ʾ��ʽ
    bianhao = ui.BianMa->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //У���ʽ
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

    //PLC���桢�ͺ�
    bianhao = ui.Plc_XingHao_XuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC���桢ͨѶ��ʽ
    bianhao = ui.PLC_TongXun_GeShi->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC���桢������
    bianhao = ui.PLC_BoTeLv_XuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC���桢����λ
    bianhao = ui.PLC_ShuJuWei_XuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC���桢У��λ
    bianhao = ui.PLC_XiaoYan_XuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //PLC���桢ֹͣλ
    bianhao = ui.PLC_TingZhiXuanZe->currentIndex();
    shuju = QString::number(bianhao);
    zoshu.append(shuju);

    //QFile file(QApplication::applicationDirPath() + "/test.txt");//��ȡ��ǰִ��Ŀ¼
    //���ļ����������򴴽�
    QFile file("test.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(zoshu.toUtf8());
    file.close();

    //һ�ŷ��ʹ�������
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




