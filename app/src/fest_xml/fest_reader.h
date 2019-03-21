#ifndef FEST_READER_H
#define FEST_READER_H

#include <vector>
#include <string>
#include <QString>
#include <tinyxml2.h>
#include "../product.h"

// TODO: Need to read <KatHandelsvare> too get more varer.

namespace apotek {
namespace xml {
/**
 * @brief The FEST_Reader class
 * Reads the fest_xml file from Legemiddelverket by using the tinyxml2 library.
 * The xml file uses Norwegian nomenclature on their sections. The stuff we are interesting in:
 *
 * navn = <NavnFormStyrke>
 * id = <Id>
 * varenr = <Varenr>
 * ean = <Ean>
 * legemiddelform = <LegemiddelformKort>
 * mengde = <Pakningsinfo><Mengde>
 *
 */
class FEST_Reader
{
public:
    explicit FEST_Reader() : m_filename{"./fest/fest251_inst.xml"} {}
    explicit FEST_Reader(std::string f) : m_filename{f} {}
    ~FEST_Reader() = default;
    std::string get_filename() { return m_filename; }
    std::vector<apotek::database::Product> get_content();
    QString get_fest_hentetdato();
private:
    std::vector<apotek::database::Product> m_varer;
    std::string m_filename;
    inline void read_file();
};


// Reads the whole FEST xml file - stores them in a std::vector<Product>
inline void FEST_Reader::read_file()
{
    using namespace tinyxml2;
    using namespace apotek::database;
    XMLDocument doc;

    XMLError read_file_error = doc.LoadFile(m_filename.c_str());
    if(read_file_error != XML_SUCCESS) return;


    // -1 means no value
    QString id{"-1"};
    QString navn{"-1"};
    int varenr{-1};
    QString ean{"-1"};
    QString legemiddelform{"-1"};
    int mengde{-1};

    XMLNode* node = doc.FirstChildElement("FEST")->FirstChildElement("KatLegemiddelpakning")->FirstChildElement("OppfLegemiddelpakning");
    if(node == nullptr) return;

    while(node != nullptr) {

        id = node->FirstChildElement("Id")->GetText();
        navn = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("NavnFormStyrke")->GetText();
        varenr = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Varenr")->IntText();

        if(node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Ean"))
            ean = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Ean")->GetText();

        if(node->FirstChildElement("Legemiddelpakning")->FirstChildElement("LegemiddelformKort")->FindAttribute("DN"))
            legemiddelform = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("LegemiddelformKort")->FindAttribute("DN")->Value();

        if(node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Pakningsinfo")->FirstChildElement("Mengde"))
            mengde = node->FirstChildElement("Legemiddelpakning")->FirstChildElement("Pakningsinfo")->FirstChildElement("Mengde")->IntText();

        Product v(id, navn, varenr, ean, legemiddelform, mengde);
        m_varer.push_back(v);
        node = node->NextSiblingElement();
    }
}

} // namespace
} // namespace
#endif // FEST_READER_H
