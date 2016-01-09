  /// \file DQMAutomaticQueryWidget.h
/*
 *
 * DQMAutomaticQueryWidget.h header template automatically generated by a class generator
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


#ifndef DQMAUTOMATICQUERYWIDGET_H
#define DQMAUTOMATICQUERYWIDGET_H

// -- dqm4hep headers
#include "dqm4hep/DQMXmlIO.h"

// -- qt headers
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>

namespace dqm4hep
{

/** DQMAutomaticQueryWidget class
 */
class DQMAutomaticQueryWidget : public QWidget, public DQMXmlIO
{
	Q_OBJECT

public:
	/** Constructor
	 */
	DQMAutomaticQueryWidget(QWidget *pParent = 0);

	/** Destructor
	 */
	~DQMAutomaticQueryWidget();

public slots:
	/** Start the automatic mode query.
	 *  timeout() is emitted when the internal timer reaches its timeout.
	 */
	void start();

	/** Stop the automatic mode query
	 */
	void stop();

public:
	/** Whether the automatic mode is running, if start() has been called.
	 */
	bool isRunning() const;

	/** Get the timeout value.
	 */
	int getTimeout() const;

	/** Export settings to xml element
	 */
	TiXmlElement *toXml() const;

	/** Import settings from xml element
	 */
	void fromXml(TiXmlElement *const pXmlElement);

signals:
	/** Emitted when the timer reaches its timeout.
	 */
	void timeout();

	/** Emitted when the timer is started
	 */
	void started();

private slots:
	/** Forward timeout signal of the internal timer to this instance
	 */
	void emitTimeout();

	/** Handle the value changed by the spin box or if user called setTimeout(t)
	 */
	void handleValueChanged(int value);

	/** Handle the start/stop button click
	 */
	void handleButtonClicked();

protected:

    QGroupBox                *m_pAutomaticModeGroupBox;
    QPushButton              *m_pAutomaticModeButton;
    QSpinBox                 *m_pUpdatePeriodSpinBox;
    QTimer                    m_automaticModeTimer;
}; 

} 

#endif  //  DQMAUTOMATICQUERYWIDGET_H
