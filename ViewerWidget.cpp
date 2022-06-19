#include   "ViewerWidget.h"
#include "GeometricalShapes.h"

ViewerWidget::ViewerWidget(QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	if (imgSize != QSize(0, 0)) {
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
	}
}
ViewerWidget::~ViewerWidget()
{
	delete painter;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img != nullptr) {
		delete painter;
		delete img;
	}
	img = new QImage(inputImg);
	if (!img) {
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	setDataPtr();
	update();

	return true;
}
bool ViewerWidget::isEmpty()
{
	if (img == nullptr) {
		return true;
	}

	if (img->size() == QSize(0, 0)) {
		return true;
	}
	return false;
}

bool ViewerWidget::changeSize(int width, int height)
{
	QSize newSize(width, height);

	if (newSize != QSize(0, 0)) {
		if (img != nullptr) {
			delete painter;
			delete img;
		}

		img = new QImage(newSize, QImage::Format_ARGB32);
		if (!img) {
			return false;
		}
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
		update();
	}

	return true;
}

void ViewerWidget::setPixel(int x, int y, uchar r, uchar g, uchar b, uchar a)
{
	r = r > 255 ? 255 : (r < 0 ? 0 : r);
	g = g > 255 ? 255 : (g < 0 ? 0 : g);
	b = b > 255 ? 255 : (b < 0 ? 0 : b);
	a = a > 255 ? 255 : (a < 0 ? 0 : a);

	size_t startbyte = y * img->bytesPerLine() + x * 4;
	data[startbyte] = b;
	data[startbyte + 1] = g;
	data[startbyte + 2] = r;
	data[startbyte + 3] = a;
}
void ViewerWidget::setPixel(int x, int y, double valR, double valG, double valB, double valA)
{
	valR = valR > 1 ? 1 : (valR < 0 ? 0 : valR);
	valG = valG > 1 ? 1 : (valG < 0 ? 0 : valG);
	valB = valB > 1 ? 1 : (valB < 0 ? 0 : valB);
	valA = valA > 1 ? 1 : (valA < 0 ? 0 : valA);

	size_t startbyte = y * img->bytesPerLine() + x * 4;
	data[startbyte] = static_cast<uchar>(255 * valB);
	data[startbyte + 1] = static_cast<uchar>(255 * valG);
	data[startbyte + 2] = static_cast<uchar>(255 * valR);
	data[startbyte + 3] = static_cast<uchar>(255 * valA);
}
void ViewerWidget::setPixel(int x, int y, const QColor& color)
{
	if (color.isValid()) {
		size_t startbyte = y * img->bytesPerLine() + x * 4;

		data[startbyte] = color.blue();
		data[startbyte + 1] = color.green();
		data[startbyte + 2] = color.red();
		data[startbyte + 3] = color.alpha();
	}
}



void ViewerWidget::DDA(QPoint start, QPoint end, QColor color,int layer)
{

	float  m, Y, zmenaX, zmenaY, X = 0;
	setZBuffer(start.x(), start.y(),color, layer);
	setZBuffer(end.x(), end.y(), color, layer);
	//setPixel(start.x(), start.y(), color);
	//setPixel(end.x(), end.y(), color);

	zmenaX = end.x() - start.x();
	zmenaY = end.y() - start.y();
	m = zmenaY / zmenaX;

	X = start.x();
	Y = start.y();

	if (-1 < m && m < 1) {
		while (X < end.x()) {
			X++;
			Y = Y + m;
			setZBuffer(X, static_cast<int>(Y), color, layer);
			//setPixel(X, trunc(Y), color);
		}
		while (X > end.x()) {
			X--;
			Y = Y - m;
			setZBuffer(X, static_cast<int>(Y), color, layer);
			//setPixel(X, trunc(Y), color);
		}
	}

	if (-1 > m || m > 1) {
		while (Y < end.y()) {
			X = X + 1 / m;
			Y++;
			setZBuffer(static_cast<int>(X),Y , color, layer);
			//setPixel(trunc(X), Y, color);
		}
		while (Y > end.y()) {
			X = X - 1 / m;
			Y--;
			setZBuffer(static_cast<int>(X), Y, color, layer);
			//setPixel(trunc(X), Y, color);
		}
	}
	update();

}
//Draw functions
void ViewerWidget::drawLine(QVector<QPoint> points, QColor shapeColor, int layer)
{
	QPoint start = points[0];
	QPoint end = points[1];
	DDA(start, end, shapeColor,layer);
	update();
}
void ViewerWidget::drawCircle(QVector<QPoint> points, QColor color,int layer)
{
	QPoint center = points[0];
	QPoint radius = points[1];


	int X, Y = 0;
	float r, p1, dvaX, dvaY = 0;
	r = sqrt(pow(radius.x() - center.x(), 2) + pow(radius.y() - center.y(), 2));
	X = 0;
	Y = r;
	p1 = 1 - r;
	dvaX = 3;
	dvaY = 2 * r + 2;

	while (X <= Y) {

		if (p1 > 0) {
			p1 = p1 - dvaY;
			Y--;
			dvaY = dvaY - 2;
		}
		else {
			p1 = p1 + dvaX;
			dvaX = dvaX + 2;
			X++;
		}
		//setPixel(X, Y, color);
		setPixel(X + center.x(), Y + center.y(), color);
		setPixel(Y + center.x(), X + center.y(), color);
		setPixel(Y + center.x(), -X + center.y(), color);
		setPixel(-X + center.x(), Y + center.y(), color);
		setPixel(-X + center.x(), -Y + center.y(), color);
		setPixel(-Y + center.x(), -X + center.y(), color);
		setPixel(-Y + center.x(), X + center.y(), color);
		setPixel(X + center.x(), -Y + center.y(), color);

		//qDebug() << "body ktore sa vykresluju " << X << "   " << Y;

	}


	setPixel(center.x(), center.y(), color);//prvy pixel
	setPixel(radius.x(), radius.y(), color); //posledny pixel
	update();

}

void ViewerWidget::drawPolygon(QVector<QPoint> points, QColor color, int layer)
{
	for (int i = 0; i < points.size() - 1; i++) {
		DDA(points[i], points[i + 1], color, layer);
	}
	DDA(points[0], points[points.size() - 1], color, layer);
}

void ViewerWidget::drawBezierCurve(QVector<QPoint> points, QColor color, int layer)
{//de Casteljauov algoritmus
	QVector<QVector<QPoint>> P; //2D pole
	P.resize(points.size()); //P_n
	for (int i = 0; i < P.size(); i++) {
		P[i].resize(P.size() - i); //P_n-1
	}
	for (int i = 0; i < points.size(); i++) {
		P[0][i] = points[i]; //prvy riadok:P_0_0 = P_0, P_0_1 = P_1,...P_0_n-1= P_n-1
	}
	double deltaT = 0.01; 
	double t = deltaT;
	QPoint Q_0 = points[0]; //prvy segment bodu krivky Q_0 na P_0
	QPoint Q_1;

	while (t < 1) {
		for (int i = 1; i < points.size(); i++) {
			for (int j = 0; j < points.size() - i; j++) {
				P[i][j] = (1 - t) * P[i - 1][j] + t * P[i - 1][j + 1];
			}
		}
		Q_1 = P[points.size() - 1][0]; //druhy segment krivky Q_1  na P_n-1_0
		QPoint Q0 = QPoint(static_cast<int>(Q_0.x()), static_cast<int>(Q_0.y()));
		QPoint Q1 = QPoint(static_cast<int>(Q_1.x()), static_cast<int>(Q_1.y()));
		//vykreslenie spojnice Q_0 a Q_1
		DDA(Q0,Q1 ,color, layer);
		Q_0 = Q_1;
		t = t + deltaT;

	}

	QPoint Q0 = QPoint(static_cast<int>(Q_0.x()), static_cast<int>(Q_0.y()));
	DDA(Q0, points[points.size() - 1], color, layer); //vykreslenie posledneho segmentu
	update();
}
void ViewerWidget::swapPoints(QPoint P1, QPoint P2)
{
	QPoint tempPoint = QPoint(0,0);
	tempPoint = P1;
	P1 = P2;
	P2 = tempPoint;

}
void ViewerWidget::drawSquare(QVector<QPoint> points, QColor color, int layer)
{
	QPoint B = QPoint(points[0].x(), points[1].y());
	QPoint D = QPoint(points[1].x(), points[0].y());
	points.push_back(B);
	points.push_back(D);
	points.swapItemsAt(1, 2);
	DDA(points[0], points[1], color, layer);
	DDA(points[0], points[3], color, layer);
	DDA(points[2], points[1], color, layer);
	DDA(points[2], points[3], color, layer);


}

float ViewerWidget::max(float t1, float t2)
{
	float maximum = INT_MIN;
	if (t1 > t2)
		maximum = t1;
	else
		maximum = t2;
	return maximum;
}

float ViewerWidget::min(float t1, float t2)
{
	float minimum = INT_MAX;
	if (t1 < t2)
		minimum = t1;
	else
		minimum = t2;
	return minimum;
}

void ViewerWidget::Cyrus_Beck(QVector<QPoint> points,QColor color,int layer)
{
	QPoint w, d;
	float d_n, w_n, t;
	float t_l = 0, t_u = 1;
	QVector <QPoint> E, norm;
	QVector <int> dx, dy;
	QPoint tempP1, tempP2;
	int x_min = 0, x_max = getImgWidth();
	int y_min = 0, y_max = getImgHeight();

	for (int i = 0; i < 2; i++) {
		if (x_min <= points[i].x() && points[i].x() <= x_max && y_min <= points[i].y() && points[i].y() <= y_max) {

			tempP1 = points[0];
			tempP2 = points[1];
			E.append(QPoint(0, 0));
			E.append(QPoint(getImgWidth(), 0));
			E.append(QPoint(getImgWidth(), getImgHeight()));
			E.append(QPoint(0, getImgHeight()));

			d = points[1] - points[0];
			for (int i = 0; i < E.size() - 1; i++) {
				dx.append(E[i + 1].x() - E[i].x());
				dy.append(E[i + 1].y() - E[i].y());
			}
			dx.append(E[3].x() - E[0].x());
			dy.append(E[3].y() - E[0].y());

			for (int i = 0; i < dx.size(); i++) {
				norm.append(QPoint(-dy[i], dx[i]));
			}


			for (int i = 0; i < E.size(); i++) {
				w = points[0] - E[i];
				d_n = (d.x() * norm[i].x()) + (d.y() * norm[i].y());
				w_n = (w.x() * norm[i].x()) + (w.y() * norm[i].y());
				if (d_n != 0) {
					t = -w_n / d_n;
					if (d_n > 0 && t <= 1) {
						t_l = max(t, t_l);
					}
					if (d_n < 0 && t >= 0) {
						t_u = min(t, t_u);
					}
				}
				if (t_l == 0 && t_u == 1)
					continue;

				if (t_l < t_u) {
					tempP1 = points[0] + (points[1] - points[0]) * t_l;
					tempP2 = points[0] + (points[1] - points[0]) * t_u;
				}
			}
			DDA(tempP1, tempP2,color, layer);
			update();
		}
	}


}

void ViewerWidget::Sutherland_Hodgman(QVector<QPoint> points, QColor color)
{
}

void ViewerWidget::translation(QPoint point, QColor color, int algType)
{
}

void ViewerWidget::initializationZBuffer()
{
	depth.resize(getImgWidth());
	for (int i = 0; i < depth.size(); i++) {
		depth[i].resize(getImgHeight());
	}

	colorForAllPixels.resize(getImgWidth());
	for (int i = 0; i < colorForAllPixels.size(); i++) {
		colorForAllPixels[i].resize(getImgHeight());
	}
	
	for (int i = 0; i < getImgWidth(); i++) {
		for (int j = 0; j < getImgHeight(); j++) {
			depth[i][j] = -10;
			colorForAllPixels[i][j] = Qt::white;
		}
	}


}

void ViewerWidget::setZBuffer(int i, int j, QColor color, int z)
{
	if (isInside(i, j)) {
		if (depth[i][j] < z) {
			colorForAllPixels[i][j] = color;
			depth[i][j] = z;
		}
	}
}

void ViewerWidget::drawZBuffer()
{
	for (int i = 0; i < getImgWidth(); i++) {
		for (int j = 0; j < getImgHeight(); j++) {
			setPixel(i, j, colorForAllPixels[i][j]);
				update();
		}
	}
}


void ViewerWidget::clear()
{
	img->fill(Qt::white);
	update();
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}