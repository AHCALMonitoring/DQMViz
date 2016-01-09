/// \file DQMJobInterfaceWidget.cc
/*
 *
 * DQMJobInterfaceWidget.cc source template automatically generated by a class generator
 * Creation date : lun. ao�t 24 2015
 *
 * This file is part of DQM4HEP libraries.
 *
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */

// -- dqm4hep headers
#include "dqm4hep/vis/DQMJobInterfaceWidget.h"
#include "DQMVizConfig.h"

// qt headers
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QWidget>
#include <QApplication>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QContextMenuEvent>

namespace dqm4hep
{

DQMJobInterfaceWidget::DQMJobInterfaceWidget(QWidget *pParent):
    QWidget(pParent)
{
    m_pJobIterface = new DimJobInterface();

    QLayout *pMainLayout = new QVBoxLayout();
    setLayout(pMainLayout);

    QHBoxLayout *pComboBoxLayout = new QHBoxLayout();

    QGroupBox *pComboBoxGroupBox = new QGroupBox();
    pMainLayout->addWidget(pComboBoxGroupBox);

    QSpacerItem *p_HSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    pComboBoxLayout->addSpacerItem(p_HSpacer);

    pKillComboBoxCaption = new QLabel("Set Kill Method");
    pComboBoxLayout->addWidget(pKillComboBoxCaption);

    m_pKillComboBoxWidget = new QComboBox();
    m_pKillComboBoxWidget->addItem("HUP (Hang Up): 1", 1);
    m_pKillComboBoxWidget->addItem("INT (Interrupt): 2", 2);
    m_pKillComboBoxWidget->addItem("QUIT (Quit): 3", 3);
    m_pKillComboBoxWidget->addItem("ABRT (Abort): 6", 6);
    m_pKillComboBoxWidget->addItem("KILL (Non-ignorable kill): 9", 9);
    m_pKillComboBoxWidget->addItem("ALRM (Alarm Clock): 14", 14);
    m_pKillComboBoxWidget->addItem("TERM (Software Term Signal): 15", 15);
    m_pKillComboBoxWidget->setCurrentIndex(1); // Default: Interrupt = kill -2

    pComboBoxLayout->addWidget(m_pKillComboBoxWidget);
    pComboBoxGroupBox->setLayout(pComboBoxLayout);

    m_pTreeWidget = new QTreeWidget();
    pMainLayout->addWidget(m_pTreeWidget);

    m_pTreeWidget->setColumnCount(4);
    m_pTreeWidget->setHeaderLabels(QStringList() << "Job Control" << "Program Name" << "PID" << "Status");

    // Create Buttons
    QWidget *pButtonWidget = new QWidget();
    pMainLayout->addWidget(pButtonWidget);

    QHBoxLayout *pButtonHLayout = new QHBoxLayout();
    pButtonWidget->setLayout(pButtonHLayout);

    m_pLoadFileButton = new QPushButton("Load file");
    pButtonHLayout->addWidget(m_pLoadFileButton);
    connect(m_pLoadFileButton, SIGNAL(clicked()), this, SLOT(loadJsonFile()));

    m_pReloadFileButton = new QPushButton("Reload file");
    pButtonHLayout->addWidget(m_pReloadFileButton);
    connect(m_pReloadFileButton, SIGNAL(clicked()), this, SLOT(reloadJsonFile()));

    QSpacerItem *p_HButtonSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    pButtonHLayout->addSpacerItem(p_HButtonSpacer);

    m_pUpdateButton = new QPushButton("Update");
    pButtonHLayout->addWidget(m_pUpdateButton);
    connect(m_pUpdateButton, SIGNAL(clicked()), this, SLOT(updateJobStatus()));


    // Create Menus / Menus Entries
    createActions();
    createMenus();

}

//-------------------------------------------------------------------------------------------------

DQMJobInterfaceWidget::~DQMJobInterfaceWidget()
{
    delete m_pJobIterface;
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::createActions()
{
    m_pLoadFileAction = new QAction("Load File", this);
    connect(m_pLoadFileAction, SIGNAL(triggered()), this, SLOT(loadJsonFile()));

    m_pReloadFileAction = new QAction("Reload File", this);
    connect(m_pReloadFileAction, SIGNAL(triggered()), this, SLOT(reloadJsonFile()));

    m_pStartHostJobsAction = new QAction("Start host jobs", this);
    connect(m_pStartHostJobsAction, SIGNAL(triggered()), this, SLOT(startHostJobs()));

    m_pClearHostJobsAction = new QAction("Kill host jobs", this);
    connect(m_pClearHostJobsAction, SIGNAL(triggered()), this, SLOT(clearHostJobs()));

    m_pKillJobAction = new QAction("Kill selected job", this);
    connect(m_pKillJobAction, SIGNAL(triggered()), this, SLOT(killSelectedJob()));

    m_pRestartJobAction = new QAction("Restart selected job", this);
    connect(m_pRestartJobAction, SIGNAL(triggered()), this, SLOT(restartSelectedJob()));

    m_pStartJobAction = new QAction("Start selected job", this);
    connect(m_pStartJobAction, SIGNAL(triggered()), this, SLOT(startSelectedJob()));

    m_pClearAllJobsAction = new QAction("Kill all jobs", this);
    connect(m_pClearAllJobsAction, SIGNAL(triggered()), this, SLOT(clearAllJobs()));

    //    m_pKillAllJobsAction = new QAction("Kill all jobs", this);
    //    connect(m_pKillAllJobsAction, SIGNAL(triggered()), this, SLOT(killAllJobs()));

    m_pRestartAllJobsAction = new QAction("Restart all jobs", this);
    connect(m_pRestartAllJobsAction, SIGNAL(triggered()), this, SLOT(restartAllJobs()));

    m_pStartAllJobsAction = new QAction("Start all jobs", this);
    connect(m_pStartAllJobsAction, SIGNAL(triggered()), this, SLOT(startAllJobs()));

    m_pUpdateAction = new QAction("Update jobs", this);
    connect(m_pUpdateAction, SIGNAL(triggered()), this, SLOT(updateJobStatus()));
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::createMenus()
{
    m_pMenuBar = new QMenuBar();
    m_pFileMenu = m_pMenuBar->addMenu("&File");
    m_pFileMenu->addAction(m_pLoadFileAction);
    m_pFileMenu->addAction(m_pReloadFileAction);
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::contextMenuEvent(QContextMenuEvent *event)
{
    m_pContextMenu = new QMenu();
    m_pContextMenu->addAction(m_pStartHostJobsAction);
    m_pContextMenu->addAction(m_pClearHostJobsAction);

    m_pContextMenu->addSeparator();
    m_pContextMenu->addAction(m_pKillJobAction);
    m_pContextMenu->addAction(m_pRestartJobAction);
    m_pContextMenu->addAction(m_pStartJobAction);

    m_pContextMenu->addSeparator();
    m_pContextMenu->addAction(m_pClearAllJobsAction);
    //    m_pContextMenu->addAction(m_pKillAllJobsAction);
    m_pContextMenu->addAction(m_pRestartAllJobsAction);
    m_pContextMenu->addAction(m_pStartAllJobsAction);

    m_pContextMenu->addSeparator();
    m_pContextMenu->addAction(m_pUpdateAction);

    m_pStartHostJobsAction->setEnabled(false);
    m_pClearHostJobsAction->setEnabled(false);
    m_pKillJobAction->setEnabled(false);
    m_pRestartJobAction->setEnabled(false);
    m_pStartJobAction->setEnabled(false);

    // Activate items as needed
    QTreeWidgetItem* pCurrentItem = m_pTreeWidget->currentItem();

    if (NULL == pCurrentItem)
    {
        m_pContextMenu->exec(event->globalPos());
        return;
    }

    if(pCurrentItem->data(0, Qt::UserRole).value<int>() == HOST_ITEM)
    {
        m_pStartHostJobsAction->setEnabled(true);
        m_pClearHostJobsAction->setEnabled(true);
    }
    else if(pCurrentItem->data(0, Qt::UserRole).value<int>() == JOB_ITEM)
    {
        QString status = pCurrentItem->text(STATUS).at(0);

        if (status.isEmpty())
            return;

        if ( status == "D" ) //Dead
        {
            if (pCurrentItem->text(PID).toInt())
                m_pRestartJobAction->setEnabled(true);
            else
                m_pStartJobAction->setEnabled(true);
        }

        else if ( status != "Z" && // Zombie
                  status != "T"     // Traced or Stopped
                  )
        {
            m_pKillJobAction->setEnabled(true);
            m_pRestartJobAction->setEnabled(true);
        }
        else
        {
            m_pStartJobAction->setEnabled(true);
        }
    }
    m_pContextMenu->exec(event->globalPos());
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::loadJsonFile()
{
    QString fileName  = QFileDialog::getOpenFileName(this, "Load file");

    if(fileName.isEmpty())
        return;

    loadJsonFile(fileName.toStdString());
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::loadJsonFile(const std::string &fileName)
{
    if(fileName.empty())
        return;

    m_pJobIterface->loadJSON(fileName);
    const Json::Value &root(m_pJobIterface->getRoot());

    m_currentJsonFile = fileName;

    loadJson(root);

    updateJobStatus();
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::reloadJsonFile()
{
    if(m_currentJsonFile.empty())
        return;

    loadJsonFile(m_currentJsonFile);
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::startHostJobs()
{
    QTreeWidgetItem* pSelectedItem = m_pTreeWidget->currentItem();

    if(!pSelectedItem)
        return;

    if(pSelectedItem->data(0, Qt::UserRole).value<int>() != HOST_ITEM)
        return;

    QString hostName = pSelectedItem->text(NAME);

    m_pJobIterface->startJobs(hostName.toStdString());
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::startSelectedJob()
{
    QTreeWidgetItem* pSelectedItem = m_pTreeWidget->currentItem();

    if(!pSelectedItem)
        return;

    if(pSelectedItem->data(0, Qt::UserRole).value<int>() != JOB_ITEM)
        return;

    QString hostName = pSelectedItem->parent()->text(NAME);
    QString jobName = pSelectedItem->text(NAME);

    m_pJobIterface->startJob(hostName.toStdString(), jobName.toStdString());
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::startAllJobs()
{
    const Json::Value &root(m_pJobIterface->getRoot());
    StringVector hostList = root["hosts"].getMemberNames();

    // loop over hosts
    for(StringVector::iterator iter = hostList.begin(), endIter = hostList.end() ;
        endIter != iter ; ++iter)
    {
        m_pJobIterface->startJobs(*iter);
    }
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::clearHostJobs()
{
    QTreeWidgetItem* pSelectedItem = m_pTreeWidget->currentItem();

    if(!pSelectedItem)
        return;

    if(pSelectedItem->data(0, Qt::UserRole).value<int>() != HOST_ITEM)
        return;

    QString hostName = pSelectedItem->text(NAME);

    m_pJobIterface->clearHostJobs(hostName.toStdString());
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::clearAllJobs()
{
    m_pJobIterface->clearAllJobs();
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::killSelectedJob()
{
    QComboBox* pKillItem = m_pKillComboBoxWidget;
    QTreeWidgetItem* pSelectedItem = m_pTreeWidget->currentItem();

    if(!pSelectedItem)
        return;

    if(pSelectedItem->data(0, Qt::UserRole).value<int>() != JOB_ITEM)
        return;

    QString hostName = pSelectedItem->parent()->text(NAME);
    QString pidStr = pSelectedItem->text(PID);
    QVariant sigVar = pKillItem->itemData(pKillItem->currentIndex());

    if(pidStr.isEmpty())
        return;

    uint32_t pid = pidStr.toUInt();
    uint32_t sig = sigVar.toUInt();

    m_pJobIterface->killJob(hostName.toStdString(), pid, sig);
}

//-------------------------------------------------------------------------------------------------

//void DQMJobInterfaceWidget::killAllJobs()
//{
//    QComboBox* pKillItem = m_pKillComboBoxWidget;

//    // loop over hosts and jobs
//    // and kill them if the pid is defined
//    for(unsigned int h=0 ; h<m_pTreeWidget->topLevelItemCount() ; h++)
//    {
//        QTreeWidgetItem *pHostItem = m_pTreeWidget->topLevelItem(h);

//        QString hostName = pHostItem->text(0);

//        for(unsigned int j=0 ; j<pHostItem->childCount() ; j++)
//        {
//            QTreeWidgetItem *pJobItem = pHostItem->child(j);

//            QString pidStr = pJobItem->text(3);
//            QVariant sigVar = pKillItem->itemData(pKillItem->currentIndex());

//            if(pidStr.isEmpty())
//                continue;

//            uint32_t pid = pidStr.toUInt();
//            uint32_t sig = sigVar.toUInt();

//            m_pJobIterface->killJob(hostName.toStdString(), pid, sig);
//        }
//    }
//}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::restartSelectedJob()
{
    QComboBox* pKillItem = m_pKillComboBoxWidget;
    QTreeWidgetItem* pSelectedItem = m_pTreeWidget->currentItem();

    if(!pSelectedItem)
        return;

    if(pSelectedItem->data(0, Qt::UserRole).value<int>() != JOB_ITEM)
        return;

    QString hostName = pSelectedItem->parent()->text(NAME);
    QString jobName = pSelectedItem->text(NAME);
    QString pidStr = pSelectedItem->text(PID);
    QVariant sigVar = pKillItem->itemData(pKillItem->currentIndex());

    if(pidStr.isEmpty())
        return;

    uint32_t pid = pidStr.toUInt();
    uint32_t sig = sigVar.toUInt();

    m_pJobIterface->restartJob(hostName.toStdString(), jobName.toStdString(), pid, sig);
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::restartAllJobs()
{
    QComboBox* pKillItem = m_pKillComboBoxWidget;

    // loop over hosts and jobs
    // and restart them if the pid is defined
    for(unsigned int h=0 ; h<m_pTreeWidget->topLevelItemCount() ; h++)
    {
        QTreeWidgetItem *pHostItem = m_pTreeWidget->topLevelItem(h);

        QString hostName = pHostItem->text(NAME);

        for(unsigned int j=0 ; j<pHostItem->childCount() ; j++)
        {
            QTreeWidgetItem *pJobItem = pHostItem->child(j);

            QString jobName = pJobItem->text(NAME);
            QString pidStr = pJobItem->text(PID);
            QVariant sigVar = pKillItem->itemData(pKillItem->currentIndex());

            if(pidStr.isEmpty())
                continue;

            uint32_t pid = pidStr.toUInt();
            uint32_t sig = sigVar.toUInt();

            m_pJobIterface->restartJob(hostName.toStdString(), jobName.toStdString(), pid, sig);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::updateJobStatus()
{
    m_pJobIterface->status();

    struct timespec timesleep;
    timesleep.tv_sec = 0;
    timesleep.tv_nsec = 500000000L;
    nanosleep(&timesleep, NULL);

    this->updateStatus(m_pJobIterface->getProcessStatusValue());
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::loadJson(const Json::Value &root)
{
    m_pTreeWidget->clear();

    StringVector hostList = root["hosts"].getMemberNames();

    // loop over hosts
    for(StringVector::iterator iter = hostList.begin(), endIter = hostList.end() ;
        endIter != iter ; ++iter)
    {
        const Json::Value &host(root["hosts"][*iter]);

        QStringList name((*iter).c_str());

        QTreeWidgetItem *pHostItem = new QTreeWidgetItem(name);

        pHostItem->setData(0, Qt::UserRole, HOST_ITEM);
        m_pTreeWidget->addTopLevelItem(pHostItem);

        // loop over jobs host
        for(unsigned int j=0 ; j<host.size() ; j++)
        {
            QStringList jobHeader;
            jobHeader << host[j]["Name"].asString().c_str() << host[j]["Program"].asString().c_str() << host[j]["PID"].asString().c_str() << host[j]["Status"].asString().c_str() ;

            QTreeWidgetItem *pJobItem = new QTreeWidgetItem(jobHeader);

            pJobItem->setData(NAME, Qt::UserRole, JOB_ITEM);
            pJobItem->setData(PROG_NAME, Qt::UserRole, JOB_PROGRAM_NAME_ITEM);
            pJobItem->setData(PID, Qt::UserRole, JOB_PID_ITEM);
            pJobItem->setData(STATUS, Qt::UserRole, JOB_STATUS_ITEM);

            pHostItem->addChild(pJobItem);

            QTreeWidgetItem *pEnvironmentItem = new QTreeWidgetItem(QStringList("ENV"));
            pEnvironmentItem->setData(0, Qt::UserRole, JOB_ENV_ITEM);
            pJobItem->addChild(pEnvironmentItem);

            const Json::Value &envJsonValue = host[j]["Environnement"];

            for(unsigned int e=0 ; e<envJsonValue.size() ; e++)
            {
                QString env(envJsonValue[e].asString().c_str());

                QStringList envVarHeader;
                envVarHeader << env;

                QTreeWidgetItem *pEnvVarItem = new QTreeWidgetItem(envVarHeader);
                pEnvVarItem->setData(0, Qt::UserRole, VARIABLE_ITEM);
                pEnvironmentItem->addChild(pEnvVarItem);
            }

            QTreeWidgetItem *pArgsItem = new QTreeWidgetItem(QStringList("ARGS"));
            pArgsItem->setData(0, Qt::UserRole, JOB_ARGS_ITEM);
            pJobItem->addChild(pArgsItem);

            const Json::Value &argsJsonValue = host[j]["Arguments"];

            for(unsigned int e=0 ; e<argsJsonValue.size() ; e++)
            {
                QString arg(argsJsonValue[e].asString().c_str());

                QStringList argVarHeader;
                argVarHeader << arg;

                QTreeWidgetItem *pArgVarItem = new QTreeWidgetItem(argVarHeader);
                pArgVarItem->setData(0, Qt::UserRole, VARIABLE_ITEM);
                pArgsItem->addChild(pArgVarItem);
            }
        }

        pHostItem->setExpanded(true);
    }

    m_pTreeWidget->header()->resizeSections(QHeaderView::Stretch);
}

//-------------------------------------------------------------------------------------------------

void DQMJobInterfaceWidget::updateStatus(const Json::Value &value)
{
    QMap<QString, QColor> stateToColorMap;
    stateToColorMap["Z"] = QColor(Qt::red); // zombie
    stateToColorMap["T"] = QColor(Qt::darkRed); // traced or stopped
    stateToColorMap["R"] = QColor(Qt::green); // running
    stateToColorMap["D"] = QColor(Qt::darkGreen); // un-interruptible sleep
    stateToColorMap["S"] = QColor(Qt::darkGreen); // interruptible sleep

    for(unsigned int i=0 ; i<m_pTreeWidget->topLevelItemCount() ; i++)
    {
        QTreeWidgetItem *pHostItem = m_pTreeWidget->topLevelItem(i);

        std::string hostName = pHostItem->text(NAME).toStdString();

        for(unsigned int j=0 ; j<pHostItem->childCount() ; j++)
        {
            QTreeWidgetItem *pJobItem = pHostItem->child(j);

            std::string jobName = pJobItem->text(NAME).toStdString();

            bool found = false;

            for(unsigned int v=0 ; v<value.size() ; v++)
            {
                std::string vHost = value[v]["HOST"].asString();
                std::string vJobName = value[v]["NAME"].asString();
                uint32_t vJobPid = value[v]["PID"].asUInt();
                std::string vJobStatus = value[v]["STATUS"].asString();

                if(vHost == hostName && vJobName == jobName)
                {
                    pJobItem->setText(STATUS, QString(vJobStatus.c_str()).trimmed());
                    pJobItem->setText(PID, QString::number(vJobPid));

                    found = true;
                    break;
                }
            }

            if(!found)
            {
                pJobItem->setText(STATUS, "DEAD");
                pJobItem->setText(PID, "");
            }

            if(pJobItem->text(STATUS) == "DEAD")
                pJobItem->setData(STATUS, Qt::ForegroundRole, QBrush(Qt::red));
            // cases available in /proc/pid/status file
            else
            {
                QMap<QString, QColor>::iterator findIter = stateToColorMap.find(pJobItem->text(STATUS).at(0));

                if(findIter != stateToColorMap.end())
                    pJobItem->setData(STATUS, Qt::ForegroundRole, QBrush(findIter.value()));
                else
                    pJobItem->setData(STATUS, Qt::ForegroundRole, QBrush(Qt::black));
            }
        }

    }

    m_pTreeWidget->header()->resizeSections(QHeaderView::Stretch);
}

}

