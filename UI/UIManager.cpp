﻿#include "UIManager.h"
#include "AppData/AppList.h"
#include <pthread.h>
#include <QThread>
#ifdef ANDROID
#include "android/log.h"
#include <unistd.h>
#endif

#include<sys/stat.h>
#include <QDir>
#ifdef SDL_SUPPORT_LIB
#include "main.h"
#include "AudioTrans/MspVRAudio.h"
#endif

CUIManager::CUIManager(AppListInterface * pList, QWidget *parent) :
    QWidget(parent)
{
    m_pAppList = pList;
}

CUIManager::~CUIManager()
{
    delete m_MainMenu;
#ifdef SDL_SUPPORT_LIB
    delete m_MspVR;
#endif
}

void CUIManager::initAppHMI()
{
    m_MainMenu=new MainMenu(m_pAppList);
    m_MainMenu->InserWidget(ID_APPLINK,new AppLink(m_pAppList, m_MainMenu->CenterWidght()));//
    m_MainMenu->InserWidget(ID_ALERT,new CAlertUI(m_pAppList));
    m_MainMenu->InserWidget(ID_AUDIOPASSTHRU,new CAudioPassThru(m_pAppList));
    m_MainMenu->InserWidget(ID_CHOICESETVR,new CChoicesetVR(m_pAppList));
    m_MainMenu->InserWidget(ID_CHOICESET,new Choiceset(m_pAppList, m_MainMenu->CenterWidght()));//
    m_MainMenu->InserWidget(ID_COMMAND,new Command(m_pAppList, m_MainMenu->CenterWidght()));//
    m_MainMenu->InserWidget(ID_SCROLLMSG,new CScrollMsg(m_pAppList));
    m_MainMenu->InserWidget(ID_SHOW,new Show(m_pAppList, m_MainMenu->CenterWidght()));//
    m_MainMenu->InserWidget(ID_NOTIFY,new Notify);
    m_MainMenu->InserWidget(ID_SLIDER,new Slider(m_pAppList));

#ifdef SDL_SUPPORT_LIB
    m_MspVR = new msp_vr_audio;
#endif
//
    connect(this,SIGNAL(onAppShowSignal(int)),this,SLOT(AppShowSlot(int)));
    connect(this,SIGNAL(onAppCloseSignal()),this,SLOT(AppCloseSlot()));
    connect(this,SIGNAL(onAppRefreshSignal()),this,SLOT(AppRefreshSlot()));

    connect(this,SIGNAL(onTestStartSignal()),this,SLOT(onTestStartSlots()));
    connect(this,SIGNAL(onTestStopSignal()),this,SLOT(onTestStopSlots()));

    //emit finishMainHMI();
}

void CUIManager::showMainUI()
{
    m_MainMenu->SetCurWidget(ID_APPLINK);
    m_MainMenu->show();
}


//show app
void CUIManager::onAppShow(int type)
{
    emit onAppShowSignal(type);
}
void CUIManager::onTestVideoStreamStart()
{
    printf("emit onTestVideoStreamStart");
    fflush(stdout);
    emit onTestStartSignal();
}
void CUIManager::onTestStartSlots()
{
    printf("onTestStartSlots");
    fflush(stdout);
    std::string str_url = m_pAppList->getActiveApp()->getUrlString();
    //_D("%s\n",str_url.data());
    m_MainMenu->StartVideoStream(str_url.c_str());
}
void CUIManager::onTestVideoStreamStop()
{
    emit onTestStopSignal();
}
void CUIManager::onTestStopSlots()
{
    m_MainMenu->StopVideoStream();
}

void CUIManager::onAppClose()
{
    emit onAppCloseSignal();
}

void CUIManager::onAppRefresh()
{
    emit onAppRefresh();
}

void CUIManager::AppShowSlot(int type)
{
    m_MainMenu->SetCurWidget(type);
}

void CUIManager::AppCloseSlot()
{

}

void CUIManager::AppRefreshSlot()
{

}

void CUIManager::setMediaColckTimer(Json::Value jsonObj)
{
    //m_pShow->setMediaColckTimer(jsonObj);
    m_MainMenu->ReceiveJson(ID_SHOW,jsonObj);
}

#include <QCoreApplication>
void CUIManager::waitMSec(int ms)
{
    QTime t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void CUIManager::tsSpeak(int VRID, std::string strText)
{
    bool ret=ts.speak(strText.data());
    while(ts.isSpeaking())
        waitMSec(100);

    switch(VRID)
    {
    case ID_DEFAULT:
        if(ret)
            m_pAppList->getActiveApp()->OnTTSSpeek(0);
        else
            m_pAppList->getActiveApp()->OnTTSSpeek(5);
        break;
    case ID_CANCEL:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(0, PERFORMAUDIOPASSTHRU_CANCEL);
        m_pAppList->getActiveApp()->OnVRCancelRecord();
        //m_pAudioPassThru->hide();
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        break;
    case ID_HELP:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(0, PERFORMAUDIOPASSTHRU_DONE);
        m_pAppList->getActiveApp()->OnVRCancelRecord();
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        break;
    case ID_EXIT:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(0, PERFORMAUDIOPASSTHRU_DONE);
        m_pAppList->getActiveApp()->OnVRCancelRecord();
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        m_pAppList->OnAppExit();
        break;
    case ID_SWITCHAPP:
        m_pAppList->getActiveApp()->OnPerformAudioPassThru(0, PERFORMAUDIOPASSTHRU_DONE);
        m_pAppList->getActiveApp()->OnVRCancelRecord();
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        break;
    default:
        m_MainMenu->ExitWidget(ID_AUDIOPASSTHRU);
        break;
    }
}
