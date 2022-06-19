#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include "ui_ImageViewer.h"
#include "ViewerWidget.h"
#include "GeometricalShapes.h"

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer(QWidget* parent = Q_NULLPTR);

private:
	Ui::ImageViewerClass* ui;
	ViewerWidget* vW;

	QColor globalColor;
	QColor shapeColor ;
	QColor fillColor;
	QSettings settings;
	QMessageBox msgBox;

	QVector<Shape> GeomShapes;
	Shape currentShape;
	
	QVector<QPoint> tempPoints;
	int count=0;
	QString type;
	bool filled;
	
	QPoint start = QPoint(0, 0);
	QPoint end = QPoint(0, 0);
	QPoint rozdiel = QPoint(0, 0);




	//Event filters
	bool eventFilter(QObject* obj, QEvent* event);

	//ViewerWidget Events
	bool ViewerWidgetEventFilter(QObject* obj, QEvent* event);
	void ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event);
	void ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event);
	void ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event);
	void ViewerWidgetLeave(ViewerWidget* w, QEvent* event);
	void ViewerWidgetEnter(ViewerWidget* w, QEvent* event);
	void ViewerWidgetWheel(ViewerWidget* w, QEvent* event);

	//ImageViewer Events
	void closeEvent(QCloseEvent* event);

	//Image functions
	bool openImage(QString filename);
	bool saveImage(QString filename);

private slots:
	void on_actionOpen_triggered();
	void on_actionSave_as_triggered();
	void on_actionClear_triggered();
	void on_actionExit_triggered();

	//Tools slots
	//void on_pushButtonSetColor_clicked();
	void on_pushButton_setOutlineColor_clicked();
	void on_pushButton_setFillColor_clicked();
	void on_listWidget_itemClicked(QListWidgetItem* item);

	//Functions
	//Shape currentLayer(int layer);
	void setcurrentLayer(int i ) {   currentShape = GeomShapes[i]; }
	Shape getcurrentLayer() { return currentShape; }
	void drawLayer(Shape tempShape);

};
