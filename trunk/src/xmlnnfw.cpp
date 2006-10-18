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

#include "ionnfw.h"
#include "neuralnet.h"
#include "messages.h"
#include "nnfwfactory.h"
#include "propertized.h"
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

const char* convString( char* out, const xmlChar* in ) {
    int lenin = xmlStrlen( in );
    int lenout = lenin;
    if ( UTF8Toisolat1( (unsigned char*)out, &lenout, (const unsigned char*)in, &lenin ) < 0 ) {
        nnfwMessage( NNFW_ERROR, "Error converting string" );
    }
    return out;
}

void parseProperty_10( xmlDocPtr doc, xmlNodePtr cur, const Propertized* obj ) {
    char buf[100];
    AbstractPropertyAccess* pacc = obj->propertySearch( convString( buf, cur->name ) );
    if ( !pacc ) {
        char msg[100];
        sprintf( msg, "the property %s doesn't exist in %s", cur->name, obj->getTypename().getString() );
        nnfwMessage( NNFW_ERROR, msg );
    }
    nnfwMessage( NNFW_WARNING, "parseProperty not yet implemented" );
    // --- property type checking
    bool ok = true;
    xmlChar* text;
    switch( pacc->type() ) {
    case Variant::t_null:
        nnfwMessage( NNFW_WARNING, "Setting a Null type Variant" );
        break;
    case Variant::t_real:
        text = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
        ok = pacc->set( Variant( (Real)( xmlXPathCastStringToNumber( text ) ) ) );
        break;
    case Variant::t_int: 
        text = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
        ok = pacc->set( Variant( (int)( xmlXPathCastStringToNumber( text ) ) ) );
        break;
    case Variant::t_uint:
        text = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
        ok = pacc->set( Variant( (unsigned int)( xmlXPathCastStringToNumber( text ) ) ) );
        break;
    case Variant::t_char:
        text = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
        // ?!?! --- ok = pacc->set( Variant( (Real)( xmlXPathCastStringToNumber( text ) ) ) );
        ok = false;
        break;
    case Variant::t_uchar:
        text = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
        // ?!?! --- ok = pacc->set( Variant( (Real)( xmlXPathCastStringToNumber( text ) ) ) );
        ok = false;
        break;
    case Variant::t_bool:
        text = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
        // --- E' case-sensitive ?!?!
        if ( !xmlStrcmp( cur->name, (const xmlChar* ) "true" ) ) {
            ok = pacc->set( Variant( true ) );
        } else {
            ok = pacc->set( Variant( false ) );
        }
        break;
    case Variant::t_string:
        text = xmlNodeListGetString( doc, cur->xmlChildrenNode, 1 );
        ok = pacc->set( Variant( convString( buf, text ) ) );
        break;
    case Variant::t_realvec:
        //--- da implementare
        ok = false;
        break;
    case Variant::t_realmat:
        //--- da implementare
        ok = false;
        break;
    case Variant::t_outfunction:
        // --- non dovrebbe arrivare qui !! ?!?!
        nnfwMessage( NNFW_ERROR, "Why don't use the outputfunciont tag for setting it" );
        ok = false;
        break;
    case Variant::t_cluster:
    case Variant::t_linker:
        nnfwMessage( NNFW_ERROR, "Cluster and Linker are own tags" );
        ok = false;
        break;
    case Variant::t_propertized:
        // --- parsing children nodes for settings others properties
        const Propertized* sub = pacc->get().getPropertized();
        cur = cur->xmlChildrenNode;
        while( cur != NULL ) {
            parseProperty_10( doc, cur, sub );
        }
        break;
    }
    if ( !ok ) {
        char msg[100];
        sprintf( msg, "There was an error settings the property %s", cur->name );
        nnfwMessage( NNFW_ERROR, msg );
    }
    return;
}

void parseCluster_10( xmlDocPtr doc, xmlNodePtr cur, BaseNeuralNet* net ) {
    char buf[100];
    // --- parsing tag <cluster>
    xmlChar* name = xmlGetProp( cur, (const xmlChar*) "name" );
    if ( !name ) {
        nnfwMessage( NNFW_ERROR, "attribute name of <cluster> is mandatory" );
    }
    xmlChar* type = xmlGetProp( cur, (const xmlChar*) "type" );
    if ( !type ) {
        nnfwMessage( NNFW_ERROR, "attribute type of <cluster> is mandatory" );
    }
    xmlChar* size = xmlGetProp( cur, (const xmlChar*) "size" );
    if ( !size ) {
        nnfwMessage( NNFW_ERROR, "attribute size of <cluster> is mandatory" );
    }
    PropertySettings prop;
    prop["name"] = convString( buf, name );
    prop["size"] = Variant( (unsigned int)(xmlXPathCastStringToNumber( size ) ) );
    Cluster* cl = Factory::createCluster( convString( buf, type ), prop );
    net->addCluster( cl );

    // --- parsing children nodes for settings others properties
    cur = cur->xmlChildrenNode;
    while( cur != NULL ) {
        if ( !xmlStrcmp( cur->name, (const xmlChar* ) "outputfunction" ) ) {
            // --- <outputfunction>
        } else if ( !xmlStrcmp( cur->name, (const xmlChar* ) "accumulate" ) ) {
            // --- <accumulate>
        } else if ( !xmlStrcmp( cur->name, (const xmlChar* ) "randomize" ) ) {
            // --- <randomize>
        } else {
            // --- nodo proprieta'
            parseProperty_10( doc, cur, cl );
        }
    }

    xmlFree(name);
    xmlFree(type);
    xmlFree(size);
}

void parseLinker_10( xmlDocPtr doc, xmlNodePtr cur, BaseNeuralNet* net ) {
}

void parseOrder_10( xmlDocPtr doc, xmlNodePtr cur, BaseNeuralNet* net ) {
}

void parseConfigure_10( xmlDocPtr doc, xmlNodePtr cur, BaseNeuralNet* net ) {
}

void parseNeuralnet_10( xmlDocPtr doc, xmlNodePtr cur, BaseNeuralNet* net ) {
    cur = cur->xmlChildrenNode;
    if ( xmlStrcmp( cur->name, (const xmlChar*) "neuralnet" ) ) {
        nnfwMessage( NNFW_ERROR, "Syntax error" );
        return;
    }
    // --- parsing tag <neuralnet>
    cur = cur->xmlChildrenNode;
    while( cur != NULL ) {
        if ( !xmlStrcmp( cur->name, (const xmlChar* ) "cluster" ) ) {
            // --- <cluster>
            parseCluster_10( doc, cur, net );
        } else if ( !xmlStrcmp( cur->name, (const xmlChar*) "linker" ) ) {
            // --- <linker>
            parseLinker_10( doc, cur, net );
        } else if ( !xmlStrcmp( cur->name, (const xmlChar*) "order" ) ) {
            // --- <order>
            parseOrder_10( doc, cur, net );
        } else if ( !xmlStrcmp( cur->name, (const xmlChar*) "configure" ) ) {
            // --- <configure>
            parseConfigure_10(doc, cur, net );
        } else {
            char msg[100];
            sprintf( msg, "Unrecognized tag: %s", cur->name );
            nnfwMessage( NNFW_WARNING, msg );
        }
        cur = cur->next;
    }

    nnfwMessage( NNFW_WARNING, "Not yet implemented" );
}

BaseNeuralNet* loadXML( const char* filename ) {
    BaseNeuralNet* net = new BaseNeuralNet();
    xmlDocPtr doc;
    doc = xmlParseFile( filename );
    if ( !doc ) {
        nnfwMessage( NNFW_ERROR, "Error reading the filename" );
        return net;
    }

    xmlNodePtr cur = xmlDocGetRootElement( doc );
    if (!cur) {
        nnfwMessage( NNFW_WARNING, "Readed an empty file" );
        return net;
    }

    if ( xmlStrcmp( cur->name, (const xmlChar*) "nnfw" ) ) {
        nnfwMessage( NNFW_ERROR, "Wrong type of document" );
        return net;
    }
    // --- checking the version of XML file
    xmlChar* ver = xmlGetProp( cur, (const xmlChar* ) "version" );
    if ( !xmlStrcmp( ver, (const xmlChar* ) "1.0" ) ) {
        //--- version 1.0
        parseNeuralnet_10( doc, cur, net );
    }
    return net;
}

bool saveXML( const char* filename, BaseNeuralNet* net ) {
    return false;
}

}

