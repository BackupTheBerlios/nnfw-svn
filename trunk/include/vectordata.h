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
#define VECTORDATA_H
#endif

#ifndef VECTORDATA_H
#define VECTORDATA_H

/*! \file
 *  \brief This file contains the template VectorData ; Don't include this file directly, instead include types.h
 *  Details...
 */

#include "memutils.h"
#include <vector>
#include "messages.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief VectorData Class
 *  \par Motivation
 *  Create a VectorData abstract type for storing data in dynamic and efficient way
 *  \par Description
 *  \par Warnings
 */
template<class T>
class VectorData : private Observer, public Observable {
public:
    //! Type of Notification
    typedef enum { datachanged = 1, datadestroying = 2 } t_notify;

    /*! \name Constructors */
    //@{

    /*! \brief Default Constructor
     */
    VectorData()
        : Observer(), Observable() {
        vsize = 0;
        allocated = 0;
        data = 0;
        view = false;
        observed = 0;
        idstart = 0;
        idend = 0;
    };

    /*! \brief Construct a vector of dimension size setting all values to defaul constructor of T
     */
    VectorData( u_int size ) 
        : Observer(), Observable() {
        vsize = size;
        allocated = size;
        if ( vsize == 0 ) {
            data = 0;
        } else {
            data = new T[vsize];
            memoryZeroing( data, vsize );
        }
        // --- view attribute
        view = false;
        observed = 0;
        idstart = 0;
        idend = 0;
    };

    /*! \brief Construct a vector of dimension size setting all the values as specified
     */
    VectorData( u_int size, T& value )
        : Observer(), Observable() {
        vsize = size;
        allocated = size;
        (vsize==0) ? (data = 0) : (data = new T[vsize]);
        for(u_int i = 0; i<size; i++) {
            data[i] = value;
        }
        // --- view attribute
        view = false;
        observed = 0;
        idstart = 0;
        idend = 0;
    };
    
    /*! \brief Construct a VectorData view
     */
    VectorData( VectorData<T>& src, u_int idStart, u_int idEnd )
        : Observer(), Observable() {
        if ( idStart > src.vsize || idEnd > src.vsize || idStart >= idEnd ) {
            nnfwMessage( NNFW_ERROR, "Wrongs indexes specified in VectorData constructor; using 0 and src.size()" );
            idstart = 0;
            idend = src.size();
        } else {
            idstart = idStart;
            idend = idEnd;
        }
        data = (src.data) + idstart;
        vsize = idend - idstart;
        allocated = 0;
        view = true;
        src.addObserver( this );
        observed = &src;
    };

    /*! \brief Construct by copying data from const T* vector
     */
    VectorData( const T* r, u_int dim )
        : Observer(), Observable() {
        data = new T[dim];
        vsize = dim;
        allocated = dim;
        memoryCopy( data, r, dim );
        // --- view attribute
        view = false;
        observed = 0;
        idstart = 0;
        idend = 0;
    };

    /*! \brief Copy-Constructor
     */
    VectorData( const VectorData& src )
        : Observer(), Observable() {
        /* --- even if the source VectorData is a view,
           --- the copy-constructor create a new fresh copy of data
        if ( src.isView() ) {
            view = true;
            observed = src.observed;
            observed->addObserver( this );
            idstart = src.idstart;
            idend = src.idend;
            data = (observed->data) + idstart;
            vsize = idend - idstart;
            allocated = 0;
        } else {
        --- */
            vsize = src.vsize;
            allocated = vsize;
            data = new T[allocated];
            memoryCopy( data, src.data, vsize );
            view = false;
            observed = 0;
        /* } */
    }

    /*! \brief Destructor
     */
    ~VectorData() {
        notifyAll( NotifyEvent( datadestroying ) );
        if ( view ) {
            observed->delObserver( this );
        } else {
            delete []data;
        }
    }
    //@}
    /*! \name Informations about VectorData */
    //@{

    /*! \brief Return the size of VectorData
     */
    u_int size() const {
        return vsize;
    };

    /*! \brief Return True if it is a VectorData view
     */
    bool isView() const {
        return view;
    };

    /*! \brief Equal Operator
     */
    bool operator==( const VectorData<T>& b ) {
        if ( this->size() != b.size() ) return false;
        VectorData<T>& self = *this;
        for( u_int i=0; i<size(); i++ ) {
            if ( self[i] != b[i] ) {
                return false;
            }
        }
        return true;
    };

    /*! \brief Not-Equal Operator
     */
    bool operator!=( const VectorData<T>& b ) {
        return !( *this == b );
    };

    //@}
    /*! \name Operations on VectorData */
    //@{

    /*! \brief Set all values to default value of T, in other words to T()
     */
    void zeroing() {
        memoryZeroing( data, vsize );
    };

    /*! \brief Set all values to value
     */
    void setAll( const T& value ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = value;
        }
    };

    /*! \brief Assign to first num element the value passed
     */
    VectorData<T>& assign( u_int num, const T& value ) {
#ifdef NNFW_DEBUG
        if ( num > vsize ) {
            nnfwMessage( NNFW_ERROR, "Wrong number of elements passed to assign method" );
            num = vsize;
        }
#endif
        for( u_int i=0; i<num; i++ ) {
            data[i] = value;
        }
        return (*this);
    };

    /*! \brief Assignment method. The sizes of VectorData must be the same.
     */
    VectorData<T>& assign( const VectorData<T>& src ) {
#ifdef NNFW_DEBUG
        if ( vsize != src.vsize ) {
            nnfwMessage( NNFW_ERROR, "Wrong number of elements between VectorData to assign method" );
            return (*this);
        }
#endif
        memoryCopy( data, src.data, vsize );
        return (*this);
    };

    /*! \brief Assignment method. The size of data to be assigned is specified by size parameter.
     *
     *  This method allow to copy data from a VectorData to another VectorData with different size.
     */
    VectorData<T>& assign( const VectorData<T>& src, u_int sizec ) {
#ifdef NNFW_DEBUG
        if ( vsize < sizec || src.size() < sizec ) {
            nnfwMessage( NNFW_ERROR, "Wrong size specified in assign method" );
            return (*this);
        }
#endif
        memoryCopy( data, src.data, sizec );
        return (*this);
    };

	/*! \name Functions that assume that T has define standard operations (+, !, ==...) */
    //@{

	/*! \brief Compare two vectors. The sizes of VectorData must be the same.
	 *
	 *  Element-by-element comparison between the two arrays. 
	 *  It returns a boolean array of the same size, with elements set to true (1) where the values are the same,
	 *  and elements set to false (0) where they are not.
     */
    VectorData<int>& compare( const VectorData<T>& b, VectorData<int>& comparison ) {
#ifdef NNFW_DEBUG
        if ( vsize != b.vsize ) {
            nnfwMessage( NNFW_ERROR, "Wrong number of elements between VectorData to compare method" );
            return (*this);
        }
#endif
        VectorData<T>& self = *this;
		comparison.resize(size());
        for( u_int i=0; i<size(); i++ ) {
            if ( self[i] == b[i] ) {
                comparison[i] = true;
            }
			else {
				comparison[i] = false;
			}
        }
		return comparison;
    };

	/*! \brief Not operation.
     */
    VectorData<int>& neg( VectorData<int>& result ) {
		result.resize(size());
        for( u_int i=0; i<size(); i++ ) {
			result[i] = ! (*this)[i] ;
        }
		return result;
    };

	/*! \brief Sum up the values of the vector.
     */
    int sum() {
		int s = 0;
        for( u_int i=0; i<size(); i++ ) {
			s += (*this)[i];
		}
		return s;
    };
	
    //@}

	/*! \brief Indexing operator
     *  Boundary check activated only when DEBUG if defined
     */
    T& operator[]( u_int index ) {
#ifdef NNFW_DEBUG
        if( index >= vsize ) {
            nnfwMessage( NNFW_ERROR, "Accessing elements outside boundary" );
            return data[0];
        }
#endif
        return data[index];
    };

    /*! \brief Indexing operator (Const Version)
     *  Boundary check activated only when DEBUG if defined
     */
    const T& operator[]( u_int index ) const {
#ifdef NNFW_DEBUG
        if( index >= vsize ) {
            nnfwMessage( NNFW_ERROR, "Accessing elements outside boundary" );
            return data[0];
        }
#endif
        return data[index];
    };

    /*! \brief Resize the VectorData
     */
    void resize( u_int newsize ) {
        if ( view ) {
            nnfwMessage( NNFW_ERROR, "It's not possible resize RealVec views" );
            return;
        }
        if ( allocated < newsize ) {
            if ( allocated == 0 ) {
                allocated = newsize+20;
                data = new T[allocated];
            } else {
                allocated = newsize+20;
                T* tmp = new T[allocated];
                memoryCopy( tmp, data, vsize );
                delete []data;
                data = tmp;
            }
        }
        if ( newsize > vsize ) {
            memoryZeroing( data+vsize, newsize-vsize );
        }
        vsize = newsize;
        // --- Notify the viewers
        notifyAll( NotifyEvent( datachanged ) );
    };

    /*! \brief Append an element; the dimesion increase by one
     */
    void append( const T& value ) {
        resize( vsize+1 );
        data[vsize-1] = value;
    };

    /*! \brief Append Operator
     */
    VectorData<T>& operator<<( const T& value ) {
        append( value );
        return (*this);
    };
    //@}
    /*! \name Operations on VectorData views */
    //@{

    /*! \brief Configure the indexes of starting and ending of this VectorData view
     *  If VectorData is not a view, then it will shows an error message
     */
    void setView( u_int idStart, u_int idEnd ) {
        if ( !view ) {
            nnfwMessage( NNFW_ERROR, "setView can be called only if VectorData is a view" );
            return;
        }
        if ( idStart > observed->vsize || idEnd > observed->vsize || idStart >= idEnd ) {
            nnfwMessage( NNFW_ERROR, "Wrongs indexes specified in VectorData setView; using 0 and observed->size()" );
            idstart = 0;
            idend = observed->size();
        }
        idstart = idStart;
        idend = idEnd;
        data = (observed->data) + idstart;
        vsize = idend - idstart;
        // --- Propagate Notify to sub-viewers
        notifyAll( NotifyEvent( datachanged ) );
    };

    /*! \brief Convert this VectorData to a view of VectorData src passed
     */
    void convertToView( VectorData<T>& src, u_int idStart, u_int idEnd ) {
        if ( observed == (&src) ) {
            setView( idStart, idEnd );
            //nnfwMessage( NNFW_ERROR, "Already view of VectorData passed to convertToView; method ignored" );
            return;
        }
        if ( view ) {
            // detach previous view
            observed->delObserver( this );
        } else if ( allocated > 0 ) {
            // remove previous data allocated
            delete []data;
        }
    
        if ( idStart > src.vsize || idEnd > src.vsize || idStart >= idEnd ) {
            nnfwMessage( NNFW_ERROR, "Wrongs indexes specified in convertToView; using 0 and src.size()" );
            idstart = 0;
            idend = src.size();
        } else {
            idstart = idStart;
            idend = idEnd;
        }
        data = (src.data) + idstart;
        vsize = idend - idstart;
        allocated = 0;
        view = true;
        observed = &src;
        observed->addObserver( this );
    };

    //@}
    /*! \name STL compatibility */
    //@{

    class vectordataIterator;
    friend class vectordataIterator;

    //! Value type
    typedef T value_type;
    //! Reference type
    typedef T& reference;
    //! Const Reference type
    typedef const T& const_reference;
    //! Iterator
    typedef vectordataIterator iterator;
    //! Const Iterator
    typedef const vectordataIterator const_iterator;
    //! Size type
    typedef size_t size_type;
    //! Difference pointer type
    typedef ptrdiff_t difference_type;

    /*! \brief Max size allowed
     */
    size_type max_size() {
        return 300000; // <- FIXME it must return the maximum space of memory allocable
    };

    /*! \brief Is Empty
     */
    bool empty() {
        return (vsize==0);
    };

    /*! \brief Append an element
     */
    void push_back( const T& value ) {
        resize( vsize+1 );
        data[vsize-1] = value;
    };

    /*! \brief Iterator at initial position
     */
    iterator begin() {
        return vectordataIterator( *this );
    };
    /*! \brief Iterator at initial position
     */
    const_iterator begin() const {
        return vectordataIterator( ((VectorData&)*this) );
    };

    /*! \brief Iterator at past-end position
     */
    iterator end() {
        return vectordataIterator( *this, vsize );
    };
    /*! \brief Iterator at past-end position
     */
    const_iterator end() const {
        return vectordataIterator( ((VectorData&)*this), vsize );
    };

    /*! \brief Erase method
     */
    iterator erase( iterator pos ) {
        if ( view ) {
            nnfwMessage( NNFW_ERROR, "you can't erase element from a VectorData view" );
        } else {
            u_int id = pos.getIndex();
            for( u_int i=id; i<vsize-1; i++ ) {
                data[id] = data[id+1];
            }
            vsize = vsize-1;
            notifyAll( NotifyEvent( datachanged ) );
        }
        return (pos);
    };

    /*! \brief Erase All elements
     */
    void clear() {
        if ( view ) {
            nnfwMessage( NNFW_ERROR, "you can't clear a VectorData view" );
        } else {
            vsize = 0;
            notifyAll( NotifyEvent( datachanged ) );
        }
    };

    /*! \brief Inner-class implements iterator over VectorData
     */
    class vectordataIterator : public std::iterator<std::bidirectional_iterator_tag,T,ptrdiff_t> {
    public:
        //! create the iterator
        vectordataIterator( VectorData& data, u_int startId = 0 ) : vecdata(data), id(startId) { /*nothing to do*/ };
        //! Copy-Constructor
        vectordataIterator( const vectordataIterator& src ) : vecdata(src.vecdata), id(src.id) { /* nothing to do */  };
        //! Assignement operator
        vectordataIterator& operator=( const vectordataIterator& src ) {
            vecdata = src.vecdata;
            id = src.id;
            return (*this);
        };
        //! equal operator
        bool operator==( const vectordataIterator& x ) const {
            return ( &vecdata == &(x.vecdata) && id == x.id );
        };
        //! not-equal operator
        bool operator!=( const vectordataIterator& x ) const {
            return ( &vecdata != &(x.vecdata) || id != x.id );
        };
        //! Accessing element (const version)
        const T& operator*() const {
            return vecdata[id];
        };
        //! Accessing element (non-const version)
        T& operator*() {
            return vecdata[id];
        };
        //! Forward movement
        vectordataIterator& operator++() {
            id = (id < vecdata.size()) ? id+1 : vecdata.size();
            return (*this);
        };
        //! Forward movement
        const vectordataIterator& operator++() const {
            id = (id < vecdata.size()) ? id+1 : vecdata.size();
            return (*this);
        };
        //! Backward movement
        vectordataIterator& operator--() {
            id = ( id > 0 ) ? id-1 : 0;
            return (*this);
        };
        //! Backward movement
        const vectordataIterator& operator--() const {
            id = ( id > 0 ) ? id-1 : 0;
            return (*this);
        };
        //! Return the id (used by VectorData for accessing)
        u_int getIndex() {
            return id;
        };
    private:
        //! Vector over iterates
        VectorData& vecdata;
        //! current index
        u_int id;
    };
    //@}

protected:

    /*! \brief Raw Data
     */
    T* rawdata() const {
        return data;
    };

    //! The actual size of VectorData
    u_int vsize;
    //! The allocated space
    u_int allocated;
    //! Data
    T* data;

    //! Is View
    bool view;
    //! start index
    u_int idstart;
    //! end index
    u_int idend;
    //! Observed VectorData
    VectorData* observed;

    //! Notify to viewers that 'data' is changed
    virtual void notify( const NotifyEvent& event ) {
        switch( event.type() ) {
        case datachanged:
            if ( idstart > observed->vsize || idend > observed->vsize ) {
                nnfwMessage( NNFW_ERROR, "Indexes become invalid after data changing; using 0 and viewed->size()" );
                idstart = 0;
                idend = observed->size();
            }
            data = (observed->data) + idstart;
            vsize = idend - idstart;
            allocated = 0;
            break;
        case datadestroying:
            nnfwMessage( NNFW_WARNING, "Destroying a VectorData before its views!!!" );
            // --- reconvert to a regular VectorData with size zero
            view = false;
            data = 0;
            vsize = 0;
            allocated = 0;
            observed = 0;
            idstart = 0;
            idend = 0;
            break;
        default:
            break;
        }
        // --- Propagate Notify to sub-viewers
        notifyAll( NotifyEvent( datachanged ) );
    };

};

}

#endif
