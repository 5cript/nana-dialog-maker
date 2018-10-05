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

    protected:
        std::string memberName_;
    };

    template <typename T>
    struct PropertyFromValueType
    {
        static_assert(std::is_same<T,T>::value, "I do not know how to generate ui elements from T, please see look for supported types");
    };
}
