#include  <nana-dialog-maker/generators/textbox_property.hpp>

namespace NanaDialogMaker
{
//#####################################################################################################################
    TextboxProperty::TextboxProperty
    (
        std::string memberName,
        nana::panel <false>* parent,
        const char* description,
        bool multiLines
    )
        : Property(std::move(memberName))
        , description_{*parent, description}
        , inputbox_{*parent}
        , dirty_{false}
    {
        inputbox_.multi_lines(multiLines);

        inputbox_.events().first_change([this](auto const&)
        {
            dirty_ = true;
        });

        inputbox_.events().destroy([this](auto const&)
        {
            alive_ = false;
        });
    }
//---------------------------------------------------------------------------------------------------------------------
    void TextboxProperty::addToPlace(nana::place& place)
    {
        place.field(memberName_.c_str()) << inputbox_;
        place.field((memberName_ + "_NANA_DIALOG_MAKER_LABEL").c_str()) << description_;
    }
//---------------------------------------------------------------------------------------------------------------------
    void TextboxProperty::store(std::string& value) const
    {
        value = inputbox_.caption();
    }
//---------------------------------------------------------------------------------------------------------------------
    void TextboxProperty::load(std::string const& value)
    {
        inputbox_.caption(value);
    }
//---------------------------------------------------------------------------------------------------------------------
    bool TextboxProperty::isDirty() const noexcept
    {
        return dirty_;
    }
//#####################################################################################################################
}
