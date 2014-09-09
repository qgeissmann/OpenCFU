#ifndef GUI_RESULTDISPLAYBASECLASS_H
#define GUI_RESULTDISPLAYBASECLASS_H

//

#include <giomm/file.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/scrolledwindow.h>
#include "Gui_ProcessorHandler.hpp"
#include "ResultMap.hpp"

class Gui_ResultDisplayBaseClass : public Gtk::Frame
{
    public:
        Gui_ResultDisplayBaseClass(sigc::signal<void,Glib::RefPtr<Gio::File>,int>& signal_update_result_map, ResultMap& result_map);
        virtual ~Gui_ResultDisplayBaseClass();

    protected:
        void on_realize();
        void on_update_view(Glib::RefPtr<Gio::File> file, int idx){this->updateView(file,idx);}
        virtual void updateView(Glib::RefPtr<Gio::File> str, int idx) = 0;

        sigc::signal<void,Glib::RefPtr<Gio::File>,int>& m_signal_update_result_map;
        ResultMap& m_result_map;
        Gtk::TreeModel::ColumnRecord m_col_model;
        Gtk::TreeView m_tree_view;

        class ModelColumnsBaseClass : public Gtk::TreeModel::ColumnRecord{};
        Glib::RefPtr<Gtk::TreeSelection> m_ref_tree_selection;
        Gtk::ScrolledWindow m_scrolled_window;
        Gtk::VBox m_vbox;
        Gtk::Label m_label;
        Gtk::HBox m_hbox;
};

#endif // GUI_RESULTDISPLAYBASECLASS_H
