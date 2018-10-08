#pragma once

#include "../property.hpp"

#include <nana/gui/widgets/combox.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>

#include <functional>

namespace NanaDialogMaker
{
    class ComboxProperty : public Property
    {
    public:
        constexpr static bool has_label = true;

        using held_type = std::string;

    public:
        ComboxProperty
        (
            std::string memberName,
            nana::panel <false>* parent,
            const char* description,
            std::function <void(nana::combox&)> initializer = {}
        );

        ComboxProperty& operator=(ComboxProperty const&) = delete;
        ComboxProperty(ComboxProperty const&) = delete;

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

    private:
        nana::label description_;
        nana::combox selector_;
        bool dirty_;
    };
}
