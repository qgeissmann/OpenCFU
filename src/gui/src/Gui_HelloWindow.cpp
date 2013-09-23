#include "Gui_HelloWindow.hpp"
//#include <gtkmm.h>
#include <gtkmm/main.h>
#include "Gui_PixbufOpener.hpp"

Gui_HelloWindow::Gui_HelloWindow(int& hello_level):
    m_hello_level(hello_level),

    m_website_blink(PACKAGE_URL,VISIT_WEBSITE),
    m_paper_blink(PLOS_URL,READ_PUBLICATION),
    m_email_blink("",REPORT_BUGS),
    m_hide_next_time(SHOW_HELLO_NEXT_TIME),
    m_button_OK(Gtk::Stock::OK)
{


    m_text.set_text(std::string(INTRO_TEXT_1)+std::string(PACKAGE_VERSION)+std::string(INTRO_TEXT_2));
    m_text.set_line_wrap();
    set_urgency_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
    set_resizable(false);
    set_decorated(false);
    set_keep_above (true);
    set_modal(true);
    set_border_width(16);

    m_text.set_justify(Gtk::JUSTIFY_CENTER);
    add(m_vbox0);
    m_vbox0.pack_start(m_hbox0);
    m_vbox0.pack_start(m_vbox1);

    m_hbox0.pack_start(m_ocfu_image);
    m_hbox0.pack_start(m_text);
    m_vbox1.pack_start(m_website_blink);
    m_vbox1.pack_start(m_paper_blink);
    m_vbox1.pack_start(m_email_blink);
    m_vbox1.pack_start(m_hsep);
    m_vbox1.pack_start(m_hbox1);
    if(hello_level > 0)
        m_hbox1.pack_start(m_hide_next_time);

    m_hbox1.pack_start(m_button_OK);


    m_text.set_use_markup(true);
    std::stringstream path;
    path << "mailto:"<<PACKAGE_BUGREPORT;
    m_email_blink.set_uri(path.str());

    m_ocfu_image.set(Gui_PixbufOpener::pixbufOpen(LOGO_IMG));

    m_hide_next_time.signal_clicked().connect(sigc::mem_fun(*this,&Gui_HelloWindow::on_check_but));

    m_website_blink.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&Gui_HelloWindow::set_keep_above),false));
    m_paper_blink.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&Gui_HelloWindow::set_keep_above),false));
    m_email_blink.signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&Gui_HelloWindow::set_keep_above),false));

    m_button_OK.signal_clicked().connect(sigc::ptr_fun(Gtk::Main::quit));

    set_focus(m_button_OK);
    m_hide_next_time.set_active(true);
    show_all();
}

void Gui_HelloWindow::on_check_but(){
    if(m_hide_next_time.get_active())
        m_hello_level = 1;
    else
        m_hello_level = 2;
}

bool Gui_HelloWindow::on_key_press_event(GdkEventKey* event){
    if(
        event->keyval == 0xff1b || //Gdk::KEY_Escape ||
        event->keyval == 0xff0d || //Gdk::GDK_KEY_Return ||
        event->keyval == 0xffff) //Gdk::GDK_KEY_Delete)
            Gtk::Main::quit();
    return true;
}

void Gui_HelloWindow::on_realize(){
    Gtk::Window::on_realize();
    Glib::RefPtr< Gdk::Screen > screen = Gdk::Screen::get_default();
    Gdk::Rectangle rect;
    screen->get_monitor_geometry(screen->get_primary_monitor(),rect);
    int ww =get_width ();
    int sw =rect.get_width();
    int wh =get_height ();
    int sh =rect.get_height();
    int x = sw/2 - ww/2 + rect.get_x();
    int y = sh/2 - wh/2 + rect.get_y();
    move (x,y);
}
