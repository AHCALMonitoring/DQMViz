  /// \file DQMGuiMonitorElement.cc
/*
 *
 * DQMGuiMonitorElement.cc source template automatically generated by a class generator
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


#include "dqm4hep/vis/DQMGuiMonitorElement.h"
#include "dqm4hep/DQMMonitorElement.h"
#include "dqm4hep/vis/DQMGuiMonitorElementClient.h"
#include "dqm4hep/vis/DQMMonitoring.h"

namespace dqm4hep
{

DQMGuiMonitorElement::DQMGuiMonitorElement(DQMMonitorElement *pMonitorElement) :
		m_pMonitorElement(pMonitorElement)
{
	/* nop */
}

//-------------------------------------------------------------------------------------------------

DQMGuiMonitorElement::~DQMGuiMonitorElement()
{
	if(NULL != m_pMonitorElement)
		delete m_pMonitorElement;
}

//-------------------------------------------------------------------------------------------------

void DQMGuiMonitorElement::update(DQMMonitorElement *pMonitorElement)
{
	if(m_pMonitorElement == pMonitorElement)
		return;

	if(m_pMonitorElement)
	{
		if(pMonitorElement)
		{
			// save previous state
			const std::string &drawOption(m_pMonitorElement->getDrawOption());
			pMonitorElement->setDrawOption(drawOption);
		}

		delete m_pMonitorElement;
	}

	m_pMonitorElement = pMonitorElement;

	emit updated();
}

//-------------------------------------------------------------------------------------------------

bool DQMGuiMonitorElement::equals(DQMMonitorElement *pMonitorElement) const
{
	if(NULL == pMonitorElement)
		return false;

	if(NULL == m_pMonitorElement)
		return false;

	const DQMPath &path = pMonitorElement->getPath();
	const std::string &moduleName = pMonitorElement->getModuleName();
	const std::string &name = pMonitorElement->getName();
	const std::string &collectorName = pMonitorElement->getCollectorName();

	bool samePath =          (path          == this->getMonitorElement()->getPath());
	bool sameModuleName =    (moduleName    == this->getMonitorElement()->getModuleName());
	bool sameName =          (name          == this->getMonitorElement()->getName());
	bool sameCollectorName = (collectorName == this->getMonitorElement()->getCollectorName());

	return (samePath && sameModuleName && sameName && sameCollectorName);
}

//-------------------------------------------------------------------------------------------------

bool DQMGuiMonitorElement::equals(DQMGuiMonitorElement *pGuiMonitorElement) const
{
	if(NULL == pGuiMonitorElement)
		return false;

	const DQMPath &path = pGuiMonitorElement->getMonitorElement()->getPath();
	const std::string &moduleName = pGuiMonitorElement->getMonitorElement()->getModuleName();
	const std::string &name = pGuiMonitorElement->getMonitorElement()->getName();
	const std::string &collectorName = pGuiMonitorElement->getMonitorElement()->getCollectorName();

	bool samePath =          (path          == this->getMonitorElement()->getPath());
	bool sameModuleName =    (moduleName    == this->getMonitorElement()->getModuleName());
	bool sameName =          (name          == this->getMonitorElement()->getName());
	bool sameCollectorName = (collectorName == this->getMonitorElement()->getCollectorName());

	return (samePath && sameModuleName && sameName && sameCollectorName);
}

//-------------------------------------------------------------------------------------------------

DQMMonitorElement *DQMGuiMonitorElement::getMonitorElement() const
{
	return m_pMonitorElement;
}

//-------------------------------------------------------------------------------------------------

void DQMGuiMonitorElement::setDrawOption(const std::string &drawOption)
{
	if(m_pMonitorElement)
	{
		m_pMonitorElement->setDrawOption(drawOption);
		emit updated();
	}
}

//-------------------------------------------------------------------------------------------------

void DQMGuiMonitorElement::setDescription(const std::string &description)
{
	if(m_pMonitorElement)
	{
		m_pMonitorElement->setDescription(description);
		emit updated();
	}
}

//-------------------------------------------------------------------------------------------------

void DQMGuiMonitorElement::setQuality(DQMQuality quality)
{
	if(m_pMonitorElement)
	{
		m_pMonitorElement->setQuality(quality);
		emit updated();
	}
}

} 
