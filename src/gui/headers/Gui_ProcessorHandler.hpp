#ifndef GUI_PROCESSORHANDLER_H
#define GUI_PROCESSORHANDLER_H

#include "defines.hpp"

#include <glibmm/dispatcher.h>
#include <glibmm/timer.h>
#include <atomic>
#include <memory>
#include <sigc++/sigc++.h>
#include "Result.hpp"
#include "Processor.hpp"
#include "defines.hpp"

#include <glibmm/thread.h>

class Gui_ProcessorHandler
{
    public:
        Gui_ProcessorHandler(Processor& processor,ProcessingOptions& opts);
        ~Gui_ProcessorHandler();


        void onRunProcess();
        Glib::Mutex& getMutex(){return m_mutex;}

        sigc::signal<void, int> signal_state(){ return m_signal_state;}

    std::shared_ptr<cv::Mat>& getGUIFilter(){return m_filter;}

    bool getSourceImgAsRGB(cv::Mat& out){
        DEV_INFOS("getting RGB");
        Glib::Mutex::Lock lock (m_mutex);
        if (m_opts.getImage().empty()){
            DEV_INFOS("getting RGB : empty");
            return false;
        }
        else{
            DEV_INFOS("getting RGB : OK");
            cv::cvtColor(m_opts.getImage(),out,cv::COLOR_BGR2RGB);
            return true;
        }
    }

    bool getMaskAsRGB(cv::Mat& out){
        Glib::Mutex::Lock lock (m_mutex);
        cv::Mat mat = m_opts.getMask().getMat();

        if (mat.empty()){
            return false;
        }
        else{
            cv::cvtColor(mat,out,cv::COLOR_GRAY2RGB);
            return true;
        }
    }

    const Result& getNumResult() {

            Glib::Mutex::Lock lock (m_mutex_result);
            return m_result;
        }

        ProcessingOptions& getOptions(){
            return m_opts;
        }

    protected:
        void runProcessOnThread();
        void tryResetFilter();
    private:
        Glib::Mutex m_mutex;
        Glib::Mutex m_mutex_result;

        Processor& m_processor;
        Glib::Thread* m_thread;
        Glib::Dispatcher m_dispa_working;
        Glib::Dispatcher m_dispa_idle;
        ProcessingOptions& m_opts;
        Result m_result;

        sigc::signal<void, int> m_signal_state;

        std::vector<std::vector<float> > m_processing_result;
        std::atomic<bool> m_to_process;
        std::atomic<bool> m_end_process;

        std::shared_ptr<cv::Mat> m_filter;
};
#endif // GUI_PROCESSORHANDLER_H

