#include "Interactor.h"
#include <QWheelEvent>

Interactor::~Interactor() {
}


void Interactor::setView(GraphicsSheet* view) {
	theView = view;
}


void Interactor::wheelEvent ( QWheelEvent* event ) {
    event->ignore();
}


void Interactor::paste() {
}
