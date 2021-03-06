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
 *     Il CopyLinker e' un oggetto che 'letteralmente' copia i dati da un Cluster ad un altro.
 *     Esistano quattro modalita' di funzionamento di questo oggetto specificate dal relativo enum
 *
 *     -# CopyLinker::In2In : copia i valori di <b>input</b> del Cluster 'from' nell'<b>input</b> del Cluster 'to'
 *     -# CopyLinker::In2Out : copia i valori di <b>input</b> del Cluster 'from' nell'<b>output</b> del Cluster 'to'
 *     -# CopyLinker::Out2In : copia i valori di <b>output</b> del Cluster 'from' nell'<b>input</b> del Cluster 'to'
 *     -# CopyLinker::Out2Out : copia i valori di <b>output</b> del Cluster 'from' nell'<b>output</b> del Cluster 'to'
 *
 *     Il CopyLinker e' utile nelle reti neurali dove c'e' un trasferimento di dati da un Cluster ad un altro che non
 *     necessita di computazione. Ad esempio, nelle reti alla Elmann, i valori di inputs di uno strato interno vengono
 *     riportati come inputs di uno strato di input della rete (CopyLinker in modalita' CopyLinker::In2In)
 *  \par Description
 *     Desc
 *  \par Warnings
 *     Quando le dimensioni tra i dati da copiare non corrispondono, allora vengono copiati il massimo numero possibile
 *     di dati. Ad esempio, in un CopyLinker in modalita' In2In tra due Cluster 'from' e 'to' di dimensione 8 e 5
 *     rispettivamente, allora solo i primi 5 dati del Cluster 'from' vengono copiati nei 5 inputs del Cluster 'to'.<br>
 *     Viceversa, se le dimensioni di 'from' e 'to' sono 5 e 8, rispettivamente, allora i 5 inputs del Cluster 'from'
 *     (tutti i dati della sorgente) vengono copiati nei primi 5 inputs del Cluster 'to'.
 */
class NNFW_API CopyLinker : public Linker {
public:
    /*! Modality of Data Copying */
    typedef enum { In2In = 0, In2Out = 1, Out2In = 2, Out2Out = 3 } CopyMode;

	/*! \name Constructors */
	//@{

    /*! Constructor
     */
    CopyLinker( Cluster* from, Cluster* to, CopyMode mode, const char* name = "unnamed" );

    /*! Construct by PropertySettings
     */
    CopyLinker( PropertySettings& prop );

    /*! Destructor
     */
    virtual ~CopyLinker();

	//@}
	/*! \name Interface */
	//@{

    /*! Change the Modality of Data Copying
     */
    void setMode( CopyMode );

    /*! Return the Modality of Data Copying
     */
    CopyMode getMode() const;

    /*! Change the Modality of Data Copying (Variant version)
     */
    bool setMode( const Variant& v );
    
    /*! Return the Modality of Data Copying (Variant version)
     */
    Variant getModeP();

    /*! Update the linker
     */
    void update();

    /*! Returns the number of neurons connected
     */
    u_int size() const;

    /*! Randomize (i.e. do nothing !!)
     */
    void randomize( Real , Real ) {
        // --- Do Nothing
    };

	/*! Clone this CopyLinker */
	virtual CopyLinker* clone() const;

	//@}

private:
    /*! View of source RealVec of copying */
    RealVec dataFrom;
    /*! View of destination RealVec of copying */
    RealVec dataTo;
    /*! True if RealVec views are been initialized */
    bool viewsIsInit;
    /*! Length of data to be copied */
    u_int dimData;
    /*! Modality of Copy */
    CopyMode mode;
};

}

#endif
