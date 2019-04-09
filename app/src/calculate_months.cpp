#include "calculate_months.h"

namespace apotek{
namespace apotekholdbarhet{

int calculate_months(const QString& holdbarhet)
{
    using namespace date;
    sys_days today = year_month_day{floor<days>(std::chrono::system_clock::now())};
    QDate holdbarhet_date = QDate::fromString(holdbarhet, apotek::constants::date_format);

    sys_days holdbarhet_sysdays = sys_days{days{holdbarhet_date.toJulianDay()} -
            (sys_days{1970_y/jan/1} - sys_days{year{-4713}/nov/24})};

    return round<months>(holdbarhet_sysdays-today).count();
}

}
}
