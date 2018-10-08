#include <nana-dialog-maker/generators/boolean_property.hpp>

namespace NanaDialogMaker
{
//#####################################################################################################################
    BooleanProperty::BooleanProperty
    (
        std::string memberName,
        nana::panel <false>* parent,
        const char* description
    )
        : Property(std::move(memberName))
        , checkbox_{*parent, description}
        , dirty_{false}
    {
        checkbox_.events().checked([this](auto const&)
        {
            dirty_ = true;
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void BooleanProperty::addToPlace(nana::place& place)
    {
        place.field(memberName_.c_str()) << checkbox_;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool BooleanProperty::isDirty() const noexcept
    {
        return dirty_;
    }
//---------------------------------------------------------------------------------------------------------------------
    void BooleanProperty::store(held_type& value) const
    {
        value = checkbox_.checked();
    }
//---------------------------------------------------------------------------------------------------------------------
    void BooleanProperty::load(held_type value)
    {
        checkbox_.check(value);
    }
//#####################################################################################################################
}
