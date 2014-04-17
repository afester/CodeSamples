/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QGridLayout>


// re-implementation of QScrollArea::sizeHint without the boundedTo restriction
class UnboundedScrollArea : public QScrollArea {
    mutable QSize widgetSize;

public:
    UnboundedScrollArea(QWidget* parent = 0) : QScrollArea(parent) {
    }

    QSize sizeHint() const {
        int f = 2 * frameWidth();
        QSize sz(f, f);
        int h = fontMetrics().height();

        if (widget()) {
             if (!widgetSize.isValid())
                 widgetSize = widgetResizable() ? widget()->sizeHint() : widget()->size();
             sz += widgetSize;
         } else {
             sz += QSize(12 * h, 8 * h);
         }
         if (verticalScrollBarPolicy() == Qt::ScrollBarAlwaysOn)
             sz.setWidth(sz.width() + verticalScrollBar()->sizeHint().width());
         if (horizontalScrollBarPolicy() == Qt::ScrollBarAlwaysOn)
             sz.setHeight(sz.height() + horizontalScrollBar()->sizeHint().height());

         return sz; // .boundedTo(QSize(36 * h, 24 * h));
    }
};


class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget* parent) {
        resize(800, 600);

        QLabel* imageLabel = new QLabel;
        QImage image("sample.png");
        imageLabel->setPixmap(QPixmap::fromImage(image));
        QWidget* wdg = imageLabel;

        // Standard QScrollArea will add scroll bars around the image label
        QScrollArea* scrollArea = new QScrollArea(this);

        // The UnboundedScrollArea shows how it should look like
        //QScrollArea* scrollArea = new UnboundedScrollArea(this);

        scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded);
        scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded);
        scrollArea->setWidget(wdg);

        QGridLayout* layout = new QGridLayout();
        layout->setMargin(0);
        layout->setHorizontalSpacing(0);
        layout->setVerticalSpacing(0);
        layout->setRowStretch(0, 1);
        layout->setRowStretch(2, 1);
        layout->setColumnStretch(0, 1);
        layout->setColumnStretch(2, 1);
        layout->addWidget(scrollArea, 1, 1);

        QWidget* centralWidget = new QWidget(this);
        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);
    }
};


int main(int argc, char ** argv) {
    QApplication app( argc, argv );

    MainWindow win(0);
    win.show();
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    return app.exec();
}
