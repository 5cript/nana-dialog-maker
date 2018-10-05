#include <nana-dialog-maker/generators/textbox_property.hpp>
#include <nana-dialog-maker/make_panel.hpp>
#include <nana-dialog-maker/layouting.hpp>
#include <nana-dialog-maker/dialog.hpp>

#include <iostream>
#include <optional>
#include <string>

struct Thing
{
    std::string id;
    std::string code;
};

// Creates a class ThingPanel in the same namespace as Thing.
NANA_DIALOG_MAKER_MAKE_PANEL
(
    /* not in a namespace */,
    Thing,
    ("ID: ", id, NanaDialogMaker::AutoProperty)
    ("Code: ", code, NanaDialogMaker::TextboxProperty)
    //("Number: ", number)
)

int main()
{
	using namespace NanaDialogMaker;
	
	// A dialog that contains a single panel for "Thing"s
	GeneratedDialog <ThingPanel> dialog;
	
	// Create a layout template type for property generators.
	using template_type = boost::fusion::map <
        boost::fusion::pair <void, LayoutTemplate>,
        boost::fusion::pair <TextboxProperty, LayoutTemplate>
    >;
	
	// Create templates
	template_type templates{
		// The template for the panel itself which will have all controls added within {} (using the fmt::format library)
		boost::fusion::make_pair<void>(LayoutTemplate{
            "<vertical margin=5 {}>"
        }),
		// The template for TextboxProperties (-> std::string)
		boost::fusion::make_pair<NanaDialogMaker::TextboxProperty>(NanaDialogMaker::LayoutTemplate{
			R"(
				<max=100
                    <vertical max=25
                        <weight=3>
                        <{0}_NANA_DIALOG_MAKER_LABEL>
                    >
                    <{0}>
                >
                <weight=5>
			)"
        })
	};
	
	// Apply layouts for each panel within the dialog and apply their generative templates
	dialog.applyGeneratedPanelLayout <ThingPanel>(templates);
	
	// Applys a layout to the dialog. Panels are named by their type. Multiple Panels of the same type cannot be split as of yet.
	// (Is there a use case for the same panel multiple times?)
	dialog.applyLayout("<ThingPanel>");
	
	nana::API::modal_window(diag);
    diag.show();

	// get data from panel controls.
    auto p = diag.data <ThingPanel>().front();
    std::cout << p.id << ": " << p.code << "\n";
}