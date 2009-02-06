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

#ifndef SIMPLECLUSTER_H
#define SIMPLECLUSTER_H

/*! \file
 */

#include "types.h"
#include "cluster.h"

namespace nnfw {

/*! \brief SimpleCluster Class
 *
 */
class NNFW_API SimpleCluster : public Cluster {
public:
	/*! \name Constructors */
	//@{
	/*! Construct a Cluster that contains numNeurons neuron */
	SimpleCluster( unsigned int numNeurons, QString name = "unnamed" );
	/*! Destructor */
	virtual ~SimpleCluster();
	//@}
	/*! \name Interface */
	//@{
	/*! Update the outputs of neurons */
	void update();
	/*! Randomize Nothing ;-) */
	void randomize( double, double ) { /* Nothing To Do */ };
	/*! Clone this SimpleCluster */
	virtual SimpleCluster* clone() const;
	/*! Return its typename */
	virtual QString typeName() {
		return "SimpleCluster";
	};
	//@}
};

}

#endif
