#pragma once

#include <nana/gui/place.hpp>
#include <string>

namespace NanaDialogMaker
{
    class Property
    {
    public:
        Property(std::string memberName);

        virtual ~Property() = default;

        /**
         *  Add components to place.
         */
        virtual void addToPlace(nana::place& place) = 0;

        /**
         *  Has a change been made?
         */
        virtual bool isDirty() const noexcept = 0;

        /**
         *  Used for optionals. Required, but can always return true.
         */
        virtual bool isSet() const noexcept = 0;

        /**
         *  Returns whether or not the control is alive or destroyed.
         */
        bool isAlive() const noexcept;

    protected:
        std::string memberName_;
        bool alive_; // set to false if deconstruction happened.
    };

    class AutoProperty
    {};

    template <typename T>
    struct PropertyFromValueType
    {
        static_assert(std::is_same<T,T>::value, "I do not know how to generate ui elements from T, please see look for supported types");
    };
}
