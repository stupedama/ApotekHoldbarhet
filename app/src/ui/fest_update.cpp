#include "fest_update.h"
#include "ui_fest_update.h"

namespace apotek {
namespace apotekholdbarhet {

FEST_Update::FEST_Update(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FEST_Update)
{
    ui->setupUi(this);
}

FEST_Update::~FEST_Update()
{
    delete ui;
}

} // namespace
} // namespace
