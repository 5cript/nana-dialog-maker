#include  <nana-dialog-maker/generators/textbox_property.hpp>

namespace NanaDialogMaker
{
//#####################################################################################################################
    TextboxProperty::TextboxProperty
    (
        std::string memberName,
        std::string const& defaultValue,
        nana::panel <false>* parent,
        const char* description,
        bool multiLines
    )
        : Property(std::move(memberName))
        , description_{*parent, description}
        , inputbox_{*parent, defaultValue}
        , dirty_{false}
    {
        inputbox_.multi_lines(multiLines);

        inputbox_.events().first_change([this](auto const&)
        {
            dirty_ = true;
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void TextboxProperty::addToPlace(nana::place& place)
    {
        place.field(memberName_.c_str()) << inputbox_;
        place.field((memberName_ + "_NANA_DIALOG_MAKER_LABEL").c_str()) << description_;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TextboxProperty::isDirty() const noexcept
    {
        return dirty_;
    }
//#####################################################################################################################
}
