#pragma once
#include <QPoint>
#include <QColor>
#include <QString>
#include <QVector>

class Shape {
private:
	QString typeOfShape;
	QVector<QPoint> points;
	QColor shapeColor;
	QColor fillColor;
	bool isFilled ;
	int layer;

public:
	Shape(QString typeOfShape,QVector<QPoint> points,QColor shapeColor,QColor fillColor,bool isFilled,int layer)
	{
		this->typeOfShape = typeOfShape, this->points = points, this->shapeColor = shapeColor, this->fillColor = fillColor, this->isFilled = isFilled, this->layer = layer;
	}
	Shape() {};
	//Set functions
	void setTypeOfShape(QString typeOfShape) { this->typeOfShape = typeOfShape; }
	void setPoints(QVector<QPoint> points) { this->points= points; }
	void setShapeColor(QColor shapeColor) { this->shapeColor= shapeColor; }
	void setFillColor(QColor fillColor) { this->fillColor= fillColor; }
	void setIsFilled(bool isFilled) { this->isFilled= isFilled; }
	void setLayer(int layer) { this->layer = layer; }
	//Get functions
	QString getTypeOfShape() { return this->typeOfShape; }
	QVector<QPoint> getPoints() { return this->points; }
	QColor getShapeColor() { return this->shapeColor; }
	QColor getFillColor() { return this->fillColor; }
	bool getIsFilled() { return this->isFilled; }
	int getLayer() { return this->layer; }

};

//
//class Line
//{
//protected:
//	QPoint start;
//	QPoint end;
//	QColor color;
//	int algType;
//public:
//	Line() {};
//	Line(QPoint start, QPoint end) { this->start = start, this->end = end; }
//	//void DDA(QPoint start, QPoint end, QColor color);
//	//void Bresenham(QPoint start, QPoint end, QColor color);
//	//void drawLine(QPoint start, QPoint end,int algType)
//};
//
//class Square : public Line
//{
//public:
//	Square() {};
//	Square(QPoint start, QPoint end) { this->start = start, this->end = end; }
//	//void drawSquare();
//};
//
//class Polygon : public Line
//{
//private:
//	QVector<QPoint> points;
//public:
//	Polygon() {};
//	Polygon(QVector<QPoint> points) { this->points; }
//	//void drawPolygon(QVector<QPoint> points);
//};
//
//class Circle 
//{
//private:
//	QPoint center;
//	QPoint radius;
//	QColor color;
//public:
//	Circle() {};
//	Circle(QPoint center, QPoint radius) { this->center, this->radius; }
//	//void drawCircle(QPoint center, QPoint radius);
//};
//
