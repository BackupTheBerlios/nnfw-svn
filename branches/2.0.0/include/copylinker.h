/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2009 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef COPYLINKER_H
#define COPYLINKER_H

/*! \file
 */

#include "types.h"
#include "linker.h"

namespace nnfw {

/*! \brief CopyLinker Class. This linker copies the outputs of a cluster to inputs of another cluster
 *
 *  \par Motivation
 *   The CopyLinker is useful in networks when one requires that data be
 *   transferred from one Cluster's terminal to another without computation.
 *   For example, in Elmann networks the values of hidden layer inputs are
 *   copied to the context layer Cluster (which acts like a short-term memory
 *   of former inputs).
 *
 *  \par Description
 *   The CopyLinker is an object that 'literally' copies the outputs from one
 *   Cluster to another.  This object has four modes of operation specified
 *   by the relevant enum:
 *   
 *   -# CopyLinker: :In2In:   copy the inputs of the 'from' Cluster to the
 *      inputs of the 'to' Cluster
 *   -# CopyLinker: :In2Out:  copy the inputs of the 'from' Cluster to the
 *      outputs of the 'to Cluster
 *   -# CopyLinker: :Out2In:  copy the outputs of the 'from' Cluster to the
 *      inputs of the 'to' Cluster
 *   -# CopyLinker: :Out2Out: copy the outputs of the 'from' Cluster to the
 *      outputs of the 'to' Cluster
 *
 *  \par Warnings
 *   If there is a mis-match in the dimensionality of the terminals connected
 *   by the CopyLinker, as much data as possible is copied.  For example; a
 *   CopyLinker set to mode 'In2In' between a 'from' and 'to' Cluster with 8
 *   and 5 inputs respectively, will copy the first 5 data elements for
 *   'from' into the inputs for 'to'.  Vice versa, if the dimensions of
 *   'from' and 'to' are 5 and 8 respectively, then the 5 inputs of the
 *   'from' cluster (all of the source data) are copied into the first 5
 *   inputs of the 'to' Cluster (with 3 neurons receiving no input).
 */
class NNFW_API CopyLinker : public Linker {
public:
	/*! Enumerator specifies the copy mode as explained in the CopyLinker description */
	typedef enum { In2In = 0, In2Out = 1, Out2In = 2, Out2Out = 3 } CopyMode;
	/*! \name Constructors */
	//@{
	/*! This constructor should be self-explanatory, post a comment on the forum if you can't work out what to do */
	CopyLinker( Cluster* from, Cluster* to, CopyMode mode, QString name = "unnamed" );
	/*! Destructor */
	virtual ~CopyLinker();
	//@}
	/*! \name Interface */
	//@{
	/*! Change the Modality of Data Copying */
	void setMode( CopyMode );
	/*! Return the Modality of Data Copying */
	CopyMode getMode() const;
	/*! Update the linker */
	void update();
	/*! Returns the number of neurons connected */
	unsigned int size() const;
	/*! Randomize (i.e. do nothing !!) */
	void randomize( double , double ) {
		// --- Do Nothing
	};
	/*! Clone this CopyLinker */
	virtual CopyLinker* clone() const;
	/*! Return its typename */
	virtual QString typeName() {
		return "CopyLinker";
	};
	//@}

private:
	/*! View of source RealVec of copying */
	RealVec dataFrom;
	/*! View of destination RealVec of copying */
	RealVec dataTo;
	/*! True if RealVec views are been initialized */
	bool viewsIsInit;
	/*! Length of data to be copied */
	unsigned int dimData;
	/*! Modality of Copy */
	CopyMode mode;
};

}

#endif
