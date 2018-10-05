#include <nana-dialog-maker/property.hpp>

namespace NanaDialogMaker
{
//#####################################################################################################################
    Property::Property(std::string memberName)
        : memberName_{std::move(memberName)}
        , alive_{true}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    bool Property::isAlive() const noexcept
    {
        return alive_;
    }
//#####################################################################################################################
}
