#include "Gui_ResultDetails.hpp"
#include <sstream>

Gui_ResultDetails::Gui_ResultDetails(sigc::signal<void,Glib::RefPtr<Gio::File>,int>& signal_update_result_map,
                                      sigc::signal<void,int>& signal_select_object,
                                      sigc::signal<void,int,bool>& signal_toggle_object,
                                      ResultMap& result_map):
    Gui_ResultDisplayBaseClass(signal_update_result_map, result_map),
     m_signal_select_object(signal_select_object),
     m_signal_toggle_object(signal_toggle_object),
     m_unselect("Unselect")
   {

    DEV_INFOS("RESUlt detail");
    set_tooltip_text(TOOLTIP_RESULT_LIST_DET);
    m_label.set_markup(RESULT_DETAIL_TITLE);

    Gtk::TreeView::Column* tmp_col;

    int c=0;
    m_tree_view.append_column("ID", m_col_model.m_id);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_id);


    ++c;
    Gtk::CellRendererToggle* renderer_active = Gtk::manage( new Gtk::CellRendererToggle());
    renderer_active->signal_toggled().connect( sigc::mem_fun(*this, & Gui_ResultDetails::on_cell_toggled));
    Gtk::TreeView::Column* column = Gtk::manage( new Gtk::TreeView::Column("Valid"));
    column->pack_start(*renderer_active, false);
    column->add_attribute(renderer_active->property_active (), m_col_model.m_valid);
    m_tree_view.append_column(*column);
    column->set_sort_column(m_col_model.m_valid);

    m_tree_view.append_column("X", m_col_model.m_x);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_x);

    m_tree_view.append_column("Y", m_col_model.m_y);
        tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_y);

    m_tree_view.append_column("ROI", m_col_model.m_roi);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_roi);



    m_tree_view.append_column("Area", m_col_model.m_area);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_area);


    m_tree_view.append_column("Radius", m_col_model.m_radius);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_radius);


    m_tree_view.append_column("R", m_col_model.m_R);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_R);

    m_tree_view.append_column("G", m_col_model.m_G);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_G);

    m_tree_view.append_column("B", m_col_model.m_B);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_B);


    m_tree_view.append_column("Hue", m_col_model.m_Hue);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_Hue);


    m_tree_view.append_column("Sat", m_col_model.m_Sat);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_Sat);


    m_tree_view.append_column("Neighbours", m_col_model.m_NinClust);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_NinClust);

    //NJL 01/SEP/2014
    m_tree_view.append_column("Cluster ID", m_col_model.m_cluster_id);
    tmp_col = m_tree_view.get_column(c++);
    if(tmp_col)
      tmp_col->set_sort_column(m_col_model.m_cluster_id);


    Gtk::CellRenderer *cell = m_tree_view.get_column_cell_renderer(0);
    Gtk::CellRendererText* cell_text = (Gtk::CellRendererText*) cell;



    Glib::PropertyProxy< Pango::AttrList > pango_attrs = cell_text->property_attributes();
    Pango::Attribute attr = Pango::Attribute::create_attr_weight(Pango::WEIGHT_BOLD );
    Pango::AttrList my_list;
    my_list.insert(attr);
    pango_attrs.set_value(my_list);


    m_ref_tree_model = Gtk::ListStore::create(m_col_model);
    m_tree_view.set_model(m_ref_tree_model);
//    m_scrolled_window.add(m_tree_view);

    m_hbox.pack_start(m_unselect,false,false);

    m_ref_tree_selection = m_tree_view.get_selection();
    m_ref_tree_selection->signal_changed().connect(sigc::mem_fun(*this, &Gui_ResultDetails::on_selection_changed));

    m_unselect.signal_clicked().connect(sigc::mem_fun(*this, &Gui_ResultDetails::on_unselect));

    DEV_INFOS("RESUlt detail OK");
   }


void Gui_ResultDetails::updateView(Glib::RefPtr<Gio::File> file, int idx){
    DEV_INFOS("Updating view");
    type_children children = m_ref_tree_model->children();
    const Result& res(m_result_map.getResultAt(idx));
    const bool NA (m_result_map.getIsNAAt(idx));
    if(NA)
        m_scrolled_window.set_sensitive(false);
    else
        m_scrolled_window.set_sensitive(true);

    unsigned int N = (int) res.size();

    if( N == children.size() && file->equal(m_old_file)){
        int i = 0;
        int sort_col = -1;
        Gtk::SortType order;
        m_ref_tree_model->get_sort_column_id(sort_col, order);
        m_ref_tree_model->set_sort_column(0,Gtk::SortType::SORT_ASCENDING );

        for(type_children::iterator iter = children.begin(); iter != children.end(); ++iter,++i){
            const OneObjectRow& oor = res.getRow(i);
            Gtk::TreeModel::Row row = *iter;

            setOneRow(row,oor,i);
        }

        m_ref_tree_model->set_sort_column(sort_col,order);

    }
    else{
        m_signal_select_object.emit(-1);
        m_ref_tree_model->clear();

        for(unsigned int i = 0; i != N; ++i){
            const OneObjectRow& oor = res.getRow(i);
            Gtk::TreeModel::Row row = *(m_ref_tree_model->append());
            setOneRow(row,oor,i);
        }
    }
    m_old_file = file;
}

void Gui_ResultDetails::setOneRow(Gtk::TreeModel::Row& row, const OneObjectRow& oor, const unsigned int i){

    row[m_col_model.m_id] = i;
    row[m_col_model.m_valid] = oor.isValid();
    cv::Point2f midpoint((oor.getPoint(0) + oor.getPoint(2)) * 0.5);
    row[m_col_model.m_x] = midpoint.x;
    row[m_col_model.m_y] = midpoint.y;
    row[m_col_model.m_roi] = oor.getROI();
    row[m_col_model.m_area] = oor.getArea();
    row[m_col_model.m_radius] = oor.getRadius();
    cv::Scalar colour = oor.getBGRMean();
    row[m_col_model.m_R] = colour[2];
    row[m_col_model.m_G] = colour[1];
    row[m_col_model.m_B] = colour[0];
    row[m_col_model.m_Hue] = oor.getHue();
    row[m_col_model.m_Sat] = oor.getSat();
    row[m_col_model.m_NinClust] = oor.getNInClust();
    std::stringstream ss;
    if (oor.getColorClusterID() == 0)
        ss<<"NA";
    else
        ss<<oor.getColorClusterID();
    row[m_col_model.m_cluster_id] = ss.str(); //NJL 13/AUG/2014
}


type_children::iterator Gui_ResultDetails::findWichRow(type_children children,int idx){
    for(type_children::iterator iter = children.begin(); iter != children.end(); ++iter){
        Gtk::TreeModel::Row tmp_row = *iter;
        if (tmp_row[m_col_model.m_id] == idx ){
                return iter;
            }
    }
    return children.end();
}
