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

#ifndef PROPERTIZED_H
#define PROPERTIZED_H

#include "types.h"
#include "clonable.h"
#include <map>
#include <string>

/*! \file
 *  \brief The infrastructure for implementing properties
 *
 *
 */


namespace nnfw {

class OutputFunction;
class Propertized;
class AbstractPropertyAccess;

//! Vector of PropertyAccess
typedef VectorData<AbstractPropertyAccess*> PropertyAccessVec;

/*! \brief Incapsulate values of different types/classes in a unified way (like union)
 */
class NNFW_API Variant {
public:
    //! type of registrable data
    typedef enum { t_null=0, t_real, t_int, t_uint, t_char, t_uchar, t_bool, t_string,
                t_realvec, t_realmat, t_outfunction, t_cluster, t_linker, t_propertized,
				t_dataptr } types;

    /*! \name Constructors */
    //@{
    //! Constructor
    Variant();
    //! Copy-Constructor
    Variant( const Variant& src );
    //! Constructor
    Variant( Real d );
    //! Constructor
    Variant( int d );
    //! Constructor
    Variant( u_int d );
    //! Constructor
    Variant( char d );
    //! Constructor
    Variant( unsigned char d );
    //! Constructor
    Variant( bool d );
    //! Constructor
    Variant( const char* d );
    //! Constructor
    Variant( char* d );
    //! Constructor
    Variant( RealVec* d );
    //! Constructor
    Variant( RealMat* d );
    //! Constructor
    Variant( OutputFunction* d );
    //! Constructor
    Variant( Cluster* d );
    //! Constructor
    Variant( Linker* d );
    //! Constructor
    Variant( Propertized* d );
	//! Generic data pointer constructor
	template<typename T>
	Variant( T* d ) {
		dtype = t_dataptr;
	    ddataptr = d;
	};
    //@}
    /*! \name Operators */
    //@{
    //! Assign operator
    Variant& operator=( const Variant& src );
    //@}
    /*! \name Informations about data contained */
    //@{
    /*! Type of data
     */
    types type() const;
    /*! Return the name of type
     */
    const char* typeName() const;
    /*! Return true if the Variant is Null
     */
    bool isNull();
    //@}
    /*! \name Retrieving data */
    //@{
    //! return the Real value
    Real getReal() const;
    //! return the Int value
    int getInt() const;
    //! return the UInt value
    u_int getUInt() const;
    //! return the Char value
    char getChar() const;
    //! return the UChar value
    unsigned char getUChar() const;
    //! return the Bool value
    bool getBool() const;
    //! return the const char* (constant string) value
    const char* getString() const;
    //! return the RealVec value
    RealVec* getRealVec() const;
    //! return the RealMat value
    RealMat* getRealMat() const;
    //! return the OutputFunction value
    OutputFunction* getOutputFunction() const;
    //! return the Cluster value
    Cluster* getCluster() const;
    //! return the Linker value
    Linker* getLinker() const;
    //! return the Propertized value
    Propertized* getPropertized() const;
	//! template to Return Generic data type
	template<typename T>
	T* getDataPtr() const {
	    checkType( t_dataptr );
		T* r = static_cast<T*>( ddataptr );
		if ( r == 0 ) {
			nError() << "Called getDataPtr with wrong type; returning NULL pointer ";
		}
		return r;
	};
    //@}

    /*! Return the name of type passed by argument
     */
    static const char* typeName( types t );

private:
    //! type registered
    types dtype;
    //! Name of type
    static const char* typen[ t_dataptr+1 ];
    Real    dreal;
    int     dint;
    u_int   duint;
    char    dchar;
    unsigned char   duchar;
    bool            dbool;
    char*           dstring;
    RealVec*         drealvec;
    RealMat*         drealmat;
    OutputFunction*  doutfun;
    Cluster*         dcluster;
    Linker*          dlinker;
    Propertized*     dprop;
	void*            ddataptr;

    //! Check type
    void checkType( types t ) const;
};

/*! \brief Encapsulates methods for accessing property data
 *
 *  Details here
 */
class NNFW_API AbstractPropertyAccess : public Clonable {
public:
    /*! \name Constructors & Destructors */
    //@{
    //! Constructor
    AbstractPropertyAccess( const char* name ) {
        u_int size = strlen(name);
        this->namep = new char[size+1];
        strcpy( this->namep, name );
    };
    //! Destructor
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

    //! Set the i-th value of Vector property
    virtual bool set( u_int i, const Variant& data ) = 0;
    //! Return the i-th value of Vector property
    virtual Variant get( u_int i ) const = 0;

    //! Return the name of property
    const char* name() const {
        return namep;
    };
    //! Return true if the property is writable
    bool isWritable() const {
        return writable;
    };
    //! Return true if the property is a Vector value
    bool isVector() const {
        return vectorv;
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
    //! Name of property
    char* namep;
    //! Type of property
    Variant::types typep;
    //! True if the property is writable
    bool writable;
    //! True if the property is a Vector of values
    bool vectorv;
};

/*! \brief Template creation of actual PropertyAccess
 *
 *  you don't have to use it directly, it's automatically instanciated by Propertized's methods
 */
template<class T>
class NNFW_TEMPLATE PropertyAccess : public AbstractPropertyAccess {
public:
    /*! \name Constructors */
    //@{
    //! Constructor
    PropertyAccess( const char* name, Variant::types t, T* o, Variant (T::*g)(), bool (T::*s)( const Variant& ) = 0 )
        : AbstractPropertyAccess( name ) {
        typep = t;
        obj = o;
        vectorv = false;
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
    //! return the value of property
    virtual Variant get() const {
        return (obj->*getPtm)();
    };
    /*! set the property
     *  \warning It doesn't check is the property is writable or not; before calling this 
	 *  method check if the property is writable via isWritable() method
     */
    virtual bool set( const Variant& data ) {
        return (obj->*setPtm)( data );
    };
    //! It always return false
    virtual bool set( u_int, const Variant& ) {
        return false;
    };
    //! It always return a Null Variant
    virtual Variant get( u_int ) const {
        return Variant();
    };

    //! Return the object whom this PropertyAccess is refered to
    virtual Propertized* object() {
        return obj;
    };
    //! Clone this
    virtual PropertyAccess* clone() const {
        return new PropertyAccess( name(), type(), obj, getPtm, setPtm );
    };
    //@}
private:
    T* obj;
    bool (T::*setPtm)( const Variant& );
    Variant (T::*getPtm)();
};

/*! \brief Template creation of actual VectorPropertyAccess
 *
 *  you don't have to use it directly, it's automatically instanciated by Propertized's methods
 */
template<class T>
class NNFW_TEMPLATE VectorPropertyAccess : public AbstractPropertyAccess {
public:
    /*! \name Constructors */
    //@{
    //! Constructor
    VectorPropertyAccess( const char* name, Variant::types t, T* o, Variant (T::*g)(u_int), bool (T::*s)(u_int, const Variant&) = 0 )
        : AbstractPropertyAccess( name ) {
        typep = t;
        obj = o;
        vectorv = true;
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
    //! It always return false
    virtual bool set( const Variant& ) {
        return false;
    };
    //! It always return a Null Variant
    virtual Variant get() const {
        return Variant();
    };
    //! return the value of property
    virtual Variant get( u_int i ) const {
        return (obj->*getPtm)(i);
    };
    /*! set the property
     *  \warning It doesn't check is the property is writable or not; before calling this
	 *  method check if the property is writable via isWritable() method
     */
    virtual bool set( u_int i, const Variant& data ) {
        return (obj->*setPtm)( i, data );
    };
    //! Return the object whom this PropertyAccess is refered to
    virtual Propertized* object() {
        return obj;
    };
    //! Clone this
    virtual VectorPropertyAccess* clone() const {
        return new VectorPropertyAccess( name(), type(), obj, getPtm, setPtm );
    };
    //@}
private:
    T* obj;
    bool (T::*setPtm)( u_int i, const Variant& );
    Variant (T::*getPtm)( u_int i );
};

/*! \brief PropertySettings
 */
typedef std::map< std::string, Variant > PropertySettings;

/*! \brief Implements the capability to access internal data via properties
 *
 *  \par Motivation
 *  This class allow to access data of very different type of Cluster, Linker, etc by a unified interface
 *  via the concept of 'property'. A property is an attribute of a class identified by a string and accessibile
 *  by methods set/getProperty.
 *  \par Description
 *  \par Warnings
 *
 *   <table class="proptable">
 *   <tr><td class="prophead" colspan="5">Properties</td></tr>
 *   <tr><th>Name</th> <th>Type [isVector]</th> <th>Access mode</th> <th>Description</th> <th>Class</th></tr>
 *   <tr><td>typename</td> <td>string</td> <td>read-only</td> <td> Class's type </td> <td>this</td> </tr>
 *   </table>
 */
class NNFW_API Propertized {
public:
    /*! \name Constructors */
    //@{
    //! Constructor
    Propertized();
    //! Destructor
    ~Propertized();

    //@}
    /*! \name Interface */
    //@{

    /*! add a property
     *  \warning this method doesn't check if a property with name name already exist, so pay attention or
     *  previous setting may be overwritten
     */
    template<class T>
    void addProperty( const char* name, Variant::types t, T* obj, Variant (T::*read)(), bool (T::*write)( const Variant& ) = 0 ) {
        PropertyAccess<T>* access = new PropertyAccess<T>( name, t, obj, read, write );
        props[name] = access;
        vecProps.append( access );
    };

    /*! add a property that holds a Vector of Variant
     *  \warning this method doesn't check if a property with name name already exist, so pay attention or
     *  previous setting may be overwritten
     */
    template<class T>
    void addVectorProperty( const char* name, Variant::types t, T* obj, Variant (T::*read)(u_int i), bool (T::*write)(u_int i, const Variant&) = 0 ) {
        VectorPropertyAccess<T>* access = new VectorPropertyAccess<T>( name, t, obj, read, write );
        props[name] = access;
        vecProps.append( access );
    };

    /*! return the property setted
     */
    Variant property( const char* name ) {
        return props[name]->get();
    };

    /*! set the property
     */
    bool setProperty( const char* name, const Variant& data ) {
        if ( props.count( name ) == 0 ) return false;
        AbstractPropertyAccess& p = *(props[name]);
        return ( p.isWritable() ? p.set(data) : false );
    };

    /*! set the i-th Variant of the Vector property
     */
    bool setVectorProperty( const char* name, u_int i, const Variant& data ) {
        if ( props.count( name ) == 0 ) return false;
        AbstractPropertyAccess& p = *(props[name]);
        return ( p.isWritable() ? p.set(i, data) : false );
    };

    /*! configure the properties by a PropertySettings
     *  \warning This method doesn't reports is some settings fails
     */
    void setProperties( PropertySettings& prop );

    /*! Return all PropertyAccess in order of registering
     */
    PropertyAccessVec& properties() const {
        return (PropertyAccessVec&)vecProps;
    };

    /*! Search for property and return it; if the property doesn't exists a NULL pointer will be returned
     */
    AbstractPropertyAccess* propertySearch( const char* name ) const {
        if ( props.count( name ) > 0 ) {
            return ((std::map< std::string, AbstractPropertyAccess* >) props)[name];
        }
        return 0;
    };

    /*! Return the typename (i.e. the name of the Class)
     */
    Variant getTypename() const {
        return Variant( vtypename );
    };

    /*! Return the typename (i.e. the name of the Class)
     */
    Variant getTypename() {
        return Variant( vtypename );
    };

	/*! Create a Variant of type required from String-type Variant passed
	 *  \warning is not possible convert to pointer-type.
	 */
	Variant convertStringTo( const Variant& str, Variant::types t );

    //@}

protected:
    /*! Set the typename<br>
     *  Use this function in all constructor of subclasses, and always set the appropriate typename
     */
    void setTypename( const char* type );

private:
    //! mapping name -> PropertyAccess
    std::map< std::string, AbstractPropertyAccess* > props;
    //! vector of all PropertyAccess in order of registering
    PropertyAccessVec vecProps;
    //! the name of the class
    char* vtypename;
};

}

#endif
