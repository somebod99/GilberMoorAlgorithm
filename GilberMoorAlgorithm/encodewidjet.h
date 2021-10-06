#ifndef ENCODEWIDJET_H
#define ENCODEWIDJET_H

#include <QWidget>

namespace Ui {
class EncodeWidjet;
}

class EncodeWidjet : public QWidget
{
    Q_OBJECT

public:
    explicit EncodeWidjet(QWidget *parent = nullptr);
    ~EncodeWidjet();

public slots:
    void slotOnClickAlphButton();
    void slotOnClickEncodeButton();

private:
    Ui::EncodeWidjet *ui;
};

#endif // ENCODEWIDJET_H
