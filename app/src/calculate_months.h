#ifndef CALCULATE_MONTHS_H
#define CALCULATE_MONTHS_H

#include <QDate>
#include <QString>
#include <date.h>
#include "constants.h"

namespace apotek{
namespace apotekholdbarhet{

int calculate_months(const QString& holdbarhet);

}
}

#endif // CALCULATE_MONTHS_H
