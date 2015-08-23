#ifndef MYTEXT_H
#define MYTEXT_H


#define PLOS_URL "http:\x2f/www.plosone.org/article/info\%3Adoi\%2F10.1371\%2Fjournal.pone.0054072"

#define ABOUT_TEXT "Thank you for using OpenCFU. Consider citing the PLoS ONE publication if you use OpenCFU in your academic work :)."

#define SHOW_HELLO_NEXT_TIME "Show this message next time"
#define VISIT_WEBSITE "Visit OpenCFU's Website"
#define READ_PUBLICATION "Read the publication on PLoS ONE"
#define REPORT_BUGS "opencfu@gmail.com"
#define INTRO_TEXT_1 "<big>Thank you for using OpenCFU ("

#define INTRO_TEXT_2 "\
)! <b>This is a beta version</b>. \
It is likely to have a few bugs. \
Please, feel free to request features and \
report any problems you would experience.</big>"


#define NO_SAVE_WARNING_MAIN "<big><b><i>Do you really want to quit?</i></b></big>"
#define NO_SAVE_WARNING_SECOND "<big>It looks like <i>some results have not been saved!</i> \
Do you want to quit OpenCFU anyway?</big>"

#define ADD_FILE_BUTTON_TEXT "Add files"
#define APPEND_OR_OVERWRITE_MESSAGE "What are we doing with those new files?"

#define FILE_REMOVED " file(s) was/were not added."

#define FILE_CORRUPTED_SECONDARY "The file(s) appear to be corrupted or inexistent. Please send a bug-repport if not."
#define FILE_REDONDANT_SECONDARY "The file(s) selected are already in the file list"
#define NO_FILES "None of the files added are valid"


#define NO_FILE_LABEL "No files yet"
#define APPEND_OR_OVERWRITE_MESSAGE2 "\
\"Overwrite\" will replace your list \
of files with the selected files whilst\"Append\" will only add \
them in the end of your current list."


#define NAME_IMAGE_FILE_SETTER "<b><i>Files:</i></b>"
#define NAME_THR_SETTER "<b><i>Threshold:</i></b>"
#define NAME_RAD_SETTER "<b><i>Radius:</i></b>"
#define NAME_MASK_SETTER "<b><i>ROIs and Mask:</i></b>"
#define NAME_COL_SELECT "<b><i>Colour filter:</i></b>"
#define NAME_LIK_SELECT "<b><i>Auto outlier filter:</i></b>"
//NJL 10/AUG/2014
#define NAME_COL_CLUSTER "<b><i>Group similar colours:</i></b>"

#define LABEL_CHECKBUTTON_MAX_RAD "Auto-Max"
#define LABEL_CHECKBUTTON_AUTO_THR "Auto"
#define LABEL_MIN_RAD "Min\t"
#define LABEL_MAX_RAD "Max\t"

#define LABEL_CHECKBUTTON_HAS_HUE_FILT "Use colour filter"
#define LABEL_HUE "Hue:\t"
#define LABEL_HUE_CENT "Mean\t"
#define LABEL_HUE_TOL "Tol\t"
#define LABEL_SATUR "Saturation:\t"
#define LABEL_SATUR_MIN "Min\t"
#define LABEL_SATUR_MAX "Max\t"

#define LABEL_CHECKBUTTON_HAS_LIK_FILT "Use outlier filter"
#define LABEL_THR "Threshold"


#define LABEL_SET_DECO_LINE_WIDTH "Line width"

#define RESULT_LABEL_BUSY "<span size=\"x-large\"><b><i>Processing:</i> \\/\\<sub>\\/\\/ </sub></b></span>"
#define RESULT_LABEL_IDLE "<span font_size=\"x-large\"><b><i>Result:</i></b></span>"

#define RESULT_DETAIL_TITLE "<b><i>Per object</i></b>"
#define RESULT_LIST_TITLE "<b><i>Per image</i></b>"
#define RESULT_LIST_CHECKBUTTON "Show objects"


#define NO_SELECTION_MESSAGE "<b>No files were selected.</b> Impossible to write a file with no results."
#define SAVE_MODE_MESSAGE "<b>What kind of output do you want?</b>"


#define SAVE_FILE_TITLE "Save Results"

#define TEXT_DELETE_SELECTED_RESULT "Delete selection"
#define TEXT_SAVE_ALL_RESULT "Save all"
#define TEXT_SAVE_SELECTED_RESULT "Save selection"



#define DRAW_MASK_TEXT "\
<big>You can draw one or several Regions Of Interest \
by clicking on the image. Whith \"Three point circle\" \
ROI means you will define circular areas by clicking on \
any three point of their circumference. For more informations \
about ROIs, you will find video tutorial on the website.</big>"


#define THREE_POINT_CIRCLE_TOOL "Three points circle"
#define CONVEX_POLYGON_TOOL "Convex polygon"


//NJL 10/AUG/2014 Colour Clustering
#define LABEL_CHECKBUTTON_HAS_CLUSTERING_DISTANCE "Recognise similar colours"
#define LABEL_CLUSTERING_DISTANCE "Coarseness:"
//NJL 13/FEB/2015
#define LABEL_CLUSTERING_POINTS "Min. Neighbours:"

/*==================TOOLTIPS==================*/

#define TOOLTIP_COLOUR_SELECTOR "Select objects by colour (Hue and Saturation)"
#define TOOLTIP_COLOUR_SELECTOR_HUE "\
The \"Mean\" Hue defines the average colour\n\
your objects should have whilst \"Tol\", defines\n\
the tolerance around the mean."

#define TOOLTIP_COLOUR_SELECTOR_SAT "\
The colour saturation can be any integer value between 0 and 255.\
"


#define TOOLTIP_IMG_SETTER "Use this tool to\nadd and browse images"
#define TOOLTIP_IMG_SETTER_FIRST "Go to first image"
#define TOOLTIP_IMG_SETTER_PREVIOUS "Go to previous image"
#define TOOLTIP_IMG_SETTER_NEXT "Go to next image"
#define TOOLTIP_IMG_SETTER_LAST "Go to last image"



#define TOOLTIP_LIK_FILT "Automatically detect outliers\n\
and exclude them.\n\
The small values of threshold\n\
will exclude more objects."


#define TOOLTIP_RAD_SETTER "Set the range of size of the objects to look for"

#define TOOLTIP_RESULT_LIST_DET "Investigate and object by clicking.\n\
Click on the \"Valid\" checkbox to exclude/include objects"

#define TOOLTIP_RESULT_LAB "Result: [Valid objects]/[All objects]"
#define TOOLTIP_RESULT_LAB_NA "Set the current result as NA"
#define TOOLTIP_RESULT_LAB_DECO "Hide/show rectangles aurount detected objects"
#define TOOLTIP_RESULT_LAB_LINE_WIDTH "Increase/decrease the size of the\n\
annotations on the image"

#define TOOLTIP_RESULT_LIST_SIMPL "See and save results"



#define TOOLTIP_THR_SETTER "The higher is the threshold, the more stringent will be analysis"

#define TOOLTIP_MASK_SETTER "To define one or several regions of interest"
#define TOOLTIP_MASK_SETTER_SHOW "Hide/show the ROIs on the image"
#define TOOLTIP_MASK_SETTER_REFRESH "Redraw current mask"

//NJL 10/AUG/2014 Colour Clustering
#define TOOLTIP_CLUSTERING "Use a density based algorithm to identify like coloured colonies"
#define TOOLTIP_CLUSTERING_DISTANCE_SELECTOR "\
The \"Coarseness\" defines the maximum difference\n\
between colours grouped as similar. A coarseness\n\
of 2.3 corresponds to a just notable difference"

//NJL 13/FEB/2015
#define TOOLTIP_CLUSTERING_MINIMUM_POINTS "\
The \"Min. Neighbours\" defines the minimum number\n\
of neighbours a point should have to be in a cluster\n\
A higher number means less chance of low density \n\
points bridging two clusters."

#endif
