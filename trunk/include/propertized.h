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

/*! \file
 *  \brief The infrastructure for implementing properties
 *
 *  \todo Variant doesn't support a generic vector of data, i.e. VectorData\<Variant\> or VectorData\<Propertized*\>
 */

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

class OutputFunction;
class Propertized;
class AbstractPropertyAccess;

//! \brief Vector of PropertyAccess
typedef VectorData<AbstractPropertyAccess*> PropertyAccessVec;

/*! \brief Incapsulate values of different types/classes in a unified way (like union)
 *  \nosubgrouping
 */
class Variant {
public:
    //! type of registrable data
    typedef enum { t_null=0, t_real, t_int, t_uint, t_char, t_uchar, t_bool, t_string,
                t_realvec, t_realmat, t_outfunction, t_cluster, t_linker, t_propertized } types;

    /*! \name Constructors
     */
    //@{

    //! \brief Constructor
    Variant();

    //! \brief Copy-Constructor
    Variant( const Variant& src );

    //! \brief Constructor
    Variant( Real d );

    //! \brief Constructor
    Variant( int d );

    //! \brief Constructor
    Variant( u_int d );

    //! \brief Constructor
    Variant( char d );

    //! \brief Constructor
    Variant( unsigned char d );

    //! \brief Constructor
    Variant( bool d );

    //! \brief Constructor
    Variant( const char* d );

    //! \brief Constructor
    Variant( RealVec* d );

    //! \brief Constructor
    Variant( RealMat* d );

    //! \brief Constructor
    Variant( OutputFunction* d );

    //! \brief Constructor
    Variant( Cluster* d );

    //! \brief Constructor
    Variant( Linker* d );

    //! \brief Constructor
    Variant( Propertized* d );

    //@}
    /*! \name Operators */
    //@{

    //! \brief Assign operator
    Variant& operator=( const Variant& src );

    //@}
    /*!  \name Informations about data contained */
    //@{

    /*! \brief Type of data
     */
    types type() const;

    /*! \brief Return the name of type
     */
    const char* typeName() const;

    /*! \brief Return true if the Variant is Null
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
    const RealVec* getRealVec() const;
    //! return the RealMat value
    const RealMat* getRealMat() const;
    //! return the OutputFunction value
    const OutputFunction* getOutputFunction() const;
    //! return the Cluster value
    const Cluster* getCluster() const;
    //! return the Linker value
    const Linker* getLinker() const;
    //! return the Propertized value
    const Propertized* getPropertized() const;

    //@}

    /*! \brief Return the name of type passed by argument
     */
    static const char* typeName( types t );

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
    char*           dstring;
    RealVec*         drealvec;
    RealMat*         drealmat;
    OutputFunction*  doutfun;
    Cluster*         dcluster;
    Linker*          dlinker;
    Propertized*     dprop;

    //! Check type
    void checkType( types t ) const;
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
 *  \par Properties
 *   <table>
 *   <tr><th>Name</th> <th>Type</th> <th>Access mode</th> <th>Description</th></tr>
 *   <tr><td>typename</td> <td>String</td> <td>read-only</td> <td> name of the class </td> </tr>
 *   </table>
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
        if ( props.count( name ) == 0 ) return false;
        AbstractPropertyAccess& p = *(props[name]);
        return ( p.isWritable() ? p.set(data) : false );
    };

    /*! \brief configure the properties by a PropertySettings
     *
     *  This method doesn't reports is some settings fails
     */
    void setProperties( PropertySettings& prop );

    /*! \brief Return all PropertyAccess in order of registering
     */
    PropertyAccessVec& properties() const {
        return (PropertyAccessVec&)vecProps;
    };

    /*! \brief Search for property and return it; if the property doesn't exists a NULL pointer will be returned
     */
    AbstractPropertyAccess* propertySearch( const char* name ) const {
        if ( props.count( name ) > 0 ) {
            return ((std::map< std::string, AbstractPropertyAccess* >) props)[name];
        }
        return 0;
    };

    /*! \brief Return the typename (i.e. the name of the Class)
     */
    Variant getTypename() const {
        return Variant( vtypename );
    };

    /*! \brief Return the typename (i.e. the name of the Class)
     */
    Variant getTypename() {
        return Variant( vtypename );
    };

    //@}

protected:
    /*! \brief Set the typename
     *
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
