/*
    Copyright Quentin Geissmann 2012-2013

    This file is part of OpenCFU

    OpenCFU is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenCFU is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenCFU.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DataMaker.hpp"
#include "Predictor.hpp"
#include "defines.hpp"

int main(int argc, char **argv){
    Predictor my_predictor;
    std::vector<signed char> categs;
    cv::Mat features;

    assert(argc == 3 || argc == 4 );

    /*If the program is called to TRAIN the model*/
    if(argv[1][0] == 't'){
        std::cout<<"\n~~~~~~~~~~~~~~TRAINING PREDICTOR~~~~~~~~~~~~~\n "<<std::endl;

        DataMaker dm(TRAINING_SET_IMG,TRAINING_SET_IMG_PS);

        if(argc == 3)
            dm.makeData(features,categs);
        else
            dm.makeDataPS(features,categs);



        my_predictor.train(features,categs);
        return 2;
        my_predictor.save(argv[2]);

        std::cout<<"\n~~~~~~~~~~~~~~PREDICTOR TRAINED~~~~~~~~~~~~~\n "<<std::endl;

    }
    /*Else, if the program is called to validate the training*/
    else{
        DataMaker dm(TRAINING_SET_IMG,TRAINING_SET_IMG_PS);
        if(argc == 3)
            dm.makeData(features,categs);
        else
            dm.makeDataPS(features,categs);

        my_predictor.loadTrainData(argv[2]);

        std::vector<signed char> pred;
        my_predictor.predict(features,pred);
        int n_false(0);
        cv::Mat m_mat(3,3,CV_32F,cv::Scalar(0));
        std::map<char,int> lut;
        lut['N'] = 0;
        lut['S'] = 1;
        lut['M'] = 2;
        char revLut[3] =  {'N','S','M'};
        std::vector<int> counts(3,0);

        for(unsigned int i = 0; i<pred.size(); i++){
            char p =pred[i];
            char r =categs[i];
            int real = lut[r];
            int pred = lut[p];
            counts[real] +=1;

            if(real != pred){
                n_false++;
            }
            m_mat.at<float>(pred,real) = m_mat.at<float>(pred,real) + 1;
        }
        cv::Mat sum_cols;
        cv::reduce(m_mat, sum_cols, 0, CV_REDUCE_SUM);
        cv::repeat(sum_cols, 3, 1, sum_cols);
        m_mat = 100*m_mat/sum_cols;
        float accur = 1 - ((float) n_false/(float) pred.size());
        std::cout<<"\n~~~~~~~~~~~~~~ASSESSING ACCURACY OF THE PREDICTOR~~~~~~~~~~~~~\n "<<std::endl;
        std::cout<<"ACCURACY = "<<accur<<std::endl;
        std::cout<<"\nTRAINNING CLASS BALANCE:"<<std::endl;

        std::cout.setf(std::ios::fixed);
        for(unsigned int i = 0; i < counts.size();i++)
            std::cout<<"N(class =="<<revLut[i]<<") "<<counts[i]<<std::endl;

        //CV2
        //std::cout<<"\nConfusion Matrix = \n REAL(top) -> PRED(left) \n"<<cv::format(m_mat,"CSV")<<"\n"<<std::endl;
        //CV3
        std::cout<<"\nConfusion Matrix = \n REAL(top) -> PRED(left) \n"<<cv::format(m_mat,cv::Formatter::FMT_CSV)<<"\n"<<std::endl;
    }
    return 0;
}
