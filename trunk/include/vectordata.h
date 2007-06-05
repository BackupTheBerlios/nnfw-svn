/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2007 Gianluca Massera <emmegian@yahoo.it>                     *
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
 *
 *  \todo creation of MathVector template, a subclass of VectorData for types that supports 
 *  math operation (+,-,*,/, etc), so it's faster and easier create vectors of int, boolean, and so on.
 */

#include "memutils.h"
#include <vector>


namespace nnfw {

/*! \brief VectorData Class
 *  \par Motivation
 *  Create a VectorData abstract type for storing data in dynamic and efficient way
 *  \par Description
 *  \par Warnings
 */
template<class T>
class NNFW_TEMPLATE VectorData : private Observer, public Observable {
public:
    //! Type of Notification
    typedef enum { datachanged = 1, datadestroying = 2 } t_notify;

    /*! \name Constructors */
    //@{

    /*! Default Constructor
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

    /*! Construct a vector of dimension size setting all values to defaul constructor of T
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

    /*! Construct a vector of dimension size setting all the values as specified
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
    
    /*! Construct a VectorData view
     */
    VectorData( VectorData<T>& src, u_int idStart, u_int idEnd )
        : Observer(), Observable() {
        if ( idStart > src.vsize || idEnd > src.vsize || idStart >= idEnd ) {
            nError() << "Wrongs indexes specified in VectorData constructor; using 0 and src.size()" ;
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

    /*! Construct by copying data from const T* vector
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

    /*! The Copy-Constructor always allocate new memory and copies the data, 
	 *  even if the source VectorData is a view.
	 *  Hence, a copy of a VectorData view is not a view but a new copy of data viewed by source.
	 *  \param src the VectorData to be copied
     */
    VectorData( const VectorData& src )
        : Observer(), Observable() {
        /* --- even if the source VectorData is a view,
           --- the copy-constructor create a new fresh copy of data */
		vsize = src.vsize;
		allocated = vsize;
		data = new T[allocated];
		memoryCopy( data, src.data, vsize );
		view = false;
		observed = 0;
    };

    /*! The assignment operator, check the size of itself and resize itself if necessary, and copy the data
	 *  using the assing method.<br>
	 *  This not change the nature of this VectorData, if it's a view the source data are assigned to the
	 *  data viewed and this remains a VectorData view of the same VectorData.
	 *  \param src the VectorData to be copied
     */
    VectorData& operator=( const VectorData& src ) {
		VectorData& self = *this;
		self.resize( src.size() );
		self.assign( src );
		return self;
	};

    /*! Destructor
     */
    ~VectorData() {
        notifyAll( NotifyEvent( datadestroying ) );
        if ( view ) {
            observed->delObserver( this );
        } else {
            delete []data;
        }
    };
    //@}
    /*! \name Informations about VectorData */
    //@{

    /*! Return the size of VectorData
     */
    u_int size() const {
        return vsize;
    };

    /*! Return True if it is a VectorData view
     */
    bool isView() const {
        return view;
    };

    /*! Equal Operator
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

    /*! Not-Equal Operator
     */
    bool operator!=( const VectorData<T>& b ) {
        return !( *this == b );
    };

    //@}
    /*! \name Operations on VectorData */
    //@{

    /*! Set all values to default value of T, in other words to T()
     */
    void zeroing() {
        memoryZeroing( data, vsize );
    };

    /*! Set all values to value
     */
    void setAll( const T& value ) {
        for( u_int i=0; i<vsize; i++ ) {
            data[i] = value;
        }
    };

    /*! Assign to first num element the value passed
     */
    VectorData<T>& assign( u_int num, const T& value ) {
#ifdef NNFW_DEBUG
        if ( num > vsize ) {
            nError() << "Wrong number of elements passed to assign method" ;
            num = vsize;
        }
#endif
        for( u_int i=0; i<num; i++ ) {
            data[i] = value;
        }
        return (*this);
    };

    /*! Assignment method. The sizes of VectorData must be the same.
     */
    VectorData<T>& assign( const VectorData<T>& src ) {
#ifdef NNFW_DEBUG
        if ( vsize != src.vsize ) {
            nError() << "Wrong number of elements between VectorData to assign method" ;
            return (*this);
        }
#endif
        memoryCopy( data, src.data, vsize );
        return (*this);
    };

    /*! Assignment method allow to copy data from a VectorData to another VectorData with different size.
	 *  \param sizec the size of data to be assigned 
     */
    VectorData<T>& assign( const VectorData<T>& src, u_int sizec ) {
#ifdef NNFW_DEBUG
        if ( vsize < sizec || src.size() < sizec ) {
            nError() << "Wrong size specified in assign method" ;
            return (*this);
        }
#endif
        memoryCopy( data, src.data, sizec );
        return (*this);
    };

	/*! Element-by-element comparison between the two VectorData. 
	 *  It returns a boolean array of the same size, with elements set to true (1) where the values are the same,
	 *  and elements set to false (0) where they are not.
	 *  \param b the 'right' VectorData to compare with itself
	 *  \param comparison the VectorData where to register the results (this is returned by method)
	 *
	 *  \warning This method require that the template class T has the operator== defined
     */
    VectorData<int>& compare( const VectorData<T>& b, VectorData<int>& comparison ) {
#ifdef NNFW_DEBUG
        if ( vsize != b.vsize ) {
            nError() << "Wrong number of elements between VectorData to compare method" ;
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

	/*! Indexing operator
     *  \warning Boundary check activated only when DEBUG if defined
     */
    T& operator[]( u_int index ) {
#ifdef NNFW_DEBUG
        if( index >= vsize ) {
            nError() << "Accessing elements outside boundary" ;
            return data[0];
        }
#endif
        return data[index];
    };

    /*! Indexing operator (Const Version)
     *  \warning Boundary check activated only when DEBUG if defined
     */
    const T& operator[]( u_int index ) const {
#ifdef NNFW_DEBUG
        if( index >= vsize ) {
            nError() << "Accessing elements outside boundary" ;
            return data[0];
        }
#endif
        return data[index];
    };

    /*! Accessing method
     *  \warning Boundary check activated only when DEBUG if defined
     */
    T& at( u_int index ) {
#ifdef NNFW_DEBUG
        if( index >= vsize ) {
            nError() << "Accessing elements outside boundary" ;
            return data[0];
        }
#endif
        return data[index];
    };

    /*! Accessing method (Const Version)
     *  \warning Boundary check activated only when DEBUG if defined
     */
    const T& at( u_int index ) const {
#ifdef NNFW_DEBUG
        if( index >= vsize ) {
            nError() << "Accessing elements outside boundary" ;
            return data[0];
        }
#endif
        return data[index];
    };

    /*! Resize the VectorData
     */
    void resize( u_int newsize ) {
        if ( view ) {
            nError() << "It's not possible resize RealVec views" ;
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

    /*! Append an element; the dimesion increase by one
     */
    void append( const T& value ) {
        resize( vsize+1 );
        data[vsize-1] = value;
    };

    /*! Append Operator; the dimesion increase by one
     */
    VectorData<T>& operator<<( const T& value ) {
        append( value );
        return (*this);
    };
    //@}
    /*! \name Operations on VectorData views */
    //@{

    /*! Configure the indexes of starting and ending of this VectorData view. <br>
     *  If VectorData is not a view, then it will shows an error message
     */
    void setView( u_int idStart, u_int idEnd ) {
        if ( !view ) {
            nError() << "setView can be called only if VectorData is a view" ;
            return;
        }
        if ( idStart > observed->vsize || idEnd > observed->vsize || idStart >= idEnd ) {
            nError() << "Wrongs indexes specified in VectorData setView; using 0 and observed->size()" ;
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

    /*! Convert this VectorData to a view of VectorData src passed
     */
    void convertToView( VectorData<T>& src, u_int idStart, u_int idEnd ) {
        if ( observed == (&src) ) {
            setView( idStart, idEnd );
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
            nError() << "Wrongs indexes specified in convertToView; using 0 and src.size()" ;
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

    /*! Max size allowed
     */
    size_type max_size() {
        return 300000; // <- FIXME it must return the maximum space of memory allocable
    };

    /*! Is Empty
     */
    bool empty() {
        return (vsize==0);
    };

    /*! Append an element
     */
    void push_back( const T& value ) {
        resize( vsize+1 );
        data[vsize-1] = value;
    };

    /*! Iterator at initial position
     */
    iterator begin() {
        return vectordataIterator( *this );
    };
    /*! Iterator at initial position
     */
    const_iterator begin() const {
        return vectordataIterator( ((VectorData&)*this) );
    };

    /*! Iterator at past-end position
     */
    iterator end() {
        return vectordataIterator( *this, vsize );
    };
    /*! Iterator at past-end position
     */
    const_iterator end() const {
        return vectordataIterator( ((VectorData&)*this), vsize );
    };

    /*! Erase method
     */
    iterator erase( iterator pos ) {
        if ( view ) {
            nError() << "you can't erase element from a VectorData view" ;
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

    /*! Erase All elements
     */
    void clear() {
        if ( view ) {
            nError() << "you can't clear a VectorData view" ;
        } else {
            vsize = 0;
            notifyAll( NotifyEvent( datachanged ) );
        }
    };

    /*! Inner-class implements iterator over VectorData
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

    /*! Raw Data
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
                nError() << "Indexes become invalid after data changing; using 0 and viewed->size()" ;
                idstart = 0;
                idend = observed->size();
            }
            data = (observed->data) + idstart;
            vsize = idend - idstart;
            allocated = 0;
            break;
        case datadestroying:
#ifdef NNFW_DEBUG
            nWarning() << "Destroying a VectorData before its views!!!" ;
#endif
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
