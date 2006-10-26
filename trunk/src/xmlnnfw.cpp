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

#include <QDomDocument>
#include <QFile>
#include <QString>

//! Namespace that contains all classes of Neural Network Framework
namespace nnfw {

void parseProperty_10( QDomElement cur, const Propertized* obj ) {
    AbstractPropertyAccess* pacc = obj->propertySearch( cur.tagName().toAscii().constData() );
    if ( !pacc ) {
        char msg[100];
        sprintf( msg, "the property %s doesn't exist in %s",
                cur.tagName().toAscii().constData(), obj->getTypename().getString() );
        nnfwMessage( NNFW_ERROR, msg );
    }
    // --- property type checking
    bool ok = true;
    QString text;
    switch( pacc->type() ) {
    case Variant::t_null:
        nnfwMessage( NNFW_WARNING, "Setting a Null type Variant" );
        break;
    case Variant::t_real:
#ifdef NNFW_DOUBLE_PRECISION
        ok = pacc->set( Variant( cur.text().toDouble() ) );
#else
        ok = pacc->set( Variant( cur.text().toFloat() ) );
#endif
        break;
    case Variant::t_int: 
        ok = pacc->set( Variant( cur.text().toInt() ) );
        break;
    case Variant::t_uint:
        ok = pacc->set( Variant( cur.text().toUInt() ) );
        break;
    case Variant::t_char:
        ok = pacc->set( Variant( cur.text().at(0).toAscii() ) );
        break;
    case Variant::t_uchar:
        ok = pacc->set( Variant( (unsigned char)(cur.text().at(0).toAscii()) ) );
        break;
    case Variant::t_bool:
        if ( cur.text().toLower() == QString( "true" ) ) {
            ok = pacc->set( Variant( true ) );
        } else {
            ok = pacc->set( Variant( false ) );
        }
        break;
    case Variant::t_string:
        ok = pacc->set( Variant( cur.text().toAscii().constData() ) );
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
        nnfwMessage( NNFW_ERROR, "Why don't use the outputfunction tag for setting it" );
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
        QDomNode child = cur.firstChild();
        while( ! child.isNull() ) {
            QDomElement e = child.toElement();
            if ( e.isNull() ) {
                child = child.nextSibling();
                continue;
            }
            parseProperty_10( e, sub );
            child = child.nextSibling();
        }
        break;
    }
    if ( !ok ) {
        char msg[100];
        sprintf( msg, "There was an error settings the property %s", cur.tagName().toAscii().constData() );
        nnfwMessage( NNFW_ERROR, msg );
    }
    return;
}

void parseCluster_10( QDomElement cur, BaseNeuralNet* net ) {
    // --- parsing tag <cluster>
    QString name = cur.attribute( "name" );
    if ( name.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute name of <cluster> is mandatory" );
    }
    QString type = cur.attribute( "type" );
    if ( type.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute type of <cluster> is mandatory" );
    }
    QString size = cur.attribute( "size" );
    if ( size.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute size of <cluster> is mandatory" );
    }
    PropertySettings prop;
    prop["name"] = name.toAscii().constData();
    prop["size"] = Variant( size.toUInt() );
    Cluster* cl = Factory::createCluster( type.toAscii().constData(), prop );
    net->addCluster( cl );

    // --- parsing children nodes for settings others properties
    QDomNode child = cur.firstChild();
    while( ! child.isNull() ) {
        QDomElement e = child.toElement();
        if ( e.isNull() ) {
            child = child.nextSibling();
            continue;
        }
        if ( e.tagName() == QString( "outputfunction" ) ) {
            // --- <outputfunction>
            nnfwMessage( NNFW_WARNING, "parsing of <outputfunction> is not yet implemented" );
        } else if ( e.tagName() == QString( "accumulate" ) ) {
            // --- <accumulate>
            cl->accumulate( e.text().toLower() == QString( "true" ) );
        } else if ( e.tagName() == QString( "randomize" ) ) {
            // --- <randomize>
            QString min = e.attribute( "min" );
            QString max = e.attribute( "max" );
            if ( min.isNull() || max.isNull() ) {
                nnfwMessage( NNFW_ERROR, "attributes min and max are mandatory in <randomize> tag" );
            }
#ifdef NNFW_DOUBLE_PRECISION
            double minV = min.toDouble();
            double maxV = max.toDouble();
#else
            float minV = min.toFloat();
            float maxV = max.toFloat();
#endif
            cl->randomize( minV, maxV );
        } else {
            // --- nodo proprieta'
            parseProperty_10( e, cl );
        }
        child = child.nextSibling();
    }
}

void parseLinker_10( QDomElement cur, BaseNeuralNet* net ) {
    // --- parsing tag <linker>
    QString name = cur.attribute( "name" );
    if ( name.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute name of <linker> is mandatory" );
    }
    QString type = cur.attribute( "type" );
    if ( type.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute type of <linker> is mandatory" );
    }
    QString from = cur.attribute( "from" );
    if ( from.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute from of <linker> is mandatory" );
    }
    QString to = cur.attribute( "to" );
    if ( to.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute to of <linker> is mandatory" );
    }
    PropertySettings prop;
    prop["name"] = name.toAscii().constData();
    Cluster* fromcl = (Cluster*)( net->getByName( from.toAscii().constData() ) );
    if ( !fromcl ) {
        nnfwMessage( NNFW_ERROR, "the 'from' Cluster doesn't exist; creation of linker skipped" );
        return;
    }
    Cluster* tocl = (Cluster*)( net->getByName( to.toAscii().constData() ) );
    if ( !tocl ) {
        nnfwMessage( NNFW_ERROR, "the 'to' Cluster doesn't exist; creation of linker skipped" );
        return;
    }
    prop["from"] = fromcl;
    prop["to"] = tocl;
    Linker* link = Factory::createLinker( type.toAscii().constData(), prop );
    net->addLinker( link );

    // --- parsing children nodes for settings others properties
    QDomNode child = cur.firstChild();
    while( ! child.isNull() ) {
        QDomElement e = child.toElement();
        if ( e.isNull() ) {
            child = child.nextSibling();
            continue;
        }
        if ( e.tagName() == QString( "randomize" ) ) {
            // --- <randomize>
            QString min = e.attribute( "min" );
            QString max = e.attribute( "max" );
            if ( min.isNull() || max.isNull() ) {
                nnfwMessage( NNFW_ERROR, "attributes min and max are mandatory in <randomize> tag" );
            }
#ifdef NNFW_DOUBLE_PRECISION
            double minV = min.toDouble();
            double maxV = max.toDouble();
#else
            float minV = min.toFloat();
            float maxV = max.toFloat();
#endif
            link->randomize( minV, maxV );
        } else {
            // --- nodo proprieta'
            parseProperty_10( e, link );
        }
        child = child.nextSibling();
    }
}

void parseOrder_10( QDomElement cur, BaseNeuralNet* net ) {
}

void parseOutputs_10( QDomElement cur, BaseNeuralNet* net ) {
}

void parseInputs_10( QDomElement cur, BaseNeuralNet* net ) {
}

void parseConfigure_10( QDomElement cur, BaseNeuralNet* net ) {
    char buf[100];
    // --- parsing tag <linker>
    QString name = cur.attribute( "name" );
    if ( name.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute name of <configure> is mandatory" );
    }
    Updatable* up = net->getByName( name.toAscii().constData() );
    if ( !up ) {
        sprintf( buf, "Updatable %s doesn't exist in the neural network", name.toAscii().constData() );
        nnfwMessage( NNFW_ERROR, buf );
    }
    // --- parsing children nodes for settings properties
    QDomNode child = cur.firstChild();
    while( ! child.isNull() ) {
        QDomElement e = child.toElement();
        if ( e.isNull() ) {
            child = child.nextSibling();
            continue;
        }
        parseProperty_10( e, up );
        child = child.nextSibling();
    }
}

void parseNeuralnet_10( QDomElement cur, BaseNeuralNet* net ) {
    QDomNode child = cur.firstChild().toElement();
    if ( child.toElement().isNull() ) {
        nnfwMessage( NNFW_ERROR, "Syntax error" );
        return;
    }
    if ( child.toElement().tagName() != QString( "neuralnet" ) ) {
        nnfwMessage( NNFW_ERROR, "Syntax error" );
        return;
    }

    // --- parsing tag <neuralnet>
    child = child.firstChild();
    while( ! child.isNull() ) {
        QDomElement e = child.toElement();
        if ( e.isNull() ) {
            child = child.nextSibling();
            continue;
        }
        if ( e.tagName() == QString( "cluster" ) ) {
            // --- <cluster>
            parseCluster_10( e, net );
        } else if ( e.tagName() == QString( "linker" ) ) {
            // --- <linker>
            parseLinker_10( e, net );
        } else if ( e.tagName() == QString( "order" ) ) {
            // --- <order>
            parseOrder_10( e, net );
        } else if ( e.tagName() == QString( "outputs" ) ) {
            // --- <outputs>
            parseOutputs_10( e, net );
        } else if ( e.tagName() == QString( "inputs" ) ) {
            // --- <inputs>
            parseInputs_10( e, net );
        } else if ( e.tagName() == QString( "configure" ) ) {
            // --- <configure>
            parseConfigure_10( e, net );
        } else {
            char msg[100];
            sprintf( msg, "Unrecognized tag: %s", e.tagName().toAscii().constData() );
            nnfwMessage( NNFW_WARNING, msg );
        }
        child = child.nextSibling();
    }
}

BaseNeuralNet* loadXML( const char* filename ) {
    BaseNeuralNet* net = new BaseNeuralNet();

    QDomDocument doc( "xmldocument" );
    QFile file( filename );
    if ( !file.open( QIODevice::ReadOnly ) ) {
        char msg[100];
        sprintf( msg, "Unable to open file %s", filename );
        nnfwMessage( NNFW_ERROR, msg );
        return net;
    }
    if ( !doc.setContent( &file ) ) {
        char msg[100];
        sprintf( msg, "Error reading file %s", filename );
        nnfwMessage( NNFW_ERROR, msg );
        return net;
    }
    file.close();

    QDomElement rootnode = doc.documentElement();
    if ( rootnode.tagName() != QString( "nnfw" ) ) {
        nnfwMessage( NNFW_ERROR, "Wrong type of document" );
        return net;
    }
    // --- checking the version of XML file
    QString ver = rootnode.attribute( "version" );
    if ( ver == QString( "1.0" ) ) {
        //--- version 1.0
        parseNeuralnet_10( rootnode, net );
    }
    return net;
}

bool saveXML( const char* filename, BaseNeuralNet* net ) {
    return false;
}

}

