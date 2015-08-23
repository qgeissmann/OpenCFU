#include "Gui_Decorator.hpp"

Gui_Decorator::Gui_Decorator(Gtk::DrawingArea* parent, Gui_ProcessorHandler& processor_hand,
                              int& idx_point_to_show, bool& show_point,Gui_DisplayRoi& ROI):
    m_parent(parent),
    m_processor_hand(processor_hand),
    m_idx_point_to_show(idx_point_to_show),
    m_show_point(show_point),
    m_ROI(ROI),
    m_line_width_multip(1),m_vector_crosses(NULL)
{
}

//Gui_Decorator::Gui_Decorator(Gtk::DrawingArea* parent, Gui_ProcessorHandler& processor_hand,Gui_DisplayRoi& ROI):
//    m_parent(parent),
//    m_processor_hand(processor_hand),
//    m_idx_point_to_show(-1),
//    m_show_point(false),
//    m_ROI(ROI),
//    m_line_width_multip(1)
//{
//}


void Gui_Decorator::drawCrosses(Cairo::RefPtr<Cairo::Context> cr){
    if(m_vector_crosses == NULL){
        DEV_INFOS("No crosses to draw");
        return;
    }
    if(!m_vector_crosses->empty()){


        float cte = std::min(m_ROI.getPixbufDim().x,m_ROI.getPixbufDim().y) / 30;
        cv::Point2f A,B,C,D;




        for(auto i : *m_vector_crosses ){
            DEV_INFOS(i);
            A = i - cv::Point2f(cte,cte);
            B = i - cv::Point2f(cte,-cte);
            C = i - cv::Point2f(-cte,-cte);
            D = i - cv::Point2f(-cte,cte);

            cr->move_to (A.x,A.y);
            cr->line_to (C.x,C.y);
            cr->move_to (B.x,B.y);
            cr->line_to (D.x,D.y);

        }
    cr->set_source_rgba(0, 0, 0,0.8);
    cr->set_line_width(cte/3);
    cr->stroke_preserve();
    cr->set_source_rgba(1.0, 1.0, 0.5,1);
    cr->set_line_width(cte/10);

    cr->stroke();
    }
}
void Gui_Decorator::decorate(){
    DEV_INFOS("Decorating");

    cv::Point2f a;
    cv::Point2f b;

    m_ROI.getABForDeco(a,b);


    const Result& result = m_processor_hand.getNumResult();

    Cairo::RefPtr<Cairo::Context> cr = m_parent->get_window()->create_cairo_context();
    cr->set_matrix(Cairo::Matrix(a.x,0,0,a.y,b.x,b.y));


    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->set_line_join(Cairo::LINE_JOIN_ROUND);



    cv::Point2f tp;


    std::vector<int> in_field_valid; //valid, but clusterID=0
    std::vector<int> in_field_invalid;
    std::vector<int> in_cluster_one;
    std::vector<int> in_cluster_two;
    std::vector<int> in_cluster_three;
    std::vector<int> in_cluster_four;
    for(unsigned int i = 0; i != result.size();i++){
        const OneObjectRow& oor = result.getRow(i);
        bool inside = false;
        for(unsigned int j = 0; j != 4;j++){
            if(m_ROI.contains(oor.getPoint(j)))
                inside = true;
        }
        //edited 13/AUG/2014 to create groups for clusters 1 and 2
        if(inside){
            if (oor.getColorClusterID() == 1){
                in_cluster_one.push_back(i);
            }
            else if (oor.getColorClusterID() == 2){
                in_cluster_two.push_back(i);
            }
            else if (oor.getColorClusterID() == 3){
                in_cluster_three.push_back(i);
            }
            else if (oor.getColorClusterID() == 4){
                in_cluster_four.push_back(i);
            }
            else {
                oor.isValid() ? in_field_valid.push_back(i) : in_field_invalid.push_back(i);
            }
        }
    }
/******************************************************************************
 *Drawing each region, migrated to a functional form 4/SEP/2014
 *****************************************************************************/
    //clusters if present
    //green
    highlightCells(cr, in_cluster_one,   0.0, 1.0, 0.0, 0.8, 3.0);
    highlightCells(cr, in_cluster_one,   0.0, 1.0, 0.0, 1.0, 1.5);

    //teal
    highlightCells(cr, in_cluster_two,   0.0, 1.0, 1.0, 0.8, 3.0);
    highlightCells(cr, in_cluster_two,   0.0, 1.0, 1.0, 1.0, 1.5);

    //purple
    highlightCells(cr, in_cluster_three, 1.0, 0.0, 1.0, 0.8, 3.0);
    highlightCells(cr, in_cluster_three, 1.0, 0.0, 1.0, 1.0, 1.5);

    //orange
    highlightCells(cr, in_cluster_four,  1.0, 0.5, 0.0, 0.8, 3.0);
    highlightCells(cr, in_cluster_four,  1.0, 0.5, 0.0, 1.0, 1.5);

    //valid cells not in  clusters
    highlightCells(cr, in_field_valid,   1.0, 1.0, 0.0, 0.8, 3.0);
    highlightCells(cr, in_field_valid,   0.0, 0.0, 1.0, 1.0, 1.5);


/******************************************************************************
 * Drawing of invalid cells, done outside of function to accomodate extra
 * lines
 *****************************************************************************/

    for(int i : in_field_invalid){
        const OneObjectRow& oor = result.getRow(i);
        if(oor.getGUIValid()  == -1){
            cr->move_to (oor.getPoint(0).x,oor.getPoint(0).y);
            cr->line_to ( oor.getPoint(1).x,oor.getPoint(1).y);
            cr->line_to ( oor.getPoint(2).x,oor.getPoint(2).y);
            cr->line_to ( oor.getPoint(3).x,oor.getPoint(3).y);
            cr->close_path();
            cr->line_to ( oor.getPoint(2).x,oor.getPoint(2).y);
        }
        else{
            cr->move_to (oor.getPoint(0).x,oor.getPoint(0).y);
            cr->line_to ( oor.getPoint(1).x,oor.getPoint(1).y);
            cr->line_to ( oor.getPoint(2).x,oor.getPoint(2).y);
            cr->line_to ( oor.getPoint(3).x,oor.getPoint(3).y);
            cr->close_path();
            cr->line_to ( oor.getPoint(2).x,oor.getPoint(2).y);
            cr->move_to (oor.getPoint(3).x,oor.getPoint(3).y);
            cr->line_to ( oor.getPoint(1).x,oor.getPoint(1).y);
        }

    }
    cr->set_line_width(5.0*m_line_width_multip);
    cr->set_source_rgba(1.0, 0.0, 0.0,0.8);
    cr->stroke_preserve();
    cr->set_line_width(2.5*m_line_width_multip);
    cr->set_source_rgba(0.0, 0.0, 0.0,1.0);
    cr->stroke();

    if(m_show_point){
        const OneObjectRow& oor = result.getRow(m_idx_point_to_show);

        cv::Point2f center = (oor.getPoint(0) + oor.getPoint(2) ) * 0.5;
        float rad = oor.getRadius() * 1.2;
        cr->arc(center.x, center.y, rad, 0.0, 2 * MY_PI);

        std::vector<cv::Point2f> relpos;
        relpos.push_back(cv::Point2f(-rad,0));
        relpos.push_back(cv::Point2f(0,+rad));
        relpos.push_back(cv::Point2f(+rad,0));
        relpos.push_back(cv::Point2f(0,-rad));

        for(auto i : relpos){
            cv::Point2f start = (i*1.5) + center;
            cv::Point2f endd = (i*0.5) + center;
            cr->move_to (start.x,start.y);
            cr->line_to (endd.x, endd.y );
        }


        cr->set_line_width(4.0*m_line_width_multip);
        cr->set_source_rgba(.0, .0, .0,0.7);
        cr->stroke_preserve();
        cr->set_line_width(2*m_line_width_multip);
        cr->set_source_rgba(1.0, 1.0, 0,1);

        cr->stroke();
        }
    DEV_INFOS("Decorating OK");
    drawCrosses(cr);
}

/**
 * Gui_Decorator::higlightCells
 * Routine for drawing squares around valid cells
 * @param Cairo::RefPtr<Cairo::Context> cr >> Pointer to the cairo drawing context
 * @param std::vector<int> cells >> vector of int values pointing to which rows in result to paint
 * @param float r >> red pixel value [0,1]
 * @param float g >> green pixel value [0,1]
 * @param float b >> blue pixel value [0,1]
 * @param float alpha >> alpha channel value [0,1]
 * @param float width >> line width
 */
void Gui_Decorator::highlightCells(Cairo::RefPtr<Cairo::Context> cr, std::vector<int> cells, float r, float g, float b, float a, float width){
    for(int i : cells){
        const OneObjectRow& oor = m_processor_hand.getNumResult().getRow(i);
        if(oor.getGUIValid()  == -1){
            cr->move_to (oor.getPoint(0).x,oor.getPoint(0).y);

            cr->line_to ( oor.getPoint(1).x,oor.getPoint(1).y);
            cr->line_to ( oor.getPoint(2).x,oor.getPoint(2).y);
            cr->line_to ( oor.getPoint(3).x,oor.getPoint(3).y);
            cr->close_path();
        }
        else{
            cr->move_to (oor.getPoint(0).x,oor.getPoint(0).y);
            cr->line_to ( oor.getPoint(1).x,oor.getPoint(1).y);
            cr->line_to ( oor.getPoint(2).x,oor.getPoint(2).y);
            cr->line_to ( oor.getPoint(3).x,oor.getPoint(3).y);
            cr->close_path();
            cr->line_to ( oor.getPoint(2).x,oor.getPoint(2).y);
            cr->move_to (oor.getPoint(3).x,oor.getPoint(3).y);
            cr->line_to ( oor.getPoint(1).x,oor.getPoint(1).y);
        }

    }
    cr->set_line_width(width*m_line_width_multip);
    cr->set_source_rgba(r,g,b,a);
    cr->stroke();
}
