#include "Gui_ColourCluster.hpp"

/**
Class to implement Colour Clustering GUI interface
Written 10/AUG/2014
*/
Gui_ColourCluster::Gui_ColourCluster(Gui_ProcessorHandler& processor_hand,const std::string str):
    Gui_OptionSetterBaseClass(processor_hand,str),
    m_adjust_clustering_distance(m_processor_hand.getOptions().getClustDist(),0.1,50.0,0.1,1.0,0.0),
    m_spin_butt_clustering_distance(m_adjust_clustering_distance, 0.0, 1),
    m_check_butt(LABEL_CHECKBUTTON_HAS_CLUSTERING_DISTANCE),
    m_lab_clustering_distance(LABEL_CLUSTERING)

{
    set_tooltip_text(TOOLTIP_CLUSTERING);
    m_hbox.set_tooltip_text(TOOLTIP_CLUSTERING_DISTANCE_SELECTOR);

    //GUI Layout
    m_vbox.pack_start(m_check_butt);
    m_vbox.pack_start(m_hbox);
    m_hbox.pack_start(m_lab_clustering_distance);
    m_hbox.pack_start(m_spin_butt_clustering_distance);

    //Signal interaction
    m_spin_butt_clustering_distance.signal_value_changed().connect( sigc::hide_return(sigc::mem_fun(*this,&Gui_ColourCluster::setOption)));
    m_check_butt.signal_toggled().connect(sigc::mem_fun(*this,&Gui_ColourCluster::on_activate_filter));
}



bool Gui_ColourCluster::updateOptions(){
    DEV_INFOS("Trying to send a processing option");
    float val_clust_dist = m_spin_butt_clustering_distance.get_value();

    bool has_clust_dist = m_check_butt.get_active();
    m_opts.setHasClustDist(has_clust_dist);

    if(has_clust_dist){
        bool success = m_opts.setClustDist(val_clust_dist);
        return success;
    }
    else
        return true;
}


void Gui_ColourCluster::updateNFiles(int i){
    if(i == 0)
        this->hide_all();
    else
        this->show_all();
    on_tick_box();
}
