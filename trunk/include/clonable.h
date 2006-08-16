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

#ifndef CLONABLE_H
#define CLONABLE_H

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief Clonable interface
 *
 *  Clonable Objects
 */
class  Clonable {
public:
    /*! \name Virtual Destrucor */
    //@{
    //! Destructor
    virtual ~Clonable() { /* Nothing to do */ };
    //@}

    /*! \name Interface */
    //@{

    //! Clone method returns a new allocated clone of this object
    virtual Clonable* clone() const = 0;

    //@}
};

}

#endif
