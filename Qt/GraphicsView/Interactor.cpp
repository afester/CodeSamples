#include "Interactor.h"

Interactor::~Interactor() {
}


void Interactor::setView(GraphicsSheet* view) {
	theView = view;
}


void Interactor::paintDecorations(EditableItem* item, QPainter* painter) {
}


void Interactor::wheelEvent ( QWheelEvent* event ) {
}


void Interactor::paste() {
}
