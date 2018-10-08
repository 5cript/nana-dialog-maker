#pragma once

#include "../property.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>

#include <functional>

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
            nana::panel <false>* parent,
            const char* description,
            std::function <void(nana::textbox&)> initializer = {}
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

        /**
         *  Used for optionals.
         */
        bool isSet() const noexcept override;

        /**
         *  Commit the ui data to the model.
         */
        void store(held_type& str) const;

        /**
         *  Load values from model to ui.
         */
        void load(held_type const& str);

        /**
         *  Create an exemplary layout with some padding.
         *  Optional to provide.
         */
        static std::string defaultTemplateString(int height, int labelWidth, int verticalPadding) noexcept
        {
            using namespace std::string_literals;

            return
                "<max="s + std::to_string(height) +
                    "<vertical max=" + std::to_string(labelWidth) +
                        "<weight=3>" +
                        "<{0}_NANA_DIALOG_MAKER_LABEL>" +
                    ">" +
                    "<{0}>" +
                ">" +
                "<weight=" + std::to_string(verticalPadding) + ">"
            ;
        }

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
