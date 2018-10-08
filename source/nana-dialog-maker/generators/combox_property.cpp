#include <nana-dialog-maker/generators/combox_property.hpp>

namespace NanaDialogMaker
{
//#####################################################################################################################
    ComboxProperty::ComboxProperty
    (
        std::string memberName,
        nana::panel <false>* parent,
        const char* description
    )
        : Property(std::move(memberName))
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void ComboxProperty::addToPlace(nana::place& place)
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    bool ComboxProperty::isDirty() const noexcept
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void ComboxProperty::store(held_type& str) const
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    void ComboxProperty::load(held_type const& str)
    {

    }
//#####################################################################################################################
}
