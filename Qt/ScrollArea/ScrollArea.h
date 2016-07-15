/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


#include <QMainWindow>
#include <QApplication>

class CustomWidget : public QWidget {

public:
    CustomWidget(QWidget* parent = 0);

    virtual QSize sizeHint() const;

protected:
    void paintEvent ( QPaintEvent * event );
};


class MainWindow : public QMainWindow {
    Q_OBJECT;

public:

    MainWindow(QWidget* parent);
    ~MainWindow();
};
