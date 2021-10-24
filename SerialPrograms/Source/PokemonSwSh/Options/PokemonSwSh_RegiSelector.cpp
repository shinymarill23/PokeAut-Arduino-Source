/*  Regi Selector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <QHBoxLayout>
#include <QLabel>
#include "Common/Qt/NoWheelComboBox.h"
#include "PokemonSwSh_RegiSelector.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{


RegiSelectorOption::RegiSelectorOption()
    : m_label("<b>Name of Regi:</b>")
    , m_default(RegiGolem::Registeel)
    , m_current(RegiGolem::Registeel)
{}
void RegiSelectorOption::load_json(const QJsonValue& json){
    size_t index = json.toInt((int)m_default);
    index = std::min(index, (size_t)4);
    m_current = (RegiGolem)index;
}
QJsonValue RegiSelectorOption::to_json() const{
    return QJsonValue((int)m_current);
}

void RegiSelectorOption::restore_defaults(){
    m_current = m_default;
};

ConfigOptionUI* RegiSelectorOption::make_ui(QWidget& parent){
    return new RegiSelectorOptionUI(parent, *this);
}




RegiSelectorOptionUI::RegiSelectorOptionUI(QWidget& parent, RegiSelectorOption& value)
    : QWidget(&parent)
    , m_value(value)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    QLabel* text = new QLabel(m_value.m_label, this);
    layout->addWidget(text, 1);
    text->setWordWrap(true);
    m_box = new NoWheelComboBox(&parent);
    layout->addWidget(m_box);
    m_box->addItem("Regirock");
    m_box->addItem("Regice");
    m_box->addItem("Registeel");
    m_box->addItem("Regieleki");
    m_box->addItem("Regidrago");
    m_box->setCurrentIndex((int)m_value.m_current);
    layout->addWidget(m_box, 1);

    connect(
        m_box, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, [=](int index){
            m_value.m_current = (RegiGolem)index;
        }
    );
}
void RegiSelectorOptionUI::restore_defaults(){
    m_value.restore_defaults();
    m_box->setCurrentIndex((int)(RegiGolem)m_value);
}



}
}
}
