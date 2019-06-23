#ifndef ADD_VARE_H
#define ADD_VARE_H

#include "../error_messages.h"
#include "../check_numbers.h"
#include "../product.h"
#include "../constants.h"
#include <QDialog>

namespace Ui {
class Add_vare;
}

namespace apotek {
namespace apotekholdbarhet {

/**
 * @brief The Add_vare class
 * Add_vare (custom products) ui adds Products to the products_saved database.
 */
class Add_vare : public QDialog
{
    Q_OBJECT
    // TODO: make a using Product = ... ?
public:

    explicit Add_vare(QWidget *parent = nullptr);
    ~Add_vare();
    void clear_error() const;
    bool check_field(const QString &check, bool number) const;
    void check_state() const;
    // setters and getters
    std::shared_ptr<apotek::database::Product> get_product() { return m_product; }
    // add to Product.
    void add_varenr(const QString& varenr);
    void add_navn(const QString& navn);
    void add_mengde(const QString &mengde);
    void add_ean(const QString &ean);
    void add_legemiddelform(const QString& legemiddelform);
    // set and get the 'state' of the label, color it red if its false
    void set_product_varenr(int varenr) { m_product->set_varenr(varenr); }
    void set_state_varenr(bool state) { m_state_varenr = state; }
    void set_state_navn(bool state) { m_state_navn = state; }
    void set_state_mengde(bool state) { m_state_mengde = state; }
    void set_state_ean(bool state) { m_state_ean = state; }
    void set_state_legemiddelform(bool state) { m_state_legemiddelform = state; }
    bool get_state_varenr() const { return m_state_varenr; }
    bool get_state_navn() const { return m_state_navn; }
    bool get_state_mengde() const { return m_state_mengde; }
    bool get_state_ean() const { return m_state_ean; }
    bool get_state_legemiddelform() const { return m_state_legemiddelform; }

signals:
    void signal_newproduct(std::shared_ptr<apotek::database::Product> product);

private slots:
    void on_button_cancel_released();
    void on_button_ok_released();

private:
    Ui::Add_vare *ui;
    std::shared_ptr<apotek::database::Product> m_product;
    bool m_state_varenr;
    bool m_state_navn;
    bool m_state_mengde;
    bool m_state_ean;
    bool m_state_legemiddelform;
};

} // namespace
} // namespace

#endif // ADD_VARE_H
