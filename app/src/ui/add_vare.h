#ifndef ADD_VARE_H
#define ADD_VARE_H

#include <QDialog>

namespace Ui {
class Add_vare;
}

namespace apotek {
namespace apotekholdbarhet {

class Add_vare : public QDialog
{
    Q_OBJECT

public:
    explicit Add_vare(QWidget *parent = nullptr);
    ~Add_vare();
    bool check_varenr() const;
    bool check_navn() const;
    bool check_mengde() const;
    bool check_ean() const;
    bool check_legemiddelform() const;

signals:
    //void signal_newproduct(QDate, int, int);

private slots:
    void on_button_cancel_released();
    void on_button_ok_released();

private:
    Ui::Add_vare *ui;
};

} // namespace
} // namespace

#endif // ADD_VARE_H
