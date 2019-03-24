#include "product.h"

namespace apotek {
namespace database {

Product::Product(const QString& i, const QString& n, int v, const QString& e, const QString& l, int m)
    :    m_id{i}, m_navn{n}, m_varenr{v}, m_ean{e}, m_legemiddelform{l}, m_mengde{m}
{
}

// checks if the Product is a valid product
// m_varenr and m_navn cannot be empty
// it also checks if varenr, ean and mengde is numbers.
bool Product::sanity_check() const
{
    using apotek::apotekholdbarhet::check_numbers;

    bool sanity{false};

    const QString string_mengde = QString::number(m_mengde);
    const QString string_varenr = QString::number(m_varenr);

    if(check_numbers(string_varenr)) {
        sanity = true;
    } else {
        sanity = false;
    }

    if(check_numbers(string_mengde)) {
        sanity = true;
    } else {
        sanity = false;
    }

    if(check_numbers(m_ean)) {
        sanity = true;
    } else {
        sanity = false;
    }

    if(string_varenr.isEmpty() || m_navn.isEmpty()) sanity = false;

    return sanity;
}

} // namespace
} // namespace
