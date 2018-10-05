#pragma once

#include <nana/gui/widgets/panel.hpp>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/adapted.hpp>

#include <utility>
#include <type_traits>

namespace NanaDialogMaker
{
    template <typename DataHolderT, typename DefaultConstructible = void>
    struct PropertyFactory
    {
        template <typename IndexT, typename U, typename... List>
        static constexpr U* makeProperty(List&&... list)
        {
            return new U
            (
                boost::fusion::extension::struct_member_name<DataHolderT, IndexT::value>::call(),
                typename U::held_type{},
                std::forward <List> (list)...
            );
        }
    };

    template <typename DataHolderT>
    struct PropertyFactory <DataHolderT, typename std::enable_if <std::is_default_constructible <DataHolderT>::value>::type>
    {
        DataHolderT defaulted;

        template <typename IndexT, typename U, typename... List>
        constexpr U* makeProperty(List&&... list)
        {
            return new U
            (
                boost::fusion::extension::struct_member_name<DataHolderT, IndexT::value>::call(),
                boost::fusion::at<IndexT>(defaulted),
                std::forward <List> (list)...
            );
        }
    };
}
