/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef FNNWRAPPER_H
#define FNNWRAPPER_H
#include "types.h"

#include "nnfw/neuralnet.h"
#include "nnfw/ionnfw.h"
#include <QObject>
#include <QString>

class FLEX_API FNNWrapper : public QObject {
	Q_OBJECT
public:
	/*! create a Wrapper for the net passed */
	FNNWrapper( nnfw::BaseNeuralNet* net ) {
		this->net = net;
	};

	/*! Save to XML file passed */
	bool saveTo( QString filename ) {
		return nnfw::saveXML( filename.toAscii().data(), net );
	};

	/*! Add a Cluster */
    void addCluster( nnfw::Cluster* c, bool isInput = false, bool isOutput = false ) {
		net->addCluster( c, isInput, isOutput );
		emit clusterAdded( c, isInput, isOutput );
	};

	/*! Add a Cluster and mark it as Input<br>
	 *  Behave exactly the same of addCluster( c, true, false )
	 */
	void addInputCluster( nnfw::Cluster* c ) {
		addCluster( c, true, false );
	};

	/*! Add a Cluster and mark it as Output<br>
	 *  Behave exactly the same of addCluster( c, false, true )
	 */
	void addOutputCluster( nnfw::Cluster* c ) {
		addCluster( c, false, true );
	};

    /*! Remove a Cluster from the network   */
    bool removeCluster( nnfw::Cluster* c ) {
		if ( net->removeCluster(c) ) {
			emit clusterRemoved(c);
			return true;
		}
		return false;
	};

    /*! Mark a Cluster as an Input Cluster of this network     */
    void markAsInput( nnfw::Cluster* c ) {
		net->markAsInput(c);
		emit markedInput(c);
	};

    /*! Mark a Cluster as an Output Cluster of this network     */
    void markAsOutput( nnfw::Cluster* c ) {
		net->markAsOutput(c);
		emit markedOutput(c);
	};

    /*! Eliminate the marks from Cluster passed
     *  \warning if a Cluster have two marker (Input and Output marks) then both marker are removed
     */
    void unmark( nnfw::Cluster* c ) {
		net->unmark(c);
		emit unmarked(c);
	};

    /*! Eliminate the marks from all Cluster present in this networks     */
    void unmarkAll() {
		net->unmarkAll();
		emit unmarkedAll();
	};

    /*! Return true if there isn't Linkers connected with Cluster c      */
    bool isIsolated( nnfw::Cluster* c ) const {
		return net->isIsolated(c);
	};

    /*! Returns the vector of Clusters contained     */
    const nnfw::ClusterVec& clusters() const {
		return net->clusters();
	};

    /*! Returns the vector of Input Clusters contained     */
    const nnfw::ClusterVec& inputClusters() const {
		return net->inputClusters();
	};

    /*! Returns the vector of Output Clusters contained     */
    const nnfw::ClusterVec& outputClusters() const {
		return net->outputClusters();
	};

    /*! Add Linker     */
    void addLinker( nnfw::Linker* l ) {
		net->addLinker(l);
		emit addedLinker(l);
	};

    /*! Remove Linker     */
    bool removeLinker( nnfw::Linker* l ) {
		if ( net->removeLinker(l) ) {
			emit removedLinker(l);
			return true;
		}
		return false;
	};

    /*! Returns the array of Linkers contained     */
    const nnfw::LinkerVec& linkers() const {
		return net->linkers();
	};

    /*! If out is true, return the Linkers outgoing from Cluster c, otherwise return incoming Linkers */
    const nnfw::LinkerVec& linkers( nnfw::Cluster* c, bool out = false ) const {
		return net->linkers(c, out);
	};

    /*! Set the order  */
    void setOrder( nnfw::Updatable* updatables[], unsigned int dim ) {
		net->setOrder( updatables, dim );
		emit orderChanged();
	};

    /*! Set the order */
    void setOrder( nnfw::UpdatableVec& ord ) {
		net->setOrder( ord );
		emit orderChanged();
	};

    /*! Return the order */
    const nnfw::UpdatableVec& order() const {
		return net->order();
	};

    /*! Step */
    void step() {
		net->step();
		emit stepped();
    };

    /*! This randomize the free parameters of the all elements of the neural net<br>
     *  This method call randomize method of every Cluster and Linker inserted
     *  \param min is the lower-bound of random number generator desired
     *  \param max is the upper-bound of random number generator desired
     */
    void randomize( nnfw::Real min, nnfw::Real max ) {
		net->randomize( min, max );
		emit randomized(min,max);
	};

    /*! Return the Updatable with the name specified<br>
     *  Returns NULL-pointer if there's no updatable object whit the name specified<br>
     *  \warning return the first that finds. If you have named different Updatables with same name
     *   there no way to retrieve all of them with this methods... call them with unique name ;-)
     */
    nnfw::Updatable* getByName( const char* name ) {
		return net->getByName( name );
	};

    /*! Return true if the Cluster is in this net */
    bool find( const nnfw::Cluster* c) const {
		return net->find(c);
	};

    /*! Return true if the Linker is in this net */
    bool find( const nnfw::Linker* l ) const {
		return net->find(l);
	};

    /*! Return true if the Updatable object is in this net */
    bool find( const nnfw::Updatable* u ) const {
		return net->find(u);
	};

signals:
	void clusterAdded( nnfw::Cluster*, bool, bool );
	void clusterRemoved( nnfw::Cluster* );
	void markedInput( nnfw::Cluster* );
	void markedOutput( nnfw::Cluster* );
	void unmarked( nnfw::Cluster* );
	void unmarkedAll();
	void addedLinker( nnfw::Linker* );
	void removedLinker( nnfw::Linker* );
	void orderChanged();
	void stepped();
	void randomized( nnfw::Real, nnfw::Real );

private:
	nnfw::BaseNeuralNet* net;
};

#endif
