#ifndef APOTEKH_INIT_H
#define APOTEKH_INIT_H

#include "../ui/apotekholdbarhet.h"

namespace apotek{
namespace apotekholdbarhet {

void apotekholdbarhet_init(ApotekHoldbarhet& ah)
{
    ah.setFixedSize(1260, 768);
    ah.set_title();
    ah.fetch_products();
    ah.set_label_colors();
}

}
}

#endif // APOTEKHOLDBARHET_INIT_H
