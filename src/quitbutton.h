#ifndef QUITBUTTON_H
#define QUITBUTTON_H

#include <QWidget>

class QuitButton : public QWidget
{
    Q_OBJECT
public:
    explicit QuitButton(QWidget *parent = 0);

signals:

public slots:
    void quit();

};

#endif // QUITBUTTON_H
