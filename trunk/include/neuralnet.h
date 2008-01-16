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

#ifndef NEURALNET_H
#define NEURALNET_H

/*! \file
 *  \brief This file contains the declaration of Neural Network Class
 *
 *  \todo The removeCluster/removeLinker are very slow because Clusters and Linkers are memorized via std::vector;
 *  for a better performances std::vector should changed to std::map
 *
 */

#include "types.h"
#include "clonable.h"
#include "cluster.h"
#include "linker.h"
#include <map>


namespace nnfw {

/*! \brief The Base Neural Network Class
 *
 * The BaseNeuralNetwork class can seen as a simple container of Clusters and Linkers<br>
 * The relationship among Clusters and Linkers must be specified by cluster and linker constructors; 
 *
 */
class NNFW_API BaseNeuralNet : public Clonable {
public:
	/*! \name Constructors */
	//@{

    /*! Construct an empty neural network
     */
    BaseNeuralNet();

    /*! Destructor
     */
    ~BaseNeuralNet();

	//@}
	/*! \name Interface */
	//@{

    /*! Add a Cluster into the neural network<br>
     * If isInput is true then the Cluster will be considered as an Input Cluster of this network<br>
     * If isOutput is true then the Cluster will be considered as an Output Cluster of this network
     */
    void addCluster( Cluster* c, bool isInput = false, bool isOutput = false );

	/*! Add a Cluster and mark it as Input<br>
	 *  Behave exactly the same of addCluster( c, true, false )
	 */
	void addInputCluster( Cluster* c ) {
		addCluster( c, true, false );
	};

	/*! Add a Cluster and mark it as Output<br>
	 *  Behave exactly the same of addCluster( c, false, true )
	 */
	void addOutputCluster( Cluster* c ) {
		addCluster( c, false, true );
	};

    /*! Remove a Cluster from the network
     */
    bool removeCluster( Cluster* c );

    /*! Mark a Cluster as an Input Cluster of this network
     */
    void markAsInput( Cluster* c );

    /*! Mark a Cluster as an Output Cluster of this network
     */
    void markAsOutput( Cluster* c );

    /*! Eliminate the marks from Cluster passed
     *  \warning if a Cluster have two marker (Input and Output marks) then both marker are removed
     */
    void unmark( Cluster* c );

    /*! Eliminate the marks from all Cluster present in this networks
     */
    void unmarkAll();

    /*! Return true if there isn't Linkers connected with Cluster c 
     */
    bool isIsolated( Cluster* c ) const;

    /*! Returns the vector of Clusters contained
     */
    const ClusterVec& clusters() const;

    /*! Returns the vector of Input Clusters contained
     */
    const ClusterVec& inputClusters() const;

    /*! Returns the vector of Output Clusters contained
     */
    const ClusterVec& outputClusters() const;

    /*! Add Linker
     */
    void addLinker( Linker* l );

    /*! Remove Linker
     */
    bool removeLinker( Linker* );

    /*! Returns the array of Linkers contained
     */
    const LinkerVec& linkers() const;

    /*! If out is true, return the Linkers outgoing from Cluster c, otherwise return incoming Linkers
     */
    const LinkerVec& linkers( Cluster* c, bool out = false ) const;

    /*! Set the order 
     */
    void setOrder( Updatable* updatables[], u_int dim );

    /*! Set the order
	 */
    void setOrder( const UpdatableVec& );

    /*! Return the order
	 */
    const UpdatableVec& order() const {
		return ups;
	};

    /*! Step
     */
    void step() {
        for( u_int i=0; i<dimUps; i++ ) {
			ups[i]->update();
        }
    };

    /*! This randomize the free parameters of the all elements of the neural net<br>
     *  This method call randomize method of every Cluster and Linker inserted
     *  \param min is the lower-bound of random number generator desired
     *  \param max is the upper-bound of random number generator desired
     */
    void randomize( Real min, Real max );

    /*! Return the Updatable with the name specified<br>
     *  Returns NULL-pointer if there's no updatable object whit the name specified<br>
     *  \warning return the first that finds. If you have named different Updatables with same name
     *   there no way to retrieve all of them with this methods... call them with unique name ;-)
     */
    Updatable* getByName( const char* );

    /*! Return true if the Cluster is in this net
     */
    bool find( const Cluster* ) const;

    /*! Return true if the Linker is in this net
     */
    bool find( const Linker* ) const;

    /*! Return true if the Updatable object is in this net
     */
    bool find( const Updatable* ) const;

	/*! Clone this BaseNeuralNet */
	BaseNeuralNet* clone() const;

	//@}

protected:
    /*! Clusters */
    ClusterVec clustersv;
    /*! Input Clusters */
    ClusterVec inclusters;
    /*! Output Clusters */
    ClusterVec outclusters;
    /*! Linkers */
    LinkerVec  linkersv;

    typedef std::map<Cluster*, LinkerVec> LinkVecMap;
    /*! mappa dei linkers entranti (cluster -> vettore linkers entranti) */
    LinkVecMap inLinks;
    /*! map of outgoing linkers (cluster -> vettore linkers uscenti) */
    LinkVecMap outLinks;

    /*! Array of Updateables ordered as specified */
    UpdatableVec ups;
    unsigned int dimUps;
};

}

#endif

