#ifndef GUI_COLOURCLUSTER_HPP
#define GUI_COLOURCLUSTER_HPP

#include "Gui_OptionSetterBaseClass.hpp"
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include "defines.hpp"
#include "text.hpp"


/**
Nathanael Lampe
09/08/2014

Based on Gui_ColourSelector
*/


class Gui_ColourCluster : public Gui_OptionSetterBaseClass
{
    public:
        Gui_ColourCluster(Gui_ProcessorHandler& processor_hand,const std::string str);
        void updateNFiles(int i);

    protected:
        void on_ClusteringDistanceChanged(){
//                int val_clustering_distance = m_spin_butt_clustering_distance.get_value();
                this->setOption();
        }
        bool setOption(){
            return Gui_OptionSetterBaseClass::setOption();
        }
        bool updateOptions();

        void on_realize(){
            this->on_tick_box();
            Gui_OptionSetterBaseClass::on_realize();}

        void on_tick_box(){ m_check_butt.get_active() ?  m_hbox.show() : m_hbox.hide(); }

        void on_activate_filter(){
            this->on_tick_box();
            this->setOption();}
    private:
        Gtk::HBox m_hbox;
        Gtk::HBox m_hbox_1;
        Gtk::HBox m_hbox_2;
//        Gtk::VBox m_vbox; //declared in parent
        Gtk::Adjustment m_adjust_clustering_distance;
        Gtk::SpinButton m_spin_butt_clustering_distance;
        Gtk::Label m_lab_clustering_distance;

        Gtk::Adjustment m_adjust_min_cluster_points;
        Gtk::SpinButton m_spin_butt_min_cluster_points;
        Gtk::Label m_lab_min_cluster_points;

        Gtk::CheckButton m_check_butt;


};

#endif // GUI_COLOURCLUSTER_HPP
