#ifndef MUTICHANNELSTIM_H
#define MUTICHANNELSTIM_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QDebug>
#include <iostream>

//design StimPatterns using enum types
typedef enum STIMMODULE{
    NEUROEYES_PARTTERN1 = 0x01,
    NEUROEYES_PARTTERN2 = 0x02,
    NEUROEYES_PARTTERN3 = 0x03,
    NEUROEYES_PARTTERN4 = 0x04,
    NEUROEYES_PARTTERN5 = 0x05,
    NEUROEYES_PARTTERN6 = 0x06,
    ECOG_PARTTERN1 = 0X07,
    ECOG_PARTTERN2 = 0X08,
    ECOG_PARTTERN3 = 0X09,
    ECOG_PARTTERN4 = 0X0a,
    ECOG_PARTTERN5 = 0X0b,
    ECOG_PARTTERN6 = 0X0c,
    ECOG_PARTTERN7 = 0X0d,
    ECOG_PARTTERN8 = 0X0e,
    ECOG_PARTTERN9 = 0X0f,
    ECOG_PARTTERN10 = 0X10,
    TEST_PARTTERN = 0X11,
}STIMMODULE;

typedef struct STIM3PARAM{
    unsigned int width;
    unsigned int period;
    unsigned int pulsenum;
}STIM3PARAM;

class mutiChannelStim : public QObject
{
    Q_OBJECT
public:
    explicit mutiChannelStim(QObject *parent = nullptr);

    char multicmd[1000000];
    int charNum;
    void multichannelStim(QMap<unsigned short, unsigned short> chAmp, STIMMODULE module);

private:
    QMap<STIMMODULE, STIM3PARAM> stimMap;
    QMap<unsigned short, unsigned short> channleAmp;
    void showBits(char *p, int len);

signals:

};

#endif // MUTICHANNELSTIM_H
