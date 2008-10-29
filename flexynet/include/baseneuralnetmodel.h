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

#ifndef BASENEURALNETMODEL_H
#define BASENEURALNETMODEL_H
#include "types.h"

#include "nnfw/neuralnet.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QStringList>

class ClusterModel;
class LinkerModel;
class UpdatableModel;

class FLEX_API BaseNeuralNetModel : public QObject {
	Q_OBJECT
public:
	/*! create a new BaseNeuralNet */
	BaseNeuralNetModel( );
	/*! create the BaseNeuralNet specified on file */
	BaseNeuralNetModel( QString filename );

public slots:
	/*! Save to XML file passed */
	bool saveTo( QString filename );
	/*! return true if model has changes not saved */
	bool hasChanges();
	/*! Add a Cluster */
	void addCluster( ClusterModel* c, bool isInput = false, bool isOutput = false );
	/*! Add a Cluster and mark it as Input<br>
	 *  Behave exactly the same of addCluster( c, true, false )
	 */
	void addInputCluster( ClusterModel* c ) {
		addCluster( c, true, false );
	};
	/*! Add a Cluster and mark it as Output<br>
	 *  Behave exactly the same of addCluster( c, false, true )
	 */
	void addOutputCluster( ClusterModel* c ) {
		addCluster( c, false, true );
	};
    /*! Remove a Cluster from the network */
    bool removeCluster( ClusterModel* c );
    /*! Mark a Cluster as an Input Cluster of this network */
    void markAsInput( ClusterModel* c );
    /*! Mark a Cluster as an Output Cluster of this network */
    void markAsOutput( ClusterModel* c );
    /*! Eliminate the marks from Cluster passed
     *  \warning if a Cluster have two marker (Input and Output marks) then both marker are removed
     */
	void unmark( ClusterModel* c );
	/*! Eliminate the marks from all Cluster present in this networks */
	void unmarkAll();
	/*! Return true if there isn't Linkers connected with Cluster c */
	bool isIsolated( ClusterModel* c ) const;
	/*! Returns the vector of Clusters contained */
	QVector<ClusterModel*> clusters() const;
	/*! Returns the vector of Input Clusters contained */
	QVector<ClusterModel*> inputClusters() const;
	/*! Returns the vector of Output Clusters contained */
	QVector<ClusterModel*> outputClusters() const;
	/*! Add Linker */
	void addLinker( LinkerModel* l );
	/*! Remove Linker */
	bool removeLinker( LinkerModel* l );
	/*! Returns the array of Linkers contained */
	QVector<LinkerModel*> linkers() const;
	/*! If out is true, return the Linkers outgoing from Cluster c, otherwise return incoming Linkers */
	QVector<LinkerModel*> linkers( ClusterModel* c, bool out = false ) const;
	/*! Set the order */
	void setOrder( QStringList order );
	/*! Return the order */
	QStringList order() const;
	/*! Step */
	void step();
	/*! This randomize the free parameters of the all elements of the neural net<br>
	 *  This method call randomize method of every Cluster and Linker inserted
	 *  \param min is the lower-bound of random number generator desired
	 *  \param max is the upper-bound of random number generator desired
	 */
	void randomize( nnfw::Real min, nnfw::Real max );
    /*! Return the Updatable with the name specified<br>
     *  Returns NULL-pointer if there's no updatable object whit the name specified<br>
     *  \warning return the first that finds. If you have named different Updatables with same name
     *   there no way to retrieve all of them with this methods... call them with unique name ;-)
     */
    UpdatableModel* getByName( QString name );
	/*! Return true if it is in this net */
	bool find( UpdatableModel* c) const;

signals:
	/*! emitted when ... */
	void clusterAdded( ClusterModel*, bool, bool );
	/*! emitted when ... */
	void clusterRemoved( ClusterModel* );
	/*! emitted when ... */
	void markedAsInput( ClusterModel* );
	/*! emitted when ... */
	void markedAsOutput( ClusterModel* );
	/*! emitted when ... */
	void unmarked( ClusterModel* );
	/*! emitted when ... */
	void unmarkedAll();
	/*! emitted when ... */
	void linkerAdded( LinkerModel* );
	/*! emitted when ... */
	void linkerRemoved( LinkerModel* );
	/*! emitted when ... */
	void orderChanged( QStringList );
	/*! emitted when ... */
	void stepped();
	/*! emitted when ... */
	void randomized();

private:
	nnfw::BaseNeuralNet* net;
	bool ischanged;
};

#endif
