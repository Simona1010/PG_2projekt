#pragma once
#include <QtWidgets>
#include "GeometricalShapes.h"
class ViewerWidget :public QWidget {
	Q_OBJECT
private:
	QSize areaSize = QSize(0, 0);
	QImage* img = nullptr;
	QPainter* painter = nullptr;
	uchar* data = nullptr;

	bool drawLineActivated = false;
	QPoint drawLineBegin = QPoint(0, 0);

	bool stateLeftButton = false;

	QVector<QVector<QColor>> colorForAllPixels;
	QVector <QVector<int>> depth;

public:
	ViewerWidget(QSize imgSize, QWidget* parent = Q_NULLPTR);
	~ViewerWidget();
	void resizeWidget(QSize size);

	//Image functions
	bool setImage(const QImage& inputImg);
	QImage* getImage() { return img; };
	bool isEmpty();
	bool changeSize(int width, int height);

	void setPixel(int x, int y, uchar r, uchar g, uchar b, uchar a = 255);
	void setPixel(int x, int y, double valR, double valG, double valB, double valA = 1.);
	void setPixel(int x, int y, const QColor& color);
	bool isInside(int x, int y) { return (x >= 0 && y >= 0 && x < img->width() && y < img->height()) ? true : false; }


	//Draw functions
	void swapPoints(QPoint P1, QPoint P2);
	void DDA(QPoint start, QPoint end, QColor color,int layer);
	void drawLine(QVector<QPoint> points, QColor shapeColor,int layer );
	void drawCircle(QVector<QPoint> points, QColor color,int layer);
	void drawPolygon(QVector<QPoint> points,QColor color, int layer);
	void drawBezierCurve(QVector<QPoint> points, QColor color, int layer);
	void drawSquare(QVector<QPoint> points, QColor color, int layer);

	//Trimming functions
	float max(float t1, float t2);
	float min(float t1, float t2);
	void Cyrus_Beck(QVector<QPoint> points, QColor color, int layer);
	void Sutherland_Hodgman(QVector<QPoint> points, QColor color);

	//Transformation
	void setTranslation(bool state) { stateLeftButton = state; }
	bool getTranslation() { return stateLeftButton; }
	void translation(QPoint point, QColor color, int algType);

	//Z-buffer
	void initializationZBuffer();
	void setZBuffer(int i, int j, QColor color, int z);
	void drawZBuffer();
	
	void setDrawLineBegin(QPoint begin) { drawLineBegin = begin; }
	QPoint getDrawLineBegin() { return drawLineBegin; }
	void setDrawLineActivated(bool state) { drawLineActivated = state; }
	bool getDrawLineActivated() { return drawLineActivated; }

	//Get/Set functions
	uchar* getData() { return data; }
	void setDataPtr() { data = img->bits(); }
	void setPainter() { painter = new QPainter(img); }

	int getImgWidth() { return img->width(); };
	int getImgHeight() { return img->height(); };

	void clear();

public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};