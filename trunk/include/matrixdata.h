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

// --- You can't include it directly
#ifndef TYPES_INCLUDES
#error "You can't include matrixdata.h directly; Instead, You have to include types.h"
// --- follow define avoid to get a lot of understandable error !
#define MATRIXDATA_H
#endif

#ifndef MATRIXDATA_H
#define MATRIXDATA_H

/*! \file
 *  \brief This file contains the template MatrixData Class; Don't include this file directly, instead include types.h
 */

#include "memutils.h"
#include "vectordata.h"


namespace nnfw {

/*! \brief MatrixData Class
 *  \par Motivation
 *  Create a MatrixData abstract type for storing data in dynamic and efficient way
 *  \par Description
 *  \par Warnings
 */
template<class T, class Vec = VectorData<T> >
class NNFW_TEMPLATE MatrixData : private Observer, private Observable {
public:
    /*! \name Constructors */
    //@{

    /*! Construct an empty matrix of dimension size
     */
    MatrixData( u_int rows, u_int cols )
        : Observer(), Observable(), data(rows*cols), rowView(rows) {
        data.zeroing();
        nrows = rows;
        ncols = cols;
        tsize = nrows*ncols;
        for( u_int i=0; i<nrows; i++ ) {
            rowView[i].convertToView( data, i*ncols, (i+1)*ncols );
        }
        // --- view attributes
        view = false;
    };

    /*! Construct a Matrix view from VectorData passed
     *  \param src is the VectorData from which this method constructs a Matrix view.
     *         The VectorData must be the same as specified in template constructing ( class Vec template parameters )
     *
     *  The view represent the piece of src delimited by rstart and rend as a MatrixData with nrows and ncols.
     *  So, the follow assert must be accomplished: rend-rstart == nrows*ncols
     *  \par Warnings:
     *  When a VectorData vec is viewed through a MatrixData mat, then a resizing of vec can lead to some strange results;
     *  In fact, the automatic adjustment of mat after a vec resizing is a bit ambiguous because is not clear which dimensions
     *  sets. So, for diambiguous the mat view will be adjusted mantaining the same numbers of the rows and adding or removing
     *  columns trying to fit the new dimension of vec. If is not possibile this can lead to views that doesn't represent
     *  the whole vec.
     */
    MatrixData( Vec& src, u_int rstart, u_int rend, u_int rows, u_int cols )
        : Observer(), Observable(), data(src, rstart, rend), rowView() {
        // --- Check the validity of dimensions
        if ( data.size() != rows*cols ) {
            nError() << "Wrongs dimensions specified in MatrixData view constructor; This MatrixData will be invalidate" ;
            rows = 0;
            cols = 0;
        }
        // --- view constructing
        view = true;
        // the local variable data is observed by this object
        data.addObserver( this );
        nrows = rows;
        ncols = cols;
        tsize = nrows*ncols;
        rowView.resize( tsize );
        for( u_int i=0; i<nrows; i++ ) {
            rowView[i].convertToView( data, i*ncols, (i+1)*ncols );
        }
    };

    /*! Destructor
     */
    ~MatrixData() {
        // --- nothing to do
    };

    //@}
    /*! \name Informations about MatrixData */
    //@{

    /*! Returns the numbers of Row */
    u_int rows() const {
        return nrows;
    };
    /*! Returns the numbers of Columns */
    u_int cols() const {
        return ncols;
    };
    /*! Returns the total numbers of elements (Rows*Columns) */
    u_int size() const {
        return tsize;
    };

    /*! Return True if it is a MatrixData view
     */
    bool isView() const {
        return view;
    };

    /*! Equal Operator
     */
    bool operator==( const MatrixData<T,Vec>& b ) {
        // --- WARNING: don't implement this method using the operator== of private data vector
        // ---    perche' se la MatrixData e' una view, dopo un resizing sbagliato del vettore origine
        // ---    potrebbe rimanere una inconsistenza critica nei dati
        if ( this->size() != b.size() ) return false;
        for( u_int i=0; i<size(); i++ ) {
            if ( this->data[i] != b.data[i] ) {
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

    /*! Resize the matrix */
    void resize( u_int rows, u_int cols ) {
        if ( view ) {
            nError() << "you can't resize a MatrixData view - use setView instead" ;
            return;
        }
        nrows = rows;
        ncols = cols;
        tsize = nrows*ncols;
        data.resize( tsize );
        rowView.resize( nrows );
        // --- Adjust the view of rows
        for( u_int i=0; i<nrows; i++ ) {
            if ( rowView[i].isView() ) {
                rowView[i].setView( i*ncols, (i+1)*ncols );
            } else {
                rowView[i].convertToView( data, i*ncols, (i+1)*ncols );
            }
        }
        // --- Notify the viewers
        // at this time, the notification has been launched by data.resize calling
        // the following methods will make sense only when Matrix-to-Matrix view will be implemented
        //notifyAll( NotifyEvent( datachanged ) );
    };

    //@}
    /*! \name Accessing Operators */
    //@{

    /*! Return a reference to element at position (row, col) */
    T& at( u_int row, u_int col ) {
#ifdef NNFW_DEBUG
        if ( row >= nrows ) {
            nError() << "Accessing an element beyond Row boundary of matrix" ;
            return data[0];
        }
        if ( col >= ncols ) {
            nError() << "Accessing an element beyond Column boundary of matrix" ;
            return data[0];
        }
#endif
        return rowView[row][col];
    };

    /*! Return a Const reference to element at position (row, col) */
    const T& at( u_int row, u_int col ) const {
#ifdef NNFW_DEBUG
        if ( row >= nrows ) {
            nError() << "Accessing an element beyond Row boundary of matrix" ;
            return data[0];
        }
        if ( col >= ncols ) {
            nError() << "Accessing an element beyond Column boundary of matrix" ;
            return data[0];
        }
#endif
        return rowView[row][col];
    };

    /*! Indexing operator<br>
     *  Boundary check activated only when DEBUG if defined
     */
    Vec& operator[]( u_int row ) {
#ifdef NNFW_DEBUG
        if( row >= nrows ) {
            nError() << "Accessing elements outside boundary" ;
            return rowView[0];
        }
#endif
        return rowView[row];
    };

    /*! Indexing operator (Const Version)<br>
     *  Boundary check activated only when DEBUG if defined
     */
    const Vec& operator[]( u_int row ) const {
#ifdef NNFW_DEBUG
        if( row >= nrows ) {
            nError() << "Accessing elements outside boundary" ;
            return rowView[0];
        }
#endif
        return rowView[row];
    };

    /*! Assign elements
     */
    MatrixData& assign( const MatrixData& src ) {
#ifdef NNFW_DEBUG
        if( rows() != src.rows() || cols() != src.cols() ) {
            nError() << "Different dimension" ;
            return (*this);
        }
#endif
        data.assign( src.data, tsize );
        return (*this);
    };

    /*! Set all values to zero
     */
    void zeroing() {
        data.zeroing();
    };


    //@}
    /*! \name STL compatibility */
    //@{

    class matrixdataIterator;
    friend class matrixdataIterator;

    /*! Value type */
    typedef T value_type;
    /*! Reference type */
    typedef T& reference;
    /*! Const Reference type */
    typedef const T& const_reference;
    /*! Iterator */
    typedef matrixdataIterator iterator;
    /*! Const Iterator */
    typedef const matrixdataIterator const_iterator;
    /*! Size type */
    typedef size_t size_type;
    /*! Difference pointer type */
    typedef ptrdiff_t difference_type;

    /*! Max size allowed
     */
    size_type max_size() {
        return 300000; // <- FIXME it must return the maximum space of memory allocable
    };

    /*! Is Empty
     */
    bool empty() {
        return (tsize==0);
    };

    /*! Clear: reduce the dimension to zero-zero matrix
     */
    void clear() {
        if ( view ) {
            nError() << "you can't clear a MatrixData view" ;
        } else {
            resize( 0, 0 );
        }
    };

    /*! Iterator at initial position
     */
    iterator begin() {
        return matrixdataIterator( *this );
    };
    /*! Iterator at initial position
     */
    const_iterator begin() const {
        return matrixdataIterator( ((MatrixData&)*this) );
    };

    /*! Iterator at past-end position
     */
    iterator end() {
        return matrixdataIterator( *this, tsize );
    };
    /*! Iterator at past-end position
     */
    const_iterator end() const {
        return matrixdataIterator( ((MatrixData&)*this), tsize );
    };

    /*! Inner-class implements iterator over MatrixData
     */
    class matrixdataIterator : public std::iterator<std::bidirectional_iterator_tag,T,ptrdiff_t> {
    public:
        /*! create the iterator */
        matrixdataIterator( MatrixData& data, u_int startId = 0 ) : vecdata(data.data), id(startId), tsize(data.tsize) { /*nothing to do*/ };
        /*! Copy-Constructor */
        matrixdataIterator( const matrixdataIterator& src ) : vecdata(src.vecdata), id(src.id), tsize(src.tsize) { /* nothing to do */  };
        /*! Assignement operator */
        matrixdataIterator& operator=( const matrixdataIterator& src ) {
            vecdata = src.vecdata;
            id = src.id;
            tsize = src.tsize;
            return (*this);
        };
        /*! equal operator */
        bool operator==( const matrixdataIterator& x ) const {
            return ( &vecdata == &(x.vecdata) && id == x.id );
        };
        /*! not-equal operator */
        bool operator!=( const matrixdataIterator& x ) const {
            return ( &vecdata != &(x.vecdata) || id != x.id );
        };
        /*! Accessing element (const version) */
        const T& operator*() const {
            return vecdata[id];
        };
        /*! Accessing element (non-const version) */
        T& operator*() {
            return vecdata[id];
        };
        /*! Forward movement */
        matrixdataIterator& operator++() {
            id = (id < tsize) ? id+1 : tsize;
            return (*this);
        };
        /*! Forward movement */
        const matrixdataIterator& operator++() const {
            id = (id < tsize) ? id+1 : tsize;
            return (*this);
        };
        /*! Backward movement */
        matrixdataIterator& operator--() {
            id = ( id > 0 ) ? id-1 : 0;
            return (*this);
        };
        /*! Backward movement */
        const matrixdataIterator& operator--() const {
            id = ( id > 0 ) ? id-1 : 0;
            return (*this);
        };
        /*! Return the id (used by VectorData for accessing) */
        u_int getIndex() {
            return id;
        };
    private:
        /*! Vector over iterates */
        Vec& vecdata;
        /*! current index */
        u_int id;
        /*! Dimension of data - same as MatrixData::size() */
        u_int tsize;
    };
    //@}

protected:
    friend class RealVec;

    /*! Return the VectorData of the data
     */
    Vec& rawdata() const {
        return (Vec&)data;
    };

private:
    /*! Numbers of Rows */
    u_int nrows;
    /*! Numbers of Columns */
    u_int ncols;
    /*! Size */
    u_int tsize;
    /*! VectorData of data */
    Vec data;
    /*! Vector of VectorData views of data */
    VectorData< Vec > rowView;

    /*! if is a MatrixData view */
    bool view;

    /*! Notify to viewers that 'data' is changed */
    virtual void notify( const NotifyEvent& event ) {
        switch( event.type() ) {
        case Vec::datachanged:
#ifdef NNFW_DEBUG
            nWarning() << "Arrange a MatrixData view after a VectorData resizing can lead to inconsistent settings - see documentation if you not sure" ;
#endif
            ncols = data.size() / nrows ;
            if ( ncols == 0 ) {
                nrows = 0;
                ncols = 0;
                tsize = 0;
                rowView.clear();
            } else {
                tsize = ncols*nrows;
                // --- Adjust the view of rows
                for( u_int i=0; i<nrows; i++ ) {
                    rowView[i].setView( i*ncols, (i+1)*ncols );
                }
            }
            break;
        case Vec::datadestroying:
#ifdef NNFW_DEBUG
            nWarning() << "Destroying a VectorData before its views!!!" ;
#endif
            // --- reconvert to a regular MatrixData with size zero
            view = false;
            nrows = 0;
            ncols = 0;
            tsize = 0;
            rowView.clear();
            break;
        default:
            break;
        }
        // --- Propagate Notify to sub-viewers
        // --- Inutile fino a che non si implementano le viste tra Matrici
        //notifyAll( matdatachanged );
    };

	/*! The Copy-Construction and Assignement is to allowed */
    MatrixData( const MatrixData& src )
        : Observer(), Observable(), data(0), rowView(0) {
	};

};

}

#endif

