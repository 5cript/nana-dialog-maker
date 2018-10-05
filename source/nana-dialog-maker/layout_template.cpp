#include <nana-dialog-maker/layout_template.hpp>

#include <fmt/core.h>

namespace NanaDialogMaker
{
//#####################################################################################################################
    LayoutTemplate::LayoutTemplate(std::string formatString)
        : formatString_{std::move(formatString)}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string LayoutTemplate::format
    (
        std::string const& memberName
    )
    {
        return fmt::format(formatString_, memberName);
    }
//#####################################################################################################################
}
