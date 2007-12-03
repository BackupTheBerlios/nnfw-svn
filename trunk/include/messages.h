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

// --- You can't include it directly
#ifndef TYPES_INCLUDES
#error "You can't include messages.h directly; Instead, You have to include types.h"
#endif

#ifndef MESSAGGES_H
#define MESSAGGES_H

namespace nnfw {

/*! \file
 *  \brief This file contains the declaration of classes for displaying warning, error and fatal messages; Don't include this file directly, instead include types.h
 *
 */

//--- This is for internal use; it encapsulate string-data
class nnfwStringPrivate;

/*! A String class that supports Unicode encodings
 */
class NNFW_API nnfwString {
public:
	/*! \name Constructors */
	//@{
	/*! Construct an empty nnfwString */
	nnfwString();
	/*! destructor */
	~nnfwString();
	/*! Copy-Constructor */
	nnfwString( const nnfwString& );
	/*! Converter */
	nnfwString( int );
	/*! Converter */
	nnfwString( unsigned int );
	/*! Converter */
	nnfwString( double );
	/*! Converter */
	nnfwString( float );
	/*! Converter */
	nnfwString( char );
	/*! Converter (it suppose an ASCII encoding) */
	nnfwString( const char* );
	//@}
	/*! \name Interface */
	//@{
	/*! append a nnfwString to this */
	nnfwString& append( const nnfwString& p );
	/*! assignment operator */
	nnfwString& operator=( const nnfwString& left );
	/*! return the UTF8 representation */
	const char* toUtf8() const;
	//@}
private:
	nnfwStringPrivate* prv;
};

/*! Base class for printing messages<br>
 */
class NNFW_API nMessage {
public:
	/*! \name Constructors */
	//@{
	/*! Constructor */
	nMessage();
	/*! Destructor */
	~nMessage() { /* nothing to do */ };
	//@}
protected:
	//--- Helper class for printing messages
	class NNFW_API msgLine {
	public:
		msgLine( nMessage* parent );
		~msgLine();
		msgLine& operator<<( const nnfwString& part ) {
			msg.append( part );
			return (*this);
		};
		void set( const nnfwString& str ) {
			msg = str;
		};
	private:
		nMessage* parent;
		nnfwString msg;
	};
	bool linePending;
	msgLine* pending;
};

/*! Print Warning Messages
 */
class NNFW_API nWarning : public nMessage {
public:
	/*! \name Interface */
	//@{
	/*! Operator for construting a message */
    nMessage::msgLine operator<<( const nnfwString& part ) {
		nMessage::msgLine line(this);
		line.set( nnfwString("== WARNING: ").append(part) );
		return line;
	};
	//@}
};

/*! Print Error Messages
 */
class NNFW_API nError : public nMessage {
public:
	/*! \name Interface */
	//@{
	/*! Operator for construting a message */
    nMessage::msgLine operator<<( const nnfwString& part ) {
		nMessage::msgLine line(this);
		line.set( nnfwString("== ERROR: ").append(part) );
		return line;
	};
	//@}
};

/*! Print Fatal Messages
 */
class NNFW_API nFatal : public nMessage {
public:
	/*! \name Interface */
	//@{
	/*! Operator for construting a message */
    nMessage::msgLine operator<<( const nnfwString& part ) {
		nMessage::msgLine line(this);
		line.set( nnfwString("== FATAL: ").append(part) );
		return line;
	};
	//@}
};

}

#endif


