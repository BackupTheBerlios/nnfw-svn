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

#ifndef UTILS_H
#define UTILS_H

/*! \brief This file contains some utilities functions
 *
 * Details...
 */

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

class BaseNeuralNet;
class LearningNetwork;

/*! \brief Function to quickly construct a FeedForward Neural Network
 *  Return a BaseNeuralNetwork that represent a feedforwar neural net.<br>
 *  La rete neurale risultante conterra' SimpleCluster connessi tramite MatrixLinker.<br>
 *  Il numero di neuroni all'interno dei SimpleCluster sono presi dal vettore layers passato.<br>
 *  Il SimpleCluster di dimensione layers[0] sara' l'input della rete neurale e quello corrispondente
 *  all'ultimo valore di layers sara' l'output. Gli altri, in ordine, saranno i SimpleCluster nascosti.
 *  \param layers il vettore che specifica le dimensioni dei SimpleCluster
 */
 BaseNeuralNet* feedForwardNet( U_IntVec layers, const char* clusterType = "BiasedCluster", const char* linkerType = "MatrixLinker" );

/*! \brief Function to quickly construct a Backpropagation Learning Network
 *  It only use GradientBiasedCluster and GradientMatrixLinker
 *  WARNINGS: Questa funzione non controlla la struttura della rete neurale, quindi e' cura di chi la usa
 *   assicurarsi che la rete neurale soddisfi i seguenti requisiti:<br>
 *   a) Deve essere una struttura FeedForward senza MatrixLinker a formare ricorrenze<br>
 *   b) Devono esistere Cluster di inputs e di outputs<br>
 *   c) La struttura principale della rete deve essere composta da BiasedCluster e MatrixLinker
 */
 LearningNetwork* backpropagationFor( BaseNeuralNet* net );

/*! \brief A quick way to sets all the parameters of Gradient types blocks in LearningNetwork passed
 * \param learnNet the LearningNewtork contains the Gradient blocks to configure
 * \param rate the learning rate
 * \param momento the momentum parameter (default is zero)
 */
 void setParamsOfGradientBlocks( LearningNetwork* learnNet, Real rate, Real momento = 0.0 );

}

#endif
