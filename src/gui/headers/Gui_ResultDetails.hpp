#ifndef GUI_RESULTDETAILS_H
#define GUI_RESULTDETAILS_H

#include <pangomm/attributes.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include "Gui_ResultDisplayBaseClass.hpp"
#include "text.hpp"


typedef Gtk::TreeModel::Children    type_children;

class Gui_ResultDetails : public Gui_ResultDisplayBaseClass{
    public:
        Gui_ResultDetails(  sigc::signal<void,Glib::RefPtr<Gio::File>,int>& signal_update_result_map,
                            sigc::signal<void,int>& signal_select_object,
                            sigc::signal<void,int,bool>& signal_toggle_object,
                            ResultMap& result_map);

        void set_selection(int i){
            if(i >=0){
                type_children::iterator chil = findWichRow(m_ref_tree_model->children(),i);
                m_ref_tree_selection->select(chil);
                m_tree_view.scroll_to_row(Gtk::TreePath(chil));

            }
            else
                on_unselect();

       }

    protected:
        void on_cell_toggled(const Glib::ustring& path){
            Gtk::TreeModel::iterator iter = m_ref_tree_model->get_iter (path);
            if (iter)
                (*iter)[m_col_model.m_valid] = !(*iter)[m_col_model.m_valid];

            m_signal_toggle_object.emit((*iter)[m_col_model.m_id],(*iter)[m_col_model.m_valid]);

            DEV_INFOS("cell toggled");
            }
        void updateView(Glib::RefPtr<Gio::File> file, int idx);

        type_children::iterator findWichRow(type_children children,int idx);

        void on_selection_changed(){
            Gtk::TreeModel::iterator iter = m_ref_tree_selection->get_selected();
            if(iter){
                  Gtk::TreeModel::Row row = *iter;
                  m_signal_select_object.emit(row[m_col_model.m_id]);
                }
        }

        void on_unselect(){
            m_ref_tree_selection->unselect_all();
            m_signal_select_object.emit(-1);
        }

        class ModelColumns : public Gtk::TreeModel::ColumnRecord{
            public:
                ModelColumns()
                {
                add(m_id);
                add(m_valid);
                add(m_x);
                add(m_y);
                add(m_roi);
                add(m_area);
                add(m_radius);
                add(m_R);
                add(m_G);
                add(m_B);
                add(m_Hue);
                add(m_Sat);
                add(m_NinClust);
                add(m_cluster_id); //NJL 13/AUG/2014
                }

                Gtk::TreeModelColumn<int> m_id;
                Gtk::TreeModelColumn<bool> m_valid;
                Gtk::TreeModelColumn<int> m_x;
                Gtk::TreeModelColumn<int> m_y;
                Gtk::TreeModelColumn<int> m_roi;
                Gtk::TreeModelColumn<int> m_area;
                Gtk::TreeModelColumn<int> m_radius;
                Gtk::TreeModelColumn<int> m_R;
                Gtk::TreeModelColumn<int> m_G;
                Gtk::TreeModelColumn<int> m_B;
                Gtk::TreeModelColumn<int> m_Hue;
                Gtk::TreeModelColumn<int> m_Sat;
                Gtk::TreeModelColumn<int> m_NinClust;
                Gtk::TreeModelColumn<std::string> m_cluster_id; //NJL 13/AUG/2014

        };

       ModelColumns m_col_model;
       void setOneRow(Gtk::TreeModel::Row& row, const OneObjectRow& oor, const unsigned int i);


    private:
        Glib::RefPtr<Gtk::ListStore> m_ref_tree_model;
        sigc::signal<void,int>& m_signal_select_object;
        sigc::signal<void,int,bool>& m_signal_toggle_object;
        Glib::RefPtr<Gio::File> m_old_file;
        Gtk::Button m_unselect;
};
#endif // GUI_RESULTDETAILS_H
