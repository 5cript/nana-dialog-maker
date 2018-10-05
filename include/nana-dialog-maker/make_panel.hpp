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

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/adapted.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#include <boost/preprocessor/tuple/elem.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#define NANA_DIALOG_MAKER_CLASS_SUFFIX Panel

namespace NanaDialogMaker::detail
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

#define NANA_DIALOG_MAKER_SEQUENCE_FACTORY_0(...)\
     ((__VA_ARGS__)) NANA_DIALOG_MAKER_SEQUENCE_FACTORY_1
#define NANA_DIALOG_MAKER_SEQUENCE_FACTORY_1(...)\
     ((__VA_ARGS__)) NANA_DIALOG_MAKER_SEQUENCE_FACTORY_0
#define NANA_DIALOG_MAKER_SEQUENCE_FACTORY_0_END
#define NANA_DIALOG_MAKER_SEQUENCE_FACTORY_1_END

#define NANA_DIALOG_MAKER_DESCRIPTION_UNPACKER(r, data, elem)\
    (BOOST_PP_TUPLE_ELEM(3, 0, elem))

#define NANA_DIALOG_MAKER_MEMBER_UNPACKER(r, data, elem)\
    (BOOST_PP_TUPLE_ELEM(3, 1, elem))

#define NANA_DIALOG_MAKER_PROPERTY_UNPACKER(r, data, elem)\
    (BOOST_PP_TUPLE_ELEM(3, 2, elem))

#define NANA_DIALOG_MAKER_ELEVENTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11

#define NANA_DIALOG_MAKER_COUNT_ARGUMENTS(...)\
    NANA_DIALOG_MAKER_ELEVENTH_ARGUMENT(dummy, ## __VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


#define NANA_DIALOG_MAKER_MAKE_PANEL_IMPL(nspace, object, seq)\
    BOOST_FUSION_ADAPT_STRUCT\
    (\
        BOOST_PP_IF(NANA_DIALOG_MAKER_COUNT_ARGUMENTS(nspace), nspace::,) object,\
        BOOST_PP_SEQ_ENUM(\
            BOOST_PP_SEQ_FOR_EACH(NANA_DIALOG_MAKER_MEMBER_UNPACKER, _, seq)\
        )\
    )\
    BOOST_PP_IF(NANA_DIALOG_MAKER_COUNT_ARGUMENTS(nspace), namespace nspace{,) \
        class BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX) : public nana::panel <false>\
        {\
        public:\
            using holder_type = object;\
        \
        public:\
            static constexpr const char* descriptions[] = {\
                BOOST_PP_SEQ_ENUM(\
                    BOOST_PP_SEQ_FOR_EACH(NANA_DIALOG_MAKER_DESCRIPTION_UNPACKER, _, seq)\
                )\
            };\
            \
            using property_types = std::tuple <BOOST_PP_SEQ_ENUM(\
                BOOST_PP_SEQ_FOR_EACH(NANA_DIALOG_MAKER_PROPERTY_UNPACKER, _, seq)\
            )>;\
            \
            static constexpr const char* name = BOOST_PP_STRINGIZE(BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX));\
            \
        private:\
            \
            std::vector <std::unique_ptr <NanaDialogMaker::Property>> properties_;\
            NanaDialogMaker::PropertyFactory <holder_type> propFactory_;\
            nana::place layout_;\
            mutable holder_type holder_;\
            mutable std::recursive_mutex holderGuard_;\
            \
            void construct()\
            {\
                namespace mpl = boost::mpl;\
                namespace fusion = boost::fusion;\
                \
                mpl::for_each<mpl::range_c<std::size_t, 0, boost::fusion::result_of::size<holder_type>::type::value>>\
                (\
                    [this](auto index)\
                    {\
                        using index_type = std::decay_t<decltype(index)>;\
                        using property_type = typename NanaDialogMaker::detail::AutoPropertyChooser <\
                            std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>,\
                            std::tuple_element_t <index_type::value, property_types>\
                        >::type;\
                        properties_.emplace_back(\
                            propFactory_.makeProperty <index_type, property_type>\
                            (\
                                this,\
                                descriptions[index_type::value]\
                            )\
                        );\
                    }\
                );\
            }\
            \
            bool areControlsAlive() const \
            {\
                if constexpr (boost::fusion::result_of::size<holder_type>::type::value > 0)\
                    return properties_.front()->isAlive();\
                return false;\
            }\
            \
        public:\
            BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)()\
                : properties_{}\
                , propFactory_{}\
                , layout_{*this}\
            {\
                construct();\
            }\
            \
            ~BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)()\
            {\
            }\
            \
            BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)(nana::window wd, bool visible = true)\
                : nana::panel <false>(wd, visible)\
                , properties_{}\
                , propFactory_{}\
                , layout_{*this}\
            {\
                construct();\
            }\
            \
            BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)(BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX) const&) = delete;\
            BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)& operator=(BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX) const&) = delete;\
            BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)(BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)&&) = default;\
            BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)& operator=(BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)&&) = default;\
            \
            std::vector <std::unique_ptr <NanaDialogMaker::Property>>& properties()\
            {\
                return properties_;\
            }\
            \
            bool isDirty() const \
            {\
                for (auto const& i : properties_)\
                    if (i->isDirty())\
                        return true;\
                return false;\
            }\
            \
            template <typename LayoutTemplateT>\
            std::string generateLayout(LayoutTemplateT const& templates)\
            {\
                namespace mpl = boost::mpl;\
                namespace fusion = boost::fusion;\
                \
                auto baseTemplate = fusion::at_key<void>(templates);\
                std::string collection{};\
                mpl::for_each<mpl::range_c<std::size_t, 0, boost::fusion::result_of::size<holder_type>::type::value>>\
                (\
                    [this, &templates, &collection](auto index)\
                    {\
                        using index_type = std::decay_t<decltype(index)>;\
                        using property_type = typename NanaDialogMaker::detail::AutoPropertyChooser <\
                            std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>,\
                            std::tuple_element_t <index_type::value, property_types>\
                        >::type;\
                        auto templ = fusion::at_key<property_type>(templates);\
                        collection += templ.format(fusion::extension::struct_member_name<holder_type, index_type::value>::call());\
                    }\
                );\
                return baseTemplate.format(collection);\
            }\
            \
            holder_type data() const\
            {\
                namespace mpl = boost::mpl;\
                namespace fusion = boost::fusion;\
                \
                std::scoped_lock <std::recursive_mutex> guard(holderGuard_);\
                \
                if (!areControlsAlive())\
                    return holder_;\
                mpl::for_each<mpl::range_c<std::size_t, 0, boost::fusion::result_of::size<holder_type>::type::value>>\
                (\
                    [this](auto index)\
                    {\
                        using index_type = std::decay_t<decltype(index)>;\
                        using property_type = typename NanaDialogMaker::detail::AutoPropertyChooser <\
                            std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>,\
                            std::tuple_element_t <index_type::value, property_types>\
                        >::type;\
                        \
                        static_cast <property_type*>(properties_[index_type::value].get())->store(\
                            fusion::at <index_type>(holder_)\
                        );\
                    }\
                );\
                return holder_;\
            }\
            \
            void data(holder_type const& holder)\
            {\
                namespace mpl = boost::mpl;\
                namespace fusion = boost::fusion;\
                \
                std::scoped_lock <std::recursive_mutex> guard(holderGuard_);\
                \
                holder_ = holder;\
                if (!areControlsAlive())\
                    return;\
                \
                mpl::for_each<mpl::range_c<std::size_t, 0, boost::fusion::result_of::size<holder_type>::type::value>>\
                (\
                    [this, &holder](auto index)\
                    {\
                        using index_type = std::decay_t<decltype(index)>;\
                        using property_type = typename NanaDialogMaker::detail::AutoPropertyChooser <\
                            std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>,\
                            std::tuple_element_t <index_type::value, property_types>\
                        >::type;\
                        \
                        static_cast <property_type*>(properties_[index_type::value].get())->load(\
                            fusion::at <index_type>(holder)\
                        );\
                    }\
                );\
            }\
            \
            nana::place& layout()\
            {\
                return layout_;\
            }\
        };\
    BOOST_PP_IF(NANA_DIALOG_MAKER_COUNT_ARGUMENTS(nspace), },)

#define NANA_DIALOG_MAKER_MAKE_PANEL(nspace, object, seq) \
    NANA_DIALOG_MAKER_MAKE_PANEL_IMPL(\
        nspace,\
        object,\
        BOOST_PP_CAT(NANA_DIALOG_MAKER_SEQUENCE_FACTORY_0 seq, _END)\
    )
