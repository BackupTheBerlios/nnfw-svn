/********************************************************************************
 *  Neural Network Framework.                                                   *
 *  Copyright (C) 2005 Gianluca Massera <emmegian@yahoo.it>                     *
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

/*! \mainpage Documentation
 *
 *  This is a Neural Network Framework for creating, simulating and analyze any neural network you can image !!
 *
 * <h2>1 L'Idea</h2>
 * <p> Il neural network framework (NNF) consente di poter costruire una rete neurale artificiale sulla base di
 *     elementi di costruzione (Building-blocks) ed attraverso regole di composizione dei building-blocks. </p>
 * <p> La rete neurale &egrave;, quindi, il risultato della combinazione dei vari elementi. </p>
 * <p> Il tutto &egrave; pensato sia per dare enorme flessibilit&agrave; ed estensibilit&agrave; delle architetture possibili, sia per
 *     mantenere al massimo l'efficienza nella simulazione della rete neurale. </p>
 *
 * <h3>1.1 Building-Blocks </h3>
 * <p> Gli elementi di costruzione si suddividono in tre classi: </p>
 * <ul>
 *    <li> <b>Cluster</b>: &egrave; un oggetto che contiene pi&ugrave; neuroni al suo interno. Definisce dei neuroni d'ingresso e dei
 *         neuroni d'uscita. Questi due insiemi non sono necessariamente identici; alcuni neuroni possono
 *         essere solo d'ingresso o solo d'uscita o nessuno dei due (nascosti all'interno del cluster). Vedere
 *         l'InnerLinker per capire come usare i neuroni nascosti del cluster. </li>
 *    <li> <b>Linker</b>: &egrave; l'oggetto che permette di connettere due o pi&ugrave; cluster insieme. In generale, il linker prende
 *         gli output dei neuroni di uno o pi&ugrave; cluster (denominati cluster d'ingresso) e li inserisce come
 *         input dei neuroni di un'altro cluster (denominato d'uscita). Possono esistere varie tipologie di linker
 *         a seconda del tipo di connessione che si vuole realizzere, alcuni esempi sono:
 *         <ul>
 *            <li> Connessione completa fra i neuroni dei cluster in ingresso con quello di uscita; </li>
 *            <li> Connessione sparsa con probabilit&agrave; P tra i neuroni del cluster in ingresso con quello d'uscita; </li>
 *            <li> Connessioni che simulano sinapsi dinamiche; </li>
 *            <li> Connessioni con time-delay. </li>
 *         </ul> </li>
 *    <li> <b>InnerLinker</b>: questo oggetto &egrave; importante per le connessioni all'interno dei neuroni di un cluster.
 *         Questo consente di connettere i neuroni che sono soltanto d'ingresso di un cluster con quelli che sono
 *         solo d'uscita o nascosti. Un esempio dell'utilit&agrave; dell'InnerLinker &egrave; la mappa di Kohonen o le reti di
 *         Hopfield. Infatti, un InnerLinker pu&ograve; connettere i neuroni di un cluster a formare una rete di Hopfield
 *         ed attraverso i Linker usare solo alcuni neuroni come ingresso e altri come output. </li>
 * </ul>
 *
 * <h3>1.2 Regole di Costruzione</h3>
 * <p> Semplicemente si creano i vari Cluster e li si connettono come desiderato attraverso i Linker.</p>
 * <p> Ad ogni cluster inserito all'interno della rete verr&agrave; associato un identificativo numerico, cos&igrave; come ai Linker e InnerLinker. </p>
 *
 * <h3>1.3 La Rete Neurale</h3>
 * <p> La rete neurale &egrave; un oggetto che contiene i vari Cluster e Linker connessi tra loro. </p>
 *
 *
 * <h2>2 Si possono realizzare tutte le reti immaginabili ???</h2>
 * <p> B&egrave;, a prima vista direi di si !! </p>
 * <ul>
 *    <li> E' possibile create strutture ibride a piacimento; Anche mischiare continue e discrete insieme... forse
 *         in futuro chiss&agrave; !! :-) </li>
 *    <li> Reti con sinpasi dinamiche o ritardate sia discrete che continue; </li>
 *    <li> Gli spiking neuron anche ?!?! Uhmm... ci devo pensare un p&ograve;; </li>
 *    <li> Le reti tipo la GasNet, che richiedono una posizione spaziale dei neuroni... potrebbe essere implementata
 *         attraverso un cluster che contiene un solo neurone che definisce una posizione spaziale del cluster....
 *         O meglio ancora con un cluster con una sua dimensione spaziale interna e che organizza i neuroni
 *         all'interno nello spazio del cluster (bi- o tri-dimensionale) ed usare un InnerLinker derivato o
 *         direttamente l'update del cluster stesso per gestire le attivazioni dipendenti dalla posizione spaziale
 *         come nelle GasNet. </li>
 *    <li> Le Ricorrenze... quante e quali pi&ugrave; vi piacciono !! Nelle reti in cui vengono copiati gli output di certi
 *         neuroni come input di altri neuroni possono essere realizzate attraverso dei Linker che connettono
 *         uno-a-uno che semplicemente eseguono una copia dal cluster d'ingresso a quello d'uscita. </li>
 * </ul>
 *
 * \todo Per generare una .DLL sotto windows ogni classe deve essere preceduta da __declspec(dllexport)
 *       (vedere qwt_globals.h per come fare)
 *
 */

/*! \file
 *  \brief This is the main header files!! Include this file in your project... and play :-)
 *
 *  Details...
 */

#include "types.h"
#include "messages.h"
#include "cluster.h"
#include "linker.h"
#include "neuralnet.h"
#include "libclusterupdaters.h"

#endif

