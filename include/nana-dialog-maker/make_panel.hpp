#pragma once

#include "property.hpp"
#include "property_factory.hpp"

#include <nana/gui/widgets/panel.hpp>

#include <vector>
#include <string>
#include <memory>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>

#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/adapted.hpp>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#include <boost/preprocessor/tuple/elem.hpp>

#include <boost/preprocessor/cat.hpp>

#define NANA_DIALOG_MAKER_CLASS_SUFFIX Panel

#define NANA_DIALOG_MAKER_SEQUENCE_FACTORY_0(...)\
     ((__VA_ARGS__)) NANA_DIALOG_MAKER_SEQUENCE_FACTORY_1
#define NANA_DIALOG_MAKER_SEQUENCE_FACTORY_1(...)\
     ((__VA_ARGS__)) NANA_DIALOG_MAKER_SEQUENCE_FACTORY_0
#define NANA_DIALOG_MAKER_SEQUENCE_FACTORY_0_END
#define NANA_DIALOG_MAKER_SEQUENCE_FACTORY_1_END

#define NANA_DIALOG_MAKER_DESCRIPTION_UNPACKER(r, data, elem)\
    (BOOST_PP_TUPLE_ELEM(2, 0, elem))

#define NANA_DIALOG_MAKER_MEMBER_UNPACKER(r, data, elem)\
    (BOOST_PP_TUPLE_ELEM(2, 1, elem))

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
        private:\
            static constexpr const char* descriptions[] = {\
                BOOST_PP_SEQ_ENUM(\
                    BOOST_PP_SEQ_FOR_EACH(NANA_DIALOG_MAKER_DESCRIPTION_UNPACKER, _, seq)\
                )\
            };\
            \
        private:\
            \
            std::vector <std::unique_ptr <NanaDialogMaker::Property>> properties_;\
            NanaDialogMaker::PropertyFactory <holder_type> propFactory_;\
            nana::place layout_;\
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
                        std::cout << fusion::extension::struct_member_name<holder_type, index_type::value>::call() << "\n";\
                        using property_type = typename NanaDialogMaker::PropertyFromValueType\
                            <std::decay_t <typename fusion::result_of::at <holder_type, index_type>::type>>::type;\
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
        public:\
            BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)()\
            {\
                construct();\
            }\
            \
            std::vector <std::unique_ptr <NanaDialogMaker::Property>>& properties()\
            {\
                return properties_;\
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
