#include "AudioPassThru.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "UI/Config/Config.h"

extern Config g_config;
CAudioPassThru::CAudioPassThru(CPopBase *parent) : CPopBase(parent)
{
    InitLayout();
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(hide()));
    m_i_performaudiopassthruID = 0;
}

CAudioPassThru::~CAudioPassThru()
{

}

void CAudioPassThru::InitLayout()
{
//    this->setFixedSize(550 * g_config.m_i_mainWindowW / 630, 265 * g_config.m_i_mainWindowH / 378);

    int iW = 550 * g_config.m_i_mainWindowW / 630;
    int iH = 265 * g_config.m_i_mainWindowH / 378;

    // Initlialize instance.
    m_labelBackground.setParent(this);
    m_labelFrame.setParent(this);
    m_labelAppName = new QLabel;
    m_labelText1 = new QLabel;
    m_labelText2 = new QLabel;
    m_labelVRIcon = new QLabel;
    m_btn1 = new CButton;
    m_btn2 = new CButton;
    m_btn3 = new CButton;
    m_btn4 = new CButton;

    // Connect button signal to solts.
    connect(m_btn1, SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));
    connect(m_btn2, SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));
    connect(m_btn3, SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));
    connect(m_btn4, SIGNAL(clicked(int)), this, SLOT(onButtonClickedSlots(int)));

    m_labelBackground.setGeometry((this->width() - iW) / 2, 0.85 * this->height() - iH, iW, iH);
    m_labelFrame.setGeometry((this->width() - iW) / 2, 0.85 * this->height() - iH, iW, iH);

    m_labelAppName->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:#EFEF33;qproperty-alignment:AlignCenter;");
    m_labelText1->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:white;qproperty-alignment:AlignCenter;");
    m_labelText2->setStyleSheet("color:white;font: 45px \"Liberation Serif\";border-radius:8px;border-bottom:1px solid gray;background-color: #121122;"); //qproperty-alignment:AlignLeft;");
    m_labelText2->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_labelVRIcon->setStyleSheet("color:white;font: 45px Liberation Serif;border-image: url(:/images/record_transparent.png);");

    m_labelAppName->setText("New Application");


    // Set button parameter. Like size, normal background image, text, etc.
    m_btn1->initParameter(122, 60, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png", "", "-");
    m_btn2->initParameter(122, 60, ":/images/softbutton_alert_left.png", ":/images/softbutton_alert_left.png", "", "-");
    m_btn3->initParameter(122, 60, ":/images/softbutton_alert_right.png", ":/images/softbutton_alert_right.png", "", "-");
    m_btn4->initParameter(122, 60, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png", "", "-");

    m_btn1->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn2->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn3->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btn4->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");

    // Set layout.
    QVBoxLayout *mLayout = new QVBoxLayout;
    QVBoxLayout *textLayout = new QVBoxLayout;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;
    QHBoxLayout *bottomLayout = new QHBoxLayout;

    leftLayout->addStretch(73);
    leftLayout->addWidget(m_labelVRIcon, 15);
    leftLayout->addStretch(12);
    leftLayout->setMargin(0);

    textLayout->addStretch(5);
    textLayout->addWidget(m_labelAppName, 25);
    textLayout->addStretch(5);
    textLayout->addWidget(m_labelText1, 25);
    textLayout->addStretch(5);
    textLayout->addWidget(m_labelText2, 25);
    textLayout->addStretch(10);

    topLayout->addStretch(3);
    topLayout->addLayout(leftLayout, 6);
    topLayout->addStretch(3);
    topLayout->addLayout(textLayout, 76);
    topLayout->addStretch(12);
    topLayout->setMargin(0);

    bottomLayout->addWidget(m_btn1);
    bottomLayout->addWidget(m_btn2);
    bottomLayout->addWidget(m_btn3);
    bottomLayout->addWidget(m_btn4);

    mLayout->addLayout(topLayout, 65);
    mLayout->addLayout(bottomLayout, 30);
    mLayout->addStretch(5);
    mLayout->setMargin(0);

    m_labelFrame.setLayout(mLayout);

    setBtnText(0,"Retry");
    setBtnText(1,"-");
    setBtnText(2,"Done");
    setBtnText(3,"Cancel");

//    setBtnID(0,PERFORMAUDIOPASSTHRU_RETYP);
//    setBtnID(1,-1);
//    setBtnID(2,PERFORMAUDIOPASSTHRU_DONE);
//    setBtnID(3,PERFORMAUDIOPASSTHRU_CANCEL);
    m_btn1->setId(PERFORMAUDIOPASSTHRU_RETYP);
    m_btn2->setId(-1);
    m_btn3->setId(PERFORMAUDIOPASSTHRU_DONE);
    m_btn4->setId(PERFORMAUDIOPASSTHRU_CANCEL);

}

void CAudioPassThru::setBtnText(int btnIdx, QString text)
{
    switch (btnIdx)
    {
    case 0:
        m_btn1->setText(text);
        break;
    case 1:
        m_btn2->setText(text);
        break;
    case 2:
        m_btn3->setText(text);
        break;
    case 3:
        m_btn4->setText(text);
        break;
    }
}

void CAudioPassThru::setAudioText(int textIdx, QString text)
{
    switch (textIdx)
    {
    case 0:
        m_labelAppName->setText(text);
        break;
    case 1:
        m_labelText1->setText(text);
        break;
    case 2:
        m_labelText2->setText(text);
        break;
    }
}

void CAudioPassThru::setTimeOut(int duration)
{
    m_timer->start(duration);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutSlots()));
}

void CAudioPassThru::timeoutSlots()
{
    m_timer->stop();
    if(m_i_performaudiopassthruID != 0)
        audioPassThruHide(m_i_performaudiopassthruID, PERFORMAUDIOPASSTHRU_TIMEOUT);
    this->hide();
}

void CAudioPassThru::setAudioPassThruID(int id)
{
    m_i_performaudiopassthruID = id;
}

void CAudioPassThru::setAudioPassThruDisplayText1(QString text)
{
    m_labelText1->setText(text);
}

void CAudioPassThru::setAudioPassThruDisplayText2(QString text)
{
    m_labelText2->setText(text);
}

void CAudioPassThru::onButtonClickedSlots(int id)
{
    if(id >= 0)
    {
        m_timer->stop();
        if(m_i_performaudiopassthruID != 0)
            emit audioPassThruHide(m_i_performaudiopassthruID, id);
        this->hide();
    }
}

void CAudioPassThru::execShow(AppDataInterface* pAppInterface)
{
    if (pAppInterface)
    {
        m_jsonData = pAppInterface->getAudioPassThruJson();
        if(!m_jsonData.isMember("id") || !m_jsonData.isMember("params"))
        {
            this->show();
            return;
        }
        this->setAudioPassThruID(m_jsonData["id"].asInt());
        this->setTimeOut(m_jsonData["params"]["maxDuration"].asInt());


        for (int i = 0; i < m_jsonData["params"]["audioPassThruDisplayTexts"].size(); i++)
        {
            if ("audioPassThruDisplayText1" == m_jsonData["params"]["audioPassThruDisplayTexts"][i]["fieldName"].asString())
            {
                this->setAudioPassThruDisplayText1(m_jsonData["params"]["audioPassThruDisplayTexts"][i]["fieldText"].asString().c_str());
            }
            else if ("audioPassThruDisplayText2" == m_jsonData["params"]["audioPassThruDisplayTexts"][i]["fieldName"].asString())
            {
                this->setAudioPassThruDisplayText2(m_jsonData["params"]["audioPassThruDisplayTexts"][i]["fieldText"].asString().c_str());
            }
        }
    }
    this->show();
}
