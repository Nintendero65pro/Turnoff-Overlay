#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header


class GuiTest : public tsl::Gui {
public:
    GuiTest(u8 arg1, u8 arg2, bool arg3) { }

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        // A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
        // If you need more information in the header or want to change it's look, use a HeaderOverlayFrame.
        auto frame = new tsl::elm::OverlayFrame("Turn Off Overlay", "v0.1");

        // A list that can contain sub elements and handles scrolling
        auto list = new tsl::elm::List();

        // List Items
        list->addItem(new tsl::elm::CategoryHeader("Options"));

        //shutdown
        auto *clickableListItem = new tsl::elm::ListItem("Turn off", "Save ur data before");
        clickableListItem->setClickListener([](u64 keys) {
            if (keys & HidNpadButton_A) {
                splExit();
                fsdevUnmountAll();
                spsmShutdown(false); //remember kids false = shutdown true = restart
                return true;
            }

            return false;
        });
        list->addItem(clickableListItem);

        //shutdown
        auto *clickableListItem2 = new tsl::elm::ListItem("Restart", "save or u lose ur vbucks");
        clickableListItem2->setClickListener([](u64 keys) {
            if (keys & HidNpadButton_A) {
                splExit();
                fsdevUnmountAll();
                spsmShutdown(true); //remember kids false = shutdown true = restart
                return true; //i clicked a i swear
            }

            return false;
        });
        list->addItem(clickableListItem2);


        // Add the list to the frame for it to be drawn
        frame->setContent(list);

        // Return the frame to have it become the top level element of this Gui
        return frame;
    }

    // Called once every frame to update values
    virtual void update() override {

    }

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override {
        return false;   // Return true here to signal the inputs have been consumed
    }
};

class OverlayTest : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys and set:sys
    virtual void initServices() override {
        spsmInitialize(); //init spsm for the pure purpose to shutdown the console, bless spsm btw
    }  // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override {}  // Called at the end to clean up all services previously initialized

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<GuiTest>(1, 2, true);  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
    }
};

int main(int argc, char **argv) {
    return tsl::loop<OverlayTest>(argc, argv);
}