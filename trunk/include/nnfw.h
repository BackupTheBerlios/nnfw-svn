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

/*! \mainpage Neural Network Framework
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
 *         Hopfield. Infatti, un InnerLinker può connettere i neuroni di un cluster a formare una rete di Hopfield
 *         ed attraverso i Linker usare solo alcuni neuroni come ingresso e altri come output. </li>
 * </ul>
 *
 * <h3>1.2 Regole di Costruzione</h3>
 * <p> Semplicemente si creano i vari cluster e li si connettono come desiderato.</p>
 * <p> Ad ogni cluster inserito all'interno della rete verr&agrave; associato un identificativo numerico, cos&igrave; come ai Linker e InnerLinker. </p>
 *
 * <h3>1.3 La Rete Neurale</h3>
 * <p> La rete neurale &egrave; un oggetto che contiene i vari cluster e linker connessi tra loro. </p>
 * <p> Ad ogni Cluster &egrave; associato un livello che ne determina la gerarchia strutturale. Il livello 0 &egrave; pensato come
 *     il livello contenente i neuroni d'ingresso della rete neurale, e l'ultimo livello come l'output della rete
 *     neurale (anche se questo non &egrave; vincolante). Almeno il livello 0 deve esistere !!! La numerazione dei livelli
 *     non ammette 'buchi'. </p>
 * <p> La rete neurale contiene un orologio temporale che tiene 'il tempo' della rete neurale o degli steps
 *     eseguiti. Questo consente di avere sia una dinamica discreta che continua all'interno della rete neurale.
 *     Infatti, le varie regole di aggiornamento dei linker potrebbero basarsi sia sul tempo che sugli steps della
 *     rete neurale </p>
 * <p> <em>Non ho ben chiaro se la rete neurale deve avere i metodi per manipolare l'architettura, oppure solamente
 *     contenere i cluster e i linker e delegare la creazione della struttura ad un ArchitectureManager che offre i
 *     metodi necessari a manipolare i cluster e i linker. </em></p>
 *
 * <h3>1.4 Spreading della rete</h3>
 * <p> Lo spreading della rete si effettua aggiornando i Cluster, i Linker e gli InnerLinker della rete neurale costruita.</p>
 * <p> Ogni Cluster, sulla base della funzione d'attivazione dei neuroni, aggiorna gli output dei neuroni sulla base
 *     dell'input attuale. </p>
 * <p> Ogni Linker preleva gli output dei neuroni dei cluster d'ingresso, effettua i calcoli necessari e registra gli
 *     input dei neuroni del cluster d'uscita. </p>
 * <p> Ogni InnerLinker aggiorna gli input dei neuroni del cluster sulla base degli input al tempo di calcolo. Gli
 *     InnerLinker sono intrinsecamente ricorrenti, e quindi terranno conto di questo. </p>
 * <p> Il coordinamento fra queste operazioni dipende dalla particolare strategia scelta. Ad esempio, nella
 *     strategia feedforward si partir&agrave; aggiornando i cluster di livello 0 e successivamente i Linker, e cos&igrave; via
 *     fino all'ultimo livello. </p>
 *
 * <h2>2 Come Mantenere l'efficienza</h2>
 * <p> Per consentire che questo meccanismo non perda d'efficienza durante lo spreading della rete, i Linker
 *     e gli InnerLinker dovranno avere accesso alla struttura interna che contiene gli output e gli input dei neuroni
 *     del cluster. Quindi, andranno fatte delle assunzioni forti all'interno della classe Cluster che tutte le
 *     classi derivate dovranno rispettare. </p>
 * <p> Durante l'aggiornamento di un Linker, questo accede direttamente alla struttura interna cos&igrave; da evitare la
 *      chiamata a funzione per ottenere il vettore degli output e dell'input dei neuroni dei cluster. </p>
 * <p> Non ho ancora idea di come far condividere le strutture di memoria senza al tempo stesso farle dipendere da una
 *     implementazione specifica !!! :-( Forse i template mi potrebbero aiutare... o forse no !!
 *     In ogni caso, forse in prima istanza un semplice array statico del C++ creato attraverso l'uso di un template
 *     potrebbe risolvere la questione. Ad esempio, un cluster di un certo numero di neuroni potrebbe essere una
 *     classe template che accetta il numero di neuroni che deve contenere. Si andrebbe a realizzare, cos&igrave;, un
 *     cluster statico, non ridimensionabile... ma che sarebbe veloce !!!
 *     O forse in prima istanza basta una classe che alloca memoria dal costruttore e poi non permette la modifica
 *     a run-time. O forse si ?!?! Mah :-) </p>
 *
 * <h2>3 Si possono realizzare tutte le reti immaginabili ???</h2>
 * <p> B&egrave;, a prima vista direi di si !! </p>
 * <ul>
 *    <li> E' possibile create strutture ibride a piacimento; Anche mischiare continue e discrete insieme... forse
 *         in futuro chiss&agrave; !! :-) </li>
 *    <li> Reti con sinpasi dinamiche o ritardate sia discrete che continue; </li>
 *    <li> Gli spiking neuron anche ?!?! Uhmm... ci devo pensare un pò; </li>
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

