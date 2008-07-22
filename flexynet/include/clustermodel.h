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

#ifndef CLUSTERMODEL_H
#define CLUSTERMODEL_H
#include "types.h"

#include "updatablemodel.h"
#include "nnfw/cluster.h"

class OutputFunctionModel;

/*! \brief Model of Cluster class
 *
 *  Details here
 */
class FLEX_API ClusterModel : public UpdatableModel {
	Q_OBJECT
public:
	/*! create a Model for the cluster passed */
	ClusterModel( nnfw::Cluster* cl );
	/*! destructor */
	~ClusterModel();
	/*! return the num of neurons */
	int numNeurons();
	/*! return the accumulate mode */
	bool isAccumulate();
	/*! return the inputs (it return a const in order to force the using of setInput method) */
	const nnfw::RealVec& inputs();
	/*! return the outputs (it return a const in order to force the using of setOutput method) */
	const nnfw::RealVec& outputs();
	/*! return the OutputFunction */
	OutputFunctionModel* getFunction();
	/*! return the Cluster */
	nnfw::Cluster* nnCluster();

public slots:
	/*! randomize */
	void randomize( nnfw::Real min, nnfw::Real max );
	/*! set Accumulate */
	void setAccumulate( bool );
	/*! set Input */
	void setInput( int i, nnfw::Real value );
	/*! set Inputs */
	void setInputs( const nnfw::RealVec& values );
	/*! reset inputs */
	void resetInputs();
	/*! set Output */
	void setOutput( int i, nnfw::Real value );
	/*! set Outputs */
	void setOutputs( const nnfw::RealVec& values );
	/*! set OutputFunction 
	 * \warning this method will create a clone of func for use internally, so
	 * the getFunction method will return always a different pointer from func
	 */
	void setFunction( OutputFunctionModel* func );
	/*! clone this Cluster Model */
	ClusterModel* clone();

signals:
	/*! emitted when randomized */
	void randomized();
	/*! emitted when change accumulate */
	void changedAccumulate( bool newvalue );
	/*! emitted when change inputs */
	void changedInput( int i, nnfw::Real newvalue );
	/*! emitted when change inputs */
	void changedInputs( const nnfw::RealVec& newvalues );
	/*! emitted when */
	void resetted();
	/*! emitted when change outputs */
	void changedOutput( int i, nnfw::Real newvalue );
	/*! emitted when change outputs */
	void changedOutputs( const nnfw::RealVec& newvalues );
	/*! emitted when change output function */
	void changedFunction( OutputFunctionModel* newfunc );

private:
	nnfw::Cluster* cluster;
	OutputFunctionModel* outfunc;
};

#endif
