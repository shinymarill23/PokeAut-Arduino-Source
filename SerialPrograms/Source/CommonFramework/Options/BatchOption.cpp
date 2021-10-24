/*  Batch Option
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include "Common/Qt/QtJsonTools.h"
#include "BatchOption.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{

//BatchOption::BatchOption(){}
void BatchOption::add_option(ConfigOption& option, QString serialization_string){
    m_options.emplace_back(&option, std::move(serialization_string));
}

void BatchOption::load_json(const QJsonValue& json){
    const QJsonObject& obj = json.toObject();
    for (auto& item : m_options){
        if (!item.second.isEmpty()){
            item.first->load_json(json_get_value_nothrow(obj, item.second));
        }
    }
}
QJsonValue BatchOption::to_json() const{
    QJsonObject obj;
    for (auto& item : m_options){
        if (!item.second.isEmpty()){
            obj.insert(item.second, item.first->to_json());
        }
    }
    return obj;
}

QString BatchOption::check_validity() const{
    for (const auto& item : m_options){
        QString error = item.first->check_validity();
        if (!error.isEmpty()){
            return error;
        }
    }
    return QString();
}
void BatchOption::restore_defaults(){
    for (const auto& item : m_options){
        item.first->restore_defaults();
    }
}


BatchOptionUI::BatchOptionUI(QWidget& parent, BatchOption& value)
    : QWidget(&parent)
    , m_value(value)
{
    QVBoxLayout* options_layout = new QVBoxLayout(this);
    options_layout->setAlignment(Qt::AlignTop);
    options_layout->setMargin(0);

    for (auto& item : m_value.m_options){
        m_options.emplace_back(item.first->make_ui(parent));
        options_layout->addWidget(m_options.back()->widget(), 0);
    }
}
void BatchOptionUI::restore_defaults(){
    for (ConfigOptionUI* item : m_options){
        item->restore_defaults();
    }
}





GroupOption::GroupOption(
    QString label,
    bool toggleable,
    bool enabled
)
    : m_label(std::move(label))
    , m_toggleable(toggleable)
    , m_enabled(enabled)
{}
inline ConfigOptionUI* GroupOption::make_ui(QWidget& parent){
    return new GroupOptionUI(parent, *this);
}
bool GroupOption::enabled() const{
    return m_enabled.load(std::memory_order_relaxed);
}
void GroupOption::load_json(const QJsonValue& json){
    BatchOption::load_json(json);
    if (m_toggleable){
        bool enabled;
        json_get_bool(enabled, json.toObject(), "Enabled");
        m_enabled.store(enabled, std::memory_order_relaxed);
        on_set_enabled(enabled);
    }
}
QJsonValue GroupOption::to_json() const{
    QJsonObject obj = BatchOption::to_json().toObject();
    if (m_toggleable){
        obj.insert("Enabled", m_enabled.load(std::memory_order_relaxed));
    }
    return obj;
}


GroupOptionUI::GroupOptionUI(QWidget& parent, GroupOption& value)
    : QWidget(&parent)
    , m_value(value)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
//    layout->setAlignment(Qt::AlignTop);
//    layout->setMargin(0);
    QGroupBox* group_box = new QGroupBox(value.m_label, this);
    group_box->setCheckable(value.m_toggleable);
    group_box->setChecked(value.enabled());
    layout->addWidget(group_box);

#if 0
    QVBoxLayout* mid_layout = new QVBoxLayout(group_box);
    QWidget* mid_widget = new QWidget(group_box);
    mid_layout->addWidget(mid_widget);
    mid_layout->setMargin(0);

    QFont font = group_box->font();
    mid_widget->setFont(font);
    font.setBold(true);
    group_box->setFont(font);
#endif

    m_options_layout = new QVBoxLayout(group_box);
    m_options_layout->setAlignment(Qt::AlignTop);
    m_options_layout->setMargin(0);

    m_placeholder = new QWidget(group_box);
    m_placeholder->setLayout(new QVBoxLayout());
    m_placeholder->layout()->addWidget(new QLabel("(double click to expand)", this));
    m_placeholder->setVisible(false);
    m_options_layout->addWidget(m_placeholder);

    for (auto& item : m_value.m_options){
        m_options.emplace_back(item.first->make_ui(parent));
        m_options_layout->addWidget(m_options.back()->widget());
    }

    connect(
        group_box, &QGroupBox::toggled,
        this, [=](bool on){
            m_value.m_enabled.store(on, std::memory_order_relaxed);
            m_value.on_set_enabled(on);
            on_set_enabled(on);
        }
    );
}
void GroupOptionUI::set_options_enabled(bool enabled){
    for (ConfigOptionUI* item : m_options){
        item->widget()->setEnabled(enabled);
    }
}
void GroupOptionUI::restore_defaults(){
    for (ConfigOptionUI* item : m_options){
        item->restore_defaults();
    }
}
void GroupOptionUI::mouseDoubleClickEvent(QMouseEvent* event){
    m_placeholder->setVisible(m_expanded);
    m_expanded = !m_expanded;
    for (ConfigOptionUI* item : m_options){
        item->widget()->setVisible(m_expanded);
    }
}





}