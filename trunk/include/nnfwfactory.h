/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                     *
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
#include "propertized.h"
#include "clonable.h"
#include <map>
#include <string>

/*! \file
 *  \brief This file contains the Factory pattern for generating Cluster, Linker and other things ;-)
 *  This class have static method for generating Cluster, Linker and Common neural network structure<br>
 *  It's possible to add new object in the Factory using the Creator template on your class and then register it
 *
 */


namespace nnfw {

/*! \brief Abstract Creator of Propertized objects
 */
class NNFW_API AbstractCreator : public Clonable {
public:
    /*! \name Interface */
    //@{

    /*! create an instance of Propertized from PropertySettings specified
     */
    virtual Propertized* create( PropertySettings& param ) const = 0;

    /*! Virtual Copy-Constructor
     */
    virtual AbstractCreator* clone() const = 0;
    //@}
};

/*! \brief Template facility to create Creator specialization
 */
template<class T>
class NNFW_TEMPLATE Creator : public AbstractCreator {
    /*! \name Interface */
    //@{

    /*! create a new Propertized Object
     */
    virtual Propertized* create( PropertySettings& param ) const {
        return ( new T(param) );
    };

    /*! Virtual Copy-Constructor
     */
    virtual Creator* clone() const {
        return new Creator();
    };
    //@}
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

    /*! Create a Cluster of class type
     */
    static Cluster* createCluster( const char* type, PropertySettings& param );

    /*! Create a Linker of class type
     */
    static Linker* createLinker( const char* type, PropertySettings& param );

    /*! Create a OutputFunction of class type
     */
    static OutputFunction* createOutputFunction( const char* type, PropertySettings& param );

    /*! Create a Propertized object different from Cluster, Linker and OutputFunction type
     */
    static Propertized* createPropertized( const char* type, PropertySettings& param );

    /*! Register a new Cluster type<br>
     *  Return true on successuful insertion<br>
     *  It use the clone() method for copying the ClusterCreator
     */
    static bool registerCluster( const AbstractCreator& c, const char* type );

    /*! Register a new Linker type<br>
     *  Return true on successuful insertion<br>
     *  It use the clone() method for copying the LinkerCreator
     */
    static bool registerLinker( const AbstractCreator& c, const char* type );

    /*! Register a new OutputFunction type<br>
     *  Return true on successuful insertion<br>
     *  It use the clone() method for copying the OutputFunctionCreator
     */
    static bool registerOutputFunction( const AbstractCreator& c, const char* type );

    /*! Register a new Propertized type<br>
     *  Return true on successuful insertion<br>
     *  It use the clone() method for copying the OutputFunctionCreator
     * \warning Never use this method for registering Cluster, Linker or OutputFunction subclasses;
	 *  These hierarchy has own special methods in factory:
	 *  registerCluster, registerLinker and registerOutputFunction
     */
    static bool registerPropertized( const AbstractCreator& c, const char* type );

private:
    /*! The constructor is private, because there is no reason to instantiate this class
     */
    Factory() {
        // You can't instantiate this class
    };

    /*! Initialization of static data
     */
    static void initFactory();

    /*! is Init ??
     */
    static bool isInit;

    /*! Map of registered Cluster types
     */
    static std::map<std::string, AbstractCreator*> clustertypes;

    /*! Map of registered Linker types
     */
    static std::map<std::string, AbstractCreator*> linkertypes;

    /*! Map of registered OutputFunction types
     */
    static std::map<std::string, AbstractCreator*> outfuntypes;

    /*! Map of registered Propertized types
     */
    static std::map<std::string, AbstractCreator*> proptypes;
};

}

#endif
