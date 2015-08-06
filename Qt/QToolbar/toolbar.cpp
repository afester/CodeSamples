/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QMenuBar>

class MainWindow : public QMainWindow {
    QAction* actionNotification;
    QMenuBar* menuBar;
    QToolBar* toolBar;

public:
    MainWindow() : QMainWindow() {
        actionNotification = new QAction(this);
        actionNotification->setCheckable(true);
        actionNotification->setChecked(false);
        actionNotification->setEnabled(true);
        actionNotification->setAutoRepeat(true);
        actionNotification->setVisible(true);
        actionNotification->setIconVisibleInMenu(false);
        actionNotification->setObjectName("actionNotification");

        toolBar = new QToolBar(this);
        toolBar->setLayoutDirection(Qt::RightToLeft);
        toolBar->setStyleSheet("");
        toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->setObjectName("toolBar");
        toolBar->addAction(actionNotification);

        menuBar = new QMenuBar(this);
        menuBar->addAction(actionNotification);

        QMenu* menu = new QMenu("Test", menuBar);
        menu->addAction(actionNotification);
        menuBar->addMenu(menu);

        actionNotification->setText("&&Notification");  // #this works
        // actionNotification.setToolTip(
        //    QApplication.translate("MainWindow", "Click to see new notifications", None,
       //                            QApplication.UnicodeUTF8))
       setMenuBar(menuBar);
       addToolBar(toolBar);
    }
};


int main(int argc, char ** argv) {
	QApplication app( argc, argv );
	MainWindow mainWindow;

	mainWindow.show();
	app.exec();

	return 0;
}
