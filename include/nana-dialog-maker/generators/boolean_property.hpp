#pragma once

#include "../property.hpp"

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/place.hpp>

namespace NanaDialogMaker
{
    class BooleanProperty : public Property
    {
    public:
        constexpr static bool has_label = false;

        using held_type = bool;

    public:
        BooleanProperty
        (
            std::string memberName,
            nana::panel <false>* parent,
            const char* description
        );

        BooleanProperty& operator=(BooleanProperty const&) = delete;
        BooleanProperty(BooleanProperty const&) = delete;

        /**
         *  Adds the components to a layout.
         */
        void addToPlace(nana::place& place) override;

        /**
         *  Has a change been made?
         */
        bool isDirty() const noexcept override;

        /**
         *  Is the value set? (for optionals).
         */
        bool isSet() const noexcept override;

        /**
         *  Commit the ui data to the model.
         */
        void store(held_type& value) const;

        /**
         *  Load values from model to ui.
         */
        void load(held_type value);

    private:
        nana::checkbox checkbox_;
        bool dirty_;
    };

    template <>
    struct PropertyFromValueType <bool>
    {
        using type = BooleanProperty;
    };
}
