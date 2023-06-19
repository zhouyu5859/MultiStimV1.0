#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("StimSoftWareV1.0");
    QLabel *lb = new QLabel(this);
    QImage img;
    img.load(":/Image/image/stim.png");
    lb->setPixmap(QPixmap::fromImage(img));
    lb->setScaledContents(true);
    lb->setGeometry(400,70,1000,350);
    lb->show();
    ui->lineEdit_3->setText("0");
    ui->lineEdit_4->setText("0");
    ui->lineEdit_5->setText("0");
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::serialPort_readyRead);
    ui->checkBox->setCheckState(Qt::Checked);
    ui->checkBox_3->setCheckState(Qt::Checked);
    multistim = new mutiChannelStim(this);
    /*
        wireless connection
    */
    tcpsocket = new QTcpSocket(this);
    ui->label->setStyleSheet("QLabel{background-color:rgb(255,0,0,255)}");
    connect(tcpsocket, &QTcpSocket::connected, this, [=](){
        ui->label->setStyleSheet("QLabel{background-color:rgb(0,255,0,255)}");
    });
    connect(tcpsocket, &QTcpSocket::disconnected, this, [=](){
        ui->label->setStyleSheet("QLabel{background-color:rgb(255,0,0,255)}");
    });
    connect(ui->pushButton_7, &QPushButton::clicked, this, [=](){
        tcpsocket->close();
    });
    connect(ui->pushButton_5, &QPushButton::clicked, this, [=](){
        tcpsocket->connectToHost(ui->lineEdit->text(),8080,QIODevice::ReadWrite,QAbstractSocket::AnyIPProtocol);
    });
    connect(tcpsocket, &QTcpSocket::readyRead, this, [=](){
        QByteArray array = tcpsocket->readAll();
        ui->textEdit_2->append(array);
    });
    /*****Save Stim parameters to text*****/
    connect(ui->actionSave_Settings, &QAction::triggered, this, [=](){
        QByteArray array;
        QDateTime date_now = QDateTime::currentDateTime();
        QString str_now = date_now.toString("yyyy_MM_dd_hh_mm_ss");
        array = "当前系统时间：";
        array += str_now;
        array +="\n";
        array += "配置的刺激参数：\n";
        QString D = ui->lineEdit_3->text();
        array += D;
        array += " us\n";
        QString Pulse_Period = ui->lineEdit_4->text();
        array += Pulse_Period;
        array += " Hz\n";
        QString Stim_Nums = ui->lineEdit_5->text();
        array += Stim_Nums;
        array += " Nums\n";
        QString Amplitude = ui->spinBox->text();
        array += Amplitude;
        array += " mA\n";
        QString filename = QFileDialog::getOpenFileName(this, "Save Stim parameters to text", "C:\\");
        QFile file(filename);
        file.open(QIODevice::Append);
        file.write(array);
        file.close();
    });
    /*******************************inserted value in Qmap for 0-1024 channles*************************/
    connect(ui->actionchannels_0_1024, &QAction::triggered, this, [=](){
        for(int i = 0; i < 1024; i++){
            this->mychannleAmp.insert(i, ui->spinBox->value());
        }
    });
    /********************************inserted value in QMap for 0-127 channles*************************/
    connect(ui->actionchannels_0_127, &QAction::triggered, this, [=](){
            for(int i = 0; i < 128; i++){
             this->mychannleAmp.insert(i, ui->spinBox->value());
            }
    });
    /********************************inserted value in QMap for 128-255 channles*************************/
    connect(ui->actionchannels_128_255, &QAction::triggered, this, [=](){
        for(int j = 128; j < 256; j++){
            this->mychannleAmp.insert(j, ui->spinBox->value());
        }
    });
    /********************************inserted value in QMap for 256-383 channles*************************/
    connect(ui->actionchannels_256_383, &QAction::triggered, this, [=](){
        for(int j = 256; j < 384; j++){
            this->mychannleAmp.insert(j, ui->spinBox->value());
        }
    });
    /********************************inserted value in QMap for 384-511 channles*************************/
    connect(ui->actionchannels_384_511, &QAction::triggered, this, [=](){
        for(int j = 384; j < 512; j++){
            this->mychannleAmp.insert(j, ui->spinBox->value());
        }
    });
    /********************************inserted value in QMap for 512-639 channles*************************/
    connect(ui->actionchannels_512_639, &QAction::triggered, this, [=](){
        for(int j = 512; j < 640; j++){
            this->mychannleAmp.insert(j, ui->spinBox->value());
        }
    });
    /********************************inserted value in QMap for 640-767 channles*************************/
    connect(ui->actionchannels_640_767, &QAction::triggered, this, [=](){
        for(int j = 640; j < 768; j++){
            this->mychannleAmp.insert(j, ui->spinBox->value());
        }
    });
    /********************************inserted value in QMap for 768-895 channles*************************/
    connect(ui->actionchannels_768_895, &QAction::triggered, this, [=](){
        for(int j = 768; j < 896; j++){
            this->mychannleAmp.insert(j, ui->spinBox->value());
        }
    });
    /********************************inserted value in QMap for 896-1023 channles*************************/
    connect(ui->actionchannels_896_1023, &QAction::triggered, this, [=](){
        for(int j = 896; j < 1024; j++){
            this->mychannleAmp.insert(j, ui->spinBox->value());
        }
    });
    /***************************************Set Stim parttern****************************************/
    connect(ui->actionNEUROEYES_PARTTERN1, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, NEUROEYES_PARTTERN1);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("120");
             ui->lineEdit_5->setText("20");
    });
    connect(ui->actionNEUROEYES_PARTTERN2, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, NEUROEYES_PARTTERN2);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("200");
             ui->lineEdit_5->setText("20");
    });
    connect(ui->actionNEUROEYES_PARTTERN3, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, NEUROEYES_PARTTERN3);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("300");
             ui->lineEdit_5->setText("20");
    });
    connect(ui->actionNEUROEYES_PARTTERN4, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, NEUROEYES_PARTTERN4);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("120");
             ui->lineEdit_5->setText("50");
    });
    connect(ui->actionNEUROEYES_PARTTERN5, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, NEUROEYES_PARTTERN5);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("200");
             ui->lineEdit_5->setText("50");
    });
    connect(ui->actionNEUROEYES_PARTTERN6, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, NEUROEYES_PARTTERN6);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("300");
             ui->lineEdit_5->setText("50");
    });
    connect(ui->actionECOG_PARTTERN1, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN1);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("60");
             ui->lineEdit_5->setText("10");
    });
    connect(ui->actionECOG_PARTTERN2, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN2);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("120");
             ui->lineEdit_5->setText("10");
    });
    connect(ui->actionECOG_PARTTERN3, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN3);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("200");
             ui->lineEdit_5->setText("10");
    });
    connect(ui->actionECOG_PARTTERN4, &QAction::triggered, this, [=](){
             multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN4);
             this->mychannleAmp.clear();
             ui->lineEdit_3->setText("100");
             ui->lineEdit_4->setText("300");
             ui->lineEdit_5->setText("10");
    });
    connect(ui->actionECOG_PARTTERN5, &QAction::triggered, this, [=](){
            multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN5);
            this->mychannleAmp.clear();
            ui->lineEdit_3->setText("100");
            ui->lineEdit_4->setText("120");
            ui->lineEdit_5->setText("20");
    });
    connect(ui->actionECOG_PARTTERN6, &QAction::triggered, this, [=](){
            multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN6);
            this->mychannleAmp.clear();
            ui->lineEdit_3->setText("100");
            ui->lineEdit_4->setText("200");
            ui->lineEdit_5->setText("20");
    });
    connect(ui->actionECOG_PARTTERN7, &QAction::triggered, this, [=](){
            multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN7);
            this->mychannleAmp.clear();
            ui->lineEdit_3->setText("100");
            ui->lineEdit_4->setText("300");
            ui->lineEdit_5->setText("20");
    });
    connect(ui->actionECOG_PARTTERN8, &QAction::triggered, this, [=](){
            multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN7);
            this->mychannleAmp.clear();
            ui->lineEdit_3->setText("100");
            ui->lineEdit_4->setText("120");
            ui->lineEdit_5->setText("50");
    });
    connect(ui->actionECOG_PARTTERN9, &QAction::triggered, this, [=](){
            multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN9);
            this->mychannleAmp.clear();
            ui->lineEdit_3->setText("100");
            ui->lineEdit_4->setText("200");
            ui->lineEdit_5->setText("50");
    });
    connect(ui->actionECOG_PARTTERN10, &QAction::triggered, this, [=](){
            multistim->multichannelStim(this->mychannleAmp, ECOG_PARTTERN10);
            this->mychannleAmp.clear();
            ui->lineEdit_3->setText("100");
            ui->lineEdit_4->setText("300");
            ui->lineEdit_5->setText("50");
    });
    connect(ui->actionTEST_PARTTERN, &QAction::triggered, this, [=](){
            multistim->multichannelStim(this->mychannleAmp, TEST_PARTTERN);
            this->mychannleAmp.clear();
            ui->lineEdit_3->setText("1000");
            ui->lineEdit_4->setText("3ms");
            ui->lineEdit_5->setText("100");
    });

    /**************************************Selected channels from 0-1023 channels************************/
    connect(ui->action000, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(0, ui->spinBox->value());
    });
    connect(ui->action001, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1, ui->spinBox->value());
    });
    connect(ui->action002, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(2, ui->spinBox->value());
    });
    connect(ui->action003, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(3, ui->spinBox->value());
    });
    connect(ui->action004, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(4, ui->spinBox->value());
    });
    connect(ui->action005, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(5, ui->spinBox->value());
    });
    connect(ui->action006, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(6, ui->spinBox->value());
    });
    connect(ui->action007, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(7, ui->spinBox->value());
    });
    connect(ui->action008, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(8, ui->spinBox->value());
    });
    connect(ui->action009, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(9, ui->spinBox->value());
    });
    connect(ui->action010, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(10, ui->spinBox->value());
    });
    connect(ui->action011, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(11, ui->spinBox->value());
    });
    connect(ui->action012, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(12, ui->spinBox->value());
    });
    connect(ui->action013, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(13, ui->spinBox->value());
    });
    connect(ui->action014, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(14, ui->spinBox->value());
    });
    connect(ui->action015, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(15, ui->spinBox->value());
    });
    connect(ui->action016, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(16, ui->spinBox->value());
    });
    connect(ui->action017, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(17, ui->spinBox->value());
    });
    connect(ui->action018, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(18, ui->spinBox->value());
    });
    connect(ui->action019, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(19, ui->spinBox->value());
    });
    connect(ui->action020, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(20, ui->spinBox->value());
    });
    connect(ui->action021, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(21, ui->spinBox->value());
    });
    connect(ui->action022, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(22, ui->spinBox->value());
    });
    connect(ui->action023, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(23, ui->spinBox->value());
    });
    connect(ui->action024, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(24, ui->spinBox->value());
    });
    connect(ui->action025, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(25, ui->spinBox->value());
    });
    connect(ui->action026, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(26, ui->spinBox->value());
    });
    connect(ui->action027, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(27, ui->spinBox->value());
    });
    connect(ui->action028, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(28, ui->spinBox->value());
    });
    connect(ui->action029, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(29, ui->spinBox->value());
    });
    connect(ui->action030, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(30, ui->spinBox->value());
    });
    connect(ui->action031, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(31, ui->spinBox->value());
    });
    connect(ui->action032, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(32, ui->spinBox->value());
    });
    connect(ui->action033, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(33, ui->spinBox->value());
    });
    connect(ui->action034, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(34, ui->spinBox->value());
    });
    connect(ui->action035, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(35, ui->spinBox->value());
    });
    connect(ui->action036, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(36, ui->spinBox->value());
    });
    connect(ui->action037, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(37, ui->spinBox->value());
    });
    connect(ui->action038, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(38, ui->spinBox->value());
    });
    connect(ui->action039, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(39, ui->spinBox->value());
    });
    connect(ui->action040, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(40, ui->spinBox->value());
    });
    connect(ui->action041, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(41, ui->spinBox->value());
    });
    connect(ui->action042, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(42, ui->spinBox->value());
    });
    connect(ui->action043, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(43, ui->spinBox->value());
    });
    connect(ui->action044, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(44, ui->spinBox->value());
    });
    connect(ui->action045, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(45, ui->spinBox->value());
    });
    connect(ui->action046, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(46, ui->spinBox->value());
    });
    connect(ui->action047, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(47, ui->spinBox->value());
    });
    connect(ui->action048, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(48, ui->spinBox->value());
    });
    connect(ui->action049, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(49, ui->spinBox->value());
    });
    connect(ui->action050, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(50, ui->spinBox->value());
    });
    connect(ui->action051, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(51, ui->spinBox->value());
    });
    connect(ui->action052, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(52, ui->spinBox->value());
    });
    connect(ui->action053, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(53, ui->spinBox->value());
    });
    connect(ui->action054, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(54, ui->spinBox->value());
    });
    connect(ui->action055, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(55, ui->spinBox->value());
    });
    connect(ui->action056, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(56, ui->spinBox->value());
    });
    connect(ui->action057, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(57, ui->spinBox->value());
    });
    connect(ui->action058, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(58, ui->spinBox->value());
    });
    connect(ui->action059, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(59, ui->spinBox->value());
    });
    connect(ui->action060, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(60, ui->spinBox->value());
    });
    connect(ui->action061, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(61, ui->spinBox->value());
    });
    connect(ui->action062, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(62, ui->spinBox->value());
    });
    connect(ui->action063, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(63, ui->spinBox->value());
    });
    connect(ui->action064, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(64, ui->spinBox->value());
    });
    connect(ui->action065, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(65, ui->spinBox->value());
    });
    connect(ui->action066, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(66, ui->spinBox->value());
    });
    connect(ui->action067, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(67, ui->spinBox->value());
    });
    connect(ui->action068, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(68, ui->spinBox->value());
    });
    connect(ui->action069, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(69, ui->spinBox->value());
    });
    connect(ui->action070, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(70, ui->spinBox->value());
    });
    connect(ui->action071, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(71, ui->spinBox->value());
    });
    connect(ui->action072, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(72, ui->spinBox->value());
    });
    connect(ui->action073, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(73, ui->spinBox->value());
    });
    connect(ui->action074, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(74, ui->spinBox->value());
    });
    connect(ui->action075, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(75, ui->spinBox->value());
    });
    connect(ui->action076, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(76, ui->spinBox->value());
    });
    connect(ui->action077, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(77, ui->spinBox->value());
    });
    connect(ui->action078, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(78, ui->spinBox->value());
    });
    connect(ui->action079, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(79, ui->spinBox->value());
    });
    connect(ui->action080, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(80, ui->spinBox->value());
    });
    connect(ui->action081, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(81, ui->spinBox->value());
    });
    connect(ui->action082, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(82, ui->spinBox->value());
    });
    connect(ui->action083, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(83, ui->spinBox->value());
    });
    connect(ui->action084, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(84, ui->spinBox->value());
    });
    connect(ui->action085, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(85, ui->spinBox->value());
    });
    connect(ui->action086, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(86, ui->spinBox->value());
    });
    connect(ui->action087, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(87, ui->spinBox->value());
    });
    connect(ui->action088, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(88, ui->spinBox->value());
    });
    connect(ui->action089, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(89, ui->spinBox->value());
    });
    connect(ui->action090, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(90, ui->spinBox->value());
    });
    connect(ui->action091, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(91, ui->spinBox->value());
    });
    connect(ui->action092, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(92, ui->spinBox->value());
    });
    connect(ui->action093, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(93, ui->spinBox->value());
    });
    connect(ui->action094, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(94, ui->spinBox->value());
    });
    connect(ui->action095, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(95, ui->spinBox->value());
    });
    connect(ui->action096, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(96, ui->spinBox->value());
    });
    connect(ui->action097, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(97, ui->spinBox->value());
    });
    connect(ui->action098, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(98, ui->spinBox->value());
    });
    connect(ui->action099, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(99, ui->spinBox->value());
    });
    connect(ui->action100, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(100, ui->spinBox->value());
    });
    connect(ui->action101, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(101, ui->spinBox->value());
    });
    connect(ui->action102, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(102, ui->spinBox->value());
    });
    connect(ui->action103, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(103, ui->spinBox->value());
    });
    connect(ui->action104, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(104, ui->spinBox->value());
    });
    connect(ui->action105, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(105, ui->spinBox->value());
    });
    connect(ui->action106, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(106, ui->spinBox->value());
    });
    connect(ui->action107, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(107, ui->spinBox->value());
    });
    connect(ui->action108, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(108, ui->spinBox->value());
    });
    connect(ui->action109, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(109, ui->spinBox->value());
    });
    connect(ui->action110, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(110, ui->spinBox->value());
    });
    connect(ui->action111, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(111, ui->spinBox->value());
    });
    connect(ui->action112, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(112, ui->spinBox->value());
    });
    connect(ui->action113, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(113, ui->spinBox->value());
    });
    connect(ui->action114, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(114, ui->spinBox->value());
    });
    connect(ui->action115, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(115, ui->spinBox->value());
    });
    connect(ui->action116, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(116, ui->spinBox->value());
    });
    connect(ui->action117, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(117, ui->spinBox->value());
    });
    connect(ui->action118, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(118, ui->spinBox->value());
    });
    connect(ui->action119, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(119, ui->spinBox->value());
    });
    connect(ui->action120, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(120, ui->spinBox->value());
    });
    connect(ui->action121, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(121, ui->spinBox->value());
    });
    connect(ui->action122, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(122, ui->spinBox->value());
    });
    connect(ui->action123, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(123, ui->spinBox->value());
    });
    connect(ui->action124, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(124, ui->spinBox->value());
    });
    connect(ui->action125, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(125, ui->spinBox->value());
    });
    connect(ui->action126, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(126, ui->spinBox->value());
    });
    connect(ui->action127, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(127, ui->spinBox->value());
    });
    connect(ui->action128, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(128, ui->spinBox->value());
    });
    connect(ui->action129, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(129, ui->spinBox->value());
    });
    connect(ui->action130, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(130, ui->spinBox->value());
    });
    connect(ui->action131, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(131, ui->spinBox->value());
    });
    connect(ui->action132, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(132, ui->spinBox->value());
    });
    connect(ui->action133, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(133, ui->spinBox->value());
    });
    connect(ui->action134, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(134, ui->spinBox->value());
    });
    connect(ui->action135, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(135, ui->spinBox->value());
    });
    connect(ui->action136, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(136, ui->spinBox->value());
    });
    connect(ui->action137, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(137, ui->spinBox->value());
    });
    connect(ui->action138, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(138, ui->spinBox->value());
    });
    connect(ui->action139, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(139, ui->spinBox->value());
    });
    connect(ui->action140, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(140, ui->spinBox->value());
    });
    connect(ui->action141, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(141, ui->spinBox->value());
    });
    connect(ui->action142, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(142, ui->spinBox->value());
    });
    connect(ui->action143, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(143, ui->spinBox->value());
    });
    connect(ui->action144, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(144, ui->spinBox->value());
    });
    connect(ui->action145, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(145, ui->spinBox->value());
    });
    connect(ui->action146, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(146, ui->spinBox->value());
    });
    connect(ui->action147, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(147, ui->spinBox->value());
    });
    connect(ui->action148, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(148, ui->spinBox->value());
    });
    connect(ui->action149, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(149, ui->spinBox->value());
    });
    connect(ui->action150, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(150, ui->spinBox->value());
    });
    connect(ui->action151, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(151, ui->spinBox->value());
    });
    connect(ui->action152, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(152, ui->spinBox->value());
    });
    connect(ui->action153, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(153, ui->spinBox->value());
    });
    connect(ui->action154, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(154, ui->spinBox->value());
    });
    connect(ui->action155, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(155, ui->spinBox->value());
    });
    connect(ui->action156, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(156, ui->spinBox->value());
    });
    connect(ui->action157, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(157, ui->spinBox->value());
    });
    connect(ui->action158, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(158, ui->spinBox->value());
    });
    connect(ui->action159, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(159, ui->spinBox->value());
    });
    connect(ui->action160, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(160, ui->spinBox->value());
    });
    connect(ui->action161, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(161, ui->spinBox->value());
    });
    connect(ui->action162, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(162, ui->spinBox->value());
    });
    connect(ui->action163, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(163, ui->spinBox->value());
    });
    connect(ui->action164, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(164, ui->spinBox->value());
    });
    connect(ui->action165, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(165, ui->spinBox->value());
    });
    connect(ui->action166, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(166, ui->spinBox->value());
    });
    connect(ui->action167, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(167, ui->spinBox->value());
    });
    connect(ui->action168, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(168, ui->spinBox->value());
    });
    connect(ui->action169, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(169, ui->spinBox->value());
    });
    connect(ui->action170, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(170, ui->spinBox->value());
    });
    connect(ui->action171, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(171, ui->spinBox->value());
    });
    connect(ui->action172, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(172, ui->spinBox->value());
    });
    connect(ui->action173, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(173, ui->spinBox->value());
    });
    connect(ui->action174, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(174, ui->spinBox->value());
    });
    connect(ui->action175, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(175, ui->spinBox->value());
    });
    connect(ui->action176, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(176, ui->spinBox->value());
    });
    connect(ui->action177, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(177, ui->spinBox->value());
    });
    connect(ui->action178, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(178, ui->spinBox->value());
    });
    connect(ui->action179, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(179, ui->spinBox->value());
    });
    connect(ui->action180, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(180, ui->spinBox->value());
    });
    connect(ui->action181, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(181, ui->spinBox->value());
    });
    connect(ui->action182, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(182, ui->spinBox->value());
    });
    connect(ui->action183, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(183, ui->spinBox->value());
    });
    connect(ui->action184, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(184, ui->spinBox->value());
    });
    connect(ui->action185, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(185, ui->spinBox->value());
    });
    connect(ui->action186, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(186, ui->spinBox->value());
    });
    connect(ui->action187, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(187, ui->spinBox->value());
    });
    connect(ui->action188, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(188, ui->spinBox->value());
    });
    connect(ui->action189, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(189, ui->spinBox->value());
    });
    connect(ui->action190, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(190, ui->spinBox->value());
    });
    connect(ui->action191, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(191, ui->spinBox->value());
    });
    connect(ui->action192, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(192, ui->spinBox->value());
    });
    connect(ui->action193, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(193, ui->spinBox->value());
    });
    connect(ui->action194, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(194, ui->spinBox->value());
    });
    connect(ui->action195, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(195, ui->spinBox->value());
    });
    connect(ui->action196, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(196, ui->spinBox->value());
    });
    connect(ui->action197, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(197, ui->spinBox->value());
    });
    connect(ui->action198, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(198, ui->spinBox->value());
    });
    connect(ui->action199, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(199, ui->spinBox->value());
    });
    connect(ui->action200, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(200, ui->spinBox->value());
    });
    connect(ui->action201, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(201, ui->spinBox->value());
    });
    connect(ui->action202, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(202, ui->spinBox->value());
    });
    connect(ui->action203, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(203, ui->spinBox->value());
    });
    connect(ui->action204, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(204, ui->spinBox->value());
    });
    connect(ui->action205, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(205, ui->spinBox->value());
    });
    connect(ui->action206, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(206, ui->spinBox->value());
    });
    connect(ui->action207, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(207, ui->spinBox->value());
    });
    connect(ui->action208, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(208, ui->spinBox->value());
    });
    connect(ui->action209, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(209, ui->spinBox->value());
    });
    connect(ui->action210, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(210, ui->spinBox->value());
    });
    connect(ui->action211, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(211, ui->spinBox->value());
    });
    connect(ui->action212, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(212, ui->spinBox->value());
    });
    connect(ui->action213, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(213, ui->spinBox->value());
    });
    connect(ui->action214, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(214, ui->spinBox->value());
    });
    connect(ui->action215, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(215, ui->spinBox->value());
    });
    connect(ui->action216, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(216, ui->spinBox->value());
    });
    connect(ui->action217, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(217, ui->spinBox->value());
    });
    connect(ui->action218, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(218, ui->spinBox->value());
    });
    connect(ui->action219, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(219, ui->spinBox->value());
    });
    connect(ui->action220, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(220, ui->spinBox->value());
    });
    connect(ui->action221, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(221, ui->spinBox->value());
    });
    connect(ui->action222, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(222, ui->spinBox->value());
    });
    connect(ui->action223, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(223, ui->spinBox->value());
    });
    connect(ui->action224, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(224, ui->spinBox->value());
    });
    connect(ui->action225, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(225, ui->spinBox->value());
    });
    connect(ui->action226, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(226, ui->spinBox->value());
    });
    connect(ui->action227, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(227, ui->spinBox->value());
    });
    connect(ui->action228, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(228, ui->spinBox->value());
    });
    connect(ui->action229, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(229, ui->spinBox->value());
    });
    connect(ui->action230, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(230, ui->spinBox->value());
    });
    connect(ui->action231, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(231, ui->spinBox->value());
    });
    connect(ui->action232, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(232, ui->spinBox->value());
    });
    connect(ui->action233, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(233, ui->spinBox->value());
    });
    connect(ui->action234, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(234, ui->spinBox->value());
    });
    connect(ui->action235, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(235, ui->spinBox->value());
    });
    connect(ui->action236, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(236, ui->spinBox->value());
    });
    connect(ui->action237, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(237, ui->spinBox->value());
    });
    connect(ui->action238, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(238, ui->spinBox->value());
    });
    connect(ui->action239, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(239, ui->spinBox->value());
    });
    connect(ui->action240, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(240, ui->spinBox->value());
    });
    connect(ui->action241, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(241, ui->spinBox->value());
    });
    connect(ui->action242, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(242, ui->spinBox->value());
    });
    connect(ui->action243, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(243, ui->spinBox->value());
    });
    connect(ui->action244, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(244, ui->spinBox->value());
    });
    connect(ui->action245, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(245, ui->spinBox->value());
    });
    connect(ui->action246, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(246, ui->spinBox->value());
    });
    connect(ui->action247, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(247, ui->spinBox->value());
    });
    connect(ui->action248, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(248, ui->spinBox->value());
    });
    connect(ui->action249, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(249, ui->spinBox->value());
    });
    connect(ui->action250, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(250, ui->spinBox->value());
    });
    connect(ui->action251, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(251, ui->spinBox->value());
    });
    connect(ui->action252, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(252, ui->spinBox->value());
    });
    connect(ui->action253, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(253, ui->spinBox->value());
    });
    connect(ui->action254, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(254, ui->spinBox->value());
    });
    connect(ui->action255, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(255, ui->spinBox->value());
    });
    connect(ui->action256, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(256, ui->spinBox->value());
    });
    connect(ui->action257, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(257, ui->spinBox->value());
    });
    connect(ui->action258, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(258, ui->spinBox->value());
    });
    connect(ui->action259, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(259, ui->spinBox->value());
    });
    connect(ui->action260, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(260, ui->spinBox->value());
    });
    connect(ui->action261, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(261, ui->spinBox->value());
    });
    connect(ui->action262, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(262, ui->spinBox->value());
    });
    connect(ui->action263, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(263, ui->spinBox->value());
    });
    connect(ui->action264, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(264, ui->spinBox->value());
    });
    connect(ui->action265, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(265, ui->spinBox->value());
    });
    connect(ui->action266, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(266, ui->spinBox->value());
    });
    connect(ui->action267, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(267, ui->spinBox->value());
    });
    connect(ui->action268, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(268, ui->spinBox->value());
    });
    connect(ui->action269, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(269, ui->spinBox->value());
    });
    connect(ui->action270, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(270, ui->spinBox->value());
    });
    connect(ui->action271, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(271, ui->spinBox->value());
    });
    connect(ui->action272, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(272, ui->spinBox->value());
    });
    connect(ui->action273, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(273, ui->spinBox->value());
    });
    connect(ui->action274, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(274, ui->spinBox->value());
    });
    connect(ui->action275, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(275, ui->spinBox->value());
    });
    connect(ui->action276, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(276, ui->spinBox->value());
    });
    connect(ui->action277, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(277, ui->spinBox->value());
    });
    connect(ui->action278, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(278, ui->spinBox->value());
    });
    connect(ui->action279, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(279, ui->spinBox->value());
    });
    connect(ui->action280, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(280, ui->spinBox->value());
    });
    connect(ui->action281, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(281, ui->spinBox->value());
    });
    connect(ui->action282, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(282, ui->spinBox->value());
    });
    connect(ui->action283, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(283, ui->spinBox->value());
    });
    connect(ui->action284, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(284, ui->spinBox->value());
    });
    connect(ui->action285, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(285, ui->spinBox->value());
    });
    connect(ui->action286, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(286, ui->spinBox->value());
    });
    connect(ui->action287, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(287, ui->spinBox->value());
    });
    connect(ui->action288, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(288, ui->spinBox->value());
    });
    connect(ui->action289, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(289, ui->spinBox->value());
    });
    connect(ui->action290, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(290, ui->spinBox->value());
    });
    connect(ui->action291, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(291, ui->spinBox->value());
    });
    connect(ui->action292, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(292, ui->spinBox->value());
    });
    connect(ui->action293, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(293, ui->spinBox->value());
    });
    connect(ui->action294, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(294, ui->spinBox->value());
    });
    connect(ui->action295, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(295, ui->spinBox->value());
    });
    connect(ui->action296, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(296, ui->spinBox->value());
    });
    connect(ui->action297, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(297, ui->spinBox->value());
    });
    connect(ui->action298, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(298, ui->spinBox->value());
    });
    connect(ui->action299, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(299, ui->spinBox->value());
    });
    connect(ui->action300, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(300, ui->spinBox->value());
    });
    connect(ui->action301, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(301, ui->spinBox->value());
    });
    connect(ui->action302, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(302, ui->spinBox->value());
    });
    connect(ui->action303, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(303, ui->spinBox->value());
    });
    connect(ui->action304, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(304, ui->spinBox->value());
    });
    connect(ui->action305, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(305, ui->spinBox->value());
    });
    connect(ui->action306, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(306, ui->spinBox->value());
    });
    connect(ui->action307, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(307, ui->spinBox->value());
    });
    connect(ui->action308, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(308, ui->spinBox->value());
    });
    connect(ui->action309, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(309, ui->spinBox->value());
    });
    connect(ui->action310, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(310, ui->spinBox->value());
    });
    connect(ui->action311, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(311, ui->spinBox->value());
    });
    connect(ui->action312, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(312, ui->spinBox->value());
    });
    connect(ui->action313, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(313, ui->spinBox->value());
    });
    connect(ui->action314, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(314, ui->spinBox->value());
    });
    connect(ui->action315, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(315, ui->spinBox->value());
    });
    connect(ui->action316, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(316, ui->spinBox->value());
    });
    connect(ui->action317, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(317, ui->spinBox->value());
    });
    connect(ui->action318, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(318, ui->spinBox->value());
    });
    connect(ui->action319, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(319, ui->spinBox->value());
    });
    connect(ui->action320, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(320, ui->spinBox->value());
    });
    connect(ui->action321, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(321, ui->spinBox->value());
    });
    connect(ui->action322, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(322, ui->spinBox->value());
    });
    connect(ui->action323, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(323, ui->spinBox->value());
    });
    connect(ui->action324, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(324, ui->spinBox->value());
    });
    connect(ui->action325, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(325, ui->spinBox->value());
    });
    connect(ui->action326, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(326, ui->spinBox->value());
    });
    connect(ui->action327, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(327, ui->spinBox->value());
    });
    connect(ui->action328, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(328, ui->spinBox->value());
    });
    connect(ui->action329, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(329, ui->spinBox->value());
    });
    connect(ui->action330, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(330, ui->spinBox->value());
    });
    connect(ui->action331, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(331, ui->spinBox->value());
    });
    connect(ui->action332, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(332, ui->spinBox->value());
    });
    connect(ui->action333, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(333, ui->spinBox->value());
    });
    connect(ui->action334, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(334, ui->spinBox->value());
    });
    connect(ui->action335, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(335, ui->spinBox->value());
    });
    connect(ui->action336, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(336, ui->spinBox->value());
    });
    connect(ui->action337, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(337, ui->spinBox->value());
    });
    connect(ui->action338, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(338, ui->spinBox->value());
    });
    connect(ui->action339, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(339, ui->spinBox->value());
    });
    connect(ui->action340, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(340, ui->spinBox->value());
    });
    connect(ui->action341, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(341, ui->spinBox->value());
    });
    connect(ui->action342, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(342, ui->spinBox->value());
    });
    connect(ui->action343, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(343, ui->spinBox->value());
    });
    connect(ui->action344, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(344, ui->spinBox->value());
    });
    connect(ui->action345, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(345, ui->spinBox->value());
    });
    connect(ui->action346, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(346, ui->spinBox->value());
    });
    connect(ui->action347, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(347, ui->spinBox->value());
    });
    connect(ui->action348, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(348, ui->spinBox->value());
    });
    connect(ui->action349, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(349, ui->spinBox->value());
    });
    connect(ui->action350, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(350, ui->spinBox->value());
    });
    connect(ui->action351, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(351, ui->spinBox->value());
    });
    connect(ui->action352, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(352, ui->spinBox->value());
    });
    connect(ui->action353, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(353, ui->spinBox->value());
    });
    connect(ui->action354, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(354, ui->spinBox->value());
    });
    connect(ui->action355, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(355, ui->spinBox->value());
    });
    connect(ui->action356, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(356, ui->spinBox->value());
    });
    connect(ui->action357, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(357, ui->spinBox->value());
    });
    connect(ui->action358, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(358, ui->spinBox->value());
    });
    connect(ui->action359, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(359, ui->spinBox->value());
    });
    connect(ui->action360, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(360, ui->spinBox->value());
    });
    connect(ui->action361, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(361, ui->spinBox->value());
    });
    connect(ui->action362, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(362, ui->spinBox->value());
    });
    connect(ui->action363, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(363, ui->spinBox->value());
    });
    connect(ui->action364, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(364, ui->spinBox->value());
    });
    connect(ui->action365, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(365, ui->spinBox->value());
    });
    connect(ui->action366, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(366, ui->spinBox->value());
    });
    connect(ui->action367, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(367, ui->spinBox->value());
    });
    connect(ui->action368, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(368, ui->spinBox->value());
    });
    connect(ui->action369, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(369, ui->spinBox->value());
    });
    connect(ui->action370, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(370, ui->spinBox->value());
    });
    connect(ui->action371, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(371, ui->spinBox->value());
    });
    connect(ui->action372, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(372, ui->spinBox->value());
    });
    connect(ui->action373, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(373, ui->spinBox->value());
    });
    connect(ui->action374, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(374, ui->spinBox->value());
    });
    connect(ui->action375, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(375, ui->spinBox->value());
    });
    connect(ui->action376, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(376, ui->spinBox->value());
    });
    connect(ui->action377, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(377, ui->spinBox->value());
    });
    connect(ui->action378, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(378, ui->spinBox->value());
    });
    connect(ui->action379, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(379, ui->spinBox->value());
    });
    connect(ui->action380, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(380, ui->spinBox->value());
    });
    connect(ui->action381, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(381, ui->spinBox->value());
    });
    connect(ui->action382, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(382, ui->spinBox->value());
    });
    connect(ui->action383, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(383, ui->spinBox->value());
    });
    connect(ui->action384, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(384, ui->spinBox->value());
    });
    connect(ui->action385, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(385, ui->spinBox->value());
    });
    connect(ui->action386, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(386, ui->spinBox->value());
    });
    connect(ui->action387, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(387, ui->spinBox->value());
    });
    connect(ui->action388, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(388, ui->spinBox->value());
    });
    connect(ui->action389, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(389, ui->spinBox->value());
    });
    connect(ui->action390, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(390, ui->spinBox->value());
    });
    connect(ui->action391, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(391, ui->spinBox->value());
    });
    connect(ui->action392, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(392, ui->spinBox->value());
    });
    connect(ui->action393, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(393, ui->spinBox->value());
    });
    connect(ui->action394, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(394, ui->spinBox->value());
    });
    connect(ui->action395, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(395, ui->spinBox->value());
    });
    connect(ui->action396, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(396, ui->spinBox->value());
    });
    connect(ui->action397, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(397, ui->spinBox->value());
    });
    connect(ui->action398, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(398, ui->spinBox->value());
    });
    connect(ui->action399, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(399, ui->spinBox->value());
    });
    connect(ui->action400, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(400, ui->spinBox->value());
    });
    connect(ui->action401, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(401, ui->spinBox->value());
    });
    connect(ui->action402, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(402, ui->spinBox->value());
    });
    connect(ui->action403, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(403, ui->spinBox->value());
    });
    connect(ui->action404, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(404, ui->spinBox->value());
    });
    connect(ui->action405, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(405, ui->spinBox->value());
    });
    connect(ui->action406, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(406, ui->spinBox->value());
    });
    connect(ui->action407, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(407, ui->spinBox->value());
    });
    connect(ui->action408, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(408, ui->spinBox->value());
    });
    connect(ui->action409, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(409, ui->spinBox->value());
    });
    connect(ui->action410, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(410, ui->spinBox->value());
    });
    connect(ui->action411, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(411, ui->spinBox->value());
    });
    connect(ui->action412, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(412, ui->spinBox->value());
    });
    connect(ui->action413, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(413, ui->spinBox->value());
    });
    connect(ui->action414, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(414, ui->spinBox->value());
    });
    connect(ui->action415, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(415, ui->spinBox->value());
    });
    connect(ui->action416, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(416, ui->spinBox->value());
    });
    connect(ui->action417, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(417, ui->spinBox->value());
    });
    connect(ui->action418, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(418, ui->spinBox->value());
    });
    connect(ui->action419, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(419, ui->spinBox->value());
    });
    connect(ui->action420, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(420, ui->spinBox->value());
    });
    connect(ui->action421, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(421, ui->spinBox->value());
    });
    connect(ui->action422, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(422, ui->spinBox->value());
    });
    connect(ui->action423, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(423, ui->spinBox->value());
    });
    connect(ui->action424, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(424, ui->spinBox->value());
    });
    connect(ui->action425, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(425, ui->spinBox->value());
    });
    connect(ui->action426, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(426, ui->spinBox->value());
    });
    connect(ui->action427, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(427, ui->spinBox->value());
    });
    connect(ui->action428, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(428, ui->spinBox->value());
    });
    connect(ui->action429, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(429, ui->spinBox->value());
    });
    connect(ui->action430, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(430, ui->spinBox->value());
    });
    connect(ui->action431, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(431, ui->spinBox->value());
    });
    connect(ui->action432, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(432, ui->spinBox->value());
    });
    connect(ui->action433, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(433, ui->spinBox->value());
    });
    connect(ui->action434, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(434, ui->spinBox->value());
    });
    connect(ui->action435, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(435, ui->spinBox->value());
    });
    connect(ui->action436, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(436, ui->spinBox->value());
    });
    connect(ui->action437, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(437, ui->spinBox->value());
    });
    connect(ui->action438, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(438, ui->spinBox->value());
    });
    connect(ui->action439, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(439, ui->spinBox->value());
    });
    connect(ui->action440, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(440, ui->spinBox->value());
    });
    connect(ui->action441, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(441, ui->spinBox->value());
    });
    connect(ui->action442, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(442, ui->spinBox->value());
    });
    connect(ui->action443, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(443, ui->spinBox->value());
    });
    connect(ui->action444, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(444, ui->spinBox->value());
    });
    connect(ui->action445, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(445, ui->spinBox->value());
    });
    connect(ui->action446, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(446, ui->spinBox->value());
    });
    connect(ui->action447, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(447, ui->spinBox->value());
    });
    connect(ui->action448, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(448, ui->spinBox->value());
    });
    connect(ui->action449, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(449, ui->spinBox->value());
    });
    connect(ui->action450, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(450, ui->spinBox->value());
    });
    connect(ui->action451, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(451, ui->spinBox->value());
    });
    connect(ui->action452, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(452, ui->spinBox->value());
    });
    connect(ui->action453, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(453, ui->spinBox->value());
    });
    connect(ui->action454, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(454, ui->spinBox->value());
    });
    connect(ui->action455, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(455, ui->spinBox->value());
    });
    connect(ui->action456, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(456, ui->spinBox->value());
    });
    connect(ui->action457, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(457, ui->spinBox->value());
    });
    connect(ui->action458, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(458, ui->spinBox->value());
    });
    connect(ui->action459, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(459, ui->spinBox->value());
    });
    connect(ui->action460, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(460, ui->spinBox->value());
    });
    connect(ui->action461, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(461, ui->spinBox->value());
    });
    connect(ui->action462, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(462, ui->spinBox->value());
    });
    connect(ui->action463, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(463, ui->spinBox->value());
    });
    connect(ui->action464, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(464, ui->spinBox->value());
    });
    connect(ui->action465, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(465, ui->spinBox->value());
    });
    connect(ui->action466, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(466, ui->spinBox->value());
    });
    connect(ui->action467, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(467, ui->spinBox->value());
    });
    connect(ui->action468, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(468, ui->spinBox->value());
    });
    connect(ui->action469, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(469, ui->spinBox->value());
    });
    connect(ui->action470, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(470, ui->spinBox->value());
    });
    connect(ui->action471, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(471, ui->spinBox->value());
    });
    connect(ui->action472, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(472, ui->spinBox->value());
    });
    connect(ui->action473, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(473, ui->spinBox->value());
    });
    connect(ui->action474, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(474, ui->spinBox->value());
    });
    connect(ui->action475, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(475, ui->spinBox->value());
    });
    connect(ui->action476, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(476, ui->spinBox->value());
    });
    connect(ui->action477, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(477, ui->spinBox->value());
    });
    connect(ui->action478, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(478, ui->spinBox->value());
    });
    connect(ui->action479, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(479, ui->spinBox->value());
    });
    connect(ui->action480, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(480, ui->spinBox->value());
    });
    connect(ui->action481, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(481, ui->spinBox->value());
    });
    connect(ui->action482, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(482, ui->spinBox->value());
    });
    connect(ui->action483, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(483, ui->spinBox->value());
    });
    connect(ui->action484, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(484, ui->spinBox->value());
    });
    connect(ui->action485, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(485, ui->spinBox->value());
    });
    connect(ui->action486, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(486, ui->spinBox->value());
    });
    connect(ui->action487, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(487, ui->spinBox->value());
    });
    connect(ui->action488, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(488, ui->spinBox->value());
    });
    connect(ui->action489, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(489, ui->spinBox->value());
    });
    connect(ui->action490, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(490, ui->spinBox->value());
    });
    connect(ui->action491, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(491, ui->spinBox->value());
    });
    connect(ui->action492, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(492, ui->spinBox->value());
    });
    connect(ui->action493, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(493, ui->spinBox->value());
    });
    connect(ui->action494, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(494, ui->spinBox->value());
    });
    connect(ui->action495, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(495, ui->spinBox->value());
    });
    connect(ui->action496, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(496, ui->spinBox->value());
    });
    connect(ui->action497, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(497, ui->spinBox->value());
    });
    connect(ui->action498, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(498, ui->spinBox->value());
    });
    connect(ui->action499, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(499, ui->spinBox->value());
    });
    connect(ui->action500, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(500, ui->spinBox->value());
    });
    connect(ui->action501, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(501, ui->spinBox->value());
    });
    connect(ui->action502, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(502, ui->spinBox->value());
    });
    connect(ui->action503, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(503, ui->spinBox->value());
    });
    connect(ui->action504, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(504, ui->spinBox->value());
    });
    connect(ui->action505, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(505, ui->spinBox->value());
    });
    connect(ui->action506, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(506, ui->spinBox->value());
    });
    connect(ui->action507, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(507, ui->spinBox->value());
    });
    connect(ui->action508, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(508, ui->spinBox->value());
    });
    connect(ui->action509, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(509, ui->spinBox->value());
    });
    connect(ui->action510, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(510, ui->spinBox->value());
    });
    connect(ui->action511, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(511, ui->spinBox->value());
    });
    connect(ui->action512, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(512, ui->spinBox->value());
    });
    connect(ui->action513, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(513, ui->spinBox->value());
    });
    connect(ui->action514, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(514, ui->spinBox->value());
    });
    connect(ui->action515, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(515, ui->spinBox->value());
    });
    connect(ui->action516, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(516, ui->spinBox->value());
    });
    connect(ui->action517, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(517, ui->spinBox->value());
    });
    connect(ui->action518, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(518, ui->spinBox->value());
    });
    connect(ui->action519, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(519, ui->spinBox->value());
    });
    connect(ui->action520, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(520, ui->spinBox->value());
    });
    connect(ui->action521, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(521, ui->spinBox->value());
    });
    connect(ui->action522, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(522, ui->spinBox->value());
    });
    connect(ui->action523, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(523, ui->spinBox->value());
    });
    connect(ui->action524, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(524, ui->spinBox->value());
    });
    connect(ui->action525, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(525, ui->spinBox->value());
    });
    connect(ui->action526, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(526, ui->spinBox->value());
    });
    connect(ui->action527, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(527, ui->spinBox->value());
    });
    connect(ui->action528, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(528, ui->spinBox->value());
    });
    connect(ui->action529, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(529, ui->spinBox->value());
    });
    connect(ui->action530, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(530, ui->spinBox->value());
    });
    connect(ui->action531, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(531, ui->spinBox->value());
    });
    connect(ui->action532, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(532, ui->spinBox->value());
    });
    connect(ui->action533, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(533, ui->spinBox->value());
    });
    connect(ui->action534, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(534, ui->spinBox->value());
    });
    connect(ui->action535, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(535, ui->spinBox->value());
    });
    connect(ui->action536, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(536, ui->spinBox->value());
    });
    connect(ui->action537, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(537, ui->spinBox->value());
    });
    connect(ui->action538, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(538, ui->spinBox->value());
    });
    connect(ui->action539, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(539, ui->spinBox->value());
    });
    connect(ui->action540, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(540, ui->spinBox->value());
    });
    connect(ui->action541, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(541, ui->spinBox->value());
    });
    connect(ui->action542, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(542, ui->spinBox->value());
    });
    connect(ui->action543, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(543, ui->spinBox->value());
    });
    connect(ui->action544, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(544, ui->spinBox->value());
    });
    connect(ui->action545, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(545, ui->spinBox->value());
    });
    connect(ui->action546, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(546, ui->spinBox->value());
    });
    connect(ui->action547, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(547, ui->spinBox->value());
    });
    connect(ui->action548, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(548, ui->spinBox->value());
    });
    connect(ui->action549, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(549, ui->spinBox->value());
    });
    connect(ui->action550, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(550, ui->spinBox->value());
    });
    connect(ui->action551, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(551, ui->spinBox->value());
    });
    connect(ui->action552, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(552, ui->spinBox->value());
    });
    connect(ui->action553, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(553, ui->spinBox->value());
    });
    connect(ui->action554, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(554, ui->spinBox->value());
    });
    connect(ui->action555, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(555, ui->spinBox->value());
    });
    connect(ui->action556, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(556, ui->spinBox->value());
    });
    connect(ui->action557, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(557, ui->spinBox->value());
    });
    connect(ui->action558, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(558, ui->spinBox->value());
    });
    connect(ui->action559, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(559, ui->spinBox->value());
    });
    connect(ui->action560, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(560, ui->spinBox->value());
    });
    connect(ui->action561, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(561, ui->spinBox->value());
    });
    connect(ui->action562, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(562, ui->spinBox->value());
    });
    connect(ui->action563, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(563, ui->spinBox->value());
    });
    connect(ui->action564, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(564, ui->spinBox->value());
    });
    connect(ui->action565, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(565, ui->spinBox->value());
    });
    connect(ui->action566, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(566, ui->spinBox->value());
    });
    connect(ui->action567, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(567, ui->spinBox->value());
    });
    connect(ui->action568, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(568, ui->spinBox->value());
    });
    connect(ui->action569, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(569, ui->spinBox->value());
    });
    connect(ui->action570, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(570, ui->spinBox->value());
    });
    connect(ui->action571, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(571, ui->spinBox->value());
    });
    connect(ui->action572, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(572, ui->spinBox->value());
    });
    connect(ui->action573, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(573, ui->spinBox->value());
    });
    connect(ui->action574, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(574, ui->spinBox->value());
    });
    connect(ui->action575, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(575, ui->spinBox->value());
    });
    connect(ui->action576, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(576, ui->spinBox->value());
    });
    connect(ui->action577, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(577, ui->spinBox->value());
    });
    connect(ui->action578, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(578, ui->spinBox->value());
    });
    connect(ui->action579, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(579, ui->spinBox->value());
    });
    connect(ui->action580, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(580, ui->spinBox->value());
    });
    connect(ui->action581, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(581, ui->spinBox->value());
    });
    connect(ui->action582, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(582, ui->spinBox->value());
    });
    connect(ui->action583, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(583, ui->spinBox->value());
    });
    connect(ui->action584, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(584, ui->spinBox->value());
    });
    connect(ui->action585, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(585, ui->spinBox->value());
    });
    connect(ui->action586, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(586, ui->spinBox->value());
    });
    connect(ui->action587, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(587, ui->spinBox->value());
    });
    connect(ui->action588, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(588, ui->spinBox->value());
    });
    connect(ui->action589, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(589, ui->spinBox->value());
    });
    connect(ui->action590, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(590, ui->spinBox->value());
    });
    connect(ui->action591, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(591, ui->spinBox->value());
    });
    connect(ui->action592, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(592, ui->spinBox->value());
    });
    connect(ui->action593, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(593, ui->spinBox->value());
    });
    connect(ui->action594, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(594, ui->spinBox->value());
    });
    connect(ui->action595, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(595, ui->spinBox->value());
    });
    connect(ui->action596, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(596, ui->spinBox->value());
    });
    connect(ui->action597, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(597, ui->spinBox->value());
    });
    connect(ui->action598, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(598, ui->spinBox->value());
    });
    connect(ui->action599, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(599, ui->spinBox->value());
    });
    connect(ui->action600, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(600, ui->spinBox->value());
    });
    connect(ui->action601, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(601, ui->spinBox->value());
    });
    connect(ui->action602, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(602, ui->spinBox->value());
    });
    connect(ui->action603, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(603, ui->spinBox->value());
    });
    connect(ui->action604, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(604, ui->spinBox->value());
    });
    connect(ui->action605, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(605, ui->spinBox->value());
    });
    connect(ui->action606, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(606, ui->spinBox->value());
    });
    connect(ui->action607, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(607, ui->spinBox->value());
    });
    connect(ui->action608, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(608, ui->spinBox->value());
    });
    connect(ui->action609, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(609, ui->spinBox->value());
    });
    connect(ui->action610, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(610, ui->spinBox->value());
    });
    connect(ui->action611, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(611, ui->spinBox->value());
    });
    connect(ui->action612, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(612, ui->spinBox->value());
    });
    connect(ui->action613, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(613, ui->spinBox->value());
    });
    connect(ui->action614, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(614, ui->spinBox->value());
    });
    connect(ui->action615, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(615, ui->spinBox->value());
    });
    connect(ui->action616, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(616, ui->spinBox->value());
    });
    connect(ui->action617, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(617, ui->spinBox->value());
    });
    connect(ui->action618, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(618, ui->spinBox->value());
    });
    connect(ui->action619, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(619, ui->spinBox->value());
    });
    connect(ui->action620, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(620, ui->spinBox->value());
    });
    connect(ui->action621, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(621, ui->spinBox->value());
    });
    connect(ui->action622, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(622, ui->spinBox->value());
    });
    connect(ui->action623, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(623, ui->spinBox->value());
    });
    connect(ui->action624, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(624, ui->spinBox->value());
    });
    connect(ui->action625, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(625, ui->spinBox->value());
    });
    connect(ui->action626, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(626, ui->spinBox->value());
    });
    connect(ui->action627, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(627, ui->spinBox->value());
    });
    connect(ui->action628, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(628, ui->spinBox->value());
    });
    connect(ui->action629, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(629, ui->spinBox->value());
    });
    connect(ui->action630, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(630, ui->spinBox->value());
    });
    connect(ui->action631, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(631, ui->spinBox->value());
    });
    connect(ui->action632, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(632, ui->spinBox->value());
    });
    connect(ui->action633, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(633, ui->spinBox->value());
    });
    connect(ui->action634, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(634, ui->spinBox->value());
    });
    connect(ui->action635, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(635, ui->spinBox->value());
    });
    connect(ui->action636, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(636, ui->spinBox->value());
    });
    connect(ui->action637, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(637, ui->spinBox->value());
    });
    connect(ui->action638, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(638, ui->spinBox->value());
    });
    connect(ui->action639, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(639, ui->spinBox->value());
    });
    connect(ui->action640, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(640, ui->spinBox->value());
    });
    connect(ui->action641, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(641, ui->spinBox->value());
    });
    connect(ui->action642, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(642, ui->spinBox->value());
    });
    connect(ui->action643, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(643, ui->spinBox->value());
    });
    connect(ui->action644, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(644, ui->spinBox->value());
    });
    connect(ui->action645, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(645, ui->spinBox->value());
    });
    connect(ui->action646, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(646, ui->spinBox->value());
    });
    connect(ui->action647, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(647, ui->spinBox->value());
    });
    connect(ui->action648, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(648, ui->spinBox->value());
    });
    connect(ui->action649, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(649, ui->spinBox->value());
    });
    connect(ui->action650, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(650, ui->spinBox->value());
    });
    connect(ui->action651, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(651, ui->spinBox->value());
    });
    connect(ui->action652, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(652, ui->spinBox->value());
    });
    connect(ui->action653, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(653, ui->spinBox->value());
    });
    connect(ui->action654, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(654, ui->spinBox->value());
    });
    connect(ui->action655, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(655, ui->spinBox->value());
    });
    connect(ui->action656, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(656, ui->spinBox->value());
    });
    connect(ui->action657, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(657, ui->spinBox->value());
    });
    connect(ui->action658, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(658, ui->spinBox->value());
    });
    connect(ui->action659, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(659, ui->spinBox->value());
    });
    connect(ui->action660, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(660, ui->spinBox->value());
    });
    connect(ui->action661, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(661, ui->spinBox->value());
    });
    connect(ui->action662, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(662, ui->spinBox->value());
    });
    connect(ui->action663, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(663, ui->spinBox->value());
    });
    connect(ui->action664, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(664, ui->spinBox->value());
    });
    connect(ui->action665, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(665, ui->spinBox->value());
    });
    connect(ui->action666, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(666, ui->spinBox->value());
    });
    connect(ui->action667, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(667, ui->spinBox->value());
    });
    connect(ui->action668, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(668, ui->spinBox->value());
    });
    connect(ui->action669, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(669, ui->spinBox->value());
    });
    connect(ui->action670, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(670, ui->spinBox->value());
    });
    connect(ui->action671, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(671, ui->spinBox->value());
    });
    connect(ui->action672, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(672, ui->spinBox->value());
    });
    connect(ui->action673, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(673, ui->spinBox->value());
    });
    connect(ui->action674, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(674, ui->spinBox->value());
    });
    connect(ui->action675, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(675, ui->spinBox->value());
    });
    connect(ui->action676, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(676, ui->spinBox->value());
    });
    connect(ui->action677, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(677, ui->spinBox->value());
    });
    connect(ui->action678, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(678, ui->spinBox->value());
    });
    connect(ui->action679, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(679, ui->spinBox->value());
    });
    connect(ui->action680, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(680, ui->spinBox->value());
    });
    connect(ui->action681, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(681, ui->spinBox->value());
    });
    connect(ui->action682, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(682, ui->spinBox->value());
    });
    connect(ui->action683, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(683, ui->spinBox->value());
    });
    connect(ui->action684, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(684, ui->spinBox->value());
    });
    connect(ui->action685, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(685, ui->spinBox->value());
    });
    connect(ui->action686, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(686, ui->spinBox->value());
    });
    connect(ui->action687, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(687, ui->spinBox->value());
    });
    connect(ui->action688, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(688, ui->spinBox->value());
    });
    connect(ui->action689, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(689, ui->spinBox->value());
    });
    connect(ui->action690, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(690, ui->spinBox->value());
    });
    connect(ui->action691, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(691, ui->spinBox->value());
    });
    connect(ui->action692, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(692, ui->spinBox->value());
    });
    connect(ui->action693, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(693, ui->spinBox->value());
    });
    connect(ui->action694, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(694, ui->spinBox->value());
    });
    connect(ui->action695, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(695, ui->spinBox->value());
    });
    connect(ui->action696, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(696, ui->spinBox->value());
    });
    connect(ui->action697, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(697, ui->spinBox->value());
    });
    connect(ui->action698, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(698, ui->spinBox->value());
    });
    connect(ui->action699, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(699, ui->spinBox->value());
    });
    connect(ui->action700, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(700, ui->spinBox->value());
    });
    connect(ui->action701, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(701, ui->spinBox->value());
    });
    connect(ui->action702, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(702, ui->spinBox->value());
    });
    connect(ui->action703, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(703, ui->spinBox->value());
    });
    connect(ui->action704, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(704, ui->spinBox->value());
    });
    connect(ui->action705, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(705, ui->spinBox->value());
    });
    connect(ui->action706, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(706, ui->spinBox->value());
    });
    connect(ui->action707, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(707, ui->spinBox->value());
    });
    connect(ui->action708, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(708, ui->spinBox->value());
    });
    connect(ui->action709, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(709, ui->spinBox->value());
    });
    connect(ui->action710, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(710, ui->spinBox->value());
    });
    connect(ui->action711, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(711, ui->spinBox->value());
    });
    connect(ui->action712, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(712, ui->spinBox->value());
    });
    connect(ui->action713, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(713, ui->spinBox->value());
    });
    connect(ui->action714, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(714, ui->spinBox->value());
    });
    connect(ui->action715, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(715, ui->spinBox->value());
    });
    connect(ui->action716, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(716, ui->spinBox->value());
    });
    connect(ui->action717, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(717, ui->spinBox->value());
    });
    connect(ui->action718, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(718, ui->spinBox->value());
    });
    connect(ui->action719, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(719, ui->spinBox->value());
    });
    connect(ui->action720, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(720, ui->spinBox->value());
    });
    connect(ui->action721, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(721, ui->spinBox->value());
    });
    connect(ui->action722, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(722, ui->spinBox->value());
    });
    connect(ui->action723, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(723, ui->spinBox->value());
    });
    connect(ui->action724, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(724, ui->spinBox->value());
    });
    connect(ui->action725, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(725, ui->spinBox->value());
    });
    connect(ui->action726, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(726, ui->spinBox->value());
    });
    connect(ui->action727, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(727, ui->spinBox->value());
    });
    connect(ui->action728, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(728, ui->spinBox->value());
    });
    connect(ui->action729, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(729, ui->spinBox->value());
    });
    connect(ui->action730, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(730, ui->spinBox->value());
    });
    connect(ui->action731, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(731, ui->spinBox->value());
    });
    connect(ui->action732, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(732, ui->spinBox->value());
    });
    connect(ui->action733, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(733, ui->spinBox->value());
    });
    connect(ui->action734, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(734, ui->spinBox->value());
    });
    connect(ui->action735, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(735, ui->spinBox->value());
    });
    connect(ui->action736, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(736, ui->spinBox->value());
    });
    connect(ui->action737, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(737, ui->spinBox->value());
    });
    connect(ui->action738, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(738, ui->spinBox->value());
    });
    connect(ui->action739, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(739, ui->spinBox->value());
    });
    connect(ui->action740, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(740, ui->spinBox->value());
    });
    connect(ui->action741, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(741, ui->spinBox->value());
    });
    connect(ui->action742, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(742, ui->spinBox->value());
    });
    connect(ui->action743, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(743, ui->spinBox->value());
    });
    connect(ui->action744, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(744, ui->spinBox->value());
    });
    connect(ui->action745, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(745, ui->spinBox->value());
    });
    connect(ui->action746, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(746, ui->spinBox->value());
    });
    connect(ui->action747, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(747, ui->spinBox->value());
    });
    connect(ui->action748, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(748, ui->spinBox->value());
    });
    connect(ui->action749, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(749, ui->spinBox->value());
    });
    connect(ui->action750, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(750, ui->spinBox->value());
    });
    connect(ui->action751, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(751, ui->spinBox->value());
    });
    connect(ui->action752, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(752, ui->spinBox->value());
    });
    connect(ui->action753, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(753, ui->spinBox->value());
    });
    connect(ui->action754, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(754, ui->spinBox->value());
    });
    connect(ui->action755, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(755, ui->spinBox->value());
    });
    connect(ui->action756, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(756, ui->spinBox->value());
    });
    connect(ui->action757, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(757, ui->spinBox->value());
    });
    connect(ui->action758, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(758, ui->spinBox->value());
    });
    connect(ui->action759, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(759, ui->spinBox->value());
    });
    connect(ui->action760, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(760, ui->spinBox->value());
    });
    connect(ui->action761, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(761, ui->spinBox->value());
    });
    connect(ui->action762, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(762, ui->spinBox->value());
    });
    connect(ui->action763, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(763, ui->spinBox->value());
    });
    connect(ui->action764, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(764, ui->spinBox->value());
    });
    connect(ui->action765, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(765, ui->spinBox->value());
    });
    connect(ui->action766, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(766, ui->spinBox->value());
    });
    connect(ui->action767, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(767, ui->spinBox->value());
    });
    connect(ui->action768, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(768, ui->spinBox->value());
    });
    connect(ui->action769, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(769, ui->spinBox->value());
    });
    connect(ui->action770, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(770, ui->spinBox->value());
    });
    connect(ui->action771, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(771, ui->spinBox->value());
    });
    connect(ui->action772, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(772, ui->spinBox->value());
    });
    connect(ui->action773, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(773, ui->spinBox->value());
    });
    connect(ui->action774, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(774, ui->spinBox->value());
    });
    connect(ui->action775, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(775, ui->spinBox->value());
    });
    connect(ui->action776, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(776, ui->spinBox->value());
    });
    connect(ui->action777, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(777, ui->spinBox->value());
    });
    connect(ui->action778, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(778, ui->spinBox->value());
    });
    connect(ui->action779, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(779, ui->spinBox->value());
    });
    connect(ui->action780, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(780, ui->spinBox->value());
    });
    connect(ui->action781, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(781, ui->spinBox->value());
    });
    connect(ui->action782, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(782, ui->spinBox->value());
    });
    connect(ui->action783, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(783, ui->spinBox->value());
    });
    connect(ui->action784, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(784, ui->spinBox->value());
    });
    connect(ui->action785, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(785, ui->spinBox->value());
    });
    connect(ui->action786, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(786, ui->spinBox->value());
    });
    connect(ui->action787, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(787, ui->spinBox->value());
    });
    connect(ui->action788, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(788, ui->spinBox->value());
    });
    connect(ui->action789, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(789, ui->spinBox->value());
    });
    connect(ui->action790, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(790, ui->spinBox->value());
    });
    connect(ui->action791, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(791, ui->spinBox->value());
    });
    connect(ui->action792, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(792, ui->spinBox->value());
    });
    connect(ui->action793, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(793, ui->spinBox->value());
    });
    connect(ui->action794, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(794, ui->spinBox->value());
    });
    connect(ui->action795, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(795, ui->spinBox->value());
    });
    connect(ui->action796, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(796, ui->spinBox->value());
    });
    connect(ui->action797, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(797, ui->spinBox->value());
    });
    connect(ui->action798, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(798, ui->spinBox->value());
    });
    connect(ui->action799, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(799, ui->spinBox->value());
    });
    connect(ui->action801, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(801, ui->spinBox->value());
    });
    connect(ui->action802, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(802, ui->spinBox->value());
    });
    connect(ui->action803, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(803, ui->spinBox->value());
    });
    connect(ui->action804, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(804, ui->spinBox->value());
    });
    connect(ui->action805, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(805, ui->spinBox->value());
    });
    connect(ui->action806, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(806, ui->spinBox->value());
    });
    connect(ui->action807, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(807, ui->spinBox->value());
    });
    connect(ui->action808, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(808, ui->spinBox->value());
    });
    connect(ui->action809, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(809, ui->spinBox->value());
    });
    connect(ui->action810, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(810, ui->spinBox->value());
    });
    connect(ui->action811, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(811, ui->spinBox->value());
    });
    connect(ui->action812, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(812, ui->spinBox->value());
    });
    connect(ui->action813, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(813, ui->spinBox->value());
    });
    connect(ui->action814, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(814, ui->spinBox->value());
    });
    connect(ui->action815, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(815, ui->spinBox->value());
    });
    connect(ui->action816, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(816, ui->spinBox->value());
    });
    connect(ui->action817, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(817, ui->spinBox->value());
    });
    connect(ui->action818, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(818, ui->spinBox->value());
    });
    connect(ui->action819, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(819, ui->spinBox->value());
    });
    connect(ui->action820, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(820, ui->spinBox->value());
    });
    connect(ui->action821, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(821, ui->spinBox->value());
    });
    connect(ui->action822, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(822, ui->spinBox->value());
    });
    connect(ui->action823, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(823, ui->spinBox->value());
    });
    connect(ui->action824, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(824, ui->spinBox->value());
    });
    connect(ui->action825, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(825, ui->spinBox->value());
    });
    connect(ui->action826, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(826, ui->spinBox->value());
    });
    connect(ui->action827, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(827, ui->spinBox->value());
    });
    connect(ui->action828, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(828, ui->spinBox->value());
    });
    connect(ui->action829, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(829, ui->spinBox->value());
    });
    connect(ui->action830, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(830, ui->spinBox->value());
    });
    connect(ui->action831, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(831, ui->spinBox->value());
    });
    connect(ui->action832, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(832, ui->spinBox->value());
    });
    connect(ui->action833, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(833, ui->spinBox->value());
    });
    connect(ui->action834, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(834, ui->spinBox->value());
    });
    connect(ui->action835, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(835, ui->spinBox->value());
    });
    connect(ui->action836, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(836, ui->spinBox->value());
    });
    connect(ui->action837, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(837, ui->spinBox->value());
    });
    connect(ui->action838, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(838, ui->spinBox->value());
    });
    connect(ui->action839, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(839, ui->spinBox->value());
    });
    connect(ui->action840, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(840, ui->spinBox->value());
    });
    connect(ui->action841, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(841, ui->spinBox->value());
    });
    connect(ui->action842, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(842, ui->spinBox->value());
    });
    connect(ui->action843, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(843, ui->spinBox->value());
    });
    connect(ui->action844, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(844, ui->spinBox->value());
    });
    connect(ui->action845, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(845, ui->spinBox->value());
    });
    connect(ui->action846, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(846, ui->spinBox->value());
    });
    connect(ui->action847, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(847, ui->spinBox->value());
    });
    connect(ui->action848, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(848, ui->spinBox->value());
    });
    connect(ui->action849, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(849, ui->spinBox->value());
    });
    connect(ui->action850, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(850, ui->spinBox->value());
    });
    connect(ui->action851, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(851, ui->spinBox->value());
    });
    connect(ui->action852, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(852, ui->spinBox->value());
    });
    connect(ui->action853, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(853, ui->spinBox->value());
    });
    connect(ui->action854, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(854, ui->spinBox->value());
    });
    connect(ui->action855, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(855, ui->spinBox->value());
    });
    connect(ui->action856, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(856, ui->spinBox->value());
    });
    connect(ui->action857, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(857, ui->spinBox->value());
    });
    connect(ui->action858, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(858, ui->spinBox->value());
    });
    connect(ui->action859, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(859, ui->spinBox->value());
    });
    connect(ui->action861, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(861, ui->spinBox->value());
    });
    connect(ui->action862, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(862, ui->spinBox->value());
    });
    connect(ui->action863, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(863, ui->spinBox->value());
    });
    connect(ui->action864, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(864, ui->spinBox->value());
    });
    connect(ui->action865, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(865, ui->spinBox->value());
    });
    connect(ui->action866, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(866, ui->spinBox->value());
    });
    connect(ui->action867, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(867, ui->spinBox->value());
    });
    connect(ui->action868, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(868, ui->spinBox->value());
    });
    connect(ui->action869, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(869, ui->spinBox->value());
    });
    connect(ui->action870, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(870, ui->spinBox->value());
    });
    connect(ui->action871, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(871, ui->spinBox->value());
    });
    connect(ui->action872, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(872, ui->spinBox->value());
    });
    connect(ui->action873, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(873, ui->spinBox->value());
    });
    connect(ui->action874, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(874, ui->spinBox->value());
    });
    connect(ui->action875, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(875, ui->spinBox->value());
    });
    connect(ui->action876, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(876, ui->spinBox->value());
    });
    connect(ui->action877, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(877, ui->spinBox->value());
    });
    connect(ui->action878, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(878, ui->spinBox->value());
    });
    connect(ui->action879, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(879, ui->spinBox->value());
    });
    connect(ui->action880, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(880, ui->spinBox->value());
    });
    connect(ui->action881, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(881, ui->spinBox->value());
    });
    connect(ui->action882, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(882, ui->spinBox->value());
    });
    connect(ui->action883, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(883, ui->spinBox->value());
    });
    connect(ui->action884, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(884, ui->spinBox->value());
    });
    connect(ui->action885, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(885, ui->spinBox->value());
    });
    connect(ui->action886, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(886, ui->spinBox->value());
    });
    connect(ui->action887, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(887, ui->spinBox->value());
    });
    connect(ui->action888, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(888, ui->spinBox->value());
    });
    connect(ui->action889, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(889, ui->spinBox->value());
    });
    connect(ui->action890, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(890, ui->spinBox->value());
    });
    connect(ui->action891, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(891, ui->spinBox->value());
    });
    connect(ui->action892, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(892, ui->spinBox->value());
    });
    connect(ui->action893, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(893, ui->spinBox->value());
    });
    connect(ui->action894, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(894, ui->spinBox->value());
    });
    connect(ui->action895, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(895, ui->spinBox->value());
    });
    connect(ui->action896, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(896, ui->spinBox->value());
    });
    connect(ui->action897, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(897, ui->spinBox->value());
    });
    connect(ui->action898, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(898, ui->spinBox->value());
    });
    connect(ui->action899, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(899, ui->spinBox->value());
    });
    connect(ui->action900, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(900, ui->spinBox->value());
    });
    connect(ui->action901, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(901, ui->spinBox->value());
    });
    connect(ui->action902, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(902, ui->spinBox->value());
    });
    connect(ui->action903, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(903, ui->spinBox->value());
    });
    connect(ui->action904, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(904, ui->spinBox->value());
    });
    connect(ui->action905, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(905, ui->spinBox->value());
    });
    connect(ui->action906, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(906, ui->spinBox->value());
    });
    connect(ui->action907, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(907, ui->spinBox->value());
    });
    connect(ui->action908, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(908, ui->spinBox->value());
    });
    connect(ui->action909, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(909, ui->spinBox->value());
    });
    connect(ui->action910, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(910, ui->spinBox->value());
    });
    connect(ui->action911, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(911, ui->spinBox->value());
    });
    connect(ui->action912, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(912, ui->spinBox->value());
    });
    connect(ui->action913, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(913, ui->spinBox->value());
    });
    connect(ui->action914, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(914, ui->spinBox->value());
    });
    connect(ui->action915, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(915, ui->spinBox->value());
    });
    connect(ui->action916, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(916, ui->spinBox->value());
    });
    connect(ui->action917, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(917, ui->spinBox->value());
    });
    connect(ui->action918, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(918, ui->spinBox->value());
    });
    connect(ui->action919, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(919, ui->spinBox->value());
    });
    connect(ui->action920, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(920, ui->spinBox->value());
    });
    connect(ui->action921, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(921, ui->spinBox->value());
    });
    connect(ui->action922, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(922, ui->spinBox->value());
    });
    connect(ui->action923, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(923, ui->spinBox->value());
    });
    connect(ui->action924, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(924, ui->spinBox->value());
    });
    connect(ui->action925, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(925, ui->spinBox->value());
    });
    connect(ui->action926, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(926, ui->spinBox->value());
    });
    connect(ui->action927, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(927, ui->spinBox->value());
    });
    connect(ui->action928, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(928, ui->spinBox->value());
    });
    connect(ui->action929, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(929, ui->spinBox->value());
    });
    connect(ui->action930, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(930, ui->spinBox->value());
    });
    connect(ui->action931, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(931, ui->spinBox->value());
    });
    connect(ui->action932, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(932, ui->spinBox->value());
    });
    connect(ui->action933, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(933, ui->spinBox->value());
    });
    connect(ui->action934, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(934, ui->spinBox->value());
    });
    connect(ui->action935, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(935, ui->spinBox->value());
    });
    connect(ui->action936, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(936, ui->spinBox->value());
    });
    connect(ui->action937, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(937, ui->spinBox->value());
    });
    connect(ui->action938, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(938, ui->spinBox->value());
    });
    connect(ui->action939, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(939, ui->spinBox->value());
    });
    connect(ui->action940, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(940, ui->spinBox->value());
    });
    connect(ui->action941, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(941, ui->spinBox->value());
    });
    connect(ui->action942, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(942, ui->spinBox->value());
    });
    connect(ui->action943, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(943, ui->spinBox->value());
    });
    connect(ui->action944, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(944, ui->spinBox->value());
    });
    connect(ui->action945, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(945, ui->spinBox->value());
    });
    connect(ui->action946, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(946, ui->spinBox->value());
    });
    connect(ui->action947, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(947, ui->spinBox->value());
    });
    connect(ui->action948, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(948, ui->spinBox->value());
    });
    connect(ui->action949, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(949, ui->spinBox->value());
    });
    connect(ui->action950, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(950, ui->spinBox->value());
    });
    connect(ui->action951, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(951, ui->spinBox->value());
    });
    connect(ui->action952, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(952, ui->spinBox->value());
    });
    connect(ui->action953, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(953, ui->spinBox->value());
    });
    connect(ui->action954, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(954, ui->spinBox->value());
    });
    connect(ui->action955, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(955, ui->spinBox->value());
    });
    connect(ui->action956, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(956, ui->spinBox->value());
    });
    connect(ui->action957, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(957, ui->spinBox->value());
    });
    connect(ui->action958, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(958, ui->spinBox->value());
    });
    connect(ui->action959, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(959, ui->spinBox->value());
    });
    connect(ui->action960, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(960, ui->spinBox->value());
    });
    connect(ui->action961, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(961, ui->spinBox->value());
    });
    connect(ui->action962, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(962, ui->spinBox->value());
    });
    connect(ui->action963, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(963, ui->spinBox->value());
    });
    connect(ui->action964, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(964, ui->spinBox->value());
    });
    connect(ui->action965, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(965, ui->spinBox->value());
    });
    connect(ui->action966, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(966, ui->spinBox->value());
    });
    connect(ui->action967, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(967, ui->spinBox->value());
    });
    connect(ui->action968, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(968, ui->spinBox->value());
    });
    connect(ui->action969, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(969, ui->spinBox->value());
    });
    connect(ui->action970, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(970, ui->spinBox->value());
    });
    connect(ui->action971, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(971, ui->spinBox->value());
    });
    connect(ui->action972, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(972, ui->spinBox->value());
    });
    connect(ui->action973, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(973, ui->spinBox->value());
    });
    connect(ui->action974, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(974, ui->spinBox->value());
    });
    connect(ui->action975, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(975, ui->spinBox->value());
    });
    connect(ui->action976, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(976, ui->spinBox->value());
    });
    connect(ui->action977, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(977, ui->spinBox->value());
    });
    connect(ui->action978, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(978, ui->spinBox->value());
    });
    connect(ui->action979, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(979, ui->spinBox->value());
    });
    connect(ui->action980, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(980, ui->spinBox->value());
    });
    connect(ui->action981, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(981, ui->spinBox->value());
    });
    connect(ui->action982, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(982, ui->spinBox->value());
    });
    connect(ui->action983, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(983, ui->spinBox->value());
    });
    connect(ui->action984, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(984, ui->spinBox->value());
    });
    connect(ui->action985, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(985, ui->spinBox->value());
    });
    connect(ui->action986, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(986, ui->spinBox->value());
    });
    connect(ui->action987, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(987, ui->spinBox->value());
    });
    connect(ui->action988, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(988, ui->spinBox->value());
    });
    connect(ui->action989, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(989, ui->spinBox->value());
    });
    connect(ui->action990, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(990, ui->spinBox->value());
    });
    connect(ui->action991, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(991, ui->spinBox->value());
    });
    connect(ui->action992, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(992, ui->spinBox->value());
    });
    connect(ui->action993, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(993, ui->spinBox->value());
    });
    connect(ui->action994, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(994, ui->spinBox->value());
    });
    connect(ui->action995, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(995, ui->spinBox->value());
    });
    connect(ui->action996, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(996, ui->spinBox->value());
    });
    connect(ui->action997, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(997, ui->spinBox->value());
    });
    connect(ui->action998, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(998, ui->spinBox->value());
    });
    connect(ui->action999, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(999, ui->spinBox->value());
    });
    connect(ui->action1000, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1000, ui->spinBox->value());
    });
    connect(ui->action1001, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1001, ui->spinBox->value());
    });
    connect(ui->action1002, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1002, ui->spinBox->value());
    });
    connect(ui->action1003, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1003, ui->spinBox->value());
    });
    connect(ui->action1004, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1004, ui->spinBox->value());
    });
    connect(ui->action1005, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1005, ui->spinBox->value());
    });
    connect(ui->action1006, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1006, ui->spinBox->value());
    });
    connect(ui->action1007, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1007, ui->spinBox->value());
    });
    connect(ui->action1008, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1008, ui->spinBox->value());
    });
    connect(ui->action1009, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1009, ui->spinBox->value());
    });
    connect(ui->action1010, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1010, ui->spinBox->value());
    });
    connect(ui->action1011, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1011, ui->spinBox->value());
    });
    connect(ui->action1012, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1012, ui->spinBox->value());
    });
    connect(ui->action1013, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1013, ui->spinBox->value());
    });
    connect(ui->action1014, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1014, ui->spinBox->value());
    });
    connect(ui->action1015, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1015, ui->spinBox->value());
    });
    connect(ui->action1016, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1016, ui->spinBox->value());
    });
    connect(ui->action1017, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1017, ui->spinBox->value());
    });
    connect(ui->action1018, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1018, ui->spinBox->value());
    });
    connect(ui->action1019, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1019, ui->spinBox->value());
    });
    connect(ui->action1020, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1020, ui->spinBox->value());
    });
    connect(ui->action1021, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1021, ui->spinBox->value());
    });
    connect(ui->action1022, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1022, ui->spinBox->value());
    });
    connect(ui->action1023, &QAction::triggered, this, [=](){
        this->mychannleAmp.insert(1023, ui->spinBox->value());
    });
    /***************Send stimulus instructions to the FPGA and clear the inserted value in QMap************/
    connect(ui->pushButton_4, &QPushButton::clicked, this, [=](){
             QTime time = QTime::currentTime();
             qDebug()<<time;
             serial->write(multistim->multicmd, (multistim->charNum)+1);
             tcpsocket->write(multistim->multicmd, (multistim->charNum)+1);
             this->mychannleAmp.clear();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::serialPort_readyRead()
{

    if(ui->checkBox->checkState() == Qt::Checked)
    {
        QByteArray temp = serial->readAll();
        QDataStream out(&temp,QIODevice::ReadWrite);//用QDataStream将字节数组读入
        while(!out.atEnd())
        {
            qint8 outChar = 0;
            out>>outChar;//每一字节填充一次，直到结束
            QString str = QString("%1 ").arg(outChar&0xFF, 2, 16, QLatin1Char('0'));//在这里转十六进制数
            ui->textEdit_2->insertPlainText(str);
        }

    }
    else
    {
        QString last_text;
        last_text = ui->textEdit_2->toPlainText();
        Receivetext = serial->readAll();
        Receivetext = Receivetext.toLatin1();
        last_text = last_text.append(Receivetext);
        ui->textEdit_2->setText(last_text);
    }
}
void MainWindow::on_pushButton_clicked()
{
    ui->comboBox->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text() == QString("Open SerialPort"))
    {
        serial->setPortName(ui->comboBox->currentText());
        serial->setBaudRate(115200);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if(!serial->open(QIODevice::ReadWrite))
        {
            QMessageBox::information(this,"error information","can't open serialPort",QMessageBox::Ok);
            return;
        }
        ui->comboBox->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setText("close SerialPort");
    }
    else
    {
        serial->close();
        ui->comboBox->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setText("Open SerialPort");
    }
}

void MainWindow::on_pushButton_3_clicked()  //serial port send data to FPGA
{
    if(ui->pushButton_2->text() == "Open SerialPort")
    {
        QMessageBox::information(this, "error information","please Open SerialPort", QMessageBox::Ok);
        return;
    }
    if(ui->checkBox_3->checkState() == Qt::Checked)
    {
        QByteArray senddata = ui->textEdit->toPlainText().toUtf8();
        int cnt = senddata.size();
        char *data = senddata.data();
        for(int i = 0; i < cnt; i++)
        {
            if(data[i] >= '0' && (data[i] <= '9'))
                continue;
            else if(data[i] >= 'a' && (data[i] <= 'f'))
                continue;
            else if(data[i] >= 'A' && (data[i] <= 'F'))
                continue;
            else if(data[i] == ' ')
                continue;
            else
            {
                QMessageBox::warning(NULL,"INFORMATION", "INPUT NON HEX");
                return;
            }

        }
        senddata = senddata.fromHex(senddata);
        serial->write(senddata);
    }
    else
    {
        QByteArray bytearray;
        Sendtext = ui->textEdit->toPlainText();
        bytearray = Sendtext.toLatin1();
        serial->write(bytearray);
    }

}

void MainWindow::on_checkBox_clicked()
{
    ui->checkBox->setCheckState(Qt::Checked);
    ui->checkBox_2->setCheckState(Qt::Unchecked);
}

void MainWindow::on_checkBox_2_clicked()
{
    ui->checkBox->setCheckState(Qt::Unchecked);
    ui->checkBox_2->setCheckState(Qt::Checked);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->textEdit_2->clear();
}


void MainWindow::on_checkBox_3_clicked()
{
    ui->checkBox_3->setCheckState(Qt::Checked);
    ui->checkBox_4->setCheckState(Qt::Unchecked);
}


void MainWindow::on_checkBox_4_clicked()
{
    ui->checkBox_3->setCheckState(Qt::Unchecked);
    ui->checkBox_4->setCheckState(Qt::Checked);
}

