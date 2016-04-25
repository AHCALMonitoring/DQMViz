  /// \file DQMLoggerWidget.cc
/*
 *
 * DQMLoggerWidget.cc source template automatically generated by a class generator
 * Creation date : jeu. mai 7 2015
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

// -- dqm4hep
#include "dqm4hep/vis/DQMLoggerWidget.h"
#include "DQMVizConfig.h"

// -- qt headers
#include <QVBoxLayout>
#include <QScrollBar>
#include <QPushButton>
#include <QIcon>

namespace dqm4hep
{

//    QIcon icon(QString(DQMViz_DIR) + "/icons/icon-arrow-down.png");

DQMLoggerWidget::DQMLoggerWidget(QWidget *pParent) :
		QGroupBox("Logging", pParent)
{
	QVBoxLayout *pLoggingLayout = new QVBoxLayout();
	setLayout(pLoggingLayout);


	// collapse/expand button area
	QHBoxLayout *pButtonLayout = new QHBoxLayout();
	pLoggingLayout->addLayout(pButtonLayout);

    QIcon icon(QString(DQMViz_DIR) + "/icons/icon-arrow-down.png");
	m_pCollapseExpandButton = new QPushButton( icon, "");
	m_pCollapseExpandButton->setFlat(true);
	pButtonLayout->addWidget(m_pCollapseExpandButton);
	pButtonLayout->setAlignment(m_pCollapseExpandButton, Qt::AlignLeft);

	// main widget area
	m_pCollapseExpandWidget = new QWidget();
	pLoggingLayout->addWidget(m_pCollapseExpandWidget);

	QVBoxLayout *pCollapseExpandLayout = new QVBoxLayout();
	m_pCollapseExpandWidget->setLayout(pCollapseExpandLayout);

	m_pLoggingWidget = new QTextEdit();
	m_pLoggingWidget->setReadOnly(true);
	pCollapseExpandLayout->addWidget(m_pLoggingWidget);

	QPushButton *pClearButton = new QPushButton("Clear");
	pCollapseExpandLayout->addWidget(pClearButton);
	pCollapseExpandLayout->setAlignment(pClearButton, Qt::AlignRight);

	connect(pClearButton, SIGNAL(clicked()), m_pLoggingWidget, SLOT(clear()));
	connect(m_pCollapseExpandButton, SIGNAL(clicked()), this, SLOT(collapseExpand()));

	// trigger a collapse event
	m_expanded = true;
	collapseExpand();
}

//-------------------------------------------------------------------------------------------------

QTextEdit *DQMLoggerWidget::logView() const
{
	return m_pLoggingWidget;
}

//-------------------------------------------------------------------------------------------------

void DQMLoggerWidget::log(const std::string &message)
{
	log(MESSAGE, message);
}

//-------------------------------------------------------------------------------------------------

void DQMLoggerWidget::log(LogLevel level, const std::string &message)
{
	Qt::GlobalColor color = getColor(level);

	QString messageHead(getLogMessageHead(level).c_str());

	// scroll to the end of text edit
	m_pLoggingWidget->horizontalScrollBar()->setValue(m_pLoggingWidget->horizontalScrollBar()->maximum());

	// set the cursor at the end
	QTextCursor cursor = m_pLoggingWidget->textCursor();
	cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
	m_pLoggingWidget->setTextCursor(cursor);

	// put the message
	m_pLoggingWidget->setTextColor(QColor(color));
	m_pLoggingWidget->insertPlainText(messageHead);
	m_pLoggingWidget->setTextColor(QColor(Qt::black));
	m_pLoggingWidget->insertPlainText(" ");
	m_pLoggingWidget->insertPlainText(QString(message.c_str()));
	m_pLoggingWidget->insertPlainText("\n");
}

//-------------------------------------------------------------------------------------------------

Qt::GlobalColor DQMLoggerWidget::getColor(LogLevel level) const
{
	switch (level)
	{
	case DEBUG :
		return Qt::darkBlue;
	case MESSAGE :
		return Qt::darkGreen;
	case WARNING :
		return Qt::darkYellow;
	case ERROR :
		return Qt::darkRed;
	default:
		return Qt::black;
	}
}

//-------------------------------------------------------------------------------------------------

std::string DQMLoggerWidget::getLogMessageHead(LogLevel level) const
{
	switch (level)
	{
	case DEBUG :
		return "[DEBUG]       ";
	case MESSAGE :
		return "[MESSAGE]  ";
	case WARNING :
		return "[WARNING] ";
	case ERROR :
		return "[ERROR]       ";
	default:
		return "";
	}
}

//-------------------------------------------------------------------------------------------------

void DQMLoggerWidget::collapseExpand()
{
	if( m_expanded ) // then collapse
	{
		m_pCollapseExpandWidget->hide();
		QIcon icon(QString(DQMViz_DIR) + "/icons/icon-arrow-right.png");
		m_pCollapseExpandButton->setIcon( icon );
		m_expanded = false;

		emit collapsed();
	}
	else  // then expand
	{
		m_pCollapseExpandWidget->show();
		QIcon icon(QString(DQMViz_DIR) + "/icons/icon-arrow-down.png");
		m_pCollapseExpandButton->setIcon( icon );
		m_expanded = true;

		emit expanded();
	}

	m_pCollapseExpandButton->clearFocus();
}

} 

