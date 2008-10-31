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

#ifndef MODELMAPS_H
#define MODELMAPS_H
#include "types.h"

#include <QObject>
#include <QMap>
#include "nnfw/cluster.h"
#include "nnfw/linker.h"
#include "nnfw/outputfunction.h"

class PropertizedModel;
class UpdatableModel;
class ClusterModel;
class LinkerModel;
class OutputFunctionModel;

/*! \brief keep trace of all models allocated associating them to nnfw classes
 *
 * \par Description
 * \par Warnings
 */
class FLEX_API ModelMaps : public QObject {
	Q_OBJECT
public:
	/*! return the instance of it */
	static ModelMaps* instance();

	/*! delete an entry */
	void delEntry( nnfw::Cluster* cl );
	/*! delete an entry */
	void delEntry( nnfw::Linker* link );
	/*! delete an entry */
	void delEntry( nnfw::OutputFunction* func );
	/*! get a ClusterModel; return NULL if it doesn't exists */
	ClusterModel* modelOf( nnfw::Cluster* cl );
	/*! get a LinkerModel; return NULL if it doesn't exists */
	LinkerModel* modelOf( nnfw::Linker* cl );
	/*! get a UpdatableModel; return NULL if it doesn't exists */
	UpdatableModel* modelOf( nnfw::Updatable* cl );
	/*! get a OutputFunctionModel; return NULL if it doesn't exists */
	OutputFunctionModel* modelOf( nnfw::OutputFunction* cl );

private:
	/*! register a model \internal */
	void addEntry( ClusterModel* model, nnfw::Cluster* cl );
	/*! register a model \internal */
	void addEntry( LinkerModel* model, nnfw::Linker* cl );
	/*! register a model \internal */
	void addEntry( OutputFunctionModel* model, nnfw::OutputFunction* cl );

	static bool inited;
	static ModelMaps* instancev;

	QMap<nnfw::Cluster*, ClusterModel*> mapcluster;
	QMap<nnfw::Linker*, LinkerModel*> maplinker;
	QMap<nnfw::OutputFunction*, OutputFunctionModel*> mapfunction;

	//--- this class follow singleton pattern
	ModelMaps();
};

#endif
