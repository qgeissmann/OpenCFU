#ifndef GUI_PIXBUFOPENER_H
#define GUI_PIXBUFOPENER_H

#include <gtkmm/drawingarea.h>
#include "defines.hpp"
#include "Result.hpp"
class Gui_PixbufOpener
{
    public:
        static Glib::RefPtr<Gdk:: Pixbuf > pixbufOpen(const std::string& name);
    private:
};

#endif // GUI_PIXBUFOPENER_H
