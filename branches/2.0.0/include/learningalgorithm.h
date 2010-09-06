/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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
#include "clonable.h"
#include <QMap>
#include <QVector>
#include <cmath>

namespace nnfw {

class BaseNeuralNet;

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
class NNFW_API Pattern {
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
	Pattern() : pinfo() { /*nothing to do*/ };
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
	const DoubleVector& inputsOf( Cluster* ) const;
	/*! return stored information if exists, otherwise it return a zero vector */
	const DoubleVector& outputsOf( Cluster* ) const;
	/*! return the stored information
	 *  \warning it silently create a new one if the Cluster passed is not present */
	PatternInfo& operator[]( Cluster* );
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
class NNFW_API LearningAlgorithm : public Clonable {
public:
	/*! \name Constructors */
	//@{
	/*! Constructor */
	LearningAlgorithm( BaseNeuralNet* net );
	/*! Destructor */
	virtual ~LearningAlgorithm();
	//@}
	/*! \name Interface */
	//@{
	/*! Return the BaseNeuralNet setted */
	BaseNeuralNet* net() {
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
	//@}

private:
	BaseNeuralNet* netp;
};

}

#endif

