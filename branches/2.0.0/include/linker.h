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

#ifndef LINKERS_H
#define LINKERS_H

/*! \file
 *  \brief This file contains the declaration of the linkers implemented
 *
 */

#include "types.h"
#include "cluster.h"
#include "updatable.h"

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

/*! \brief Abstract Linker Class. This define the common interface among Linkers
 *
 */
class NNFW_API Linker : public Updatable {
public:
	/*! \name Constructors */
	//@{
	/*! Construct */
	Linker( Cluster* from, Cluster* to, QString name = "unnamed" );
	//@}
	/*! \name Interface */
	//@{
	/*! Return the Cluster From */
	Cluster* from() const {
		return fromc;
	};
	/*! Return the Cluster to */
	Cluster* to() const {
		return toc;
	};
	/*! Returns the 'size' of the linker<br>
	 * The actual means of 'size' will be specified by sub-classes that implementes concrete linkers
	 */
	virtual unsigned int size() const = 0;
	/*! Randomize the parameters of the Linker<br>
	 * The parameters randomized by this method will be specified by sub-classes
	 */
	virtual void randomize( double min, double max ) = 0;
	//@}
private:
	/*! incoming Cluster */
	Cluster* fromc;
	/*! outgoing Cluster */
	Cluster* toc;
};

}

#endif
