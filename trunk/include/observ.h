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

// --- You can't include it directly
#ifndef TYPES_INCLUDES
#error "You can't include realmat.h directly; Instead, You have to include types.h"
// --- follow define avoid to get a lot of understandable error !
#define OBSERV_H
#endif

#ifndef OBSERV_H
#define OBSERV_H

/*! \file
 *  \brief This file contains the pattern Observer/Observable ; Don't include this file directly, instead include types.h
 *  Details...
 */

#include <list>
#include <algorithm>


namespace nnfw {

/*! \brief Event Class
 *  \par Motivation
 *  Incapsulate information about 'the why of notification to Observer by Observable'
 *  \par Description
 *  \par Warnings
 */
class NotifyEvent {
public:
    //! Constructor
    NotifyEvent( int type = 0 ) : etype(type) { /* Nothing to do */ };
    //! Return the type of this Notification Event 
    int type() const {
        return etype;
    };
protected:
    //! type of event
    int etype;
};

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
    virtual void notify( const NotifyEvent& ) = 0;
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
    void notifyAll( const NotifyEvent& event = NotifyEvent() ) {
        std::list<Observer*>::iterator it = observers.begin();
        std::list<Observer*>::iterator end = observers.end();
        while( it != end ) {
            (*it)->notify( event );
            it++;
        }
    };

private:
    //! list of ObserverDelegate indexed by event
    std::list<Observer*> observers;
};

}

#endif
