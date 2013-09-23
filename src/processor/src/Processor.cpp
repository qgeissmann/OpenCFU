#include "Processor.hpp"
#include "Step_1.hpp"
#include "Step_2.hpp"
#include "Step_3.hpp"
#include "Step_4.hpp"
#include "Step_FiltGUI.hpp"
#include "Step_FiltHS.hpp"
#include "Step_FiltIPosition2D.hpp"
#include "Step_FiltLik.hpp"
#include <sstream>

Processor::Processor(ProcessingOptions& opts):
    m_opts(opts),
    m_is_busy(false)

{


    cv::FileStorage fs;
    std::string path = std::string("./")+std::string(TRAINED_CLASSIF_XML_FILE);
    fs.open(path, cv::FileStorage::READ);
    DEV_INFOS("Trying to open any local trained classifier: "<<path);
    if (!fs.isOpened()){

        #if defined _WIN64 || defined _WIN32
        path = std::string("../")+std::string(TRAINED_CLASSIF_XML_FILE);
        #else
        path = std::string(INSTALLDIR)+std::string("/")+std::string(TRAINED_CLASSIF_XML_FILE);
        #endif
        DEV_INFOS("failed. Falling back on installation path: "<<path);
        fs.open(path, cv::FileStorage::READ);
        if(!fs.isOpened()){
            DEV_INFOS("NO TRAINED CLASSIFIER!! CHECK INSTALLATION"<<path);
            assert(!fs.isOpened());
        }
    }


    m_predictor.loadTrainData(path);

    std::string path_ps = std::string("./")+std::string(TRAINED_CLASSIF_PS_XML_FILE);
    fs.open(path_ps, cv::FileStorage::READ);
    DEV_INFOS("Trying to open any local trained classifier: "<<path_ps);
    if (!fs.isOpened()){

        #if defined _WIN64 || defined _WIN32
        path_ps = std::string("../")+std::string(TRAINED_CLASSIF_PS_XML_FILE);
        #else
        path_ps = std::string(INSTALLDIR)+std::string("/")+std::string(TRAINED_CLASSIF_PS_XML_FILE);
        #endif
        DEV_INFOS("failed. Falling back on installation path: "<<path_ps);
        fs.open(path_ps, cv::FileStorage::READ);
        if(!fs.isOpened()){
            DEV_INFOS("NO TRAINED CLASSIFIER!! CHECK INSTALLATION"<<path_ps);
            assert(!fs.isOpened());
        }
    }


    m_predictor_ps.loadTrainData(path_ps);

    m_processing_steps.push_back(new Step_1(opts));
    m_processing_steps.push_back(new Step_2(opts));
    m_processing_steps.push_back(new Step_3(opts,m_predictor));
    m_processing_steps.push_back(new Step_4(opts,m_predictor,m_predictor_ps,m_raw_img));
    m_processing_steps.push_back(new Step_FiltGUI(opts));
    m_processing_steps.push_back(new Step_FiltIPosition2D(opts));
    m_processing_steps.push_back(new Step_FiltHS(opts));
    m_processing_steps.push_back(new Step_FiltLik(opts));
    DEV_INFOS("Processor constructed");
}

void Processor::runAll(){
    DEV_INFOS("Running processor...");

    assert(!m_opts.getImage().empty());
    void* tmp_step_results = (void*) &m_opts.getImage();
    m_raw_img = m_opts.getImage();
    bool pipeBrocken = false;

    Result result;
    for(auto& step : m_processing_steps ){
        pipeBrocken = step->run(tmp_step_results,pipeBrocken);
    }

    m_result = (Result*) tmp_step_results;
    DEV_INFOS("Processor run finished.");
    DEV_INFOS("N = "<<m_result->getNValid());
}

Processor::~Processor()
{
    DEV_INFOS("Destructing Processor...");
    for(auto &it : m_processing_steps) {
        delete it;
    }
    DEV_INFOS("Processor destroyed");
}

void Processor::writeResult(){

    std::cout   << "IsValid"<<","
                << "X"<<","
                << "Y"<<","
                << "ROI"<<","
                << "N_in_clust"<<","
                << "Area"<<","
                << "Radius"<<","
                << "Hue" <<","
                << "Saturation" <<","
                << "Rmean" <<","
                << "Gmean" <<","
                << "Bmean" <<","
                << "Rsd" <<","
                << "Gsd" <<","
                << "Bsd" << std::endl;



    for(unsigned int i = 0; i != m_result->size();i++){
        const OneObjectRow& oor = m_result->getRow(i);
        cv::Point2f center = (oor.getPoint(0) + oor.getPoint(2) ) * 0.5;
        std::cout   << oor.isValid()<<","
                    << center.x<<","
                    << center.y<<","
                    << oor.getROI()<<","
                    << oor.getNInClust()<<","
                    << oor.getArea()<<","
                    << oor.getRadius()<<","
                    << oor.getHue()<<","
                    << oor.getSat()<<","
                    << oor.getBGRMean()[2]<<","
                    << oor.getBGRMean()[1]<<","
                    << oor.getBGRMean()[0]<<","
                    << oor.getBGRSd()[2]<<","
                    << oor.getBGRSd()[1]<<","
                    << oor.getBGRSd()[0]<<std::endl;
    }
}

