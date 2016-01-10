/*
 *
 * DQMCanvasArea.cc source template automatically generated by a class generator
 * Creation date : mar. oct. 13 2015
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


#include "dqm4hep/vis/DQMCanvasArea.h"
#include "dqm4hep/vis/DQMMonitoring.h"
#include "dqm4hep/vis/DQMMonitoringView.h"
#include "dqm4hep/vis/DQMMonitoringModel.h"
#include "dqm4hep/vis/DQMMonitoringController.h"
#include "dqm4hep/vis/DQMCanvas.h"
#include "dqm4hep/vis/DQMGuiMonitorElement.h"

namespace dqm4hep
{

DQMCanvasArea::DQMCanvasArea(DQMMonitoring *pMonitoring) :
		m_pMonitoring(pMonitoring)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setAcceptDrops(true);
}

//-------------------------------------------------------------------------------------------------

DQMCanvasArea::~DQMCanvasArea()
{
	/* nop */
}

//-------------------------------------------------------------------------------------------------

DQMMonitoring *DQMCanvasArea::getMonitoring() const
{
	return m_pMonitoring;
}

//-------------------------------------------------------------------------------------------------

bool DQMCanvasArea::isDrawn(DQMGuiMonitorElement *pMonitorElement) const
{
	return (NULL == this->getCanvas(pMonitorElement));
}

//-------------------------------------------------------------------------------------------------

bool DQMCanvasArea::removeCanvas(DQMCanvas *pCanvasToRemove, bool deleteLater)
{
	if(NULL == pCanvasToRemove)
		return false;

	QList<QMdiSubWindow*> subWindowList = this->subWindowList();

	for(int i=0 ; subWindowList.count() ; i++)
	{
		if(pCanvasToRemove == subWindowList.at(i))
		{
			this->removeSubWindow(pCanvasToRemove);

			if(deleteLater)
				pCanvasToRemove->deleteLater();

			return true;
		}
	}

	return false;
}

//-------------------------------------------------------------------------------------------------

void DQMCanvasArea::addCanvas(DQMCanvas *pCanvasToAdd)
{
	if(NULL == pCanvasToAdd)
		return;

	QList<QMdiSubWindow*> subWindowList = this->subWindowList();

	if(!subWindowList.contains(pCanvasToAdd))
		this->addSubWindow(pCanvasToAdd);
}

//-------------------------------------------------------------------------------------------------

DQMCanvas *DQMCanvasArea::getCanvas(DQMGuiMonitorElement *pMonitorElement) const
{
	if(NULL == pMonitorElement)
		return NULL;

	QList<QMdiSubWindow*>  subWindowList = this->subWindowList();

	for(int i=0 ; i<subWindowList.count() ; i++)
	{
		DQMCanvas *pCanvas = (DQMCanvas *) subWindowList.at(i);

		if(!pCanvas)
			continue;

		DQMGuiMonitorElement *pCurrentMonitorElement = pCanvas->getCurrentMonitorElement();

		if(NULL == pCurrentMonitorElement)
			continue;

		if(pMonitorElement == pCurrentMonitorElement)
			return pCanvas;

		if(pMonitorElement->equals(pCurrentMonitorElement))
			return pCanvas;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------------

DQMCanvas *DQMCanvasArea::createCanvas(DQMGuiMonitorElement *pMonitorElement)
{
	// find if this monitor element is drawn an a canvas
	DQMCanvas *pCanvas = this->getCanvas(pMonitorElement);

	if(!pCanvas)
	{
		pCanvas = new DQMCanvas(this->getMonitoring());
		this->addCanvas(pCanvas);
		pCanvas->show();
	}

	pCanvas->show();

	// draw it
	pCanvas->draw(pMonitorElement);

	return pCanvas;
}

//-------------------------------------------------------------------------------------------------

DQMCanvas *DQMCanvasArea::getCanvas(int index) const
{
	return (DQMCanvas *) this->subWindowList().at(index);
}

//-------------------------------------------------------------------------------------------------

bool DQMCanvasArea::contains(DQMCanvas *pCanvas) const
{
	if(!pCanvas)
		return false;

	QList<QMdiSubWindow*>  subWindowList = this->subWindowList();

	for(int i=0 ; subWindowList.count() ; i++)
	{
		DQMCanvas *pSubWindow = (DQMCanvas*) subWindowList.at(i);

		if(!pSubWindow)
			continue;

		if(pSubWindow == pCanvas)
			return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------

int DQMCanvasArea::canvasCount() const
{
	return subWindowList().count();
}

//-------------------------------------------------------------------------------------------------

DQMCanvas *DQMCanvasArea::draw(DQMGuiMonitorElement *pMonitorElement)
{
	// find if this monitor element is drawn an a canvas
	DQMCanvas *pCanvas = this->getCanvas(pMonitorElement);

	DQMCanvas *pCurrentCanvas = qobject_cast<DQMCanvas *>(this->currentSubWindow());
	bool showMaximized = false;
	bool canvasCreated = false;

	if(NULL != pCurrentCanvas && pCurrentCanvas->isMaximized())
		showMaximized = true;

	// if not, create it
	if(NULL == pCanvas)
	{
		pCanvas = new DQMCanvas(this->getMonitoring());
		this->addCanvas(pCanvas);
		canvasCreated = true;
	}

	if(canvasCreated)
	{
		if(showMaximized)
			pCanvas->showMaximized();
		else
			pCanvas->show();
	}
	else
	{
		if(showMaximized)
			pCanvas->showMaximized();
		else
			this->setActiveSubWindow(pCanvas);
	}

	// draw it
	pCanvas->draw(pMonitorElement);

	return pCanvas;
}

//-------------------------------------------------------------------------------------------------

void DQMCanvasArea::clear()
{
	QList<QMdiSubWindow*>  subWindowList = this->subWindowList();

	for(int i=0 ; i<subWindowList.count() ; i++)
	{
		QMdiSubWindow *pSubWindow = subWindowList.at(i);

		this->removeSubWindow(pSubWindow);
		pSubWindow->deleteLater();
	}
}

//-------------------------------------------------------------------------------------------------

void DQMCanvasArea::dragEnterEvent(QDragEnterEvent *pEvent)
{
	// check mime type
    if (pEvent->mimeData()->hasFormat("dqm4hep/me-item-list"))
    	pEvent->acceptProposedAction();
}

//-------------------------------------------------------------------------------------------------

void DQMCanvasArea::dropEvent(QDropEvent *pEvent)
{
	if(pEvent->proposedAction() != Qt::CopyAction)
		return;

	const QByteArray &byteArray(pEvent->mimeData()->data("dqm4hep/me-item-list"));

	QDataStream dataStream(byteArray);

	qint32 nElements = 0;
	dataStream >> nElements;

	if(nElements == 0)
		return;

	// accept action only if correct type
	// and byte array is not empty
	pEvent->acceptProposedAction();

	int nDrawElements = 0;

	// deserialize the items, query the element
	// to model and draw them on the fly
	for(int i=0 ; i < nElements ; i++)
	{
		QString collectorName;
		QString moduleName;
		QString fullPathName;
		QString monitorElementName;

		dataStream >> collectorName >> moduleName >> fullPathName >> monitorElementName;

		DQMGuiMonitorElement *pMonitorElement =
				this->getMonitoring()->getModel()->findMonitorElement(
						collectorName.toStdString(),
						moduleName.toStdString(),
						fullPathName.toStdString(),
						monitorElementName.toStdString());

		if(!pMonitorElement)
			continue;

		this->draw(pMonitorElement);
		nDrawElements++;
	}

	if(nDrawElements == nElements)
		this->getMonitoring()->getController()->log(MESSAGE, QString("Number of drawn monitor elements : %1").arg(nDrawElements).toStdString());
	else if(nDrawElements < nElements && nDrawElements != 0)
		this->getMonitoring()->getController()->log(WARNING, QString("Monitor elements drawn (%1 success, %2 failed)").arg(nDrawElements).arg(nElements-nDrawElements).toStdString());
	else
		this->getMonitoring()->getController()->log(ERROR, QString("No monitor elements drawn ! (%1 requested)").arg(nElements).toStdString());
}

} 
