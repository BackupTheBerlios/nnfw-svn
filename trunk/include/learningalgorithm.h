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

#ifndef LEARNINGALGORITHM_H
#define LEARNINGALGORITHM_H

/*! \file
 */

#include "types.h"
#include "modifier.h"
#include <map>

namespace nnfw {

class BaseNeuralNet;

/*! \brief LearningAlgorithm object
 *
 *  The LearningAlgorithm object is a the abstract class from which to implement learning algorithms
 */
class NNFW_API LearningAlgorithm {
public:
	/*! \name Constructors */
	//@{

    //! Constructor
    LearningAlgorithm( BaseNeuralNet* net );
    //! Destructor
    virtual ~LearningAlgorithm();

	//@}
	/*! \name Interface */
	//@{

	//! Return the BaseNeuralNet setted
	BaseNeuralNet* net() {
		return netp;
	};
    //! Modify the object
    virtual void learn() = 0;

	/*! Configure a VectorModifier for learning the Cluster's property specified
     *  \warning the previous modifier setted will be destroyed
	 *
	 *  \param c Cluster to learn
	 *  \param propname the property of Cluster to be learned (it must be a RealVec)
	 *  \param vm the VectorModifier that modify the propname of Cluster specified
	 */
	void setVectorModifier( Cluster* c, const char* propname, const VectorModifier& vm );

	/*! Configure a MatrixModifier for learning the Cluster's property specified
     *  \warning the previous modifier setted will be destroyed
	 *
	 *  \param c Cluster to learn
	 *  \param propname the property of Cluster to be learned (it must be a RealMat)
	 *  \param mm the MatrixModifier that modify the propname of Cluster specified
	 */
	void setMatrixModifier( Cluster* c, const char* propname, const MatrixModifier& mm );

	/*! Configure a VectorModifier for learning the Linker's property specified
     *  \warning the previous modifier setted will be destroyed
	 *
	 *  \param c Linker to learn
	 *  \param propname the property of Linker to be learned (it must be a RealVec)
	 *  \param vm the VectorModifier that modify the propname of Linker specified
	 */
	void setVectorModifier( Linker* c, const char* propname, const VectorModifier& vm );

	/*! Configure a MatrixModifier for learning the Linker's property specified
     *  \warning the previous modifier setted will be destroyed
	 *
	 *  \param c Linker to learn
	 *  \param propname the property of Linker to be learned (it must be a RealMat)
	 *  \param mm the MatrixModifier that modify the propname of Linker specified
	 */
	void setMatrixModifier( Linker* c, const char* propname, const MatrixModifier& mm );

	//@}

private:
	BaseNeuralNet* netp;
	//! map from Cluster to Modifier
	std::map<Cluster*, Modifier*> cmods;
	//! map from Linker to Modifier
	std::map<Linker*, Modifier*> lmods;
};

}

#endif

