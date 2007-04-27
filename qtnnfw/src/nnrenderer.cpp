
#include "nnrenderer.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include <cmath>

class ClusterRenderer : public QGraphicsItem {
public:
	ClusterRenderer( Cluster* cl ) {
		this->cl = cl;
		dim = (int)cl->size();
		//--- calculate the rectangle
		recw = dim*14;
		rech = 14;
		rectlx = -recw/2;
		rectly = -rech/2;
		setFlag(ItemIsMovable);
		setZValue(1);
	};
	
	QRectF boundingRect() const {
		return QRectF( rectlx, rectly, recw, rech );
	};
	QPainterPath shape() const {
		QPainterPath path;
		path.addRect(rectlx, rectly, recw, rech);
		return path;
	};
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
		Q_UNUSED(widget);
		//--- shadow
/*		painter->setPen(Qt::NoPen);
		painter->setBrush(Qt::darkGray);
		painter->drawRect(-7, -7, 20, 20);*/
		if (option->state & QStyle::State_Sunken) {
			painter->setBrush( Qt::gray );
		} else {
			painter->setBrush( Qt::white );
		}
		painter->setPen(QPen(Qt::black, 1));
		painter->drawRect(rectlx, rectly, recw, rech);
		if ( option->levelOfDetail > 2 ) {
			//--- draw the name
			painter->setPen(QPen(Qt::red, 1));
			painter->drawText( QRect(rectlx, rectly, recw, rech), Qt::AlignHCenter, QString(cl->getName()) );
			//--- draw neurons
			painter->setPen(QPen( Qt::black, 1));
			painter->setBrush( Qt::transparent );
			for( int i=0; i<dim; i++ ) {
				painter->drawEllipse( rectlx+( i*14 )+2, rectly+2, 10, 10 );
			}
		} else {
			//--- draw neurons
			for( int i=0; i<dim; i++ ) {
				painter->drawEllipse( rectlx+( i*14 )+2, rectly+2, 10, 10 );
			}
		}
	};
	void addLinker( LinkerRenderer* lr );

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) {
		update();
		QGraphicsItem::mousePressEvent(event);
	}

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
		update();
		QGraphicsItem::mouseReleaseEvent(event);
	}
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	Cluster* cl;
	int dim;
	int recw;
	int rech;
	int rectlx;
	int rectly;
	QVector<LinkerRenderer*> lks;
};


static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

class LinkerRenderer : public QGraphicsItem {
public:
	LinkerRenderer(ClusterRenderer *sourceNode, ClusterRenderer *destNode) : arrowSize(10) {
		setAcceptedMouseButtons(0);
		source = sourceNode;
		dest = destNode;
		source->addLinker( this );
		dest->addLinker( this );
		adjust();
	};

	void adjust() {
		if (!source || !dest) {
			return;
		}
		if ( source == dest ) {
			//--- it's a self-loop
			QRectF rec = source->boundingRect();
			removeFromIndex();
			sourcePoint = mapFromItem( source, -rec.width()/4.0, -rec.height()/2.0 );
			destPoint = mapFromItem( source, -rec.width()/4.0, +rec.height()/2.0 );
			addToIndex();
			return;
		}
		//--- normal condition		
		qreal mstep = 0.01f;
		QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
		qreal ts = 0.0f;
		QRectF r = mapFromItem( source, source->boundingRect() ).boundingRect();
		while( r.contains( line.pointAt( ts ) ) ) {
			ts += mstep;
			if ( ts > 1.0f ) {
				ts = 0.0f;
				break;
			}
		}
		qreal te = 1.0f;
		r = mapFromItem( dest, dest->boundingRect() ).boundingRect();
		while( r.contains( line.pointAt( te ) ) ) {
			te -= mstep;
			if ( te < 0.0f ) {
				te = 1.0f;
				break;
			}
		}
		
		removeFromIndex();
		sourcePoint = line.pointAt(ts);
		destPoint = line.pointAt(te);
		addToIndex();
	};

protected:
	QRectF boundingRect() const {
		if (!source || !dest) {
			return QRectF();
		}
		qreal penWidth = 1;
		qreal extra = (penWidth + arrowSize) / 2.0;
		if ( source == dest ) {
			//---its a self-loop
			QRectF rec = source->boundingRect();
			QPointF c1  = mapFromItem( source, -rec.width()/2.0, -rec.height()*1.5f );
			QPointF c2  = mapFromItem( source, -rec.width(), -rec.height()*1.5f );
			QPointF mid = mapFromItem( source, -rec.width(), 0 );
			QPointF c3  = mapFromItem( source, -rec.width(), +rec.height()*1.5f );
			QPointF c4  = mapFromItem( source, -rec.width()/2.0, +rec.height()*1.5f );
			QPainterPath path;
			path.moveTo( sourcePoint );
			path.cubicTo( c1, c2, mid );
			path.cubicTo( c3, c4, destPoint );
			return path.boundingRect().adjusted(-extra, -extra, extra, extra);
		}
		return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
											destPoint.y() - sourcePoint.y()) )
			.normalized()
			.adjusted(-extra, -extra, extra, extra);
	};

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
		if (!source || !dest) {
			return;
		}
		QLineF line;
		if ( source == dest ) {
			//--- it's a self-loop
			QRectF rec = source->boundingRect();
			QPointF c1  = mapFromItem( source, -rec.width()/2.0, -rec.height()*1.5f );
			QPointF c2  = mapFromItem( source, -rec.width(), -rec.height()*1.5f );
			QPointF mid = mapFromItem( source, -rec.width(), 0 );
			QPointF c3  = mapFromItem( source, -rec.width(), +rec.height()*1.5f );
			QPointF c4  = mapFromItem( source, -rec.width()/2.0, +rec.height()*1.5f );
			QPainterPath path;
			path.moveTo( sourcePoint );
			path.cubicTo( c1, c2, mid );
			path.cubicTo( c3, c4, destPoint );
			painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			painter->drawPath( path );
			//--- angle of Arrow
			line = QLineF( c4, destPoint );
		} else {		
			//--- normal condition
			line = QLineF(sourcePoint, destPoint);
			painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			painter->drawLine(line);
		}
		
		// Draw the arrows if there's enough room
		double angle = ::acos(line.dx() / line.length());
		if (line.dy() >= 0) {
			angle = TwoPi - angle;
		}
		QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
												cos(angle - Pi / 3) * arrowSize);
		QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
												cos(angle - Pi + Pi / 3) * arrowSize);		
		painter->setBrush(Qt::black);
		painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	};

private:
    ClusterRenderer *source, *dest;

	QPointF sourcePoint;
	QPointF destPoint;
	qreal arrowSize; 
};

void ClusterRenderer::addLinker( LinkerRenderer* lr ) {
	lks.append( lr );
	lr->adjust();
}

QVariant ClusterRenderer::itemChange(GraphicsItemChange change, const QVariant &value) {
	switch (change) {
	case ItemPositionChange:
		foreach( LinkerRenderer *lr, lks ) {
			lr->adjust();
		}
		break;
	default:
		break;
	};
	return QGraphicsItem::itemChange(change, value);
}



NNRenderer::NNRenderer( QWidget* parent ): QGraphicsView(parent), clmap(), lkmap() {
	QGraphicsScene* scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setSceneRect(-200, -200, 400, 400);
	setScene(scene);
 	setCacheMode(CacheBackground);
 	setRenderHint(QPainter::Antialiasing);
 	setTransformationAnchor(AnchorUnderMouse);
 	setResizeAnchor(AnchorViewCenter);
	setBackgroundBrush( QColor( 250, 240, 230 ) );

	scaleDisabled = true;
	setMinimumSize(400, 400);
	setWindowTitle(tr("NNFW Neural Network Renderer"));
}

void NNRenderer::setNeuralNet( BaseNeuralNet* net ) {
	QList<QGraphicsItem *> list = scene()->items();
	QList<QGraphicsItem *>::Iterator it = list.begin();
	for (; it != list.end(); ++it) {
		if ( *it ) {
			delete *it;
		}
	}
	this->nn = net;
	if ( net == 0 ) {
		setMatrix( QMatrix() );
		scaleDisabled = true;
		update();
		return;
	}
	const ClusterVec& cls = nn->clusters();
	int xpos = -170; int ypos = -170;
	for( unsigned int i=0; i<cls.size(); i++ ) {
		ClusterRenderer* cs = new ClusterRenderer( cls[i] );
		scene()->addItem( cs );
		cs->setPos( xpos, ypos );
		xpos += 30;
		ypos += 30;
		clmap[ cls[i] ] = cs;
	}

	const LinkerVec& lks = nn->linkers();
	for( unsigned int i=0; i<lks.size(); i++ ) {
		ClusterRenderer* src = clmap[ lks[i]->from() ];
		ClusterRenderer* dst = clmap[ lks[i]->to() ];
		LinkerRenderer* lr = new LinkerRenderer( src, dst );
		scene()->addItem( lr );
		lkmap[ lks[i] ] = lr;
	}
	scaleDisabled = false;
	defaultPositioning();
	update();
}

void NNRenderer::defaultPositioning() {
	if ( nn == 0 ) {
		return;
	}
	float xspace = 15;
	float yspace = 70;
	float xpos = 0;
	float ypos = 0;
	const ClusterVec& cls = nn->inputClusters();
	QVector<ClusterRenderer*> lays;
	for( unsigned int i=0; i<cls.size(); i++ ) {
		lays.append( clmap[cls[i]] );
	}
	// --- start to set positions
	bool ended = false;
	int idstart = 0;
	int idend = lays.size();
	while( !ended ) {
		ended = true;
		for( int i=idstart; i<idend; i++ ) {
			ClusterRenderer* r = lays[i];
			QRectF rect = r->boundingRect();
			r->setPos( xpos+rect.width()/2.0f, ypos+rect.height()/2.0f );
			xpos += rect.width()+xspace;
			// add to next layers
			const LinkerVec& lks = nn->linkers( clmap.key(lays[i]), true );
			for( unsigned int j=0; j<lks.size(); j++ ) {
				ClusterRenderer* tor = clmap[lks[j]->to()];
				if ( lays.indexOf( tor ) != -1 ) continue;
				lays.append( tor );
				ended = false;
			}
		}
		xpos = 0;
		ypos -= yspace;
		idstart = idend;
		idend = lays.size();
	}
	QList<LinkerRenderer*> lsr = lkmap.values();
	for( int i=0; i<lsr.size(); i++ ) {
		lsr[i]->adjust();
	};
}

void NNRenderer::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
	case Qt::Key_Plus:
		scaleView(1.2);
		break;
	case Qt::Key_Minus:
		scaleView(1 / 1.2);
		break;
     default:
		QGraphicsView::keyPressEvent(event);
	}
}

void NNRenderer::wheelEvent(QWheelEvent *event) {
	scaleView(pow((double)2, -event->delta() / 240.0));
}

void NNRenderer::drawBackground(QPainter *painter, const QRectF &rect) {
	QGraphicsView::drawBackground( painter, rect );

	painter->setBrush(Qt::black);
	painter->drawLine( -30, 0, +30, 0 );
	painter->drawLine( 0, -30, 0, +30 );
	
/*	// Shadow
	QRectF sceneRect = this->sceneRect();
	QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
	QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
	if (rightShadow.intersects(rect) || rightShadow.contains(rect))
		painter->fillRect(rightShadow, Qt::darkGray);
	if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
		painter->fillRect(bottomShadow, Qt::darkGray);*/

}

void NNRenderer::scaleView( qreal scaleFactor ) {
	if ( scaleDisabled ) {
		return;
	}
	qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100) {
		return;
	}
	scale(scaleFactor, scaleFactor);
}
