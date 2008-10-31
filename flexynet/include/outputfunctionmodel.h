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

#ifndef OUTPUTFUNCTIONMODEL_H
#define OUTPUTFUNCTIONMODEL_H
#include "types.h"

#include "propertizedmodel.h"
#include "nnfw/outputfunction.h"

class ClusterModel;

/*! \brief Model of OutputFunction class
 *
 * \par Description
 * \par Warnings
 */
class FLEX_API OutputFunctionModel : public PropertizedModel {
	Q_OBJECT
public:
	/*! create a Model for the OutputFunction passed */
	OutputFunctionModel( nnfw::OutputFunction* cl );
	/*! destructor */
	~OutputFunctionModel();
	/*! return the underlying OutputFunction */
	nnfw::OutputFunction* nnFunction();

public slots:
	/*! apply it */
	void apply( nnfw::RealVec& inputs, nnfw::RealVec& outputs );
	/*! apply it */
	nnfw::Real apply( nnfw::Real in );

signals:

private:
	nnfw::OutputFunction* function;
};

#endif
