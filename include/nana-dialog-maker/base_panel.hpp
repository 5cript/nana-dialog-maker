#pragma once

#include "property.hpp"
#include "property_factory.hpp"
#include "layout_template.hpp"

#include <nana/gui/widgets/panel.hpp>

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <mutex>
#include <thread>
#include <type_traits>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>

#include <boost/fusion/sequence/intrinsic/has_key.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/adapted.hpp>

namespace NanaDialogMaker
{

    namespace detail
    {
        template <typename ValueT, typename T>
        struct AutoPropertyChooser
        {
            using type = T;
        };

        template <typename ValueT>
        struct AutoPropertyChooser <ValueT, AutoProperty>
        {
            using type = typename NanaDialogMaker::PropertyFromValueType <ValueT>::type;
        };
    }

    template <typename DerivedT, typename HolderT>
    class BasePanel : public nana::panel <false>
    {
        public:
            using holder_type = HolderT;
            using panel_type = DerivedT;

        private: // Members
            std::vector <std::unique_ptr <NanaDialogMaker::Property>> properties_;
            nana::place layout_;
            mutable holder_type holder_;
            mutable std::recursive_mutex holderGuard_;

        public: // Interface
            BasePanel()
                : properties_{}
                , layout_{*this}
            {
            }

            BasePanel(nana::window wd, bool visible = true)
                : nana::panel <false>(wd, visible)
                , properties_{}
                , layout_{*this}
            {
            }

            template <typename T>
            void constructProperties(T const& argumentMap)
            {
                namespace mpl = boost::mpl;
                namespace fusion = boost::fusion;

                mpl::for_each<mpl::range_c<std::size_t, 0, boost::fusion::result_of::size<holder_type>::type::value>>
                (
                    [this, &argumentMap](auto index)
                    {
                        using index_type = std::decay_t<decltype(index)>;
                        using property_type = typename NanaDialogMaker::detail::AutoPropertyChooser <
                            std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>,
                            std::tuple_element_t <index_type::value, typename DerivedT::property_types>
                        >::type;

                        using index_as_integral_c = std::integral_constant <int, index_type::value>;
                        if constexpr (!boost::fusion::result_of::has_key <T, index_as_integral_c>::value)
                        {
                            properties_.emplace_back
                            (
                                new property_type
                                (
                                    boost::fusion::extension::struct_member_name<holder_type, index_type::value>::call(),
                                    this,
                                    DerivedT::descriptions[index_type::value]
                                )
                            );
                        }
                        else
                        {
                            properties_.emplace_back(std::apply(
                                [this](auto&&... args)
                                {
                                    return new property_type{
                                        boost::fusion::extension::struct_member_name<holder_type, index_type::value>::call(),
                                        this,
                                        DerivedT::descriptions[index_type::value],
                                        std::forward <decltype(args)>(args)...
                                    };
                                },
                                boost::fusion::at_key <index_as_integral_c>(argumentMap)
                            ));
                        }
                    }
                );
            }

            bool areControlsAlive() const
            {
                if constexpr (boost::fusion::result_of::size<holder_type>::type::value > 0)
                    return properties_.front()->isAlive();
                return false;
            }

            ~BasePanel() = default;

            BasePanel(BasePanel const&) = delete;
            BasePanel& operator=(BasePanel const&) = delete;
            BasePanel(BasePanel&&) = default;
            BasePanel& operator=(BasePanel&&) = default;

            std::vector <std::unique_ptr <NanaDialogMaker::Property>>& properties()
            {
                return properties_;
            }

            bool isDirty() const
            {
                for (auto const& i : properties_)
                    if (i->isDirty())
                        return true;
                return false;
            }

            template <typename LayoutTemplateT>
            std::string generateLayout(LayoutTemplateT const& templates)
            {
                namespace mpl = boost::mpl;
                namespace fusion = boost::fusion;

                auto baseTemplate = fusion::at_key<void>(templates);
                std::string collection{};
                mpl::for_each<mpl::range_c<std::size_t, 0, boost::fusion::result_of::size<holder_type>::type::value>>
                (
                    [this, &templates, &collection](auto index)
                    {
                        using index_type = std::decay_t<decltype(index)>;
                        using property_type = typename NanaDialogMaker::detail::AutoPropertyChooser <
                            std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>,
                            std::tuple_element_t <index_type::value, typename DerivedT::property_types>
                        >::type;
                        auto templ = fusion::at_key<property_type>(templates);
                        collection += templ.format(fusion::extension::struct_member_name<holder_type, index_type::value>::call());
                    }
                );
                return baseTemplate.format(collection);
            }

            holder_type data() const
            {
                namespace mpl = boost::mpl;
                namespace fusion = boost::fusion;

                std::scoped_lock <std::recursive_mutex> guard(holderGuard_);

                if (!areControlsAlive())
                    return holder_;
                mpl::for_each<mpl::range_c<std::size_t, 0, boost::fusion::result_of::size<holder_type>::type::value>>
                (
                    [this](auto index)
                    {
                        using index_type = std::decay_t<decltype(index)>;
                        using property_type = typename NanaDialogMaker::detail::AutoPropertyChooser <
                            std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>,
                            std::tuple_element_t <index_type::value, typename DerivedT::property_types>
                        >::type;

                        static_cast <property_type*>(properties_[index_type::value].get())->store(
                            fusion::at <index_type>(holder_)
                        );
                    }
                );
                return holder_;
            }

            void data(holder_type const& holder)
            {
                namespace mpl = boost::mpl;
                namespace fusion = boost::fusion;

                std::scoped_lock <std::recursive_mutex> guard(holderGuard_);

                holder_ = holder;
                if (!areControlsAlive())
                    return;

                mpl::for_each<mpl::range_c<std::size_t, 0, boost::fusion::result_of::size<holder_type>::type::value>>
                (
                    [this, &holder](auto index)
                    {
                        using index_type = std::decay_t<decltype(index)>;
                        using property_type = typename NanaDialogMaker::detail::AutoPropertyChooser <
                            std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>,
                            std::tuple_element_t <index_type::value, typename DerivedT::property_types>
                        >::type;

                        static_cast <property_type*>(properties_[index_type::value].get())->load(
                            fusion::at <index_type>(holder)
                        );
                    }
                );
            }

            nana::place& layout()
            {
                return layout_;
            }
        };
}
