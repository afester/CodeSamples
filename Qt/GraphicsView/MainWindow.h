/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QMainWindow>

class GraphicsSheet;
class QGraphicsRectItem;
class Interactor;

class MainWindow : public QMainWindow {
    Q_OBJECT;

    QAction *actionLoad;
    QAction *actionSave;
    QAction *actionSaveAs;

    QAction *actionSelect;
    QAction *actionNewLineItem;
    QAction *actionNewRectItem;
    QAction *actionNewTextItem;
    QAction *actionNewCircleItem;
    QAction *actionNewEllipseItem;

    GraphicsSheet* graphicsSheet;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;
    QGraphicsRectItem* item1;

    Interactor* selectInteractor;
    Interactor* newLineItemInteractor;
    Interactor* newRectItemInteractor;
    Interactor* newTextItemInteractor;
    Interactor* newCircleItemInteractor;
    Interactor* newEllipseItemInteractor;

public:
    MainWindow(QWidget* parent);

    ~MainWindow();

public slots:
#if 0
	void printInfo();
    void rotateItem();
    void resizeItem();
#endif

    void doActionLoad();
    void doActionSave();
    void doActionSaveAs();
    void toggleActionSelect();
    void doActionSelect();
    void doActionNewLineItem();
    void doActionNewRectItem();
    void doActionNewTextItem();
    void doActionNewCircleItem();
    void doActionNewEllipseItem();
};
