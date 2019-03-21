#ifndef FEST_UPDATE_H
#define FEST_UPDATE_H

#include <QDialog>

namespace Ui {
class FEST_Update;
}

namespace apotek {
namespace apotekholdbarhet {

class FEST_Update : public QDialog
{
    Q_OBJECT

public:
    explicit FEST_Update(QWidget *parent = nullptr);
    ~FEST_Update();

private:
    Ui::FEST_Update *ui;
};

} // namespace
} // namespace

#endif // FEST_UPDATE_H
