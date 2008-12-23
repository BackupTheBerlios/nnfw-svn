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

#ifndef NNFWFACTORY_H
#define NNFWFACTORY_H

#include "types.h"
#include "clonable.h"
#include <QMap>
#include <QString>

/*! \file
 *  \brief This file contains the Factory pattern for generating Cluster, Linker and other things ;-)
 *  This class have static method for generating Cluster, Linker and Common neural network structure<br>
 *  It's possible to add new object in the Factory using the Creator template on your class and then register it
 *
 */


namespace nnfw {

/*! \brief Abstract Modifier for Updatable objects
 */
class NNFW_API AbstractModifier : public Clonable {
public:
	/*! \name Interface */
	//@{

	/*! set the learnable object */
	virtual void setUpdatable( Updatable* tolearn ) {
		learnable = tolearn;
	};

	/*! apply the rule changing the Updatable object */
	virtual void rule( double r, const RealVec& x, const RealVec& y ) const = 0;

	/*! Virtual Copy-Constructor */
	virtual AbstractModifier* clone() const = 0;
	//@}
protected:
	// --- learnable object
	Updatable* learnable;
};

/*! \brief Factory Class
 *
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 *   the methods that creates object (createCluster, createLinker, etc) doesn't check if the Creator registered
 *   returns the right type of object... so, pay attention to register the right kind of Creator,
 *   otherwise you will get a runtime-error when you try to use them
 */
class NNFW_API Factory {
public:
	/*! \name Static Interface */
	//@{

	/*! Return a Modifier for Updatable object passed */
	static AbstractModifier* createModifierFor( Updatable* objectToLearn );

	/*! Register a new Modifier for type passed */
	static bool registerModifier( const AbstractModifier& m, const char* type );

    /*! Initialization of static data \internal */
    static void initFactory();
	//@}

private:
    /*! The constructor is private, because there is no reason to instantiate this class
     */
    Factory() {
        // You can't instantiate this class
    };

    /*! is Init ?? */
    static bool isInit;

	/*! Map of registered Modifiers */
	static QMap<QString, AbstractModifier*> modtypes;
};

}

#endif
