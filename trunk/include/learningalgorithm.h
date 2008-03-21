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

#ifndef LEARNINGALGORITHM_H
#define LEARNINGALGORITHM_H

/*! \file
 */

#include "types.h"
#include "neuralnet.h"
#include <map>

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
 *  //--- retrieve stored information
 *  pat[aCluster].inputs;
 *  pat[anotherCluster].outputs;
 *  \endcode
 *
 *  \par Warnings
 *
 *  \todo Supports for storing all properties of Cluster... storing Linkers info ?!?!
 */
class Pattern {
public:
	/*! \name Constructors */
	//@{
	/*! Construct an empty Pattern */
	Pattern();
	/*! Destructor */
	~Pattern();

	//@}
	/*! \name Interface */
	//@{

	//@}
	/*! \name Nested Structures */
	//@{
	class PatternInfo {
	public:
		RealVec inputs;
		RealVec outputs;
	};
	//@}
private:
	std::map<Cluster*, PatternInfo>;
};

/*! \brief LearningAlgorithm object
 *
 *  The LearningAlgorithm object is a the abstract class from which to implement learning algorithms
 */
class NNFW_API LearningAlgorithm {
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
    /*! Modify the object */
    virtual void learn() = 0;

	//@}

private:
	BaseNeuralNet* netp;
};

}

#endif

