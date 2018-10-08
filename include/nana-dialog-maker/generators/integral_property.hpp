#pragma once

#include "../property.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>

#include <functional>
#include <sstream>
#include <cctype>

namespace NanaDialogMaker
{
    template <typename IntegralT>
    class IntegralProperty : public Property
    {
    public:
        constexpr static bool has_label = true;

        using held_type = IntegralT;

    private:
        nana::label description_;
        nana::textbox inputbox_;
        bool dirty_;

    public:
        IntegralProperty
        (
            std::string memberName,
            nana::panel <false>* parent,
            const char* description,
            std::function <void(nana::textbox&)> initializer = {}
        )
            : Property(std::move(memberName))
            , description_{*parent, description}
            , inputbox_{*parent}
            , dirty_{false}
        {
            inputbox_.set_accept([](auto c){
                return std::isdigit(c);
            });

            inputbox_.events().first_change([this](auto const&)
            {
                dirty_ = true;
            });

            inputbox_.events().destroy([this](auto const&)
            {
                alive_ = false;
            });
        }

        IntegralProperty& operator=(IntegralProperty const&) = delete;
        IntegralProperty(IntegralProperty const&) = delete;

        /**
         *  Adds the components to a layout.
         */
        void addToPlace(nana::place& place) override
        {
            place.field(memberName_.c_str()) << inputbox_;
            place.field((memberName_ + "_NANA_DIALOG_MAKER_LABEL").c_str()) << description_;
        }

        /**
         *  Has a change been made?
         */
        bool isDirty() const noexcept override
        {
            return dirty_;
        }

        /**
         *  Commit the ui data to the model.
         */
        void store(held_type& value) const
        {
            std::stringstream sstr{inputbox_.caption()};
            sstr >> value;
        }

        /**
         *  Load values from model to ui.
         */
        void load(held_type const& value)
        {
            inputbox_.caption(std::to_string(value));
        }

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
    };

    #define NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(T)\
    template <>\
    struct PropertyFromValueType <T>\
    {\
        using type = IntegralProperty <T>;\
    }

    NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(int);
    NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(long);
    NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(long long);
    NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(short);
    NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(unsigned int);
    NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(unsigned long);
    NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(unsigned long long);
    NANA_DIALOG_MAKER_SPECIALIZE_INT_AUTO_PROP(unsigned short);
}
