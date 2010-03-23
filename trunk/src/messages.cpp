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

#include "types.h"
#include <iostream>
#include <QString>
#include <cstdio>

extern void exit( int status );


namespace nnfw {

class nnfwStringPrivate {
public:
	QString qstr;
};

nnfwString::nnfwString() {
	prv = new nnfwStringPrivate();
}

nnfwString::~nnfwString() {
	delete prv;
}

nnfwString::nnfwString( const nnfwString& src ) {
	prv = new nnfwStringPrivate();
	prv->qstr = src.prv->qstr;
}

nnfwString::nnfwString( int i ) {
	prv = new nnfwStringPrivate();
	prv->qstr = QString("%1").arg(i);
}

nnfwString::nnfwString( u_int i ) {
	prv = new nnfwStringPrivate();
	prv->qstr = QString("%1").arg(i);
}

nnfwString::nnfwString( double v ) {
	prv = new nnfwStringPrivate();
	prv->qstr = QString("%1").arg(v);
}

nnfwString::nnfwString( float v ) {
	prv = new nnfwStringPrivate();
	prv->qstr = QString("%1").arg(v);
}

nnfwString::nnfwString( char c ) {
	prv = new nnfwStringPrivate();
	prv->qstr = QString("%1").arg(c);
}

nnfwString::nnfwString( const char* s ) {
	prv = new nnfwStringPrivate();
	prv->qstr = QString("%1").arg(s);
}

nnfwString& nnfwString::append( const nnfwString& p ) {
	prv->qstr.append( p.prv->qstr );
	return (*this);
}

nnfwString& nnfwString::operator=( const nnfwString& left ) {
	prv->qstr = left.prv->qstr;
	return (*this);
}

bool nnfwString::operator==( const nnfwString& left ) {
	return ( prv->qstr == left.prv->qstr );
}

const char* nnfwString::toUtf8() const {
	return prv->qstr.toUtf8().data();
}

nMessage::nMessage() {
	linePending = false;
}

nMessage::msgLine::msgLine( nMessage* parent ) : msg() {
	this->parent = parent;
	if ( parent->linePending ) {
		delete (parent->pending);
	}
	parent->linePending = true;
	parent->pending = this;
}

nMessage::msgLine::~msgLine() {
	printf( "%s\n", msg.toUtf8() );
	parent->linePending = false;
    return;
}

}

