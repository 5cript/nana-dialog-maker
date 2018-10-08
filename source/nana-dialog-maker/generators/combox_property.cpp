#include <nana-dialog-maker/generators/combox_property.hpp>

#include <stdexcept>

namespace NanaDialogMaker
{
//#####################################################################################################################
    ComboxProperty::ComboxProperty
    (
        std::string memberName,
        nana::panel <false>* parent,
        const char* description,
        std::vector <std::string> const& options,
        bool editable
    )
        : Property(std::move(memberName))
        , description_{*parent, description}
        , selector_{*parent}
    {
        selector_.editable(editable);
        for (auto const& i : options)
            selector_.push_back(i);
    }
//---------------------------------------------------------------------------------------------------------------------
    void ComboxProperty::addToPlace(nana::place& place)
    {
        place.field(memberName_.c_str()) << selector_;
        place.field((memberName_ + "_NANA_DIALOG_MAKER_LABEL").c_str()) << description_;
    }
//---------------------------------------------------------------------------------------------------------------------
    bool ComboxProperty::isDirty() const noexcept
    {
        return dirty_;
    }
//---------------------------------------------------------------------------------------------------------------------
    void ComboxProperty::store(held_type& str) const
    {
        if (selector_.the_number_of_options() != 0 && selector_.option() >= 0 && selector_.option() < selector_.the_number_of_options())
            str = selector_.text(selector_.option());
    }
//---------------------------------------------------------------------------------------------------------------------
    void ComboxProperty::load(held_type const& str)
    {
        for (std::size_t i = 0; i != selector_.the_number_of_options(); ++i)
        {
            if (str == selector_.text(i))
            {
                selector_.option(i);
                return;
            }
        }

        // not found
        if (selector_.editable())
            selector_.push_back(str);

        throw std::invalid_argument("Given string is not a valid option for Combobox.");
    }
//#####################################################################################################################
}
