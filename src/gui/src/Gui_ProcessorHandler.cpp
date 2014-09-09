#include "Gui_ProcessorHandler.hpp"

Gui_ProcessorHandler::Gui_ProcessorHandler(Processor& processor,ProcessingOptions& opts):
    m_processor(processor),
    m_opts(opts),
    m_to_process(false),
    m_end_process(false),
    m_filter(new cv::Mat)
{
    m_thread = Glib::Thread::create(sigc::mem_fun(*this,&Gui_ProcessorHandler::runProcessOnThread),true);

    m_dispa_idle.connect( sigc::bind(sigc::mem_fun(m_signal_state,&sigc::signal<void, int>::emit),PROCESSOR_IDLE_SIGNAL));
    m_dispa_working.connect( sigc::bind(sigc::mem_fun(m_signal_state,&sigc::signal<void, int>::emit),PROCESSOR_WORKING_SIGNAL));
}


Gui_ProcessorHandler::~Gui_ProcessorHandler(){
    m_end_process = true; //request thread to stop
    m_thread->join(); //wait for thread to stop;
}
void Gui_ProcessorHandler::onRunProcess(){
    DEV_INFOS("Asking processor to re run");
    m_to_process = true;
}

void Gui_ProcessorHandler::runProcessOnThread(){
    while(!m_end_process){
        Glib::Timer timer;
        if(m_to_process){

            {
            Glib::Mutex::Lock lock (m_mutex);
            DEV_INFOS("DBG SETTING PROC OPTIONS filter size :"<<m_opts.getGUIFilter().rows);
            m_processor.setOptions(m_opts);
            }


            m_dispa_working.emit();

            m_to_process = false;
            m_processor.runAll();
            const Result& result(m_processor.getNumResult());
            {
            Glib::Mutex::Lock lock (m_mutex_result);
            m_result = result;
            }


            int us_to_sleep = std::max<int>(10e3 - int(timer.elapsed()*1e6),0);
            Glib::usleep(us_to_sleep);

            if(!m_to_process){
               m_dispa_idle.emit();
            }

            tryResetFilter();
        }

        int us_to_sleep = std::max<int>(10e3 - int(timer.elapsed()*1e6),0);
        Glib::usleep(us_to_sleep);


    }

}


void Gui_ProcessorHandler::tryResetFilter(){
    if(!m_result.getSameObjects() || m_filter->empty())
        cv::Mat(m_result.size(),1,CV_8S,cv::Scalar(-1)).copyTo(*m_filter);

}
