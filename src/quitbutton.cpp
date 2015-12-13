#include "quitbutton.h"

QuitButton::QuitButton(QWidget *parent) :
    QWidget(parent)
{
}

void QuitButton::quit() {
    close();

}
