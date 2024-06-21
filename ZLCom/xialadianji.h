#pragma once

#include <QComboBox>
#include <QMouseEvent>

class xialadianji  : public QComboBox
{
	Q_OBJECT

public:
	explicit xialadianji(QWidget* parent);
	~xialadianji();
protected:
	virtual void mousePressEvent(QMouseEvent* e);  

signals:
	void clicked();
};
