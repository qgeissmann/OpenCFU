#ifndef GUI_HELLOWINDOW_H
#define GUI_HELLOWINDOW_H

#include "defines.hpp"
#include "text.hpp"
#include <gtkmm/stock.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/button.h>
#include <gtkmm/linkbutton.h>
#include <gtkmm/image.h>


#if defined _WIN64 || defined _WIN32
#include <windows.h>
#endif

class PortableLinkButton: public Gtk::LinkButton
{
    public:
    PortableLinkButton(const Glib::ustring& a,const Glib::ustring& b):
        Gtk::LinkButton(a,b){}
#if defined _WIN64 || defined _WIN32
    protected:

            void on_pressed(){
                ShellExecute(NULL, "open", this->get_uri().c_str(), NULL, NULL, SW_SHOWNORMAL);
                set_visited();
            }

#endif
};

class Gui_HelloWindow: public Gtk::Window
{
    public:
        Gui_HelloWindow(int& hello_level);

    protected:
        void on_realize();
//        bool on_delete_event(GdkEventAny* event);
        void on_check_but();
        bool on_key_press_event(GdkEventKey* event);

    private:
        int& m_hello_level;
        Gtk::HBox m_hbox0,m_hbox1;
        Gtk::VBox m_vbox0, m_vbox1;
        Gtk::Label m_text;
        PortableLinkButton m_website_blink,m_paper_blink, m_email_blink;
        Gtk::Image m_ocfu_image;
        Gtk::CheckButton m_hide_next_time;
        Gtk::Button m_button_OK;
        Gtk::HSeparator m_hsep;


};

#endif // GUI_HELLOWINDOW_H
