#ifndef RESULTMAP_H
#define RESULTMAP_H


#include <giomm/file.h>
#include <unordered_map>
#include "Result.hpp"


class functorHash{
    public:
      int operator() (const Glib::RefPtr<Gio::File> file) const{
            return file->hash();
          }
};

class functorEqual{
    public:
        bool operator() (const Glib::RefPtr<Gio::File> file1,const Glib::RefPtr<Gio::File> file2) const{
            return file1->equal(file2);
        }
};

class ResultMap{
    public:
        ResultMap(){};

        int append(const  Glib::RefPtr<Gio::File> file, const Result& res);
        int setFileToNA(const  Glib::RefPtr<Gio::File> file,bool NA){
            std::unordered_map <Glib::RefPtr<Gio::File>,int,functorHash,functorEqual>::iterator ite
             = m_idx_from_file.find(file);
            if(ite != m_idx_from_file.end()){
               int where = ite->second;
               std::get<1>(m_results[where]) = NA;
               return where;
            }
            else{
                DEV_INFOS("WARNING: this file is not in the result MAP");
                return 0;
            }

        }
        bool getIsFileNA(const  Glib::RefPtr<Gio::File> file){
            std::unordered_map <Glib::RefPtr<Gio::File>,int,functorHash,functorEqual>::iterator ite
             = m_idx_from_file.find(file);
            if(ite != m_idx_from_file.end())
               return std::get<1>(m_results[ite->second]);
            else
                return false;

        }
        bool getIsNAAt(const int idx) const{
            return std::get<1>(m_results[idx]);
        }

        void setCommentAt(int idx, const Glib::ustring& comment){
            std::get<2>(m_results[idx]) = comment;
        }

       const Glib::ustring& getCommentAt(const int idx) const{
            return std::get<2>(m_results[idx]);
        }

        const Result& getResultAt(const int idx) const{
            return std::get<0>(m_results[idx]);
        }

        const Glib::RefPtr<Gio::File> getFileFromIdx(const int idx)const {
            Glib::RefPtr<Gio::File> f = m_reverse_map.find(idx)->second;
            return f;
        }
        void deleteAt(const int idx){
            Glib::RefPtr<Gio::File> f = m_reverse_map.find(idx)->second;
            m_idx_from_file.erase(f);
            m_results.erase(m_results.begin() + idx);
            m_reverse_map.erase(idx);

        }

    private:
        std::vector <std::tuple<Result,bool,Glib::ustring> > m_results;
        std::unordered_map < Glib::RefPtr<Gio::File>,int,functorHash,functorEqual> m_idx_from_file;
        std::unordered_map < int,Glib::RefPtr<Gio::File> > m_reverse_map;
};


#endif // RESULTMAP_H
