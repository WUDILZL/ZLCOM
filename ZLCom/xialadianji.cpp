#include "xialadianji.h"

xialadianji::xialadianji(QWidget *parent)
	: QComboBox(parent)
{}

xialadianji::~xialadianji()
{}

void xialadianji::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();  //¥•∑¢clicked–≈∫≈
    }
    QComboBox::mousePressEvent(event);
}
