#ifndef DECODEWIDJET_H
#define DECODEWIDJET_H

#include <QWidget>

namespace Ui {
class DecodeWidjet;
}

class DecodeWidjet : public QWidget
{
    Q_OBJECT

public:
    explicit DecodeWidjet(QWidget *parent = nullptr);
    ~DecodeWidjet();

public slots:
    void slotOnClickAlphButton();
    void slotOnClickDecodeButton();

private:
    Ui::DecodeWidjet *ui;
};

#endif // DECODEWIDJET_H
