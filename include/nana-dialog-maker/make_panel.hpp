#pragma once

#include "base_panel.hpp"

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#include <boost/preprocessor/tuple/elem.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <type_traits>

#define NANA_DIALOG_MAKER_CLASS_SUFFIX Panel

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

#define NANA_DIALOG_MAKER_INDEX_TYPE_NAMES(r, data, i, elem) \
    using BOOST_PP_TUPLE_ELEM(3, 1, elem) = std::integral_constant<int, i>;


#define NANA_DIALOG_MAKER_MAKE_PANEL_IMPL(nspace, object, seq)\
    BOOST_FUSION_ADAPT_STRUCT\
    (\
        BOOST_PP_IF(NANA_DIALOG_MAKER_COUNT_ARGUMENTS(nspace), nspace::,) object,\
        BOOST_PP_SEQ_ENUM(\
            BOOST_PP_SEQ_FOR_EACH(NANA_DIALOG_MAKER_MEMBER_UNPACKER, _, seq)\
        )\
    )\
    BOOST_PP_IF(NANA_DIALOG_MAKER_COUNT_ARGUMENTS(nspace), namespace nspace{,) \
        class BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX)\
            : public NanaDialogMaker::BasePanel <BOOST_PP_CAT(object, NANA_DIALOG_MAKER_CLASS_SUFFIX), object>\
        {\
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
            struct Ids\
            {\
                BOOST_PP_SEQ_FOR_EACH_I(NANA_DIALOG_MAKER_INDEX_TYPE_NAMES, _, seq)\
            };\
            \
        public:\
            using BasePanel::BasePanel;\
        };\
    BOOST_PP_IF(NANA_DIALOG_MAKER_COUNT_ARGUMENTS(nspace), },)

#define NANA_DIALOG_MAKER_MAKE_PANEL(nspace, object, seq) \
    NANA_DIALOG_MAKER_MAKE_PANEL_IMPL(\
        nspace,\
        object,\
        BOOST_PP_CAT(NANA_DIALOG_MAKER_SEQUENCE_FACTORY_0 seq, _END)\
    )
