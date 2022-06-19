#include "ImageViewer.h"

ImageViewer::ImageViewer(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::ImageViewerClass)
{
	ui->setupUi(this);
	vW = new ViewerWidget(QSize(500, 500));
	ui->scrollArea->setWidget(vW);

	ui->scrollArea->setBackgroundRole(QPalette::Dark);
	ui->scrollArea->setWidgetResizable(true);
	ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	vW->setObjectName("ViewerWidget");
	vW->installEventFilter(this);

	globalColor = Qt::blue;
	shapeColor = Qt::black;
	fillColor = Qt::green;

	QString style_sheet = QString("background-color: #%1;").arg(globalColor.rgba(), 0, 16);
	//ui->pushButtonSetColor->setStyleSheet(style_sheet);
	ui->pushButton_setOutlineColor->setStyleSheet(style_sheet);
}

// Event filters
bool ImageViewer::eventFilter(QObject* obj, QEvent* event)
{
	if (obj->objectName() == "ViewerWidget") {
		return ViewerWidgetEventFilter(obj, event);
	}
	return false;
}

//ViewerWidget Events
bool ImageViewer::ViewerWidgetEventFilter(QObject* obj, QEvent* event)
{
	ViewerWidget* w = static_cast<ViewerWidget*>(obj);

	if (!w) {
		return false;
	}

	if (event->type() == QEvent::MouseButtonPress) {
		ViewerWidgetMouseButtonPress(w, event);
	}
	else if (event->type() == QEvent::MouseButtonRelease) {
		ViewerWidgetMouseButtonRelease(w, event);
	}
	else if (event->type() == QEvent::MouseMove) {
		ViewerWidgetMouseMove(w, event);
	}
	else if (event->type() == QEvent::Leave) {
		ViewerWidgetLeave(w, event);
	}
	else if (event->type() == QEvent::Enter) {
		ViewerWidgetEnter(w, event);
	}
	else if (event->type() == QEvent::Wheel) {
		ViewerWidgetWheel(w, event);
	}

	return QObject::eventFilter(obj, event);
}
void ImageViewer::ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton)
	{
		tempPoints.push_back(e->pos());
		//if (w->getDrawLineActivated()) {
		//	w->drawLine(w->getDrawLineBegin(), e->pos(), globalColor);
		//	w->setDrawLineActivated(false);
		//}
		//else {
		//	w->setDrawLineBegin(e->pos());
		//	w->setDrawLineActivated(true);
		//	w->setPixel(e->pos().x(), e->pos().y(), globalColor);
		//	w->update();
		//}
	}
	if (e->button() == Qt::RightButton) {
		if (ui->toolButtonDrawLine->isChecked()) {
			//w->drawLine(tempPoints, shapeColor, 0);
			type = "Line";
			filled = false;
		
		}
		if (ui->toolButtonDrawCircle->isChecked()) {
			//w->drawCircle(tempPoints, shapeColor,0);
			type = "Circle";
			filled = false;
			//if (ui->pushButton_setFillColor()->isChecked()) {
			//	filled = true;
			//}
		}
		if (ui->toolButtonDrawPolygon->isChecked()) {
			//w->drawPolygon(tempPoints, shapeColor, 0);
			type = "Polygon";
			filled = false;
			//if (ui->pushButton_setFillColor()->isChecked()) {
			//	filled = true;
			//}
		}

		if (ui->toolButtonBezierCurve->isChecked()) {
			type = "Bezier";
			filled = false;
		}

		if (ui->toolButtonSquare->isChecked()) {
			type = "Square";
			filled = false;
		}
		



		GeomShapes.resize(count + 1);
		
		/*GeomShapes[count].setPoints(tempPoints);
		GeomShapes[count].setTypeOfShape(type);
		GeomShapes[count].setShapeColor(shapeColor);
		GeomShapes[count].setLayer(count);*/
		Shape shape(type, tempPoints, shapeColor, fillColor, filled, count);
		GeomShapes.push_back(shape);
		drawLayer(shape);
		tempPoints.clear();
		count += 1;

		QListWidgetItem* item = new QListWidgetItem;
		item->setText("Layer " + QString::number(count));
		item->setCheckState(Qt::Unchecked);
		ui->listWidget->addItem(item);


	//bool ischeck=ui->listWidget->item(0)->checkState();
	}
	if (e->button() == Qt::LeftButton && ui->toolButtonTranslation->isChecked()) {
		qDebug() << "PRESS translation";
		w->setTranslation(true);
		start = e->pos();
	}
}
void ImageViewer::ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);
	if (e->button() == Qt::LeftButton && w->getTranslation()) {
		w->setTranslation(false);
		qDebug() << "REALEASE translation";
	}
}
void ImageViewer::ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event)
{
	QMouseEvent* e = static_cast<QMouseEvent*>(event);

	if (e->button() == Qt::NoButton && w->getTranslation()) {
		w->clear();
		qDebug() << "MOVE translation";
		Shape tempShape = getcurrentLayer();
		end = e->pos();
		rozdiel = end - start;
		
		QVector<QPoint> tempPoints = tempShape.getPoints();
		for (int i = 0; i < tempPoints.size(); i++) {
			tempPoints[i] = tempPoints[i] + rozdiel;
		}
		
		tempShape.setPoints(tempPoints);
		qDebug() << "in move " << tempPoints;
		start = e->pos();
		////////////////////
		getcurrentLayer().setPoints(tempPoints);
		///////////////////
		drawLayer(tempShape);

	}
	
	
}
void ImageViewer::ViewerWidgetLeave(ViewerWidget* w, QEvent* event)
{
}
void ImageViewer::ViewerWidgetEnter(ViewerWidget* w, QEvent* event)
{
}
void ImageViewer::ViewerWidgetWheel(ViewerWidget* w, QEvent* event)
{
	QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
}

//ImageViewer Events
void ImageViewer::closeEvent(QCloseEvent* event)
{
	if (QMessageBox::Yes == QMessageBox::question(this, "Close Confirmation", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No))
	{
		event->accept();
	}
	else {
		event->ignore();
	}
}

//Image functions
bool ImageViewer::openImage(QString filename)
{
	QImage loadedImg(filename);
	if (!loadedImg.isNull()) {
		return vW->setImage(loadedImg);
	}
	return false;
}
bool ImageViewer::saveImage(QString filename)
{
	QFileInfo fi(filename);
	QString extension = fi.completeSuffix();

	QImage* img = vW->getImage();
	return img->save(filename, extension.toStdString().c_str());
}

//Slots
void ImageViewer::on_actionOpen_triggered()
{
	QString folder = settings.value("folder_img_load_path", "").toString();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getOpenFileName(this, "Load image", folder, fileFilter);
	if (fileName.isEmpty()) { return; }

	QFileInfo fi(fileName);
	settings.setValue("folder_img_load_path", fi.absoluteDir().absolutePath());

	if (!openImage(fileName)) {
		msgBox.setText("Unable to open image.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
	}
}
void ImageViewer::on_actionSave_as_triggered()
{
	QString folder = settings.value("folder_img_save_path", "").toString();

	QString fileFilter = "Image data (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm .*xbm .* xpm);;All files (*)";
	QString fileName = QFileDialog::getSaveFileName(this, "Save image", folder, fileFilter);
	if (!fileName.isEmpty()) {
		QFileInfo fi(fileName);
		settings.setValue("folder_img_save_path", fi.absoluteDir().absolutePath());

		if (!saveImage(fileName)) {
			msgBox.setText("Unable to save image.");
			msgBox.setIcon(QMessageBox::Warning);
		}
		else {
			msgBox.setText(QString("File %1 saved.").arg(fileName));
			msgBox.setIcon(QMessageBox::Information);
		}
		msgBox.exec();
	}
}
void ImageViewer::on_actionClear_triggered()
{
	vW->clear();
}
void ImageViewer::on_actionExit_triggered()
{
	this->close();
}

//void ImageViewer::on_pushButtonSetColor_clicked()
//{
//	QColor newColor = QColorDialog::getColor(globalColor, this);
//	if (newColor.isValid()) {
//		QString style_sheet = QString("background-color: #%1;").arg(newColor.rgba(), 0, 16);
//		ui->pushButtonSetColor->setStyleSheet(style_sheet);
//		globalColor = newColor;
//	}
//}

void ImageViewer::on_pushButton_setOutlineColor_clicked()
{
	QColor newColor = QColorDialog::getColor(globalColor, this);
	if (newColor.isValid()) {
		QString style_sheet = QString("background-color: #%1;").arg(newColor.rgba(), 0, 16);
		ui->pushButton_setOutlineColor->setStyleSheet(style_sheet);
		shapeColor = newColor;
		//currentShape.setShapeColor(shapeColor);
		getcurrentLayer().setShapeColor(shapeColor);
		drawLayer(getcurrentLayer());
	}
}

void ImageViewer::on_pushButton_setFillColor_clicked()
{
	QColor newColor = QColorDialog::getColor(globalColor, this);
	if (newColor.isValid()) {
		QString style_sheet = QString("background-color: #%1;").arg(newColor.rgba(), 0, 16);
		ui->pushButton_setFillColor->setStyleSheet(style_sheet);
		fillColor = newColor;
		//drawLayer(getcurrentLayer());
		//GeomShapes[count].setShapeColor(fillColor);
	}
}

void ImageViewer::on_listWidget_itemClicked(QListWidgetItem* item)
{
	if (item->checkState() == Qt::Checked) {

		QString layerName = item->text();
		int layerOrder = layerName.split(" ").at(1).toInt();
		setcurrentLayer(layerOrder);
		qDebug()<<"currnet item " << getcurrentLayer().getPoints();
		qDebug() << "currnet item " << getcurrentLayer().getTypeOfShape();
		//drawLayer(getcurrentLayer());
	//	if (layerOrder > 1) {
	//		if (ui->pushButtonLayerUp->isChecked()) {
	//			setcurrentLayer(layerOrder + 1);
	//		}
	//		if (ui->pushButtonLayerDown->isChecked()) {
	//			setcurrentLayer(layerOrder - 1);
	//		}
	//	}
	//	drawLayer(getcurrentLayer());
	}

}

void ImageViewer::drawLayer(Shape tempShape)
{
	//Shape tempShape = getcurrentLayer();

	qDebug() << "in draw layer";
	qDebug() << tempShape.getTypeOfShape();
	qDebug() << tempShape.getPoints();
	qDebug() << tempShape.getShapeColor();
	//qDebug() << tempShape.getTypeOfShape();
	//qDebug() << tempShape.getShapeColor();
	//qDebug() << tempShape.getIsFilled();
	//qDebug() << tempShape.getFillColor();

	//tempShape.setShapeColor(Qt::red);

	vW->initializationZBuffer();

	/*if (tempShape.getTypeOfShape() == "Line") {
		vW->drawLine(tempShape.getPoints(), tempShape.getShapeColor(), tempShape.getLayer());
	}
	if (tempShape.getTypeOfShape() == "Circle") {
		vW->drawCircle(tempShape.getPoints(), tempShape.getShapeColor(), tempShape.getLayer());
	}
	if (tempShape.getTypeOfShape() == "Polygon") {
		vW->drawPolygon(tempShape.getPoints(), tempShape.getShapeColor(), tempShape.getLayer());
	}
	if (tempShape.getTypeOfShape() == "Bezier") {
		vW->drawBezierCurve(tempShape.getPoints(), tempShape.getShapeColor(), tempShape.getLayer());
	}
	if (tempShape.getTypeOfShape() == "Square") {
		vW->drawSquare(tempShape.getPoints(), tempShape.getShapeColor(), tempShape.getLayer());
	}*/


	for (int i = 0; i < GeomShapes.size(); i++) {
		if (GeomShapes[i].getTypeOfShape() == "Line") {
			vW->drawLine(GeomShapes[i].getPoints(), GeomShapes[i].getShapeColor(), GeomShapes[i].getLayer());
		}
		if (GeomShapes[i].getTypeOfShape() == "Circle") {
			vW->drawCircle(GeomShapes[i].getPoints(), GeomShapes[i].getShapeColor(), GeomShapes[i].getLayer());
		}
		if (GeomShapes[i].getTypeOfShape() == "Polygon") {
			vW->drawPolygon(GeomShapes[i].getPoints(), GeomShapes[i].getShapeColor(), GeomShapes[i].getLayer());
		}
		if (GeomShapes[i].getTypeOfShape() == "Bezier") {
			vW->drawBezierCurve(GeomShapes[i].getPoints(), GeomShapes[i].getShapeColor(), GeomShapes[i].getLayer());
		}
		if (GeomShapes[i].getTypeOfShape() == "Square") {
			vW->drawSquare(GeomShapes[i].getPoints(), GeomShapes[i].getShapeColor(), GeomShapes[i].getLayer());
		}
	}


	vW->drawZBuffer();






}

//Shape ImageViewer::currentLayer(int layer)
//{
//	qDebug() <<  GeomShapes[layer].getPoints();
//	GeomShapes[layer].setShapeColor(shapeColor);
//	
//	return GeomShapes[layer];
//}
