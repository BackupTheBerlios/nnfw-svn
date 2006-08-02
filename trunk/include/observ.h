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

#ifndef OBSERV_H
#define OBSERV_H

/*! \file
 *  \brief This file contains the pattern Observer/Observable ; Don't include this file directly, instead include types.h
 *  Details...
 */

#include <list>
#include "messages.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Observer Class
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 */
class Observer {
public:
    /*! \name Constructors */

    /*! \brief virtual Destructor
     */
    virtual ~Observer() { /* -- Nothing to do -- */ };

    /*! \name Interface */

    /*! \brief called when the Observable notifies changes
     */
    virtual void notify() = 0;
};


/*! \brief Observable Class
 *  \par Motivation
 *  \par Description
 *  \par Warnings
 */
class Observable {
public:
    /*! \name Constructors */

    /*! \brief Constructor
     */
    Observable() : observers() {
        // --- nothing to do
    };

    /*! \name Interface */

    /*! \brief Add a new Observer watching the event i-th
     */
    void addObserver( Observer* ob ) {
        std::list<Observer*>::iterator it = std::find( observers.begin(), observers.end(), ob );
        if ( it != observers.end() ) return;
        observers.push_back( ob );
    };

    /*! \brief Remove the Observer watching the event i-th
     */
    void delObserver( Observer* ob ) {
        std::list<Observer*>::iterator it = std::find( observers.begin(), observers.end(), ob );
        if ( it == observers.end() ) return;
        observers.erase( it );
    };

    /*! \brief Notify the event i-th
     */
    void notifyAll() {
        std::list<Observer*>::iterator it = observers.begin();
        std::list<Observer*>::iterator end = observers.end();
        while( it != end ) {
            (*it)->notify();
            it++;
        }
    };

private:
    //! list of ObserverDelegate indexed by event
    std::list<Observer*> observers;
};

}

#endif