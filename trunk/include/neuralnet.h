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

#ifndef NEURALNET_H
#define NEURALNET_H

/*! \file
 *  \brief This file contains the declaration of Neural Network Class
 *
 *  Details ...
 */

#include "types.h"
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
class NNFW_API BaseNeuralNet {
public:
    /*! \brief Construct an empty neural network
     *
     * Details...
     */
    BaseNeuralNet();

    /*! \brief Destructor
     *
     * Details
     */
    ~BaseNeuralNet();

    /*! \brief Add a Cluster into the neural network
     *
     * If isInput is true then the Cluster will be considered as an Input Cluster of this network
     * If isOutput is true then the Cluster will be considered as an Output Cluster of this network
     */
    void addCluster( Cluster* c, bool isInput = false, bool isOutput = false );

	/*! \brief Add a Cluster and mark it as Input
	 *
	 *  Behave exactly the same of addCluster( c, true, false )
	 */
	void addInputCluster( Cluster* c ) {
		addCluster( c, true, false );
	};

	/*! \brief Add a Cluster and mark it as Output
	 *
	 *  Behave exactly the same of addCluster( c, false, true )
	 */
	void addOutputCluster( Cluster* c ) {
		addCluster( c, false, true );
	};

    /*! \brief Remove a Cluster from the network
     *
     * Details...
     */
    bool removeCluster( Cluster* c );

    /*! \brief Mark a Cluster as an Input Cluster of this network
     *
     * Details
     */
    void markAsInput( Cluster* c );

    /*! \brief Mark a Cluster as an Output Cluster of this network
     *
     * Details
     */
    void markAsOutput( Cluster* c );

    /*! \brief Eliminate the marks from Cluster passed
     *
     *  Warning: if a Cluster have two marker (Input and Output marks) then both marker are removed
     */
    void unmark( Cluster* c );

    /*! \brief Eliminate the marks from all Cluster present in this networks
     *
     * Details
     */
    void unmarkAll();

    /*! \brief Return true if there isn't Linkers connected with Cluster c 
     *
     * Details..
     */
    bool isIsolated( Cluster* c ) const;

    /*! \brief Returns the vector of Clusters contained
     *
     * Details...
     */
    const ClusterVec& clusters() const;

    /*! \brief Returns the vector of Input Clusters contained
     *
     *  Gli Input Cluster sono restituiti nell'ordine con il quale sono stati marcati (?!?!?)
     *  
     */
    const ClusterVec& inputClusters() const;

    /*! \brief Returns the vector of Output Clusters contained
     *
     * Details...
     */
    const ClusterVec& outputClusters() const;

    /*! \brief Add Linker
     *
     * Details...
     */
    void addLinker( Linker* l );

    /*! \brief Remove Linker
     *
     * Details..
     */
    bool removeLinker( Linker* );

    /*! \brief Returns the array of Linkers contained
     *
     * Details...
     */
    const LinkerVec& linkers() const;

    /*! \brief If out is true, return the Linkers outgoing from Cluster c, otherwise return incoming Linkers
     *
     * Details ...
     */
    const LinkerVec& linkers( Cluster* c, bool out = false ) const;

    /*! \brief Set the order 
     *
     * Details..
     */
    void setOrder( Updatable* updatables[], u_int dim );

    /*! \brief Set the order
	 *
	 * Details
	 */
    void setOrder( UpdatableVec& );

    /*! \brief Return the order
	 *
	 * Details
	 */
    const UpdatableVec& order() const {
		return ups;
	};

    /*! \brief Step
     *
     *  Details
     */
    void step() {
        for( u_int i=0; i<dimUps; i++ ) {
			ups[i]->update();
        }
    };

    /*! \brief This randomize the free parameters of the all elements of the neural net
     *
     *  This method call randomize method of every Cluster and Linker inserted
     *  \param min is the lower-bound of random number generator desired
     *  \param max is the upper-bound of random number generator desired
     */
    void randomize( Real min, Real max );

    /*! \brief Return the Updatable with the name specified
     *
     *  Returns NULL-pointer if there's no updatable object whit the name specified<br>
     *  WARNING: return the first that finds. If you have named different Updatables with same name
     *   there no way to retrieve all of them with this methods... call them with unique name ;-)
     */
    Updatable* getByName( const char* );

    /*! \brief Return true if the Cluster is in this net
     *
     * Details
     */
    bool find( const Cluster* ) const;

    /*! \brief Return true if the Linker is in this net
     *
     * Details
     */
    bool find( const Linker* ) const;

    /*! \brief Return true if the Updatable object is in this net
     *
     * Details
     */
    bool find( const Updatable* ) const;

protected:
    //! Clusters
    ClusterVec clustersv;
    //! Input Clusters
    ClusterVec inclusters;
    //! Output Clusters
    ClusterVec outclusters;
    //! Linkers
    LinkerVec  linkersv;

    typedef std::map<Cluster*, LinkerVec> LinkVecMap;
    //! mappa dei linkers entranti (cluster -> vettore linkers entranti)
    LinkVecMap inLinks;
    //! map of outgoing linkers (cluster -> vettore linkers uscenti)
    LinkVecMap outLinks;

    //! Array of Updateables ordered as specified
    UpdatableVec ups;
    unsigned int dimUps;
};

}

#endif

