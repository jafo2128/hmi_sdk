﻿#include "Slider.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "UI/Config/Config.h"
#include "Common/PopBase.h"

Slider::Slider(AppListInterface * pList, QWidget *parent) :
  CPopBase(pList, parent),
    m_iPos(0)
{
    InitLayout();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeoutSlots()));
    connect(this,SIGNAL(sliderClicked(int,int,int)),this,SLOT(sliderClickedSlots(int,int,int)));
    connect(this, SIGNAL(onSpaceCliced()), this, SLOT(onButtonCancelClicked()));
}

void Slider::InitLayout()
{
    int iW = ui_app_width;
    int iH = ui_app_height;

//    m_labelBackground.setParent(this);
//    m_labelFrame.setParent(this);

    m_labelText1 = new QLabel;
    m_labelText2 = new QLabel;
    m_labelText3 = new QLabel;
    m_btnSoft1 = new CButton;
    m_btnSoft2 = new CButton;
    m_btnSoft3 = new CButton;
    m_btnSoft4 = new CButton;

    connect(m_btnSoft1, SIGNAL(clicked()), this, SLOT(onButtonSaveClicked()));
    connect(m_btnSoft2, SIGNAL(clicked()), this, SLOT(onMoveLeftSlot()));
    connect(m_btnSoft3, SIGNAL(clicked()), this, SLOT(onMoveRightSlot()));
    connect(m_btnSoft4, SIGNAL(clicked()), this, SLOT(onButtonCancelClicked()));

//    m_labelBackground.setGeometry((this->width() - iW) / 2, 0.85 * this->height() - iH, iW, iH);
//    m_labelFrame.setGeometry((this->width() - iW) / 2, 0.85 * this->height() - iH, iW, iH);

    m_labelText1->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText2->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText3->setStyleSheet("border:0px;font: 45px \"Liberation Serif\";color:rgb(255,255,254)");
    m_labelText1->setAlignment(Qt::AlignCenter);
    m_labelText2->setAlignment(Qt::AlignCenter);
    m_labelText3->setAlignment(Qt::AlignCenter);

//    //test
//    QString tmp[5] = {"Hello", "Jack", "Moky", "Luckey", "Mono"};
//    setSliderStrings(tmp, 5, 1);
//    setSliderTitle("Test string.");
//    setPosition(3);

    m_btnSoft1->initParameter(ui_aler_width, ui_aler_height, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png", "", "Close");
    m_btnSoft2->initParameter(ui_aler_width, ui_aler_height, ":/images/softbutton_alert_left.png", ":/images/softbutton_alert_left.png", "", "");
    m_btnSoft2->setIconExtra(":/images/leftarrow.png");
    m_btnSoft3->initParameter(ui_aler_width, ui_aler_height, ":/images/softbutton_alert_right.png", ":/images/softbutton_alert_right.png", "", "");
    m_btnSoft3->setIconExtra(":/images/rightarrow.png");
    m_btnSoft4->initParameter(ui_aler_width, ui_aler_height, ":/images/softbutton_alert.png", ":/images/softbutton_alert.png", "", "Cancel");

    m_btnSoft1->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft2->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft3->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");
    m_btnSoft4->setTextStyle("border:0px;font: 42px \"Liberation Serif\";color:rgb(255,255,254)");

    QHBoxLayout *btnLayout = new QHBoxLayout;
    QVBoxLayout *mLayout = new QVBoxLayout;

    btnLayout->addWidget(m_btnSoft1, 25);
    btnLayout->addWidget(m_btnSoft2, 25);
    btnLayout->addWidget(m_btnSoft3, 25);
    btnLayout->addWidget(m_btnSoft4, 25);

    mLayout->addWidget(m_labelText1, 24);
    mLayout->addWidget(m_labelText2, 24);
    mLayout->addWidget(m_labelText3, 24);
    mLayout->addLayout(btnLayout, 23);
    mLayout->addStretch(5);
    mLayout->setMargin(0);

//    m_labelFrame.setLayout(mLayout);
    this->setLayout(mLayout);
    m_btnSoft1->setText("Save");
}

void Slider::sliderClickedSlots( int code, int sliderid, int sliderPosition)
{
    //_D("code=%d:%d:%d\n",code,sliderid,sliderPosition);
    m_pList->getActiveApp()->OnSliderResponse(code, sliderid,sliderPosition);
    this->showCurUI(ID_SHOW);
}

void Slider::onMoveLeftSlot()
{
    if (m_iPos > 0)
    {
        m_iPos--;
        updateScreen();
    }
}

void Slider::onMoveRightSlot()
{
    if (m_iPos < m_Strings.size() - 1)
    {
        m_iPos++;
        updateScreen();
    }
}

void Slider::setSliderTitle(QString text)
{
    m_labelText1->setText(text);
}

//void Slider::setSliderStrings(QString* pStr, int iCnt, int iPos)
void Slider::setSliderStrings(std::vector<std::string> vec_strSliter, int iPos)
{
    m_Strings.clear();
    for (int i = 0; i < vec_strSliter.size(); i++)
    {
        m_Strings.push_back(vec_strSliter.at(i).data());
    }

    m_iPos = iPos-1;

    updateScreen();
}

bool Slider::setPosition(int iPos)
{
    if (iPos < 0 || iPos >= m_Strings.size())
    {
        return false;
    }

    m_iPos = iPos;
    updateScreen();

    return true;
}

void Slider::updateScreen()
{
    m_timer->stop();
    m_timer->start();

    m_EditText.clear();
    for (int i = 0; i < m_Strings.size(); i++)
    {
        if (i == m_iPos)
        {
            m_EditText.push_back("|");
        }
        else
        {
            m_EditText.push_back(".");
        }
    }
    m_labelText2->setText("<" + m_EditText + ">");
    m_labelText3->setText(m_Strings[m_iPos]);
}

void Slider::setSliderID(int id)
{
    m_i_sliderID = id;
}

void Slider::setTimeOut(int duration)
{
    m_timer->setInterval(duration);
    m_timer->start();

}

void Slider::timeoutSlots()
{
    m_timer->stop();
    emit sliderClicked(SLIDER_TIMEOUT, m_i_sliderID, m_iPos);
    this->hide();
}

void Slider::onButtonSaveClicked()
{
    m_timer->stop();
    emit sliderClicked(SLIDER_OK, m_i_sliderID, m_iPos);
    this->hide();
}
void Slider::onButtonCancelClicked()
{
    m_timer->stop();
    emit sliderClicked(SLIDER_ABORTED, m_i_sliderID, m_iPos);
    this->hide();
}

void Slider::execShow()
{
    if (m_pList->getActiveApp())
    {
        m_jsonData = m_pList->getActiveApp()->getSlider();
//        this->setAppID(m_jsonData["params"]["appID"].asInt());
        this->setSliderID(m_jsonData["id"].asInt());
        this->setTimeOut(m_jsonData["params"]["timeout"].asInt());

        int numTicks = m_jsonData["params"]["numTicks"].asInt();
        int position = m_jsonData["params"]["position"].asInt();

        this->setSliderTitle(m_jsonData["params"]["sliderHeader"].asString().data());

        std::vector <std::string > vec_strSliter;
        vec_strSliter.clear();
        if (m_jsonData["params"].isMember("sliderFooter"))
        {
            for(int i = 0; i < m_jsonData["params"]["sliderFooter"].size(); i++)
            {
                vec_strSliter.push_back(m_jsonData["params"]["sliderFooter"][i].asString());
            }
        }
        while(vec_strSliter.size() < numTicks)
        {
            vec_strSliter.push_back("-");
        }

        this->setSliderStrings(vec_strSliter,position);
    }
    this->show();
}
