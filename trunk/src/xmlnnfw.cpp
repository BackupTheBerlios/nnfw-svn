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
#include "nnfwfactory.h"
#include "propertized.h"
#include "outputfunction.h"
#include "liboutputfunctions.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

namespace nnfw {

void parseOutputFunction_10( QDomElement cur, Cluster* cl );

void parseProperty_10( QDomElement cur, const Propertized* obj ) {
    AbstractPropertyAccess* pacc = obj->propertySearch( cur.tagName().toAscii().constData() );
    if ( !pacc ) {
        char msg[100];
        sprintf( msg, "the property %s doesn't exist in %s",
                cur.tagName().toAscii().constData(), obj->getTypename().getString() );
        nnfwMessage( NNFW_ERROR, msg );
    }
    // --- check if it's writable
    if ( !pacc->isWritable() ) {
        char msg[100];
        sprintf( msg, "Attempt to set the read-only property %s",
                cur.tagName().toAscii().constData() );
        nnfwMessage( NNFW_ERROR, msg );
    }
    // --- check if it's a Vector Property
    int index = -1;
    if ( pacc->isVector() ) {
        // at the moment, it's mandatory to speficy the index with attribute 'i'
        QString is = cur.attribute( "i" );
        if ( is.isNull() ) {
            char msg[100];
            sprintf( msg, "the property %s is a vector and you have to specify the attribute 'i'",
                    cur.tagName().toAscii().constData() );
            nnfwMessage( NNFW_ERROR, msg );
        }
        index = is.toInt();
    }
    // --- property type checking
    QString text = cur.text().simplified(); // --- used by all
    QStringList list; // --- used by realvec, realmat
    RealVec vec; // --- used by realvec
    RealMat mat(0,0); // --- used by realmat
    const RealMat* vmat; // --- used by realmat
    int rows, cols, count; // --- used by realmat
    QString type; // --- used by outfunction
    PropertySettings prop; // --- used by outfunction & propertized
    QDomNode child; // --- used by outfunction & propertized
    Variant ret; // --- Variant to set after switch
    Propertized* sub = 0; // --- when != 0 then it'll recursively call parseProperty_10 onto sub's child nodes
    switch( pacc->type() ) {
    case Variant::t_null:
        nnfwMessage( NNFW_WARNING, "Setting a Null type Variant" );
        break;
    case Variant::t_real:
#ifdef NNFW_DOUBLE_PRECISION
        ret = Variant( text.toDouble() );
#else
        ret = Variant( text.toFloat() );
#endif
        break;
    case Variant::t_int: 
        ret = Variant( text.toInt() );
        break;
    case Variant::t_uint:
        ret = Variant( text.toUInt() );
        break;
    case Variant::t_char:
        ret = Variant( text.at(0).toAscii() );
        break;
    case Variant::t_uchar:
        ret = Variant( (unsigned char)(text.at(0).toAscii()) );
        break;
    case Variant::t_bool:
        if ( text.toLower() == QString( "true" ) ) {
            ret = Variant( true );
        } else {
            ret = Variant( false );
        }
        break;
    case Variant::t_string:
        ret = Variant( text.toAscii().constData() );
        break;
    case Variant::t_realvec:
        list = text.split( ' ', QString::SkipEmptyParts );
        vec.resize(0);
        for( int i=0; i<list.size(); i++ ) {
#ifdef NNFW_DOUBLE_PRECISION
            vec << list[i].toDouble();
#else
            vec << list[i].toFloat();
#endif
        }
        ret = Variant( &vec );
        break;
    case Variant::t_realmat:
        list = text.split( ' ', QString::SkipEmptyParts );
        if ( index != -1 ) {
            vmat = pacc->get( index ).getRealMat();
        } else {
            vmat = pacc->get().getRealMat();
        }
        rows = vmat->rows();
        cols = vmat->cols();
        if ( list.size() != rows*cols ) {
            nnfwMessage( NNFW_ERROR, "Wrong RealMat dimension" );
            return;
        }
        mat.resize( rows, cols );
        count = 0;
        for( int r=0; r<rows; r++ ) {
            for( int c=0; c<cols; c++ ) {
#ifdef NNFW_DOUBLE_PRECISION
                mat[r][c] = list[count].toDouble();
#else
                mat[r][c] = list[count].toFloat();
#endif
                count++;
            }
        }
        ret = Variant( &mat );
        break;
    case Variant::t_outfunction:
        type = cur.attribute( "type" );
        if ( type.isNull() ) {
            if ( index != -1 ) {
                sub = pacc->get( index ).getOutputFunction();
            } else {
                sub = pacc->get().getOutputFunction();
            }
        } else {
            sub = Factory::createOutputFunction( type.toAscii().constData(), prop );
        }
        ret = Variant( (OutputFunction*)(sub) );
        break;
    case Variant::t_cluster:
    case Variant::t_linker:
        nnfwMessage( NNFW_ERROR, "Cluster and Linker are own tags" );
        return;
    case Variant::t_propertized:
        type = cur.attribute( "type" );
        if ( type.isNull() ) {
            if ( index != -1 ) {
                sub = pacc->get( index ).getPropertized();
            } else {
                sub = pacc->get().getPropertized();
            }
        } else {
            sub = Factory::createPropertized( type.toAscii().constData(), prop );
        }
        ret = Variant( sub );
        break;
    }
    bool ok = true;
    if ( index != -1 ) {
        ok = pacc->set( index, ret );
    } else {
        ok = pacc->set( ret );
    }
    if ( !ok ) {
        char msg[100];
        sprintf( msg, "There was an error settings the property %s", cur.tagName().toAscii().constData() );
        nnfwMessage( NNFW_ERROR, msg );
    }
    if ( sub ) {
        // --- re-get again because the value passed by Variant is temporary
        Variant v;
        if ( index != -1 ) {
            v = pacc->get( index );
        } else {
            v = pacc->get();
        }
        switch( pacc->type() ) {
        case Variant::t_outfunction:
            sub = v.getOutputFunction();
            break;
        case Variant::t_propertized:
            sub = v.getPropertized();
            break;
        default: break;
        }
        // --- parsing children nodes for settings others properties
        child = cur.firstChild();
        while( ! child.isNull() ) {
            QDomElement e = child.toElement();
            if ( e.isNull() ) {
                child = child.nextSibling();
                continue;
            }
            parseProperty_10( e, sub );
            child = child.nextSibling();
        }
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
    // --- parsing tag <order>
    QStringList list = cur.text().simplified().split( ' ', QString::SkipEmptyParts );
    UpdatableVec ord;
    for( int i=0; i<list.size(); i++ ) {
        Updatable* up = net->getByName( list[i].toAscii().constData() );
        if ( up ) {
            ord << up;
        } else {
            char msg[100];
            sprintf( msg, "The Updatable %s specified in <order> doesn't exists", list[i].toAscii().constData() );
            nnfwMessage( NNFW_ERROR, msg );
        }
    }
    net->setOrder( ord );
}

void parseOutputs_10( QDomElement cur, BaseNeuralNet* net ) {
    // --- parsing tag <outputs>
    QStringList list = cur.text().simplified().split( ' ', QString::SkipEmptyParts );
    for( int i=0; i<list.size(); i++ ) {
        Cluster* up = dynamic_cast<Cluster*>( net->getByName( list[i].toAscii().constData() ) );
        if ( up ) {
            net->markAsOutput( up );
        } else {
            char msg[100];
            sprintf( msg, "The Cluster %s specified in <outputs> doesn't exists", list[i].toAscii().constData() );
            nnfwMessage( NNFW_ERROR, msg );
        }
    }
}

void parseInputs_10( QDomElement cur, BaseNeuralNet* net ) {
    // --- parsing tag <inputs>
    QStringList list = cur.text().simplified().split( ' ', QString::SkipEmptyParts );
    for( int i=0; i<list.size(); i++ ) {
        Cluster* up = dynamic_cast<Cluster*>( net->getByName( list[i].toAscii().constData() ) );
        if ( up ) {
            net->markAsInput( up );
        } else {
            char msg[100];
            sprintf( msg, "The Cluster %s specified in <inputs> doesn't exists", list[i].toAscii().constData() );
            nnfwMessage( NNFW_ERROR, msg );
        }
    }
}

void parseConfigure_10( QDomElement cur, BaseNeuralNet* net ) {
    // --- parsing tag <configure>
    QString name = cur.attribute( "name" );
    if ( name.isNull() ) {
        nnfwMessage( NNFW_ERROR, "attribute name of <configure> is mandatory" );
    }
    Updatable* up = net->getByName( name.toAscii().constData() );
    if ( !up ) {
        char msg[100];
        sprintf( msg, "Updatable %s doesn't exist in the neural network", name.toAscii().constData() );
        nnfwMessage( NNFW_ERROR, msg );
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

void saveProperties( QDomDocument doc, QDomElement parent, Propertized* obj, QStringList skip = QStringList() ) {
    PropertyAccessVec& pvec = obj->properties();
    for( u_int i=0; i<pvec.size(); i++ ) {
        AbstractPropertyAccess* p = pvec[i];
        if ( skip.contains( QString( p->name() ) ) ) {
            continue;
        }
        if ( p->isVector() ) {
            char msg[100];
            sprintf( msg, "Saving vector property is not yet supported; the property %s will not be saved", p->name() );
            nnfwMessage( NNFW_ERROR, msg );
            continue;
        }
        QDomElement elem = doc.createElement( p->name() );
        parent.appendChild( elem );
        QDomNode sub;
        Variant v = p->get();
        QString complex; // --- used for create string representation of RealVec and RealMat
        switch( p->type() ) {
        case Variant::t_null:
            break;
        case Variant::t_real:
            sub = doc.createTextNode( QString(" %1 ").arg( v.getReal() ) );
            break;
        case Variant::t_int:
            sub = doc.createTextNode( QString(" %1 ").arg( v.getInt() ) );
            break;
        case Variant::t_uint:
            sub = doc.createTextNode( QString(" %1 ").arg( v.getUInt() ) );
            break;
        case Variant::t_char:
            sub = doc.createTextNode( QString(" %1 ").arg( v.getChar() ) );
            break;
        case Variant::t_uchar:
            sub = doc.createTextNode( QString(" %1 ").arg( v.getUChar() ) );
            break;
        case Variant::t_bool:
            if ( v.getBool() ) {
                sub = doc.createTextNode( " true " );
            } else {
                sub = doc.createTextNode( " false " );
            }
            break;
        case Variant::t_string:
            sub = doc.createTextNode( QString(" %1 ").arg( v.getString() ) );
            break;
        case Variant::t_realvec:
            const RealVec* rv = v.getRealVec();
            for( u_int i=0; i<rv->size(); i++ ) {
                complex.append( QString(" %1").arg( rv->at(i) ) );
            }
            complex.append( " " );
            sub = doc.createTextNode( complex );
            break;
        case Variant::t_realmat:
            const RealMat* mv = v.getRealMat();
            for( u_int r=0; r<mv->rows(); r++ ) {
                for( u_int c=0; c<mv->cols(); c++ ) {
                    complex.append( QString(" %1").arg( mv->at( r, c ) ) );
                }
                complex.append( " " );
                //complex.append( "\n" );
            }
            sub = doc.createTextNode( complex );
            break;
        case Variant::t_outfunction:
            elem.setAttribute( "type", v.getOutputFunction()->getTypename().getString() );
            saveProperties( doc, elem, v.getOutputFunction(), QStringList() << "typename" );
            break;
        case Variant::t_cluster:
            nnfwMessage( NNFW_ERROR, "Saving a property of type Cluster is not handled" );
            break;
        case Variant::t_linker:
            nnfwMessage( NNFW_ERROR, "Saving a property of type Linker is not handled" );
            break;
        case Variant::t_propertized:
            elem.setAttribute( "type", v.getPropertized()->getTypename().getString() );
            saveProperties( doc, elem, v.getPropertized(), QStringList() << "typename" );
            break;
        }
        if ( !sub.isNull() ) {
            elem.appendChild( sub );
        }
    }
}

bool saveXML( const char* filename, BaseNeuralNet* net ) {
    QDomDocument doc("nnfw-xml");
    QDomElement root = doc.createElement( "nnfw" );
    root.setAttribute( "version", "1.0" );
    doc.appendChild( root );

    QDomElement nn = doc.createElement( "neuralnet" );
    root.appendChild( nn );

    const ClusterVec& cls = net->clusters();
    for( unsigned int i=0; i<cls.size(); i++ ) {
        QDomElement elem = doc.createElement( "cluster" );
        elem.setAttribute( "size", cls[i]->size() );
        elem.setAttribute( "type", cls[i]->getTypename().getString() );
        elem.setAttribute( "name", cls[i]->getName() );
        nn.appendChild( elem );
        saveProperties( doc, elem, cls[i], QStringList() << "size" << "typename" << "name" );
    }

    const LinkerVec& ls = net->linkers();
    for( unsigned int i=0; i<ls.size(); i++ ) {
        QDomElement elem = doc.createElement( "linker" );
        elem.setAttribute( "to", ls[i]->to()->getName() );
        elem.setAttribute( "from", ls[i]->from()->getName() );
        elem.setAttribute( "type", ls[i]->getTypename().getString() );
        elem.setAttribute( "name", ls[i]->getName() );
        nn.appendChild( elem );
        saveProperties( doc, elem, ls[i], QStringList() << "to" << "from" << "typename" << "name" );
    }

    QFile file( filename );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        char msg[100];
        sprintf( msg, "Unable to open file %s", filename );
        nnfwMessage( NNFW_ERROR, msg );
        return false;
    }
    QTextStream out(&file);
    out << doc.toString(2);
    return true;
}

}

