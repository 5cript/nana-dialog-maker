#pragma once

#include "../property.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>

namespace NanaDialogMaker
{
    class TextboxProperty : public Property
    {
    public:
        constexpr static bool has_label = true;

        using held_type = std::string;

    public:
        TextboxProperty
        (
            std::string memberName,
            std::string const& defaultValue,
            nana::panel <false>* parent,
            const char* description,
            bool multiLines = false
        );

        TextboxProperty& operator=(TextboxProperty const&) = delete;
        TextboxProperty(TextboxProperty const&) = delete;

        /**
         *  Adds the components to a layout.
         */
        void addToPlace(nana::place& place) override;

        /**
         *  Has a change been made?
         */
        bool isDirty() const noexcept override;

    private:
        nana::label description_;
        nana::textbox inputbox_;
        bool dirty_;
    };

    template <>
    struct PropertyFromValueType <std::string>
    {
        using type = TextboxProperty;
    };
}
