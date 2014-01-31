
#include <QMainWindow>

class TestMainWindow;

class TheMainWindow : public QMainWindow {
	Q_OBJECT

	TestMainWindow* mainUi;
	QWidget* messageBox;
	
public:
	TheMainWindow();

	virtual ~TheMainWindow();

public slots:
	void toggleWidget();
};
