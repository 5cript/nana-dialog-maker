#pragma once

#include <nana/gui/widgets/panel.hpp>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/adapted.hpp>

#include <utility>
#include <type_traits>

namespace NanaDialogMaker
{
    template <typename DataHolderT>
    struct PropertyFactory
    {
        template <typename IndexT, typename U, typename... List>
        static constexpr U* makeProperty(List&&... list)
        {
            return new U
            (
                boost::fusion::extension::struct_member_name<DataHolderT, IndexT::value>::call(),
                std::forward <List> (list)...
            );
        }
    };
}
