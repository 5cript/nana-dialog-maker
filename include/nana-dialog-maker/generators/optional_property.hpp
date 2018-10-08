#pragma once

#include "../property.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/place.hpp>

#include <optional>
#include <functional>

namespace NanaDialogMaker
{
    template <typename PropertyT>
    class OptionalProperty : public PropertyT
    {
    public:
        using PropertyT::PropertyT;

        /**
         *  Commit the ui data to the model.
         */
        void store(std::optional <typename PropertyT::held_type>& value) const
        {
            if (PropertyT::isSet())
            {
                typename PropertyT::held_type temp;
                PropertyT::store(temp);
                value = temp;
            }
            else
                value = std::nullopt;
        }

        /**
         *  Load values from model to ui.
         */
        void load(std::optional <typename PropertyT::held_type> const& value)
        {
            if (value)
                load(value.value());
        }
    };

    template <typename T>
    struct PropertyFromValueType <std::optional <T>>
    {
        using type = OptionalProperty <typename PropertyFromValueType<T>::type>;
    };
}
