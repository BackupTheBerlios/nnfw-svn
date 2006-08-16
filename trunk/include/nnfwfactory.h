/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2006 Gianluca Massera <emmegian@yahoo.it>                     *
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
#include <map>
#include <string>

/*! \file
 *  \brief This file contains the Factory pattern for generating Cluster, Linker and other things ;-)
 *  This class have static method for generating Cluster, Linker and Common neural network structure<br>
 *  It's possible to add new object in the Factory re-implementing the Creator for your class and then register it
 */

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Class for encapsulate special parameter to pass to ClusterCreator class and its subclasses
 */
class ClusterCreatorParameters {
public:
    //! Constructor
    ClusterCreatorParameters( u_int numN, const char* name ) {
        this->numNeurons = numN;
        u_int size = strlen(name);
        this->name = new char[size+1];
        strcpy( this->name, name );
    };
    //! \brief Number of neurons
    u_int numNeurons;
    //! \brief Name of Cluster
    char* name;
};

/*! \brief Class for encapsulate special parameter to pass to LinkerCreator class and its subclasses
 */
class LinkerCreatorParameters {
public:
    //! Constructor
    LinkerCreatorParameters( Cluster* from, Cluster* to, const char* name ) {
        this->from = from;
        this->to = to;
        u_int size = strlen(name);
        this->name = new char[size+1];
        strcpy( this->name, name );
    };
    //! \brief Cluster 'from'
    Cluster* from;
    //! \brief Cluster 'to'
    Cluster* to;
    //! \brief Name of Linker
    char* name;
};

/*! \brief ClusterCreator base class
 */
class ClusterCreator : public Clonable {
public:
    //! \brief for suppresing annoying warnings ;-)
    virtual ~ClusterCreator() { };

    /*! \brief create a new Cluster
     */
    virtual Cluster* create( const ClusterCreatorParameters& param ) const = 0;

    /*! \brief Virtual Copy-Constructor
     */
    virtual ClusterCreator* clone() const = 0;
};

/*! \brief LinkerCreator base class
 */
class LinkerCreator : public Clonable {
public:
    //! \brief for suppresing annoying warnings ;-)
    virtual ~LinkerCreator() { };

    /*! \brief create a new Linker
     */
    virtual Linker* create( const LinkerCreatorParameters& param ) const = 0;

    /*! \brief Virtual Copy-Constructor
     */
    virtual LinkerCreator* clone() const = 0;
};

/*! \brief Factory Class
 */
class Factory {
public:

    /*! \brief Create a Cluster of class type
     */
    static Cluster* createCluster( const char* type, const ClusterCreatorParameters& param );

    /*! \brief Create a Linker of class type
     */
    static Linker* createLinker( const char* type, const LinkerCreatorParameters& param );

    /*! \brief Register a new Cluster type
     *
     *  Return true on successuful insertion<br>
     *  It use the clone() method for copying the ClusterCreator
     */
    static bool registerCluster( const ClusterCreator& c, const char* type );

    /*! \brief Register a new Linker type
     *
     *  Return true on successuful insertion<br>
     *  It use the clone() method for copying the LinkerCreator
     */
    static bool registerLinker( const LinkerCreator& c, const char* type );

private:
    /*! \brief The constructor is private, because there is no reason to instantiate this class
     */
    Factory() {
        // You can't instantiate this class
    };

    /*! \brief Initialization of static data
     */
    static void initFactory();

    /*! \brief is Init ??
     */
    static bool isInit;

    /*! \brief Map of registered Cluster types
     */
    static std::map<std::string, ClusterCreator*> clustertypes;

    /*! \brief Map of registered Linker types
     */
    static std::map<std::string, LinkerCreator*> linkertypes;
};

}

#endif
