/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005-2011 Gianluca Massera <emmegian@yahoo.it>                *
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

#ifndef NNFW_H
#define NNFW_H

/*! \mainpage Neural Network Framework (release 2.0.0)
 *  <h1>Class Reference</h1>
 *
 *  This is a Neural Network Framework for creating, simulating and analyze any neural network you can image !!
 *  <br><br>
 *
 *
 *
 */

/*! \file
 *  \brief This is the main header files!! Include this file in your project... and play :-)
 *
 * \todo It's really useful this include file ?!?! I think that a file including everything is not very useful
 *       rather a set of files that includes only some categories of classes: example libclusters.h may includes
 *       all subclasses of Cluster, liblinkers.h may includes all subclasses of Linker, and so on.
 */

/*! \namespace nnfw
 *  \brief the namespace that contains all
 */

#include "types.h"
#include "cluster.h"
#include "linker.h"
#include "outputfunction.h"
#include "neuralnet.h"

/*! SEE THE LIST OF TODOs HERE */
// TODO:
// 	- Add resetInnerStatus function to both Updatable and OutputFunction classes (virtual function, implementation in base classes does nothing). When
// 	  saving something (cluster, linker or outputfunction), the whole status is saved. This means that the resetInnerStatus should be called also after
// 	  reloading the network from file to have a resetted network

//---------- WORKING ON
// 	- Remove properties and use Laral Configuration classes
// al momento sono arrivato a Cluster.cpp, per quanto riguarda la configurazione avverrà nel seguente modo:
//  - dal configure di NeuralNetwork inizialmente si attiverà il flag per mantenere gli oggetti creati durante la configurazione
//    dopodichè si chiamerà getObjectFromParameter per i vari cluster e linker specificati nei parametri della rete
//    all'interno del configure di Cluster.cpp verrà creato l'outputFunction, qui andrà fatta attenzione a controllare
//    che l'outputFunction sia unico per il cluster in oggetto, alcuni modi possono essere:
//    1) far si che il gruppo abbia come prefisso forzato il gruppo del cluster; esempio se in [PIPPO] specifico outfuncion = Sigmoid ; allora si cercherà la outputfunction da creare nel gruppo [PIPPO/Sigmoid]
//    2) invece di un parametro, si può far si chè la outputfunction venga create sempre a partire da un gruppo con un nome fissato; ad esempio per il cluster [PIPPO] si cercherà sempre il gruppo [PIPPO/OutFunction]
// --- la soluzione 2 e' quella selezionata per essere implementato

//  - Lanciare un eccezione in OutputFunction::setCluster per evitare che venga settato su più di un Cluster

// 	- Make the whole library Exception safe (an exception could be thrown during configuration of clusters, linkers, output functions, base neural net...)

#endif

