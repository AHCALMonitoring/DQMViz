  /// \file DQMMonitoringModel.cc
/*
 *
 * DQMMonitoringModel.cc source template automatically generated by a class generator
 * Creation date : lun. oct. 12 2015
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


#include "dqm4hep/vis/DQMMonitoringModel.h"
#include "dqm4hep/vis/DQMMonitoring.h"
#include "dqm4hep/vis/DQMMonitoringView.h"
#include "dqm4hep/vis/DQMMonitoringController.h"
#include "dqm4hep/vis/DQMMonitorElementView.h"
#include "dqm4hep/vis/DQMGuiMonitorElement.h"
#include "dqm4hep/vis/DQMGuiMonitorElementClient.h"
#include "dqm4hep/vis/DQMDeleteLaterObject.h"
#include "dqm4hep/DQMMonitorElement.h"

#include <algorithm>

#include <QMap>

namespace dqm4hep
{

DQMMonitoringModel::DQMMonitoringModel(DQMMonitoring *pMonitoring) :
		m_pMonitoring(pMonitoring)
{
	m_pMonitoring->setModel(this);
}

//-------------------------------------------------------------------------------------------------

DQMMonitoringModel::~DQMMonitoringModel() 
{
	clear();
}

//-------------------------------------------------------------------------------------------------

DQMMonitoring *DQMMonitoringModel::getMonitoring() const
{
	return m_pMonitoring;
}

//-------------------------------------------------------------------------------------------------

void DQMMonitoringModel::updateMonitorElement(DQMMonitorElementPtr &monitorElement)
{
	if(!monitorElement)
		return;

	for(DQMGuiMonitorElementList::iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		if((*iter)->equals(monitorElement))
		{
			// replace it (swap)
			DQMGuiMonitorElement *pGuiMonitorElement = *iter;
			pGuiMonitorElement->update(monitorElement);

			// notify view
			this->getMonitoring()->getView()->getMonitorElementView()->updateMonitorElement(pGuiMonitorElement);

			return;
		}
	}

	// create a new one
	DQMGuiMonitorElement *pGuiMonitorElement = this->createGuiMonitorElement(monitorElement);

	// register it
	m_monitorElementList.push_back(pGuiMonitorElement);

	// notify view
	this->getMonitoring()->getView()->getMonitorElementView()->addMonitorElement(pGuiMonitorElement);
}

//-------------------------------------------------------------------------------------------------

void DQMMonitoringModel::updateMonitorElement(DQMGuiMonitorElement *pMonitorElement)
{
	if(!pMonitorElement)
		return;

	DQMGuiMonitorElement *pFindMonitorElement = this->findMonitorElement(pMonitorElement->getMonitorElement()->getCollectorName(),
			pMonitorElement->getMonitorElement()->getModuleName(),
			pMonitorElement->getMonitorElement()->getPath().getPath(),
			pMonitorElement->getMonitorElement()->getName());

	if(!pFindMonitorElement)
	{
		// register it
		m_monitorElementList.push_back(pMonitorElement);

		// notify view
		this->getMonitoring()->getView()->getMonitorElementView()->addMonitorElement(pMonitorElement);
	}
	else
	{
		DQMMonitorElementPtr monitorElement = pMonitorElement->getMonitorElement();
		pFindMonitorElement->update(monitorElement);

		// notify view
		this->getMonitoring()->getView()->getMonitorElementView()->updateMonitorElement(pMonitorElement);
	}
}

//-------------------------------------------------------------------------------------------------

void DQMMonitoringModel::removeMonitorElement(DQMGuiMonitorElement *pMonitorElement)
{
	if(!pMonitorElement)
		return;

	for(DQMGuiMonitorElementList::iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		DQMGuiMonitorElement *pGuiMonitorElement = *iter;

		if((*iter)->equals(pMonitorElement))
		{
			// remove it
			m_monitorElementList.erase(iter);

			// notify view
			this->getMonitoring()->getView()->getMonitorElementView()->removeMonitorElement(pGuiMonitorElement);

			break;
		}
	}

	pMonitorElement->deleteLater();
}

//-------------------------------------------------------------------------------------------------

void DQMMonitoringModel::removeMonitorElement(const std::string &collectorName, const std::string &moduleName,
		const std::string &fullPath, const std::string &name)
{

	for(DQMGuiMonitorElementList::iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		const std::string &meCollectorName = (*iter)->getMonitorElement()->getCollectorName();
		const DQMPath     &mePath = (*iter)->getMonitorElement()->getPath();
		const std::string &meModuleName = (*iter)->getMonitorElement()->getModuleName();
		const std::string &meName = (*iter)->getMonitorElement()->getName();

		bool sameCollectorName = (collectorName == meCollectorName);
		bool samePath =          (fullPath          == mePath);
		bool sameModuleName =    (moduleName    == meModuleName);
		bool sameName =          (name          == meName);

		if(sameCollectorName && samePath && sameModuleName && sameName)
		{
			DQMGuiMonitorElement *pGuiMonitorElement = *iter;

			// remove it
			m_monitorElementList.erase(iter);
			pGuiMonitorElement->deleteLater();

			this->getMonitoring()->getView()->getMonitorElementView()->removeMonitorElement(pGuiMonitorElement);

			break;
		}
	}
}

//-------------------------------------------------------------------------------------------------

void DQMMonitoringModel::clear()
{
	for(DQMGuiMonitorElementList::iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		DQMGuiMonitorElement *pGuiMonitorElement = *iter;

		this->getMonitoring()->getView()->getMonitorElementView()->removeMonitorElement(pGuiMonitorElement);
		pGuiMonitorElement->deleteLater();
	}

	m_monitorElementList.clear();
}

//-------------------------------------------------------------------------------------------------

void DQMMonitoringModel::clear(const std::string &collectorName)
{
	for(DQMGuiMonitorElementList::iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		const std::string &thisCollectorName = (*iter)->getMonitorElement()->getCollectorName();

		if(thisCollectorName == collectorName)
		{
			DQMGuiMonitorElement *pGuiMonitorElement = *iter;

			// remove it
			m_monitorElementList.erase(iter);
			iter--;

			// notify view
			this->getMonitoring()->getView()->getMonitorElementView()->removeMonitorElement(pGuiMonitorElement);

			// delete it
			pGuiMonitorElement->deleteLater();
		}
	}
}

//-------------------------------------------------------------------------------------------------

void DQMMonitoringModel::clear(const std::string &collectorName, const std::string &moduleName)
{
	for(DQMGuiMonitorElementList::iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		const std::string &thisCollectorName = (*iter)->getMonitorElement()->getCollectorName();
		const std::string &thisModuleName = (*iter)->getMonitorElement()->getModuleName();

		if(thisCollectorName == collectorName && thisModuleName == moduleName)
		{
			DQMGuiMonitorElement *pGuiMonitorElement = *iter;

			// remove it
			m_monitorElementList.erase(iter);
			iter--;

			// notify view
			this->getMonitoring()->getView()->getMonitorElementView()->removeMonitorElement(pGuiMonitorElement);

			// delete it
			pGuiMonitorElement->deleteLater();
		}
	}
}

//-------------------------------------------------------------------------------------------------

bool DQMMonitoringModel::monitorElementExists(DQMGuiMonitorElement *pMonitorElement) const
{
	if(!pMonitorElement)
		return false;

	for(DQMGuiMonitorElementList::const_iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		if((*iter)->equals(pMonitorElement))
			return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------

DQMGuiMonitorElement *DQMMonitoringModel::createGuiMonitorElement(DQMMonitorElementPtr &monitorElement) const
{
	if(NULL == monitorElement)
		return NULL;

	return new DQMGuiMonitorElement(monitorElement);
}

//-------------------------------------------------------------------------------------------------

DQMGuiMonitorElement *DQMMonitoringModel::getOrCreateGuiMonitorElement(const std::string &collectorName, const std::string &moduleName,
		const std::string &fullPath, const std::string &name)
{
	DQMPath path = fullPath;
	DQMGuiMonitorElement *pMonitorElement = NULL;

	for(DQMGuiMonitorElementList::const_iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		const std::string &meCollectorName = (*iter)->getMonitorElement()->getCollectorName();
		const DQMPath     &mePath = (*iter)->getMonitorElement()->getPath();
		const std::string &meModuleName = (*iter)->getMonitorElement()->getModuleName();
		const std::string &meName = (*iter)->getMonitorElement()->getName();

		bool sameCollectorName = (collectorName == meCollectorName);
		bool samePath =          (path          == mePath);
		bool sameModuleName =    (moduleName    == meModuleName);
		bool sameName =          (name          == meName);

		if(sameCollectorName && samePath && sameModuleName && sameName)
		{
			pMonitorElement = (*iter);
			break;
		}
	}

	if(NULL == pMonitorElement)
	{
		pMonitorElement = this->createGuiMonitorElement(collectorName, moduleName, fullPath, name);

		// register it
		m_monitorElementList.push_back(pMonitorElement);

		// notify view
		this->getMonitoring()->getView()->getMonitorElementView()->addMonitorElement(pMonitorElement);
	}

	return pMonitorElement;
}

//-------------------------------------------------------------------------------------------------

void DQMMonitoringModel::loadMonitorElementInfoList(const std::string &collectorName, const DQMMonitorElementInfoList &nameList)
{
	DQMGuiMonitorElementList addedMonitorElement;

	for(DQMMonitorElementInfoList::const_iterator meIter = nameList.begin(), meEndIter = nameList.end() ;
			meEndIter != meIter ; ++meIter)
	{
		DQMPath path = meIter->find( DQMKey::ME_PATH )->second;
		std::string moduleName = meIter->find( DQMKey::MODULE_NAME )->second;
		std::string monitorElementName = meIter->find( DQMKey::ME_NAME )->second;
		DQMGuiMonitorElement *pMonitorElement = NULL;

		for(DQMGuiMonitorElementList::const_iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
				endIter != iter ; ++iter)
		{
			const std::string &meCollectorName = (*iter)->getMonitorElement()->getCollectorName();
			const DQMPath     &mePath = (*iter)->getMonitorElement()->getPath();
			const std::string &meModuleName = (*iter)->getMonitorElement()->getModuleName();
			const std::string &meName = (*iter)->getMonitorElement()->getName();

			bool sameCollectorName = (collectorName      == meCollectorName);
			bool samePath =          (path               == mePath);
			bool sameModuleName =    (moduleName         == meModuleName);
			bool sameName =          (monitorElementName == meName);

			if(sameCollectorName && samePath && sameModuleName && sameName)
			{
				pMonitorElement = (*iter);
				break;
			}
		}

		if(NULL == pMonitorElement)
		{
			pMonitorElement = this->createGuiMonitorElement(collectorName, moduleName, path.getPath(), monitorElementName);

			// add it to tmp list
			addedMonitorElement.push_back(pMonitorElement);

			// notify view
			this->getMonitoring()->getView()->getMonitorElementView()->addMonitorElement(pMonitorElement);
		}
	}

	// register new content !
	m_monitorElementList.insert(m_monitorElementList.end(), addedMonitorElement.begin(), addedMonitorElement.end());
}

//-------------------------------------------------------------------------------------------------

DQMGuiMonitorElement *DQMMonitoringModel::createGuiMonitorElement(const std::string &collectorName, const std::string &moduleName,
			const std::string &fullPath, const std::string &name) const
{
	DQMMonitorElementPtr monitorElement = std::make_shared<DQMMonitorElement>(NO_ELEMENT_TYPE, name, "", moduleName);
	monitorElement->setCollectorName(collectorName);
	monitorElement->setPath(DQMPath(fullPath));

	return this->createGuiMonitorElement(monitorElement);
}

//-------------------------------------------------------------------------------------------------

DQMGuiMonitorElement *DQMMonitoringModel::findMonitorElement(const std::string &collectorName, const std::string &moduleName,
		const std::string &fullPath, const std::string &name) const
{
	DQMPath path = fullPath;

	for(DQMGuiMonitorElementList::const_iterator iter = m_monitorElementList.begin(), endIter = m_monitorElementList.end() ;
			endIter != iter ; ++iter)
	{
		const std::string &meCollectorName = (*iter)->getMonitorElement()->getCollectorName();
		const DQMPath     &mePath = (*iter)->getMonitorElement()->getPath();
		const std::string &meModuleName = (*iter)->getMonitorElement()->getModuleName();
		const std::string &meName = (*iter)->getMonitorElement()->getName();

		bool sameCollectorName = (collectorName == meCollectorName);
		bool samePath =          (path          == mePath);
		bool sameModuleName =    (moduleName    == meModuleName);
		bool sameName =          (name          == meName);

		if(sameCollectorName && samePath && sameModuleName && sameName)
		{
			return (*iter);
		}
	}

	return NULL;
}

} 

