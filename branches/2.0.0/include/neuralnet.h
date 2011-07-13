/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2011 Gianluca Massera <emmegian@yahoo.it>                *
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
 */

#include "types.h"
#include <factory/parametersettable.h>
#include "cluster.h"
#include "linker.h"

namespace nnfw {

/*! \brief The Neural Network Class
 *
 * The NeuralNet class can seen as a simple container of Clusters and Linkers<br>
 * The relationship among Clusters and Linkers must be specified by cluster and linker constructors;
 * 
 * When the NeuralNet is configured from a file, it is configured in the following way:
 *  - the parameters for configuring a NeuralNet are: inputClusters, outputClusters, spreadOrder, clustersList, linkersList;
 *    all parameters consist in a list of the group name where is present the corresponding object configuration
 *  - the only mandatory parameter is spreadOrder
 *  - inputClusters is the list of all Clusters considered the input layer of the NeuralNet
 *  - outputClusters is the list of all Clusters considered the output layer of the NeuralNet
 *  - clustersList and LinkersList are the full list of all Clusters and Linkers to put into the network;
 *    in some cases, these parameters are redundant, because if a Cluster or a Linker is specified into spreadOrder
 *    then it will be automatically put into to the NeuralNet
 *  - all sub-groups of the group containing the parameters of NeuralNet are considered object to be created and
 *    to be put into the NeuralNet
 * 
 * Some examples of configuration files using the INI format.
 * The first example shows how to create a NeuralNet with two Cluster as input and one Cluster as output connected by two linkers. This example uses all parameters even if some are redundant
 * \code
 * inputClusters = input1 input2
 * outputClusters = output1
 * spreadOrder = input1 input2 linker1 linker2 output1
 * clustersList = input1 input2 output1
 * linkersList = linker1 linker2
 * 
 * [input1]
 * type = FakeCluster
 * 
 * [input2]
 * type = SimpleCluster
 * [input2/OutFunction]
 * type = StepFunction
 * min = -1.0
 * max = +1.0
 * threshold = 0.5
 * 
 * [output1]
 * type = BiasedCluster
 * [output1/OutFunction]
 * type = SigmoidFunction
 * lambda = 0.5
 * 
 * [linker1]
 * type = DotLinker
 * from = input1
 * to = output1
 * 
 * [linker2]
 * type = DotLinker
 * from = input2
 * to = output1
 * \endcode
 *
 * The same NeuralNet can be configured without specifing clustersList and linkersList with the following file INI:
 * \code
 * inputClusters = input1 input2
 * outputClusters = output1
 * spreadOrder = input1 input2 linker1 linker2 output1
 * 
 * [input1]
 * type = FakeCluster
 * 
 * [input2]
 * type = SimpleCluster
 * [input2/OutFunction]
 * type = StepFunction
 * min = -1.0
 * max = +1.0
 * threshold = 0.5
 * 
 * [output1]
 * type = BiasedCluster
 * [output1/OutFunction]
 * type = SigmoidFunction
 * lambda = 0.5
 * 
 * [linker1]
 * type = DotLinker
 * from = input1
 * to = output1
 * 
 * [linker2]
 * type = DotLinker
 * from = input2
 * to = output1
 * \endcode
 * 
 * Let's suppose that you want to change the NeuralNet of the example above removing input1 as input Cluster of the NeuralNet
 * then the two following configurations results in the same NeuralNet:
 * \code
 * inputClusters = input2
 * outputClusters = output1
 * spreadOrder = input1 input2 linker1 linker2 output1
 * 
 * ...
 * \endcode
 * \code
 * inputClusters = input2
 * outputClusters = output1
 * spreadOrder = input2 linker1 linker2 output1
 * 
 * ...
 * \endcode
 * Because even if in the last example the input1 is not specified in any of the NeuralNet parameters,
 * it will be created because [input1] is a subgroup of the main group where the NeuralNet parameters are present
 * 
 * To understand better this situation, let's put all NeuralNet parameters into a group called [Net1], the above example
 * will appear in this way:
 * \code
 * [Net1]
 * inputClusters = input2
 * outputClusters = output1
 * spreadOrder = input2 linker1 linker2 output1
 * 
 * [input1]
 * type = FakeCluster
 * 
 * [input2]
 * type = SimpleCluster
 * [input2/OutFunction]
 * type = StepFunction
 * min = -1.0
 * max = +1.0
 * threshold = 0.5
 * 
 * [output1]
 * type = BiasedCluster
 * [output1/OutFunction]
 * type = SigmoidFunction
 * lambda = 0.5
 * 
 * [linker1]
 * type = DotLinker
 * from = input1
 * to = output1
 * 
 * [linker2]
 * type = DotLinker
 * from = input2
 * to = output1
 * \endcode
 * 
 * Now the input1 will be not automatically added to the NeuralNet because the group [input1] is not anymore a subgroup of
 * the group ([Net1]) where the NeuralNet parameters are present.
 * In this case, if you want that also [input1] will be put into the NeuralNet you have two options:
 *  - change the group of [input1] to [Net1/input1]; but this also means that the corresponding Cluster will
 *    be named "Net1/input1" instead of "input1"
 *  - add a clustersList parameter where you declare the complete list of Clusters including input1; in this way
 *    the name of the Cluster will remain "input1"
 * The two alternatives appear in the following way:
 * \code
 * [Net1]
 * inputClusters = input2
 * outputClusters = output1
 * spreadOrder = input2 linker1 linker2 output1
 * 
 * [Net1/input1]
 * type = FakeCluster
 * ...
 * \endcode
 * \code
 * [Net1]
 * inputClusters = input2
 * outputClusters = output1
 * spreadOrder = input2 linker1 linker2 output1
 * clustersList = input1 input2 output1
 * 
 * [input1]
 * type = FakeCluster
 * ...
 * \endcode
 * 
 */
class NNFW_API NeuralNet : public ParameterSettableWithConfigureFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Construct an empty neural network */
	NeuralNet();
	/*! Destructor */
	~NeuralNet();
	//@}
	/*! \name Interface */
	//@{
	/*! Set the name of the NeuralNet
	 *  It's useful when loading from ConfigurationParameters more than NeuralNet or from
	 *  LearningAlgorithms subclasses
	 */
	void setName( QString name ) {
		namev = name;
	};
	/*! return the name of the NeuralNet */
	QString name() {
		return namev;
	};
	/*! Add a Cluster into the neural network<br>
	 * If isInput is true then the Cluster will be considered as an Input Cluster of this network<br>
	 * If isOutput is true then the Cluster will be considered as an Output Cluster of this network */
	void addCluster( Cluster* c, bool isInput = false, bool isOutput = false );
	/*! Add a Cluster and mark it as Input<br>
	 *  Behave exactly the same of addCluster( c, true, false ) */
	void addInputCluster( Cluster* c ) {
		addCluster( c, true, false );
	};
	/*! Add a Cluster and mark it as Output<br>
	 *  Behave exactly the same of addCluster( c, false, true ) */
	void addOutputCluster( Cluster* c ) {
		addCluster( c, false, true );
	};
	/*! Remove a Cluster from the network */
	bool removeCluster( Cluster* c );
	/*! Mark a Cluster as an Input Cluster of this network */
	void markAsInput( Cluster* c );
	/*! Mark a Cluster as an Output Cluster of this network */
	void markAsOutput( Cluster* c );
	/*! Eliminate the marks from Cluster passed
	 *  \warning if a Cluster have two marker (Input and Output marks) then both marker are removed */
	void unmark( Cluster* c );
	/*! Eliminate the marks from all Cluster present in this networks */
	void unmarkAll();
	/*! Return true if there isn't any Linker connected with Cluster c */
	bool isIsolated( Cluster* c ) const;
	/*! Returns the vector of Clusters contained */
	ClusterList clusters() const;
	/*! Returns the vector of Input Clusters contained */
	ClusterList inputClusters() const;
	/*! Returns the vector of Output Clusters contained */
	ClusterList outputClusters() const;
	/*! Returns the vector of Hidden Clusters contained (i.e. UnMarked Clusters) */
	ClusterList hiddenClusters() const;
	/*! Add Linker */
	void addLinker( Linker* l );
	/*! Remove Linker */
	bool removeLinker( Linker* );
	/*! Returns the array of Linkers contained */
	LinkerList linkers() const;
	/*! If out is true, return the Linkers outgoing from Cluster c, otherwise return incoming Linkers */
	LinkerList linkers( Cluster* c, bool out = false ) const;
	/*! Set the order */
	void setOrder( Updatable* updatables[], unsigned int dim );
	/*! Set the order */
	void setOrder( const UpdatableList& );
	/*! Return the order */
	UpdatableList order() const {
		return ups;
	};
	/*! Step */
	void step() {
		for( unsigned int i=0; i<dimUps; i++ ) {
			ups[i]->update();
		}
	};
	/*! This randomize the free parameters of the all elements of the neural net<br>
	 *  This method call randomize method of every Cluster and Linker inserted
	 *  \param min is the lower-bound of random number generator desired
	 *  \param max is the upper-bound of random number generator desired
	 */
	void randomize( double min, double max );
	/*! Search into the net for the presence of an Updatable with name aName;
	 *  on success set the pointer aPointer and return it,
	 *  otherwise it set aPointer to zero and return zero.<br>
	 *  This allow to use it both into an if-statement and an assignment:
	 *  \code
	 *  BiasedCluster* bias1;
	 *  BiasedCluster* bias2;
	 *  if ( byName("aName", bias1 ) ) {
	 *      //--- ok, there is a BiasedCluster with name "aName"
	 *      //--- now bias1 points to the BiasedCluster with name "aName"
	 *  } else {
	 *      //--- error, there is no BiasedCluster with that name
	 *      //--- now bias1 is NULL
	 *  }
	 *  //--- you can also use it for assignment:
	 *  bias2 = byName("aName", bias1);
	 *  \endcode
	 */
	template<class PointerTo>
	PointerTo byName( QString aName, PointerTo& aPointer ) {
		aPointer = dynamic_cast<PointerTo>( getByName(aName) );
		return aPointer;
	};
	/*! Return the Updatable with the name specified<br>
	 *  Returns NULL-pointer if there's no updatable object whit the name specified<br>
	 *  \warning return the first that finds. If you have named different Updatables with same name
	 *   there no way to retrieve all of them with this methods... call them with unique name ;-)
	 */
	Updatable* getByName( QString );
	/*! Return true if the Cluster is in this net
	 */
	bool find( const Cluster* ) const;
	/*! Return true if the Linker is in this net
	 */
	bool find( const Linker* ) const;
	/*! Return true if the Updatable object is in this net
	 */
	bool find( const Updatable* ) const;
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 *
	 * \param params the configuration parameters object with parameters to
	 *               use
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters. This is guaranteed to end with the
	 *               separator character when called by the factory, so you
	 *               don't need to add one
	 */
	virtual void configure(ConfigurationParameters& params, QString prefix);
	/**
	 * \brief Save the actual status of parameters into the ConfigurationParameters object passed
	 *
	 * \param params the configuration parameters object on which save actual parameters
	 * \param prefix the prefix to use to access the object configuration
	 *               parameters.
	 */
	virtual void save(ConfigurationParameters& params, QString prefix);
	//@}
protected:
	/*! name of this NeuralNet */
	QString namev;
	/*! Clusters */
	ClusterList clustersv;
	/*! Input Clusters */
	ClusterList inclusters;
	/*! Output Clusters */
	ClusterList outclusters;
	/*! unmarked Clusters */
	ClusterList hidclusters;
	/*! Linkers */
	LinkerList  linkersv;

	typedef QMap<QString, Cluster*> ClustersMap;
	/*! map name -> Cluster* */
	ClustersMap clsMap;

	typedef QMap<Cluster*, LinkerList> LinkVecMap;
	/*! mappa dei linkers entranti (cluster -> vettore linkers entranti) */
	LinkVecMap inLinks;
	/*! map of outgoing linkers (cluster -> vettore linkers uscenti) */
	LinkVecMap outLinks;

	typedef QMap<QString, Linker*> LinkersMap;
	/*! map name -> Linker* */
	LinkersMap lksMap;

	/*! Array of Updateables ordered as specified */
	UpdatableList ups;
	unsigned int dimUps;
};

}

#endif

