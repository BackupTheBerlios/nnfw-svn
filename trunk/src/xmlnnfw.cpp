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
#include <QDebug>

namespace nnfw {

void parseProperty_10( QDomElement cur, const Propertized* obj ) {
    AbstractPropertyAccess* pacc = obj->propertySearch( cur.tagName().toAscii().constData() );
    if ( !pacc ) {
        nError() << "the property " << cur.tagName().toAscii().constData() << " doesn't exist in "
                 << obj->getTypename().getString();
		return;
    }
    // --- check if it's writable
    if ( !pacc->isWritable() ) {
        nError() << "Attempt to set the read-only property " << cur.tagName().toAscii().constData();
		return;
    }
    // --- check if it's a Vector Property
    int index = -1;
    if ( pacc->isVector() ) {
        // at the moment, it's mandatory to speficy the index with attribute 'i'
        QString is = cur.attribute( "i" );
        if ( is.isNull() ) {
            nError() << "the property " << cur.tagName().toAscii().constData()
					 << " is a vector and you have to specify the attribute 'i'";
			return;
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
		nError() << "Specified a Null type in property " << cur.tagName().toAscii().constData();
		return;
        break;
	case Variant::t_dataptr:
		nError() << "Specified unhandled type in property " << cur.tagName().toAscii().constData();
		return;
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
            nError() << "Wrong RealMat dimension; passed: " << list.size() << "; expected: " << rows*cols;
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
        nError() << "Cluster and Linker are own tags" ;
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
		nError() << "There was an error settings the property " << cur.tagName().toAscii().constData();
		return;
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
        nError() << "attribute name of <cluster> is mandatory" ;
		return;
    }
    QString type = cur.attribute( "type" );
    if ( type.isNull() ) {
        nError() << "attribute type of <cluster> is mandatory" ;
		return;
    }
    QString size = cur.attribute( "size" );
    if ( size.isNull() ) {
        nError() << "attribute size of <cluster> is mandatory" ;
		return;
    }
    PropertySettings prop;
    prop["name"] = name.toAscii().constData();
    prop["numNeurons"] = Variant( size.toAscii().data() );
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
                nError() << "attributes min and max are mandatory in <randomize> tag" ;
				continue;
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

void parseCluster_11( QDomElement cur, BaseNeuralNet* net ) {
    // --- parsing tag <cluster>
    PropertySettings prop;
	QDomNamedNodeMap amap = cur.attributes();
	for( int i=0; i<amap.count(); i++ ) {
		QDomAttr item = amap.item( i ).toAttr();
		std::string name = item.name().toStdString();
		std::string value = item.value().toStdString();
		prop[name] = Variant( value.data() );
	}
    if ( prop["type"].isNull() ) {
        nError() << "attribute type of <cluster> is mandatory" ;
		return;
    }
	//--- add meta-informations
	prop["baseneuralnet"] = Variant( net );
    Cluster* cl = Factory::createCluster( prop["type"].getString(), prop );
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
                nError() << "attributes min and max are mandatory in <randomize> tag" ;
				continue;
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
        nError() << "attribute name of <linker> is mandatory" ;
		return;
    }
    QString type = cur.attribute( "type" );
    if ( type.isNull() ) {
        nError() << "attribute type of <linker> is mandatory" ;
		return;
    }
    QString from = cur.attribute( "from" );
    if ( from.isNull() ) {
        nError() << "attribute from of <linker> is mandatory" ;
		return;
    }
    QString to = cur.attribute( "to" );
    if ( to.isNull() ) {
        nError() << "attribute to of <linker> is mandatory" ;
		return;
    }
    PropertySettings prop;
    prop["name"] = name.toAscii().constData();
    Cluster* fromcl = (Cluster*)( net->getByName( from.toAscii().constData() ) );
    if ( !fromcl ) {
        nError() << "the 'from' Cluster doesn't exist; creation of linker "
				 << name.toAscii().constData() << "skipped" ;
        return;
    }
    Cluster* tocl = (Cluster*)( net->getByName( to.toAscii().constData() ) );
    if ( !tocl ) {
        nError() << "the 'to' Cluster doesn't exist; creation of linker "
				 << name.toAscii().constData() << " skipped" ;
        return;
    }
	//--- add meta-informations ... required from version 0.9.0
	prop["baseneuralnet"] = Variant( net );
    prop["from"] = Variant( from.toAscii().constData() ); // from version 0.9.0 constructors require strings;
    prop["to"] = Variant( to.toAscii().constData() );     // from version 0.9.0 constructors require strings;
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
                nError() << "attributes min and max are mandatory in <randomize> tag" ;
				continue;
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

void parseLinker_11( QDomElement cur, BaseNeuralNet* net ) {
    // --- parsing tag <linker>
    PropertySettings prop;
	QDomNamedNodeMap amap = cur.attributes();
	for( int i=0; i<amap.count(); i++ ) {
		QDomAttr item = amap.item( i ).toAttr();
		std::string name = item.name().toStdString();
		std::string value = item.value().toStdString();
		prop[name] = Variant( value.data() );
	}
    if ( prop["type"].isNull() ) {
        nFatal() << "attribute type of <linker> is mandatory" ;
		return;
    }
	//--- add meta-informations
	prop["baseneuralnet"] = Variant( net );
    Linker* link = Factory::createLinker( prop["type"].getString(), prop );
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
                nError() << "attributes min and max are mandatory in <randomize> tag" ;
				continue;
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
            nWarning() << "The Updatable " << list[i].toAscii().constData() 
					   << " specified in <order> doesn't exists";
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
            nWarning() << "The Cluster %s " << list[i].toAscii().constData()
					   << " specified in <outputs> doesn't exists";
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
            nWarning() << "The Cluster %s " << list[i].toAscii().constData()
					   << " specified in <inpputs> doesn't exists";
        }
    }
}

void parseConfigure_10( QDomElement cur, BaseNeuralNet* net ) {
    // --- parsing tag <configure>
    QString name = cur.attribute( "name" );
    if ( name.isNull() ) {
        nError() << "attribute name of <configure> is mandatory" ;
		return;
    }
    Updatable* up = net->getByName( name.toAscii().constData() );
    if ( !up ) {
        nError() << "Updatable " << name.toAscii().constData() << " doesn't exist in the neural network";
		return;
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
        nError() << "Syntax error" ;
        return;
    }
    if ( child.toElement().tagName() != QString( "neuralnet" ) ) {
        nError() << "Syntax error; Do you forget the <neuralnet> tag ??" ;
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
            nWarning() << "Unrecognized tag: " << e.tagName().toAscii().constData();
        }
        child = child.nextSibling();
    }
}

void parseNeuralnet_11( QDomElement cur, BaseNeuralNet* net ) {
    QDomNode child = cur.firstChild().toElement();
    if ( child.toElement().isNull() ) {
        nError() << "Syntax error" ;
        return;
    }
    if ( child.toElement().tagName() != QString( "neuralnet" ) ) {
        nError() << "Syntax error; Do you forget the <neuralnet> tag ??" ;
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
            parseCluster_11( e, net );
        } else if ( e.tagName() == QString( "linker" ) ) {
            // --- <linker>
			parseLinker_11( e, net );
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
            nWarning() << "Unrecognized tag: " << e.tagName().toAscii().constData();
        }
        child = child.nextSibling();
    }
}

BaseNeuralNet* loadXML( const char* filename ) {
    BaseNeuralNet* net = new BaseNeuralNet();

    QDomDocument doc( "xmldocument" );
    QFile file( filename );
    if ( !file.open( QIODevice::ReadOnly ) ) {
        nError() << "Unable to open file " << filename;
        return net;
    }
    if ( !doc.setContent( &file ) ) {
        nError() << "Error reading file " << filename ;
        return net;
    }
    file.close();

    QDomElement rootnode = doc.documentElement();
    if ( rootnode.tagName() != QString( "nnfw" ) ) {
        nError() << "Wrong type of document; Do you forget the root node <nnfw> ??" ;
        return net;
    }
    // --- checking the version of XML file
    QString ver = rootnode.attribute( "version" );
    if ( ver == QString( "1.0" ) ) {
        //--- version 1.0
        parseNeuralnet_10( rootnode, net );
    } else if ( ver == QString( "1.1" ) ) {
        //--- version 1.1
        parseNeuralnet_11( rootnode, net );
	}
    return net;
}

void saveProperties( QDomDocument doc, QDomElement parent, Propertized* obj, QStringList skip, int precision );

int calcLevelOfIndentation( QDomNode node ) {
	int i=0;
	while( ! node.parentNode().isNull() ) {
		i++;
		node = node.parentNode();
	}
	return i;
}

QDomNode createPropertyFragment( Variant v, QDomDocument doc, QDomElement elem, int precision ) {
    QDomNode sub;
    QString complex; // --- used for create string representation of RealVec and RealMat
    const RealVec* rv;
    const RealMat* mv;
	//--- create a string used to indent row of matrices
	int level = calcLevelOfIndentation( elem );
	QString indentation;
	for( int i=0; i<level-1; i++ ) {
		indentation.append( "  " );
	}

	switch( v.type() ) {
    case Variant::t_null:
        break;
	case Variant::t_dataptr:
		nError() << "Impossible to save a generic data pointer";
		break;
    case Variant::t_real:
        sub = doc.createTextNode( QString(" %1 ").arg( v.getReal(), 0, 'g', precision ) );
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
        rv = v.getRealVec();
        for( u_int i=0; i<rv->size(); i++ ) {
            complex.append( QString(" %1").arg( rv->at(i), 0, 'g', precision ) );
        }
        complex.append( " " );
        sub = doc.createTextNode( complex );
        break;
    case Variant::t_realmat:
        mv = v.getRealMat();
        for( u_int r=0; r<mv->rows(); r++ ) {
			complex.append( "\n" + indentation );
            for( u_int c=0; c<mv->cols(); c++ ) {
				complex.append( QString("%1").arg( mv->at( r, c ), qMax(6, precision+4), 'g', precision ) );
            }
        }
		complex.append( "\n" + indentation );
        sub = doc.createTextNode( complex );
        break;
    case Variant::t_outfunction:
        elem.setAttribute( "type", v.getOutputFunction()->getTypename().getString() );
        saveProperties( doc, elem, v.getOutputFunction(), QStringList() << "typename", precision );
        break;
    case Variant::t_cluster:
        nError() << "Saving a property of type Cluster is not handled" ;
        break;
    case Variant::t_linker:
        nError() << "Saving a property of type Linker is not handled" ;
        break;
    case Variant::t_propertized:
        elem.setAttribute( "type", v.getPropertized()->getTypename().getString() );
        saveProperties( doc, elem, v.getPropertized(), QStringList() << "typename", precision );
        break;
    }
    return sub;
}

QString createAttributeContent( Variant v, int precision ) {
    //QString complex; // --- used for create string representation of RealVec and RealMat
/*    const RealVec* rv;
    const RealMat* mv;*/
    switch( v.type() ) {
    case Variant::t_null:
        break;
	case Variant::t_dataptr:
		nError() << "Impossible to save a generic data pointer";
		return QString();
		break;
    case Variant::t_real:
        return QString("%1").arg( v.getReal(), 0, 'g', precision );
        break;
    case Variant::t_int:
        return QString("%1").arg( v.getInt() );
        break;
    case Variant::t_uint:
        return QString("%1").arg( v.getUInt() );
        break;
    case Variant::t_char:
        return QString("%1").arg( v.getChar() );
        break;
    case Variant::t_uchar:
        return QString("%1").arg( v.getUChar() );
        break;
    case Variant::t_bool:
        if ( v.getBool() ) {
            return QString("true");
        } else {
            return QString("false");
        }
        break;
    case Variant::t_string:
        return QString("%1").arg( v.getString() );
        break;
    case Variant::t_realvec:
		nWarning() << "RealVec read-only property not handled yet";
		return QString();
/*        rv = v.getRealVec();
        for( u_int i=0; i<rv->size(); i++ ) {
            complex.append( QString(" %1").arg( rv->at(i) ) );
        }
        complex.append( " " );
        sub = doc.createTextNode( complex );*/
        break;
    case Variant::t_realmat:
		nWarning() << "RealMat read-only property not handled yet";
		return QString();
/*        mv = v.getRealMat();
        for( u_int r=0; r<mv->rows(); r++ ) {
            for( u_int c=0; c<mv->cols(); c++ ) {
                complex.append( QString(" %1").arg( mv->at( r, c ) ) );
            }
            complex.append( " " );
            //complex.append( "\n" );
        }
        sub = doc.createTextNode( complex );*/
        break;
    case Variant::t_outfunction:
		nWarning() << "OutFunction read-only property not handled yet";
		return QString();
/*        elem.setAttribute( "type", v.getOutputFunction()->getTypename().getString() );
        saveProperties( doc, elem, v.getOutputFunction(), QStringList() << "typename" );*/
        break;
    case Variant::t_cluster:
        nError() << "Saving a property of type Cluster is not handled" ;
        break;
    case Variant::t_linker:
        nError() << "Saving a property of type Linker is not handled" ;
        break;
    case Variant::t_propertized:
        nError() << "Saving a property of type Cluster is not handled" ;
/*        elem.setAttribute( "type", v.getPropertized()->getTypename().getString() );
        saveProperties( doc, elem, v.getPropertized(), QStringList() << "typename" );*/
        break;
    }
    return QString();
}

void saveProperties( QDomDocument doc, QDomElement parent, Propertized* obj, QStringList skip, int precision ) {
    PropertyAccessVec& pvec = obj->properties();
    for( u_int i=0; i<pvec.size(); i++ ) {
        AbstractPropertyAccess* p = pvec[i];
        if ( skip.contains( QString( p->name() ) ) ) {
            continue;
        }
        if ( ! p->isVector() ) {
			if ( p->isWritable() ) {
				QDomElement elem = doc.createElement( p->name() );
				parent.appendChild( elem );
				QDomNode sub = createPropertyFragment( p->get(), doc, elem, precision );
				if ( !sub.isNull() ) {
					elem.appendChild( sub );
				}
			} else {
				parent.setAttribute( p->name(), createAttributeContent( p->get(), precision ) );
			}
            continue;
        }
        // --- Vector property
        int id = 0;
        Variant v;
        while( !(v = p->get(id)).isNull() ) {
            QDomElement elem = doc.createElement( p->name() );
            elem.setAttribute( "i", QString("%1").arg(id) );
            parent.appendChild( elem );
            QDomNode sub = createPropertyFragment( v, doc, elem, precision );
            if ( !sub.isNull() ) {
                elem.appendChild( sub );
            }
            id++;
        }
    }
}

bool saveXML( const char* filename, BaseNeuralNet* net, const char* skipList ) {
	return saveXML( filename, net, -1, skipList );
}

bool saveXML( const char* filename, BaseNeuralNet* net, int precision, const char* skipList ) {
    QDomDocument doc("nnfw-xml");
    QDomElement root = doc.createElement( "nnfw" );
    root.setAttribute( "version", "1.1" );
    doc.appendChild( root );

    QDomElement nn = doc.createElement( "neuralnet" );
    root.appendChild( nn );

	//--- configure the skiplist
	QStringList userSkipList;
	if ( skipList ) {
		QString str = QString(skipList).simplified();
		userSkipList = str.split( ' ', QString::SkipEmptyParts );
	}
	//--- add properties saved explicitly into <cluster> and <linker> attributes
	userSkipList << "numNeurons" << "typename" << "name" << "to" << "from";

    const ClusterVec& cls = net->clusters();
    for( unsigned int i=0; i<cls.size(); i++ ) {
        QDomElement elem = doc.createElement( "cluster" );
        elem.setAttribute( "numNeurons", cls[i]->numNeurons() );
        elem.setAttribute( "type", cls[i]->getTypename().getString() );
        elem.setAttribute( "name", cls[i]->name() );
        saveProperties( doc, elem, cls[i], userSkipList, precision );
        nn.appendChild( elem );
    }

    const LinkerVec& ls = net->linkers();
    for( unsigned int i=0; i<ls.size(); i++ ) {
        QDomElement elem = doc.createElement( "linker" );
        elem.setAttribute( "to", ls[i]->to()->name() );
        elem.setAttribute( "from", ls[i]->from()->name() );
        elem.setAttribute( "type", ls[i]->getTypename().getString() );
        elem.setAttribute( "name", ls[i]->name() );
        nn.appendChild( elem );
        saveProperties( doc, elem, ls[i], userSkipList, precision );
    }

	QDomElement elem = doc.createElement( "inputs" );
	QString str;
	const ClusterVec& cl1 = net->inputClusters();
	for( unsigned int i=0; i<cl1.size(); i++ ) {
		str += QString("%1 ").arg(cl1[i]->name());
	}
	nn.appendChild( elem );
	elem.appendChild( doc.createTextNode( str ) );

	elem = doc.createElement( "outputs" );
	str = "";
	const ClusterVec& cl2 = net->outputClusters();
	for( unsigned int i=0; i<cl2.size(); i++ ) {
		str += QString("%1 ").arg(cl2[i]->name());
	}
	nn.appendChild( elem );
	elem.appendChild( doc.createTextNode( str ) );

	elem = doc.createElement( "order" );
	str = "";
	const UpdatableVec& uls = net->order();
	for( unsigned int i=0; i<uls.size(); i++ ) {
		str += QString("%1 ").arg(uls[i]->name());
	}
	nn.appendChild( elem );
	elem.appendChild( doc.createTextNode( str ) );

    QFile file( filename );
    if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        nError() << "Unable to open file " << filename ;
        return false;
    }
    QTextStream out(&file);
    out << doc.toString(2);
    return true;
}

}
