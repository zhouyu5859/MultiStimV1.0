#include "mutichannelstim.h"

mutiChannelStim::mutiChannelStim(QObject *parent) : QObject(parent)
{
    this->stimMap.insert(NEUROEYES_PARTTERN1, {100, 8300, 20});
    this->stimMap.insert(NEUROEYES_PARTTERN2, {100, 5000, 20});
    this->stimMap.insert(NEUROEYES_PARTTERN3, {100, 3300, 20});
    this->stimMap.insert(NEUROEYES_PARTTERN4, {100, 8300, 50});
    this->stimMap.insert(NEUROEYES_PARTTERN5, {100, 8300, 50});
    this->stimMap.insert(NEUROEYES_PARTTERN6, {100, 3300, 50});
    this->stimMap.insert(ECOG_PARTTERN1, {100, 16600, 10});
    this->stimMap.insert(ECOG_PARTTERN2, {100, 8300, 10});
    this->stimMap.insert(ECOG_PARTTERN3, {100, 5000, 10});
    this->stimMap.insert(ECOG_PARTTERN4, {100, 3300, 10});
    this->stimMap.insert(ECOG_PARTTERN5, {100, 8300, 20});
    this->stimMap.insert(ECOG_PARTTERN6, {100, 5000, 20});
    this->stimMap.insert(ECOG_PARTTERN7, {100, 3300, 20});
    this->stimMap.insert(ECOG_PARTTERN8, {100, 8300, 50});
    this->stimMap.insert(ECOG_PARTTERN9, {100, 5000, 50});
    this->stimMap.insert(ECOG_PARTTERN10, {100, 3300, 50});
    this->stimMap.insert(TEST_PARTTERN, {1000, 1000, 100}); //{1000, 2000, 5}脉宽10*100us,刺激间隔为（2000/1000=2）倍的脉宽，刺激（正负一对刺激算一个）数量为5个
}

void mutiChannelStim::multichannelStim(QMap<unsigned short, unsigned short> chAmp, STIMMODULE module)
{
    unsigned short n_multiwidth = 0;
    unsigned short n_multiperiod = 0;
    unsigned short n_multipulsenums = 0;
    memset(multicmd, 0, sizeof(multicmd)/sizeof(multicmd[0]));
    multicmd[0] = (char)0xaa;
    QList<unsigned short> chAmpKeys = chAmp.keys();
    unsigned short chNum = static_cast<unsigned short>(chAmpKeys.length());
    for(int i = 0; i < chNum; ++i){
        unsigned short channel = chAmpKeys.at(i);
        QList<unsigned short> valueList = chAmp.values(channel);
        unsigned short amp = ((unsigned short)valueList.last())/10;
        qDebug()<<"********"<<"i:"<<i<<"channel: "<<channel<<"amp:"<<amp;
        multicmd[8+i*4]   = (char)(channel >> 8);
        multicmd[8+i*4+1] = (char)(channel & 0x00ff);
        //multicmd[8+i*4+2] = (char)(amp >> 8);
        multicmd[8+i*4+2] = (char)0x80;
        multicmd[8+i*4+3] = (char)(amp & 0x00ff);
    }
    multicmd[1] = (char)(chNum >> 8);
    multicmd[2] = (char)(chNum & 0x00ff);

    switch(module){
    case NEUROEYES_PARTTERN1:
        n_multiwidth = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN1).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN1).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(NEUROEYES_PARTTERN1).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case NEUROEYES_PARTTERN2:
        n_multiwidth = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN2).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN2).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(NEUROEYES_PARTTERN2).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case NEUROEYES_PARTTERN3:
        n_multiwidth = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN3).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN3).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(NEUROEYES_PARTTERN3).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case NEUROEYES_PARTTERN4:
        n_multiwidth = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN4).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN4).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(NEUROEYES_PARTTERN4).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case NEUROEYES_PARTTERN5:
        n_multiwidth = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN5).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN5).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(NEUROEYES_PARTTERN5).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case NEUROEYES_PARTTERN6:
        n_multiwidth = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN6).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(NEUROEYES_PARTTERN6).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(NEUROEYES_PARTTERN6).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN1:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN1).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN1).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN1).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN2:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN2).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN2).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN2).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN3:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN3).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN3).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN3).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN4:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN4).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN4).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN4).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN5:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN5).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN5).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN5).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN6:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN6).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN6).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN6).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN7:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN7).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN7).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN7).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN8:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN8).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN8).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN8).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN9:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN9).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN9).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN9).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case ECOG_PARTTERN10:
        n_multiwidth = (unsigned short)((this->stimMap.value(ECOG_PARTTERN10).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(ECOG_PARTTERN10).period)/100);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(ECOG_PARTTERN10).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    break;
    case TEST_PARTTERN:
        n_multiwidth = (unsigned short)((this->stimMap.value(TEST_PARTTERN).width)/100);
        multicmd[3] = (char)(n_multiwidth >> 8);
        multicmd[4] = (char)(n_multiwidth & 0x00ff);
        n_multiperiod = (unsigned short)((this->stimMap.value(TEST_PARTTERN).period)/1000);
        multicmd[5] = (char)(n_multiperiod);
        n_multipulsenums = (unsigned short)(this->stimMap.value(TEST_PARTTERN).pulsenum);
        multicmd[6] = (char)(n_multipulsenums >> 8);
        multicmd[7] = (char)(n_multipulsenums & 0x00ff);
    default:    break;
    }
    charNum = 4 * chNum + 8;
    //int charNum = 4 * chNum + 8;
    unsigned char check = 0x00;
    for(int i = 0; i < charNum; ++i){
        check ^= multicmd[i];
    }
    multicmd[charNum] = check;

    showBits(multicmd, charNum+1);
}

void mutiChannelStim::showBits(char *p, int len)
{
    for(int i = 0; i < len; ++i){
        for(int j = 0; j < 8; ++j){
            char num = p[i];
            std::cout<<(bool)((num<<j)&0x80);
        }
        std::cout<<" ";
    }
    std::cout<<std::endl;
}
