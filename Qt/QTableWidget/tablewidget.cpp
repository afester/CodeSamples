/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QApplication>
#include <QDialog>
#include "ui_tablewidget.h"


void highlightRow(QTableWidget* tableWidget, int row, const QColor& color) {
    for (int column = 0;  column < tableWidget->columnCount();  column++) {
	QTableWidgetItem* item = tableWidget->item(row, column);
        item->setBackground( color );
    }
}

int main(int argc, char ** argv) {
	QApplication app( argc, argv );
	QMainWindow mainWindow;
	Ui_MainWindow ui;
	ui.setupUi(&mainWindow);

	ui.tableWidget->setColumnCount(3);
	ui.tableWidget->setRowCount(5);

	for (int row = 0;  row < ui.tableWidget->rowCount();  row++) {
	    for (int column = 0;  column < ui.tableWidget->columnCount();  column++) {
		ui.tableWidget->setItem(row, column, new QTableWidgetItem(QString::number(row) + "." + QString::number(column)));
	    }
	}

	// highlight row 3
	highlightRow(ui.tableWidget, 3, QColor(255, 0, 0));

	mainWindow.show();
	app.exec();

	return 0;
}
