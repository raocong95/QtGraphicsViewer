#ifndef MODIFIERSPANEL_H
#define MODIFIERSPANEL_H

#include <QWidget>

namespace Ui {
class ModifiersPanel;
}

class ModifiersPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ModifiersPanel(QWidget *parent = 0);
    ~ModifiersPanel();

    Ui::ModifiersPanel *ui;
};

#endif // MODIFIERSPANEL_H
