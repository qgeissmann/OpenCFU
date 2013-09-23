#ifndef GUI_USERFILTERSETTER_H
#define GUI_USERFILTERSETTER_H

#include "defines.hpp"
#include <memory>
#include "Gui_OptionSetterBaseClass.hpp"

class Gui_UserFilterSetter : public Gui_OptionSetterBaseClass
{
    public:
        Gui_UserFilterSetter(Gui_ProcessorHandler& processor_hand);
        void on_click_object(int obj, int but);
        void on_toggle_object(int obj, bool t);
    protected:
        bool updateOptions();
        void updateNFiles(int i){};
    private:
        int m_idx_to_switch;
        std::shared_ptr<cv::Mat> m_filter;

};

#endif // GUI_USERFILTERSETTER_H
