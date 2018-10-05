#pragma once

#include <string>
#include <iostream>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/adapted.hpp>

namespace NanaDialogMaker
{
    template <typename PanelT>
    void applyLayout(PanelT& panel, std::string const& layoutString)
    {
        auto& layout = panel.layout();
        auto& props = panel.properties();
        for (auto& prop : props)
            prop->addToPlace(layout);

        std::cout << layoutString << "\n";

        layout.div(layoutString);
        layout.collocate();
    }
}
