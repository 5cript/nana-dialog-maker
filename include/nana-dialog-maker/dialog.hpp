#pragma once

#include "layouting.hpp"

#include <nana/gui/widgets/form.hpp>
#include <nana/gui/place.hpp>

#include <type_traits>

namespace NanaDialogMaker
{
    template <typename... Panels>
    class GeneratedDialog : public nana::form
    {
    private:
        std::tuple <Panels...> panels_;
        nana::place layout_;

    public:
        explicit GeneratedDialog()
            : panels_{((void)sizeof(Panels), *this)...}
            , layout_{*this}
        {
        }

        template <typename T, typename LayoutTemplateT>
        void applyGeneratedPanelLayout(LayoutTemplateT const& templates)
        {
            auto conditionalApply = [this, &templates](auto& panel)
            {
                using panel_t = std::decay_t <decltype(panel)>;
                if constexpr (std::is_same_v <panel_t, T>)
                    NanaDialogMaker::applyLayout(panel, panel.generateLayout(templates));
            };

            std::apply
            (
                [&conditionalApply](auto&... panel)
                {
                    (conditionalApply(panel), ...);
                },
                panels_
            );
        }

        template <typename T>
        void applyPanelLayout(std::string const& layoutString)
        {
            auto conditionalApply = [this, &layoutString](auto& panel)
            {
                using panel_t = std::decay_t <decltype(panel)>;
                if constexpr (std::is_same_v <panel_t, T>)
                    NanaDialogMaker::applyLayout(panel, layoutString);
            };

            std::apply
            (
                [&conditionalApply](auto&... panel)
                {
                    (conditionalApply(panel), ...);
                },
                panels_
            );
        }

        void applyLayout(std::string const& layoutString)
        {
            auto addPanelToLayout = [this](auto const& panel)
            {
                using T = std::decay_t <decltype(panel)>;
                layout_.field(T::name) << panel;
            };

            std::apply
            (
                [&addPanelToLayout](auto const&... panel)
                {
                    (addPanelToLayout(panel), ...);
                },
                panels_
            );

            layout_.div(layoutString);
            layout_.collocate();
        }
    };
}
