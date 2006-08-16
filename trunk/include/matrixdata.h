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

#ifndef MATRIXDATA_H
#define MATRIXDATA_H

/*! \file
 *  \brief This file contains the template MatrixData Class; Don't include this file directly, instead include types.h
 *  Details...
 */

#include "memutils.h"
#include "vectordata.h"
#include "messages.h"

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

/*! \brief MatrixData Class
 *  \par Motivation
 *  Create a MatrixData abstract type for storing data in dynamic and efficient way
 *  \par Description
 *  \par Warnings
 */
template<class T, class Vec = VectorData<T> >
class MatrixData : private Observer, private Observable {
public:
    /*! \name Constructors */
    //@{

    /*! \brief Construct an empty matrix of dimension size
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

    /*! \brief Destructor
     */
    ~MatrixData() {
        // --- nothing to do
    };

    //@}
    /*! \name Informations about MatrixData */
    //@{

    //! Returns the numbers of Row
    u_int rows() const {
        return nrows;
    };
    //! Returns the numbers of Columns
    u_int cols() const {
        return ncols;
    };
    //! Returns the total numbers of elements (Rows*Columns)
    u_int size() const {
        return tsize;
    };

    //! Resize the matrix
    void resize( u_int rows, u_int cols ) {
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
    };

    //! Return a reference to element at position (row, col)
    T& at( u_int row, u_int col ) {
#ifdef NNFW_DEBUG
        if ( row >= nrows ) {
            nnfwMessage( NNFW_ERROR, "Accessing an element beyond Row boundary of matrix" );
            return data[0];
        }
        if ( col >= ncols ) {
            nnfwMessage( NNFW_ERROR, "Accessing an element beyond Column boundary of matrix" );
            return data[0];
        }
#endif
        return rowView[row][col];
    };

    //@}
    /*! \name Accessing Operators */
    //@{

    //! Return a Const reference to element at position (row, col)
    const T& at( u_int row, u_int col ) const {
#ifdef NNFW_DEBUG
        if ( row >= nrows ) {
            nnfwMessage( NNFW_ERROR, "Accessing an element beyond Row boundary of matrix" );
            return data[0];
        }
        if ( col >= ncols ) {
            nnfwMessage( NNFW_ERROR, "Accessing an element beyond Column boundary of matrix" );
            return data[0];
        }
#endif
        return rowView[row][col];
    };

    /*! \brief Indexing operator
     *  Boundary check activated only when DEBUG if defined
     */
    Vec& operator[]( u_int row ) {
#ifdef NNFW_DEBUG
        if( row >= nrows ) {
            nnfwMessage( NNFW_ERROR, "Accessing elements outside boundary" );
            return rowView[0];
        }
#endif
        return rowView[row];
    };

    /*! \brief Indexing operator (Const Version)
     *  Boundary check activated only when DEBUG if defined
     */
    const Vec& operator[]( u_int row ) const {
#ifdef NNFW_DEBUG
        if( row >= nrows ) {
            nnfwMessage( NNFW_ERROR, "Accessing elements outside boundary" );
            return rowView[0];
        }
#endif
        return rowView[row];
    };

    /*! \brief Assign elements
     */
    MatrixData& assign( const MatrixData& src ) {
#ifdef NNFW_DEBUG
        if( rows() != src.rows() || cols() != src.cols() ) {
            nnfwMessage( NNFW_ERROR, "Different dimension" );
            return (*this);
        }
#endif
        data.assign( src.data );
        return (*this);
    };

    /*! \brief Set all values to zero
     */
    void zeroing() {
        data.zeroing();
    };


    //@}
    /*! \name STL compatibility */
    //@{

    class matrixdataIterator;
    friend class matrixdataIterator;

    //! Value type
    typedef T value_type;
    //! Reference type
    typedef T& reference;
    //! Const Reference type
    typedef const T& const_reference;
    //! Iterator
    typedef matrixdataIterator iterator;
    //! Const Iterator
    typedef const matrixdataIterator const_iterator;
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
        return (tsize==0);
    };

    /*! \brief Clear: reduce the dimension to zero-zero matrix
     */
    void clear() {
        resize( 0, 0 );
    };

    /*! \brief Iterator at initial position
     */
    iterator begin() {
        return matrixdataIterator( *this );
    };
    /*! \brief Iterator at initial position
     */
    const_iterator begin() const {
        return matrixdataIterator( ((MatrixData&)*this) );
    };

    /*! \brief Iterator at past-end position
     */
    iterator end() {
        return matrixdataIterator( *this, tsize );
    };
    /*! \brief Iterator at past-end position
     */
    const_iterator end() const {
        return matrixdataIterator( ((MatrixData&)*this), tsize );
    };

    /*! \brief Inner-class implements iterator over MatrixData
     */
    class matrixdataIterator : public std::iterator<std::bidirectional_iterator_tag,T,ptrdiff_t> {
    public:
        //! create the iterator
        matrixdataIterator( MatrixData& data, u_int startId = 0 ) : vecdata(data.data), id(startId) { /*nothing to do*/ };
        //! Copy-Constructor
        matrixdataIterator( const matrixdataIterator& src ) : vecdata(src.vecdata), id(src.id) { /* nothing to do */  };
        //! Assignement operator
        matrixdataIterator& operator=( const matrixdataIterator& src ) {
            vecdata = src.vecdata;
            id = src.id;
            return (*this);
        };
        //! equal operator
        bool operator==( const matrixdataIterator& x ) const {
            return ( &vecdata == &(x.vecdata) && id == x.id );
        };
        //! not-equal operator
        bool operator!=( const matrixdataIterator& x ) const {
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
        matrixdataIterator& operator++() {
            id = (id < vecdata.size()) ? id+1 : vecdata.size();
            return (*this);
        };
        //! Forward movement
        const matrixdataIterator& operator++() const {
            id = (id < vecdata.size()) ? id+1 : vecdata.size();
            return (*this);
        };
        //! Backward movement
        matrixdataIterator& operator--() {
            id = ( id > 0 ) ? id-1 : 0;
            return (*this);
        };
        //! Backward movement
        const matrixdataIterator& operator--() const {
            id = ( id > 0 ) ? id-1 : 0;
            return (*this);
        };
        //! Return the id (used by VectorData for accessing)
        u_int getIndex() {
            return id;
        };
    private:
        //! Vector over iterates
        Vec& vecdata;
        //! current index
        u_int id;
    };
    //@}

protected:
    /*! \brief Return the VectorData of the data
     */
    Vec& rawdata() const {
        return (Vec&)data;
    };

private:
    //! Numbers of Rows
    u_int nrows;
    //! Numbers of Columns
    u_int ncols;
    //! Size
    u_int tsize;
    //! VectorData of data
    Vec data;
    //! Vector of VectorData views of data
    VectorData< Vec > rowView;

    //! if is a MatrixData view
    bool view;
    //! Observed MatrixData
    MatrixData* observed;

    //! Notify to viewers that 'data' is changed
    virtual void notify() {
        // --- Propagate Notify to sub-viewers
        notifyAll();
    };

};

}

#endif

