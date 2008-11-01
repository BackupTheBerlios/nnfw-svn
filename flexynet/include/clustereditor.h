/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2008 Gianluca Massera <emmegian@yahoo.it>                *
 *                                                                              *
 *  This program is free software; you can redistribute it and/or modify        *
 *  it under the terms of the GNU General Public License as published by        *
 *  the Free Software Foundation; either version 2 of the License, or           *
 *  (at your option) any later version.                                         *
 *                                                                              *
 *  This program is distributed in the hope that it will be useful,             *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 *  GNU General Public License for more details.                                *
 *                                                                              *
 *  You should have received a copy of the GNU General Public License           *
 *  along with this program; if not, write to the Free Software                 *
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  *
 ********************************************************************************/

#ifndef CLUSTEREDITOR_H
#define CLUSTEREDITOR_H
#include "types.h"

#include <QWidget>

class ClusterModel;

/*! \brief ClusterEditor Widget
 *
 *  \par Description
 *  \par Warnings
 */
class FLEX_API ClusterEditor : public QWidget {
	Q_OBJECT
public:
	/*! Constructor */
	ClusterEditor( QWidget* parent = 0, Qt::WindowFlags f = 0 );
	
public slots:
	/*! set the ClusterModel to edit */
	void setClusterModel( ClusterModel* );
	/*! \return the current ClusterModel in editing */
	ClusterModel* cluster();
	
private:
	/*! ClusterModel */
	ClusterModel* model;

};

#endif
