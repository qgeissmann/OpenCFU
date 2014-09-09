#include "Gui_ResultFileWriter.hpp"


Gui_ResultFileWriter::Gui_ResultFileWriter(Gtk::Widget* parent):
    m_parent_widg(parent),m_up_to_date(true)
{

}



void Gui_ResultFileWriter::writeHeader(const bool detail,const ResultMap& res_map, const std::vector<int>& idxs){

    Glib::RefPtr< Gio::FileOutputStream > fout = m_out_file->replace();
    std::stringstream ss;
    if(detail){
        for (auto i : idxs){
            if(res_map.getResultAt(i).size() > 0){
                const OneObjectRow& oor = res_map.getResultAt(i).getRow(0);
                ss << "Full_Path, "<<oor.printHeader()<<", Comment"<<std::endl;
            }
        }

    }
    else{
        ss<<"ID, ROI, File_name, N_Objects, N_Excluded, Comment, Full_Path"<<std::endl;
    }
    fout->write(ss.str());
    fout->close();
}

void Gui_ResultFileWriter::writeRows(const bool detail,const ResultMap& res_map,const std::vector<int>& idxs){
    Glib::RefPtr< Gio::FileOutputStream > fout = m_out_file->append_to();

    if(detail){
        for (auto i : idxs){
            const Result& res_ref = res_map.getResultAt(i);

            const std::string& path = (res_map.getFileFromIdx(i)->get_path());
            const std::string& comment = res_map.getCommentAt(i);

            if(!res_map.getIsNAAt(i)){
                for (unsigned int j=0; j<res_ref.size();++j){
                    std::stringstream ss;
                    const OneObjectRow& oor = res_ref.getRow(j);
                    ss <<"\""<<path<<"\", "<< oor.print()<<", \""<<comment<<"\""<<std::endl;
                    fout->write(ss.str());
                }
            }
        }
    }
    else{
        for (auto i : idxs){
            const Result& res_ref = res_map.getResultAt(i);
            Glib::RefPtr<Gio::File> tmp_file = res_map.getFileFromIdx(i);

            //results per roi table:
            std::map < uint,std::pair<uint,uint> > table;
            for(uint i=0; i != (uint)res_ref.size(); ++i){
                OneObjectRow object = res_ref.getRow(i);
                int roi = object.getROI();
                if (roi > 0){
                    if(object.getGUIValid())
                        ++(table[roi].first);
                    else
                        ++(table[roi].second);
                }
            }

            const std::string& comment = res_map.getCommentAt(i);

            std::stringstream ss;
            for(auto& t : table){
                ss  <<i<<", "<<t.first<<", \""<<tmp_file->get_basename()<<"\",";
                auto val_unval = t.second;
                if(!res_map.getIsNAAt(i))
                    ss<<val_unval.first<<","<<val_unval.second<<",";
                else
                    ss<<"NA,NA,";

                ss<<comment<<", \""<<tmp_file->get_path()<<"\""<<std::endl;

            fout->write(ss.str());}
        }
    }
    fout->flush ();
    fout->close();

}
bool Gui_ResultFileWriter::saveSelection(const ResultMap& res_map, const std::vector<int>& idxs){

    if(idxs.empty()){
        NoSelectionMessage msg;
        msg.set_transient_for(*((Gtk::Window*) m_parent_widg->get_toplevel()));
        msg.run();
        return false;
    }
    Glib::ustring file_uri;

    bool detailed_result(false);

    while(file_uri.empty()){
        int result;
        {
        SaveModeMessage save_mode;
        save_mode.set_transient_for(*((Gtk::Window*) m_parent_widg->get_toplevel()));
        result = save_mode.run();
        }
        switch(result){
            case Gtk::RESPONSE_ACCEPT: //summary
                detailed_result = false;
                break;
            case Gtk::RESPONSE_OK: //Detail
                detailed_result = true;
                break;

            default:
                return false;
                break;
        }
        std::string dir_uri;

        if(m_out_file)
            if (m_out_file->query_exists())
                dir_uri = m_out_file->get_uri();


        SaveFileChooser file_chooser(dir_uri);
        file_chooser.set_transient_for(*((Gtk::Window*) m_parent_widg->get_toplevel()));
        result = file_chooser.run();
        if(result == Gtk::RESPONSE_OK)
            file_uri = file_chooser.get_uri();
    }

    m_out_file = Gio::File::create_for_uri(file_uri);

    writeHeader(detailed_result,res_map,idxs);
    writeRows(detailed_result,res_map,idxs);
    m_up_to_date = true;
    return true;
}
