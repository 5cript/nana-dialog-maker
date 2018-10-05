#pragma once

#include <string>

namespace NanaDialogMaker
{
    class LayoutTemplate
    {
    public:
        LayoutTemplate(std::string formatString);
        ~LayoutTemplate() = default;

        virtual std::string format
        (
            std::string const& memberName
        );

    private:
        std::string formatString_;
    };
}
