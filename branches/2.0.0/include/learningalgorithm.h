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

#ifndef LEARNINGALGORITHM_H
#define LEARNINGALGORITHM_H

/*! \file
 */

#include "types.h"
#include "neuralnet.h"
#include <QMap>
#include <QVector>
#include <cmath>
#include <factory/parametersettable.h>
#include <configuration/configurationparameters.h>

namespace nnfw {

class NeuralNet;

/*! \brief Pattern object
 *
 *  \par Motivation
 *  The Pattern object represent a configuration (partial or complete) of the network that can
 *  be used by a learning algorithm as model to learn.
 *  A LearningAlgorithm will use Pattern for setup network's inputs, spread network and eventually
 *  calculate the error, and modify the network's paramenters on the basis of neurons activities
 *  in order to get closer to neuron activities presented by the Pattern
 *
 *  \par Description
 *  Pattern simply associates inputs/outputs pairs to Clusters:
 *  \code
 *  Pattern pat;
 *  pat->setInputsOf( anInputCluster, Inputs );
 *  pat->setOutputsOf( anOutputCluster, Outputs );
 *  pat->setInputOutputsOf( anHiddenClusterToReset, Inputs, Outputs );
 *  //--- retrieve stored information:
 *  pat[aCluster].inputs;
 *  pat[anotherCluster].outputs;
 *  //--- add/modify informations:
 *  pat[aNewCluster].outputs.assign( aRealVecOfData );
 *  pat[aNewCluster].outputs += 3.0;
 *  \endcode
 *
 *  \par Warnings
 *  Pay attention when you use operator[] because it silently add new data. Like QMap::operator[]
 *
 */
class NNFW_API Pattern : public ParameterSettableWithConfigureFunction {
public:
	/*! \name Nested Structures */
	//@{
	class PatternInfo {
	public:
		DoubleVector inputs;
		DoubleVector outputs;
	};
	//@}
	/*! \name Constructors */
	//@{
	/*! Construct an empty Pattern */
	Pattern() : ParameterSettableWithConfigureFunction(), pinfo() { /*nothing to do*/ };
	/*! Destructor */
	~Pattern() { /*nothing to do*/ };
	//@}
	/*! \name Interface */
	//@{
	/*! set the inputs associated with Cluster passed */
	void setInputsOf( Cluster*, const DoubleVector& );
	/*! set the outputs associated with Cluster passed */
	void setOutputsOf( Cluster*, const DoubleVector& );
	/*! set the both inputs and outputs associated with Cluster passed */
	void setInputsOutputsOf( Cluster*, const DoubleVector& inputs, const DoubleVector& outputs );
	/*! return stored information if exists, otherwise it return a zero vector */
	DoubleVector inputsOf( Cluster* ) const;
	/*! return stored information if exists, otherwise it return a zero vector */
	DoubleVector outputsOf( Cluster* ) const;
	/*! return the stored information
	 *  \warning it silently create a new one if the Cluster passed is not present */
	PatternInfo& operator[]( Cluster* );
	/**
	 * \brief Configures the object using a ConfigurationParameters object
	 * 
	 * The Pattern has to be declared using a sequence of parameters with the following schema:
	 * \code
	 * [aPattern]
	 * cluster:1 = nameOfCluster
	 * inputs:1 = list of the inputs associated to cluster:1
	 * outputs:1 = list of the outputs associated to cluster:1
	 * ...
	 * cluster:i = nameOf_ith_Cluster
	 * inputs:i = list of the inputs associated to cluster:i
	 * outputs:i = list of the outputs associated to cluster:i
	 * \endcode
	 * Essentialy, the parameters are grouped using the identifier string after the ':' char.
	 * And the inputs and outputs with a given identifier are associated to the cluster with the same identifier.
	 * If a cluster:i is present, then at least inputs:i or outputs:i parameter must be present
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
private:
	mutable QMap<Cluster*, PatternInfo> pinfo;
};

/*! \brief PatternSet type
 *  \par Motivation
 *  It represent a collection of Pattern object. It could be a Learning Set or a Training Set
 *  \par Description
 *  PatternSet is simply a QVector<Pattern>
 *  \par Warnings
 */
typedef QVector<Pattern> PatternSet;

/*! \brief LearningAlgorithm object
 *
 *  The LearningAlgorithm object is a the abstract class from which to implement learning algorithms
 */
class NNFW_API LearningAlgorithm : public ParameterSettableWithConfigureFunction {
public:
	/*! \name Constructors */
	//@{
	/*! Constructor */
	LearningAlgorithm( NeuralNet* net );
	/*! Constructor */
	LearningAlgorithm();
	/*! Destructor */
	virtual ~LearningAlgorithm();
	//@}
	/*! \name Interface */
	//@{
	/*! Set the NeuralNet to learn */
	void setNeuralNet( NeuralNet* net ) {
		netp = net;
		this->neuralNetChanged();
	};
	/*! Return the NeuralNet setted */
	NeuralNet* neuralNet() {
		return netp;
	};
	/*! a single step of learning algorithm */
	virtual void learn() = 0;
	/*! Modify the object tring to learn the pattern passed */
	virtual void learn( const Pattern& ) = 0;
	/*! Modify the object tring to learn all patterns present into PatternSet passed */
	virtual void learnOnSet( const PatternSet& set ) {
		for( int i=0; i<(int)set.size(); i++ ) {
			learn( set[i] );
		}
	};
	/*! Calculate the Mean Square Error respect to Pattern passed */
	virtual double calculateMSE( const Pattern& ) = 0;
	/*! Calculate the Mean Square Error respect to all Patterns passed */
	virtual double calculateMSEOnSet( const PatternSet& set ) {
		double mseacc = 0.0;
		int dim = (int)set.size();
		for( int i=0; i<dim; i++ ) {
			mseacc += calculateMSE( set[i] );
		}
		return mseacc/dim;
	};
	/*! Calculate the Root Mean Square Deviation, i.e. the square root of MSE */
	double calculateRMSD( const Pattern& p ) {
		return sqrt( calculateMSE( p ) );
	};
	/*! Calculate the Root Mean Square Deviation, i.e. the square root of MSE */
	double calculateRMSDOnSet( const PatternSet& p ) {
		return sqrt( calculateMSEOnSet( p ) );
	};
	/*! Utility function for loading a PatternSet from a ConfigurationParameters */
	PatternSet loadPatternSet( ConfigurationParameters& params, QString path, QString prefix );
	/*! Utility function for saving a PatternSet to a ConfigurationParameters */
	void savePatternSet( PatternSet& set, ConfigurationParameters& params, QString prefix );
	//@}
protected:
	/*! Implemented by subclasses for configuring internal structure when the NeuralNet has been setted */
	virtual void neuralNetChanged() = 0;
private:
	NeuralNet* netp;
};

}

#endif

