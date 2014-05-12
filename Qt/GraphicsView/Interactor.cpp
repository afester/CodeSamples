#include "Interactor.h"

Interactor::~Interactor() {
}


void Interactor::setView(GraphicsSheet* view) {
	theView = view;
}


void Interactor::wheelEvent ( QWheelEvent* event ) {
    Q_UNUSED(event);
}


void Interactor::paste() {
}
