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
            events().unload([this](auto const&) {
                auto apply = [this](auto& panel)
                {
                    // store all remaining data via sideeffect, so the data does not get lost.
                    data <std::decay_t <decltype(panel)>>();
                };

                std::apply([&apply](auto&... panel){(apply(panel), ...);},panels_);
            });
        }

        /**
         *  Loads data out of the ui panel.
         */
        template <typename T>
        std::vector <typename T::holder_type> data()
        {
            std::vector <typename T::holder_type> results;
            auto conditionalApply = [this, &results](auto const& panel)
            {
                using panel_t = std::decay_t <decltype(panel)>;
                if constexpr (std::is_same_v <panel_t, T>)
                {
                    auto data = panel.data();
                    results.push_back(data);
                }
            };

            std::apply([&conditionalApply](auto const&... panel){(conditionalApply(panel), ...);},panels_);
            return results;
        }

        /**
         *  stores data into the ui panel.
         */
        template <typename T>
        void data(typename T::holder_type const& values)
        {
            auto conditionalApply = [this, &values](auto& panel)
            {
                using panel_t = std::decay_t <decltype(panel)>;
                if constexpr (std::is_same_v <panel_t, T>)
                    panel.data(values);
            };

            std::apply([&conditionalApply](auto&... panel){(conditionalApply(panel), ...);},panels_);
        }

        /**
         *  Has any changes been made to any panel of type T?
         */
        template <typename T>
        bool isDirty()
        {
            bool anyDirty = false;
            auto conditionalApply = [this, &anyDirty](auto& panel)
            {
                using panel_t = std::decay_t <decltype(panel)>;
                if constexpr (std::is_same_v <panel_t, T>)
                    anyDirty |= panel.isDirty();
            };

            std::apply([&conditionalApply](auto&... panel){(conditionalApply(panel), ...);},panels_);
            return anyDirty;
        }

        /**
         *  Give layout templates for each type of property.
         *  These templates are used to generate layouts which are then applied.
         */
        template <typename T, typename LayoutTemplateT>
        void applyGeneratedPanelLayout(LayoutTemplateT const& templates)
        {
            auto conditionalApply = [this, &templates](auto& panel)
            {
                using panel_t = std::decay_t <decltype(panel)>;
                if constexpr (std::is_same_v <panel_t, T>)
                    NanaDialogMaker::applyLayout(panel, panel.generateLayout(templates));
            };

            std::apply([&conditionalApply](auto&... panel){(conditionalApply(panel), ...);},panels_);
        }

        /**
         *  Applies a layout to a all panels of the give type with layoutString.
         *  Prefer to use applyGeneratedPanelLayout
         */
        template <typename T>
        void applyPanelLayout(std::string const& layoutString)
        {
            auto conditionalApply = [this, &layoutString](auto& panel)
            {
                using panel_t = std::decay_t <decltype(panel)>;
                if constexpr (std::is_same_v <panel_t, T>)
                    NanaDialogMaker::applyLayout(panel, layoutString);
            };

            std::apply([&conditionalApply](auto&... panel){(conditionalApply(panel), ...);},panels_);
        }

        /**
         *  Applies a layout to the dialog that encompasses all panels.
         *  Use to layout panels (side by side, vertical, whatever).
         */
        void applyLayout(std::string const& layoutString)
        {
            auto addPanelToLayout = [this](auto const& panel)
            {
                using T = std::decay_t <decltype(panel)>;
                layout_.field(T::name) << panel;
            };

            std::apply([&addPanelToLayout](auto&... panel){(addPanelToLayout(panel), ...);},panels_);

            layout_.div(layoutString);
            layout_.collocate();
        }
    };
}
