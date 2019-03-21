#include "product.h"

namespace apotek {
namespace database {

Product::Product(const QString& i, const QString& n, int v, const QString& e, const QString& l, int m)
    :    m_id{i}, m_navn{n}, m_varenr{v}, m_ean{e}, m_legemiddelform{l}, m_mengde{m}
{
}

} // namespace
} // namespace
