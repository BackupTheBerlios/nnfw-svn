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

#ifndef LINKERMODEL_H
#define LINKERMODEL_H
#include "types.h"

#include "updatablemodel.h"
#include "nnfw/linker.h"

class ClusterModel;

/*! \brief Model of Linker class
 *
 *  Details here
 */
class FLEX_API LinkerModel : public UpdatableModel {
	Q_OBJECT
public:
	/*! create a Model for the Linker passed */
	LinkerModel( nnfw::Linker* link );
	/*! destructor */
	~LinkerModel();
	/*! return the Linker */
	nnfw::Linker* nnLinker();

public slots:
	/*! return from ClusterModel */
	ClusterModel* from();
	/*! return to ClusterModel */
	ClusterModel* to();
	/*! randomize it */
	void randomize( nnfw::Real min, nnfw::Real max );
	/*! clone this Linker Model */
	LinkerModel* clone();

signals:
	/*! emitted when randomized */
	void randomized();

private:
	ClusterModel* fromm;
	ClusterModel* tom;
	nnfw::Linker* linker;
};

#endif
