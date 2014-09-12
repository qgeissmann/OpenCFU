#ifndef GUI_RESULTLISTDISPLAY_H
#define GUI_RESULTLISTDISPLAY_H

#include <sstream>
#include <pangomm/attributes.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/treestore.h>
#include "Gui_ResultDisplayBaseClass.hpp"
#include "Gui_ResultFileWriter.hpp"
#include "text.hpp"




typedef Gtk::TreeModel::Children    type_children;

class Gui_ResultListDisplay : public Gui_ResultDisplayBaseClass{
    public:
        Gui_ResultListDisplay(sigc::signal<void,Glib::RefPtr<Gio::File>,int>& signal_update_result_map, ResultMap& result_map);
        bool isAllSaved(){return m_file_writer.getInUpToDate();}

    protected:
        type_children::iterator findWichRow(Gtk::TreeModel::Children& children, int idx);
        void on_save_all();
        void on_save();
        void on_delete();
        void on_edit_comment(const Glib::ustring& path, const Glib::ustring& text);
        void makeIndecesOfSelected(const Gtk::TreeModel::iterator& ite){
            m_indeces_of_selected.push_back((*ite)[m_col_model.m_col_id]);
        }
        void updateView(Glib::RefPtr<Gio::File> file, int idx);
        int getDepthFromPath(const Glib::ustring& path);

        class ModelColumns : public Gtk::TreeModel::ColumnRecord{
            public:
                ModelColumns()
                { add(m_col_id);
//
                add(m_col_n_objects);
                add(m_col_n_excluded);
                add(m_comment);
                add(m_col_path);
                add(m_col_name);
/**
                add(m_col_clust1);
                add(m_col_clust2);
                add(m_col_clust3);
*/
                }

                Gtk::TreeModelColumn<int> m_col_id;
                Gtk::TreeModelColumn<std::string > m_col_n_objects;
                Gtk::TreeModelColumn<std::string> m_col_n_excluded;
                Gtk::TreeModelColumn<Glib::ustring> m_comment;
                Gtk::TreeModelColumn<std::string> m_col_path;
                Gtk::TreeModelColumn<std::string> m_col_name;
/**             Gtk::TreeModelColumn<std::string> m_col_clust1;
                Gtk::TreeModelColumn<std::string> m_col_clust2;
                Gtk::TreeModelColumn<std::string> m_col_clust3;*/
        };
       ModelColumns m_col_model;
    private:
        Glib::RefPtr<Gtk::TreeStore> m_ref_tree_model;
        Gtk::Button m_save_selected;
        Gtk::Button m_save_all;
        Gtk::Button m_delete_selected;
        std::vector<int> m_indeces_of_selected;
        Gui_ResultFileWriter m_file_writer;
};



#endif // GUI_RESULTLISTDISPLAY_H
