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

#ifndef PROPERTIZED_H
#define PROPERTIZED_H

#include "types.h"
#include "clonable.h"
#include <map>
#include <string>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

class Propertized;
class AbstractPropertyAccess;

//! \brief Vector of PropertyAccess
typedef VectorData<AbstractPropertyAccess*> PropertyAccessVec;

/*! \brief Incapsulate values of different types/classes in a unified way (like union)
 */
class Variant {
public:
    //! type of registrable data
    typedef enum { t_null=0, t_real, t_int, t_uint, t_char, t_uchar, t_bool, t_string,
                t_realvec, t_realmat, t_propertized } types;

    /*! \name Constructors */
    //@{

    //! \brief Constructor
    Variant() {
        dtype = t_null;
    };

    //! \brief Copy-Constructor
    Variant( const Variant& src ) {
        dtype = src.dtype;
        switch( dtype ) {
        case t_null: break;
        case t_real: dreal = src.dreal; break;
        case t_int: dint = src.dint; break;
        case t_uint: duint = src.duint; break;
        case t_char: dchar = src.dchar; break;
        case t_uchar: duchar = src.duchar; break;
        case t_bool: dbool = src.dbool; break;
        case t_string: dstring = src.dstring; break;
        case t_realvec: drealvec = src.drealvec; break;
        case t_realmat: drealmat = src.drealmat; break;
        case t_propertized: dprop = src.dprop; break;
        }
    };

    //! \brief Constructor
    Variant( Real d ) {
        dtype = t_real;
        dreal = d;
    };

    //! \brief Constructor
    Variant( int d ) {
        dtype = t_int;
        dint = d;
    };

    //! \brief Constructor
    Variant( u_int d ) {
        dtype = t_uint;
        duint = d;
    };

    //! \brief Constructor
    Variant( char d ) {
        dtype = t_char;
        dchar = d;
    };

    //! \brief Constructor
    Variant( unsigned char d ) {
        dtype = t_uchar;
        duchar = d;
    };

    //! \brief Constructor
    Variant( bool d ) {
        dtype = t_int;
        dbool = d;
    };

    //! \brief Constructor
    Variant( const char* d ) {
        dtype = t_string;
        dstring = d;
    };

    //! \brief Constructor
    Variant( RealVec* d ) {
        dtype = t_realvec;
        drealvec = d;
    };

    //! \brief Constructor
    Variant( RealMat* d ) {
        dtype = t_realmat;
        drealmat = d;
    };

    //! \brief Constructor
    Variant( Propertized* d ) {
        dtype = t_propertized;
        dprop = d;
    };

    //@}
    /*! \name Operators */
    //@{

    //! \brief Assign operator
    Variant& operator=( const Variant& src ) {
        dtype = src.dtype;
        switch( dtype ) {
        case t_null: break;
        case t_real: dreal = src.dreal; break;
        case t_int: dint = src.dint; break;
        case t_uint: duint = src.duint; break;
        case t_char: dchar = src.dchar; break;
        case t_uchar: duchar = src.duchar; break;
        case t_bool: dbool = src.dbool; break;
        case t_string: dstring = src.dstring; break;
        case t_realvec: drealvec = src.drealvec; break;
        case t_realmat: drealmat = src.drealmat; break;
        case t_propertized: dprop = src.dprop; break;
        }
        return (*this);
    };

    //@}
    /*! \name Informations about data contained */
    //@{

    /*! \brief Type of data
     */
    types type() const {
        return dtype;
    };

    /*! \brief Return the name of type
     */
    const char* typeName() const {
        return typen[dtype];
    };

    /*! \brief Return the name of type passed by argument
     */
    static const char* typeName( types t ) {
        return typen[t];
    };

    //@}
    /*! \name Retrieving data */
    //@{

    //! return the Real value
    Real getReal() const {
        checkType( t_real );
        return dreal;
    };
    //! return the Int value
    int getInt() const {
        checkType( t_int );
        return dint;
    };
    //! return the UInt value
    u_int getUInt() const {
        checkType( t_uint );
        return duint;
    };
    //! return the Char value
    char getChar() const {
        checkType( t_char );
        return dchar;
    };
    //! return the UChar value
    unsigned char getUChar() const {
        checkType( t_uchar );
        return duchar;
    };
    //! return the Bool value
    bool getBool() const {
        checkType( t_bool );
        return dbool;
    };
    //! return the const char* (constant string) value
    const char* getString() const {
        checkType( t_string );
        return dstring;
    };
    //! return the RealVec value
    const RealVec* getRealVec() const {
        checkType( t_realvec );
        return drealvec;
    };
    //! return the RealMat value
    const RealMat* getRealMat() const {
        checkType( t_realmat );
        return drealmat;
    };
    //! return the Propertized value
    const Propertized* getPropertized() const {
        checkType( t_propertized );
        return dprop;
    };

    //@}

private:
    //! type registered
    types dtype;
    //! Name of type
    static const char* typen[ t_propertized+1 ];
    Real    dreal;
    int     dint;
    u_int   duint;
    char    dchar;
    unsigned char   duchar;
    bool            dbool;
    const char*     dstring;
    RealVec*         drealvec;
    RealMat*         drealmat;
    Propertized*     dprop;

    //! Check type
    void checkType( types t ) const {
        if ( t != dtype ) {
            char msg[100];
            sprintf( msg, "Attempt to access a %s type instead of the right type %s; unpredictable result", typen[t], typen[dtype] );
            nnfwMessage( NNFW_ERROR, msg );
        }
        return;
    };
};

/*! \brief Encapsulates methods for accessing property data
 */
class AbstractPropertyAccess : public Clonable {
public:
    /*! \name Constructors */
    //@{
    //! \brief Constructor
    AbstractPropertyAccess( const char* name ) {
        u_int size = strlen(name);
        this->namep = new char[size+1];
        strcpy( this->namep, name );
    };
    //! \brief Destructor
    virtual ~AbstractPropertyAccess() {
        delete []namep;
    };
    //@}
    /*! \name Interface */
    //@{
    //! Set the value of property
    virtual bool set( const Variant& data ) = 0;
    //! Return the value of property
    virtual Variant get() const = 0;
    //! Return the name of property
    const char* name() const {
        return namep;
    };
    //! Return true if the property is writable
    bool isWritable() const {
        return writable;
    };
    //! Return the type of property
    Variant::types type() const {
        return typep;
    };
    //! Return the object whom this AbstractPropertyAccess is refered to
    virtual Propertized* object() = 0;
    //! Clone this
    virtual AbstractPropertyAccess* clone() const = 0;
    //@}
protected:
    //! \brief Name of property
    char* namep;
    //! \brief Type of property
    Variant::types typep;
    //! \brief True if the property is writable
    bool writable;
};

/*! \brief Template creation of actual PropertyAccess
 *
 *  you don't have to use it directly, it's automatically instanciated by Propertized's methods
 */
template<class T>
class PropertyAccess : public AbstractPropertyAccess {
public:
    /*! \name Constructors */
    //@{
    //! \brief Constructor
    PropertyAccess( const char* name, Variant::types t, T* o, Variant (T::*g)(), bool (T::*s)( const Variant& ) = 0 )
        : AbstractPropertyAccess( name ) {
        typep = t;
        obj = o;
        if ( s == 0 ) {
            writable = false;
            setPtm = 0;
        } else {
            writable = true;
            setPtm = s;
        }
        getPtm = g;
    };
    //@}
    /*! \name Interface */
    //@{
    //! \brief return the value of property
    virtual Variant get() const {
        return (obj->*getPtm)();
    };
    /*! \brief set the property
     *
     *  \par Warnings:
     *  It doesn't check is the property is writable or not; before calling this method check if the property is
     *  writable via isWritable() method
     */
    virtual bool set( const Variant& data ) {
        return (obj->*setPtm)( data );
    };
    //! Return the object whom this PropertyAccess is refered to
    virtual Propertized* object() {
        return obj;
    };
    //! \brief Clone this
    virtual PropertyAccess* clone() const {
        return new PropertyAccess( name(), type(), obj, getPtm, setPtm );
    };
    //@}
private:
    T* obj;
    bool (T::*setPtm)( const Variant& );
    Variant (T::*getPtm)();
};


/*! \brief Implements the capability to access internal data via properties
 *
 *  \par Motivation
 *  This class allow to access data of very different type of Cluster, Linker, etc by a unified interface
 *  via the concept of 'property'. A property is an attribute of a class identified by a string and accessibile
 *  by methods set/getProperty.
 *  \par Description
 *  \par Warnings
 */
class Propertized {
public:
    /*! \name Constructors */
    //@{
    //! \brief Constructor
    Propertized();
    //! \brief Destructor
    ~Propertized();

    //@}
    /*! \name Interface */
    //@{

    /*! \brief add a property
     *
     *  \par Warnings:
     *  this method doesn't check if a property with name name already exist, so pay attention or
     *  previous setting may be overwritten
     */
    template<class T>
    void addProperty( const char* name, Variant::types t, T* obj, Variant (T::*read)(), bool (T::*write)( const Variant& ) = 0 ) {
        PropertyAccess<T>* access = new PropertyAccess<T>( name, t, obj, read, write );
        props[name] = access;
        vecProps.append( access );
    };

    /*! \brief return the property setted
     */
    Variant property( const char* name ) {
        return props[name]->get();
    };

    /*! \brief set the property
     */
    bool setProperty( const char* name, const Variant& data ) {
        AbstractPropertyAccess& p = *(props[name]);
        return ( p.isWritable() ? p.set(data) : false );
    };

    /*! \brief Return all PropertyAccess in order of registering
     */
    PropertyAccessVec& properties() const {
        return (PropertyAccessVec&)vecProps;
    };
    //@}

private:
    //! mapping name -> PropertyAccess
    std::map< std::string, AbstractPropertyAccess* > props;
    //! vector of all PropertyAccess in order of registering
    PropertyAccessVec vecProps;
};

}

#endif
