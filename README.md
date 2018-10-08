# nana-dialog-maker
Generates very basic input form dialogs that are repetitive to make in nana.

Structures can be adapted to become nana panels with logical controls that allow for their modification.

Support:
    - Single Line TextBox for std::string
    - Checkbox for bool
    - Number Edits (restricted textbox)
    - Combox -> select from a few options.
	- Create your own! Derive from NanaDialogMaker::Property and please read the section about it down below

Dependencies:
    - boost fusion
    - fmt::format here on github
	- nana

## Long Example
```C++
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
        boost::fusion::make_pair<TextboxProperty>(LayoutTemplate{
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
    };

	// You have to create the widgets in the panels manually
	// The reason is that you may want or need to pass arguments to the 
	// constructors. Passing parameters is optional though.
	dialog.constructPanel <PersonPanel>();
    
    // Apply layouts for each panel within the dialog and apply their generative templates
    dialog.applyGeneratedPanelLayout <ThingPanel>(templates);
    
    // Applys a layout to the dialog. Panels are named by their type. Multiple Panels of the same type cannot be split as of yet.
    // (Is there a use case for the same panel multiple times?)
    dialog.applyLayout("<ThingPanel>");
    
    nana::API::modal_window(diag);
    diag.show();

    // get data from panel controls.
    // the data function can also be used to fill the form with previous data.
    auto p = diag.data <ThingPanel>().front();
    std::cout << p.id << ": " << p.code << "\n";
}
```

## NANA_DIALOG_MAKER_MAKE_PANEL
This macro is an extension to BOOST_FUSION_ADAPT_STRUCT.
Apart from using BOOST_FUSION_ADAPT_STRUCT directly, it generates a class that inherits from NanaDialogMaker::BasePanel.
The Parameters are as follows:
```C++
NANA_DIALOG_MAKER_MAKE_PANEL
(
	Some::Namespace::Your::Data::Holder::Is::In,
	DataHoldingClassName, // The generated class will get Panel appended "DataHoldingClassNamePanel"
	// The description is shown in a label associated with each ui element, except the ui element provides its own descriptory label
	// WidgetGeneratorType is something that derives from NanaDialogMaker::Property
	// Some are already provided in <nana-dialog-maker/generators>
	("Some description for the input: ", memberName, WidgetGeneratorType)
)
```

## BasePanel / Generated Panel
This library chooses to generate panels, not forms, because you can then embed panels into other windows and do more advanced stuff with them,
like having them behind tabs!

### Synopsis
```C++
template <typename DerivedT, typename HolderT, bool hasBackground>
class BasePanel : public nana::panel <hasBackground>
{
public:
	using holder_type = HolderT;
	using panel_type = DerivedT; // CRTP

	explicit BasePanel(nana::window wd, bool visible = true);

	/**
	 *  Constructs all elements within the panel. MUST BE CALLED.
	 *  It forwards the arguments from the boost::fusion::map to the constructors, where passed.
	 */
	template <typename T>
	void constructProperties(T const& argumentMap);

	/**
 	 *  Returns whether the controls are alive or not having a modal dialog with panels in it
 	 *  will kill the panels when the dialog hides (which is dumb behavior imho).
	 */
	bool areControlsAlive() const;

	/**
	 *  Retrieve a reference to the internal property store that has'em all.
	 */
	std::vector <std::unique_ptr <NanaDialogMaker::Property>>& properties();

	/**
	 *  Has any of the controls been modified?
	 */
	bool isDirty() const;

	/**
	 *  Generates a layout for the entire panel that encompasses all widgets.
	 *  Each property_type has its own template from which a part of the layout is built.
	 */
	template <typename LayoutTemplateT>
	std::string generateLayout(LayoutTemplateT const& templates);

	/**
     *  Extract all the data from the ui elements, or from the memorized storage if controls are dead.
     *  This does require the user to call this function once in the owning dialog unload, or all data is lost.
     */
	holder_type data() const;

	/**
	 *  Sets the ui elements to contain the data of "holder".
	 */
	void data(holder_type const& holder);

	/**
	 *  Retrieve the nana::place of this panel.
	 */
	nana::place& layout();
}
```

## Generated Dialog
The library also offers a convenience dialog that can wrap one or more panels (uniqueness is not enforced and having
the same panel in there multiple times is supported, but I question the relevance).

### Synopsis
```C++
template <typename... Panels>
class GeneratedDialog : public nana::form
{
	explicit GeneratedDialog(nana::rectangle const& rect = nana::API::make_center(300, 200), const nana::appearance& appearance={});

	/**
	 *  Loads the data of the specified panel type.
	 *  Stored in a vector, because multiple panels of the same type may exist. Ordering of template arguments is preserved.
	 */
	template <typename T>
	std::vector <typename T::holder_type> data();

	/**
	 *  Constructs the respective panel. All panels of the same type are constructed with the same parameters.
	 *	Argument map is a fusion::map with a integral_constant as the key to indicate the members. Please see the examples
	 */
	template <typename T, typename ArgumentMap = boost::fusion::map<>>
	void constructPanel(ArgumentMap&& map = {});

	/**
	 *  stores data into the ui panel.
	 */
	template <typename T>
	void data(typename T::holder_type const& values);

	/**
	 *  Has any changes been made to any panel of type T?
	 */
	template <typename T>
	bool isDirty();

	/**
	 *  Give layout templates for each type of property.
	 *  These templates are used to generate layouts which are then applied.
	 */
	template <typename T, typename LayoutTemplateT>
	void applyGeneratedPanelLayout(LayoutTemplateT const& templates);

	/**
	 *  Applies a layout to a all panels of the give type with layoutString.
	 *  Prefer to use applyGeneratedPanelLayout
	 */
	template <typename T>
	void applyPanelLayout(std::string const& layoutString);

	/**
	 *  Applies a layout to the dialog that encompasses all panels.
	 *  Use to layout panels (side by side, vertical, whatever).
	 */
	void applyLayout(std::string const& layoutString);
};
```