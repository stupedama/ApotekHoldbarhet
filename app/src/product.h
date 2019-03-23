#ifndef APOTEKH_PRODUCT_H
#define APOTEKH_PRODUCT_H

#include <QString>

namespace apotek {
namespace database {

/**
 * @brief The Product class
 * Is a class for storing and handling data from the fest_xml file.
 * It also handles the searching by varenr, ean and name.
 */
class Product {
public:
    explicit Product() = default;
    explicit Product(const QString& i, const QString& n, int v, const QString& e, const QString& l, int m);
    ~Product() = default;

    struct Find_vare;
    struct Find_ean;

    // setters
    void set_id(const QString& i) { m_id = i; }
    void set_navn(const QString& n) { m_navn = n; }
    void set_varenr(int v) { m_varenr = v; }
    void set_ean(const QString& e) { m_ean = e; }
    void set_legemiddelform(const QString& l) { m_legemiddelform = l; }
    void set_mengde(int m) { m_mengde = m; }
    void set_holdbarhet(const QString& h) { m_holdbarhet = h; }
    void set_lokasjon(const QString& l) { m_lokasjon = l; }

    // getters
    QString get_id() const { return m_id; }
    QString get_navn() const { return m_navn; }
    int get_varenr() const { return m_varenr; }
    QString get_ean() const { return m_ean; }
    QString get_legemiddelform() const { return m_legemiddelform; }
    int get_mengde() const { return m_mengde; }
    QString get_holdbarhet() const { return m_holdbarhet; }
    QString get_lokasjon() const { return m_lokasjon; }

private:
    QString m_id;
    QString m_navn;
    int m_varenr;
    QString m_ean;
    QString m_legemiddelform;
    int m_mengde;
    QString m_holdbarhet;
    QString m_lokasjon;
};

struct Product::Find_vare{
    Find_vare(int v) : m_varenr{v} {}
    Find_vare(const QString& n) : m_navn{n} {}

    bool operator() (const Product& rhs) const
    {
        if(m_navn.isEmpty()) {
            return m_varenr == rhs.m_varenr;
        } else {
            QString s = rhs.m_navn;
            return s.contains(m_navn, Qt::CaseInsensitive);
        }
    }

    int m_varenr;
    QString m_navn;
};

struct Product::Find_ean{
    Find_ean(const QString& e) : m_ean{e} {}

    bool operator() (const Product& rhs)
    {
        if(!rhs.m_ean.isEmpty()) {
            QString s = rhs.m_ean;
            return s.contains(m_ean, Qt::CaseInsensitive);
        }
        return false;
    }
    QString m_ean;
};

} // namespace
} // namespace
#endif // APOTEKH_PRODUCT_H
