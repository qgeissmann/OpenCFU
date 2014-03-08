#include "ResultMap.hpp"
#include <time.h>

int ResultMap::append(const  Glib::RefPtr<Gio::File> file, const Result& res){

    std::unordered_map <Glib::RefPtr<Gio::File>,int,functorHash,functorEqual>::iterator ite = m_idx_from_file.find(file);

    int where = 0;

    if(ite != m_idx_from_file.end()){
        DEV_INFOS("Found");
        bool wasNA (std::get<1>(m_results[ite->second]));
        Glib::ustring old_comment (std::get<2>(m_results[ite->second]));
        m_results[ite->second] = std::make_tuple(res,wasNA,old_comment);
        where = ite->second;
    }

    else{
        DEV_INFOS(" N ot Found");
        time_t now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d [%X]: no comment", &tstruct);

        Glib::ustring new_coment(buf);

        m_results.push_back(std::make_tuple(res,false,new_coment));
        where = m_results.size()-1;
    }

    m_idx_from_file[file] = where;
    m_reverse_map[where] = file;

    return where;
}
