#include "fest_reader.h"

namespace apotek {
namespace xml {

QString FEST_Reader::get_fest_hentetdato()
{
    using namespace tinyxml2;

    XMLDocument doc;

    tinyxml2::XMLError read_file_error = doc.LoadFile(m_filename.c_str());
    if(read_file_error != tinyxml2::XML_SUCCESS) return "error";

    XMLNode* node = doc.FirstChildElement("FEST");
    if(node == nullptr) return "error";

    std::string dato = node->FirstChildElement("HentetDato")->GetText();

    return QString::fromStdString(dato);
}

std::vector<apotek::database::Product> FEST_Reader::get_content()
{
    m_varer.clear();
    read_file();

    return m_varer;
}

} // namespace
} // namespace
