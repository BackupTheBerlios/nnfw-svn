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

#ifndef NORMLINKER_H
#define NORMLINKER_H

/*! \file
 */

#include "types.h"
#include "matrixlinker.h"

PROBLEMI:
	- La classe Linker non permette di specificare i Cluster from e to dopo
	  la creazione, ma la factory si aspetta di usare la funzione configure
	  di ParameterSettable dopo la creazione dell'oggetto (che deve avere un
	  costruttore che non prende parametri)
	- Anche se si risolvesse il problema di sopra, come fa un Linker ad
	  ottenere i puntatori ai cluster from e to dai loro nomi (che ricava
	  dai parametri settati)? È lecito lasciare questa cosa alla classe
	  BaseNeuralNet? (Credo di sì, con i parametri un linker costruisce se
	  stesso, poi la rete che lo contiene si preoccupa di connetterlo a
	  dovere)

namespace nnfw {

/*! \brief NormLinker Class
 *
 */
class NNFW_API NormLinker : public MatrixLinker {
public:
	/*! \name Constructors */
	//@{
	/*! Connect clusters with a complete connections */
	NormLinker( Cluster* from, Cluster* to, QString name = "unnamed" );
	/*! Destructor */
	virtual ~NormLinker();
	//@}
	/*! \name Interface */
	//@{
	/*! Performs the euclidean norm calculation */
	void update();
	/*! Clone this NormLinker */
	virtual NormLinker* clone() const;
	/*! Return its typename */
	virtual QString typeName() {
		return "NormLinker";
	};
	#warning IMPLEMENT THESE TWO FUNCTIONS (configure AND save)
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
	virtual void configure(const ConfigurationParameters& params, QString prefix);
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
	// temp data
	DoubleVector temp;
};

}

#endif
