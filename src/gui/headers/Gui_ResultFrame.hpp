#ifndef GUI_RESULTFRAME_H
#define GUI_RESULTFRAME_H


#include "defines.hpp"
#include <sigc++/sigc++.h>
#include <gtkmm/paned.h>
#include "ResultMap.hpp"
#include "Gui_ProcessorHandler.hpp"
#include "Gui_ResultListDisplay.hpp"
#include "Gui_ResultDetails.hpp"
#include "Gui_UserFilterSetter.hpp"

class Gui_ResultFrame : public Gtk::HPaned
{
    public:
        Gui_ResultFrame(Gui_ProcessorHandler& processor_hand, sigc::signal<void,bool>& signal_set_to_NA, ResultMap& result_map);
        sigc::signal<void,int>& signal_object_selected(){
            return m_signal_select_object;}

        void on_click_object(int i,int but);
        bool isAllSaved(){return m_result_display.isAllSaved();}

        ResultMap& getResultMap(){
            DEV_INFOS("getting result map from result frame");
            return m_result_map;}

    protected:
        void on_processor_idle(int state);
        void on_set_current_to_NA(bool NA);
        void on_realize();
    private:
        ResultMap& m_result_map;
        Gui_ProcessorHandler& m_processor_hand;
        Gui_ResultListDisplay m_result_display;
        Gui_ResultDetails m_result_detail;
        Gui_UserFilterSetter m_user_filter;

        sigc::signal<void,Glib::RefPtr<Gio::File>,int> m_signal_update_result_map;
        sigc::signal<void,int> m_signal_select_object;
        sigc::signal<void,int,bool> m_signal_toggle_object;
};


#endif // GUI_RESULTFRAME_H
