
#include <nana-dialog-maker/generators/textbox_property.hpp>
#include <nana-dialog-maker/generators/boolean_property.hpp>
#include <nana-dialog-maker/generators/combox_property.hpp>
#include <nana-dialog-maker/generators/integral_property.hpp>
#include <nana-dialog-maker/generators/optional_property.hpp>
#include <nana-dialog-maker/make_panel.hpp>
#include <nana-dialog-maker/layouting.hpp>
#include <nana-dialog-maker/dialog.hpp>

#include <nana/gui/widgets/panel.hpp>

#include <iostream>
#include <optional>
#include <string>

namespace DataStructures
{
    struct Person
    {
        std::string firstName;
        std::optional <std::string> lastName;
        int number;
    };
}

struct Thing
{
    std::string id;
    std::string code;
    bool bla;
};

NANA_DIALOG_MAKER_MAKE_PANEL
(
    DataStructures,
    Person,
    ("First Name: ", firstName, NanaDialogMaker::ComboxProperty)
    ("Last Name: ", lastName, NanaDialogMaker::AutoProperty)
    ("Number: ", number, NanaDialogMaker::AutoProperty)
)

NANA_DIALOG_MAKER_MAKE_PANEL
(
    /* not in a namespace */,
    Thing,
    ("ID: ", id, NanaDialogMaker::AutoProperty)
    ("Code: ", code, NanaDialogMaker::TextboxProperty)
    ("Blubber: ", bla, NanaDialogMaker::AutoProperty)
    //("Number: ", number)
)

int main()
{
    using namespace DataStructures;

    NanaDialogMaker::GeneratedDialog <PersonPanel> dialog;
    dialog.constructPanel <PersonPanel>
    (
        boost::fusion::map <
            boost::fusion::pair <PersonPanel::Ids::firstName, std::tuple <std::vector <std::string>, bool>>,
            boost::fusion::pair <PersonPanel::Ids::lastName, std::tuple <std::function <void(nana::textbox&)>>>
        >
        {
            boost::fusion::make_pair <PersonPanel::Ids::firstName>(std::tuple <std::vector <std::string>, bool>{
                {"Tim", "Hans", "Franz"},
                false
            }),
            boost::fusion::make_pair <PersonPanel::Ids::lastName>(std::tuple <std::function <void(nana::textbox&)>>{
                [](nana::textbox& tb)
                {
                    tb.multi_lines(true);
                }
            })
        }
    );
    dialog.applyGeneratedPanelLayout <PersonPanel>
    (
        boost::fusion::map <
            boost::fusion::pair <void, NanaDialogMaker::LayoutTemplate>,
            boost::fusion::pair <NanaDialogMaker::TextboxProperty, NanaDialogMaker::LayoutTemplate>,
            boost::fusion::pair <NanaDialogMaker::ComboxProperty, NanaDialogMaker::LayoutTemplate>,
            boost::fusion::pair <NanaDialogMaker::IntegralProperty <int>, NanaDialogMaker::LayoutTemplate>,
            boost::fusion::pair <NanaDialogMaker::BooleanProperty, NanaDialogMaker::LayoutTemplate>,
            boost::fusion::pair <NanaDialogMaker::OptionalProperty <NanaDialogMaker::TextboxProperty>, NanaDialogMaker::LayoutTemplate>
        >{
            boost::fusion::make_pair<void>(NanaDialogMaker::LayoutTemplate{
                "<vertical margin=5 {}>"
            }),
            boost::fusion::make_pair<NanaDialogMaker::TextboxProperty>(NanaDialogMaker::LayoutTemplate{
                R"(
				<max=25
                    <vertical max=100
                        <weight=3>
                        <{0}_NANA_DIALOG_MAKER_LABEL>
                    >
                    <{0}>
                >
                <weight=5>
                )"
            }),
            boost::fusion::make_pair<NanaDialogMaker::ComboxProperty>(NanaDialogMaker::LayoutTemplate{
                R"(
				<max=25
                    <vertical max=100
                        <weight=3>
                        <{0}_NANA_DIALOG_MAKER_LABEL>
                    >
                    <{0}>
                >
                <weight=5>
                )"
            }),
            boost::fusion::make_pair<NanaDialogMaker::IntegralProperty <int>>(NanaDialogMaker::LayoutTemplate{
                R"(
				<max=25
                    <vertical max=100
                        <weight=3>
                        <{0}_NANA_DIALOG_MAKER_LABEL>
                    >
                    <{0}>
                >
                <weight=5>
                )"
            }),
            boost::fusion::make_pair<NanaDialogMaker::BooleanProperty>(NanaDialogMaker::LayoutTemplate{
                "<{}>"
            }),
            boost::fusion::make_pair<NanaDialogMaker::OptionalProperty <NanaDialogMaker::TextboxProperty>>(NanaDialogMaker::LayoutTemplate{
                R"(
				<max=25
                    <vertical max=100
                        <weight=3>
                        <{0}_NANA_DIALOG_MAKER_LABEL>
                    >
                    <{0}>
                >
                <weight=5>
                )"
            })
        }
    );
    dialog.applyLayout("<vertical <PersonPanel>>");

    nana::API::modal_window(dialog);
    dialog.show();

    auto p = dialog.data <PersonPanel>().front();
    std::cout << p.firstName << ":" << p.lastName.value() << "\n";

    return 0;
}
