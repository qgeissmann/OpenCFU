#include "Gui_ControlPanel.hpp"

Gui_ControlPanel::Gui_ControlPanel(Gui_ProcessorHandler& processor_hand,ResultMap& result_map, Gui_ConfigIO& config):
    m_processor_hand(processor_hand),
    m_result_label(m_processor_hand,result_map),
    m_file_setter(processor_hand,NAME_IMAGE_FILE_SETTER, config),
    m_thr_setter(processor_hand,NAME_THR_SETTER),
    m_rad_setter(processor_hand,NAME_RAD_SETTER),
    m_mask_setter(processor_hand,NAME_MASK_SETTER),
    m_col_select(processor_hand,NAME_COL_SELECT),
    m_lik_select(processor_hand,NAME_LIK_SELECT),
    m_col_cluster(processor_hand,NAME_COL_CLUSTER), //NJL 10/AUG/2014
    m_about_opencfu_button("About OpenCFU")


{
        this->set_border_width(5);

        this->add(m_vbox);
        m_vbox.pack_start(m_result_label,false,false);
        m_vbox.pack_start(m_file_setter,false,false);
        m_vbox.pack_start(m_thr_setter,false,false);
        m_vbox.pack_start(m_rad_setter,false,false);
        m_vbox.pack_start(m_mask_setter,false,false);
        m_vbox.pack_start(m_col_select,false,false);
        m_vbox.pack_start(m_lik_select,false,false);
        m_vbox.pack_start(m_col_cluster,false,false); //NJL 10/AUG/2014
        m_vbox.pack_end(m_about_opencfu_button,false,false);

        m_file_setter.signal_publish_N_files().connect( sigc::mem_fun(m_thr_setter,&Gui_ThresholdSetter::updateNFiles));
        m_file_setter.signal_publish_N_files().connect( sigc::mem_fun(m_rad_setter,&Gui_RadiusSetter::updateNFiles));
        m_file_setter.signal_publish_N_files().connect( sigc::mem_fun(m_mask_setter,&Gui_MaskSetter::updateNFiles));
        m_file_setter.signal_publish_N_files().connect( sigc::mem_fun(m_result_label,&Gui_ResultLabel::updateNFiles));
        m_file_setter.signal_publish_N_files().connect( sigc::mem_fun(m_col_select,&Gui_ColourSelector::updateNFiles));
        m_file_setter.signal_publish_N_files().connect( sigc::mem_fun(m_lik_select,&Gui_LikFiltSelector::updateNFiles));
        m_file_setter.signal_publish_N_files().connect( sigc::mem_fun(m_col_cluster,&Gui_ColourCluster::updateNFiles)); //NJL 10/AUG/2014
        m_about_opencfu_button.signal_clicked().connect( sigc::mem_fun(*this,&Gui_ControlPanel::on_about_button));


}

void Gui_ControlPanel::on_about_button(){
    Gtk::Window* parent =  (Gtk::Window*) get_toplevel();
    MyAboutDialog about;
    about.set_transient_for(*parent);
    DEV_INFOS(parent);
    about.run();
}
