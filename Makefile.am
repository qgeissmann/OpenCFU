AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

AM_CXXFLAGS = ${OPENCV_CFLAGS} ${OPENMP_CFLAGS} -Isrc/ -Wall
#AM_LDFLAGS =    -fopenmp
AM_LDFLAGS =   -Wall ${OPENMP_CFLAGS}


if WIN32
AM_CXXFLAGS += -mms-bitfields
AM_LDFLAGS += -Wl,-subsystem,windows
endif

if DEV_MODE
	AM_CXXFLAGS += -pg
	AM_LDFLAGS += -pg
	BETA = BETA
endif

noinst_PROGRAMS = trainer

trainer_SOURCES = 	src/classifier/main.cpp \
					src/classifier/src/DataMaker.cpp \
					src/processor/src/Predictor.cpp \
					src/processor/src/ContourFamily.cpp \
					src/processor/src/Features.cpp

trainer_SOURCES += src/classifier/headers/DataMaker.hpp src/defines.hpp

trainer_CPPFLAGS = 	-Isrc/classifier/headers/ \
					-Isrc/processor/headers/

trainer_LDADD = ${OPENCV_LIBS}


TRAINED_CLASSIF = data/trainedClassifier.xml
TRAINED_CLASSIF_PS = data/trainedClassifierPS.xml

$(TRAINED_CLASSIF): trainer
	./trainer t $(TRAINED_CLASSIF)
	./trainer w $(TRAINED_CLASSIF)


$(TRAINED_CLASSIF_PS): trainer
	./trainer t $(TRAINED_CLASSIF_PS) ps
	./trainer w $(TRAINED_CLASSIF_PS) ps

clean-local:
	rm -f $(TRAINED_CLASSIF)
	rm -f $(TRAINED_CLASSIF_PS)
	rm -f *.tar.gz
	rm -f *.exe


DESKTOP_FILE = $(PKG_SCRIPT_DIR)/opencfu.desktop

EXTRA_DIST = \
data/training-set1/Mult_B_0.png\
data/training-set1/Mult_C_0.png\
data/training-set1/Mult_D_0.png\
data/training-set1/Neg_B_0.png\
data/training-set1/Sing_A_0.png\
data/training-set1/Sing_C_0.png\
data/training-set2/Neg_psB_1.png\
data/training-set2/Sing_psA_1.png\
data/training-set2/Sing_psc_1.png\
$(DESKTOP_FILE)

#######################################################
bin_PROGRAMS = opencfu

#NJL 13/AUG/2014
#Step_ColourCluster.cpp/hpp added
opencfu_SOURCES = 	src/main.cpp \
					src/processor/src/ArgumentParser.cpp \
					src/processor/src/ProcessingOptions.cpp\
					src/processor/src/Step_BaseClass.cpp\
					src/processor/src/Processor.cpp\
					src/processor/src/Step_2.cpp\
					src/processor/src/Step_3.cpp\
					src/processor/src/Result.cpp\
					src/processor/src/Features.cpp\
					src/processor/src/Step_FiltGUI.cpp\
					src/processor/src/Step_FiltHS.cpp\
					src/processor/src/Step_FiltIPosition2D.cpp\
					src/processor/src/Step_FiltLik.cpp\
					src/processor/src/Step_1.cpp\
					src/processor/src/Step_4.cpp\
					src/processor/src/ContourFamily.cpp\
					src/processor/src/Predictor.cpp\
					src/processor/src/ContourSpliter.cpp\
					src/processor/src/MaskROI.cpp\
					src/processor/src/Step_ColourCluster.cpp



opencfu_SOURCES +=  src/processor/headers/Features.hpp \
                    src/processor/headers/Step_BaseClass.hpp\
                    src/processor/headers/Predictor.hpp\
                    src/processor/headers/ProcessingOptions.hpp\
                    src/processor/headers/Step_4.hpp\
                    src/processor/headers/Step_1.hpp\
                    src/processor/headers/Processor.hpp\
                    src/processor/headers/Result.hpp\
                    src/processor/headers/ContourSpliter.hpp\
                    src/processor/headers/Step_FiltLik.hpp\
                    src/processor/headers/Step_3.hpp\
                    src/processor/headers/Step_FiltHS.hpp\
                    src/processor/headers/Step_FiltGUI.hpp\
                    src/processor/headers/Step_FiltIPosition2D.hpp\
                    src/processor/headers/Step_2.hpp\
                    src/processor/headers/ArgumentParser.hpp\
                    src/processor/headers/ContourFamily.hpp\
                    src/processor/headers/MaskROI.hpp\
		    src/processor/headers/Step_ColourCluster.hpp


opencfu_CPPFLAGS = 	 -Isrc/processor/headers/
opencfu_CPPFLAGS += -DINSTALLDIR='"$(pkgdatadir)"'
opencfu_LDADD = ${OPENCV_LIBS}



opencfudir = $(pkgdatadir)
nobase_dist_opencfu_DATA = $(TRAINED_CLASSIF) $(TRAINED_CLASSIF_PS)

if USE_GUI
nobase_dist_opencfu_DATA += data/banner.png data/logo.png data/noise-texture.png
endif

#\
#data/icon_128px.png data/icon_64px.png data/icon_48px.png data/icon_24px.png

###LINUX PACKAGING

if USE_GUI
applicationdir=$(datadir)/applications
dist_application_DATA=packagingScripts/opencfu.desktop

appiconSVGdir=$(datadir)/icons/hicolor/scalable/apps
dist_appiconSVG_DATA=data/icons/scalable/opencfu.svg

appicon128dir=$(datadir)/icons/hicolor/128x128/apps
dist_appicon128_DATA=data/icons/128x128/opencfu.png

appicon48dir=$(datadir)/icons/hicolor/48x48/apps
dist_appicon48_DATA=data/icons/48x48/opencfu.png
endif
##

if USE_GUI

if WIN32
AM_CXXFLAGS += -mwindows
AM_LDFLAGS += -mwindows
endif

#NJL 13/AUG/2014
#Gui_ColourCluster.cpp/hpp added
opencfu_SOURCES += src/gui/src/Gui_ColourSelector.cpp\
					src/gui/src/Gui_ColourWheel.cpp\
					src/gui/src/Gui_ControlPanel.cpp\
					src/gui/src/Gui_Decorator.cpp\
					src/gui/src/Gui_DisplayBaseClass.cpp\
					src/gui/src/Gui_Display.cpp\
					src/gui/src/Gui_DisplayRoi.cpp\
					src/gui/src/Gui_ImgFileSetter.cpp\
					src/gui/src/Gui_LikFiltSelector.cpp\
					src/gui/src/Gui_MouseHandler.cpp\
					src/gui/src/Gui_MaskSetter.cpp\
					src/gui/src/Gui_MyApplication.cpp\
					src/gui/src/Gui_MyWindow.cpp\
					src/gui/src/Gui_OptionSetterBaseClass.cpp\
					src/gui/src/Gui_ProcessorHandler.cpp\
					src/gui/src/Gui_RadiusSetter.cpp\
					src/gui/src/Gui_ResultDetails.cpp\
					src/gui/src/Gui_ResultDisplayBaseClass.cpp\
					src/gui/src/Gui_ResultFileWriter.cpp\
					src/gui/src/Gui_ResultFrame.cpp\
					src/gui/src/Gui_ResultLabel.cpp\
					src/gui/src/Gui_ResultListDisplay.cpp\
					src/gui/src/Gui_ThresholdSetter.cpp\
					src/gui/src/Gui_ConfigIO.cpp\
					src/gui/src/Gui_HelloWindow.cpp\
					src/gui/src/ResultMap.cpp\
					src/gui/src/Gui_UserFilterSetter.cpp\
					src/gui/src/Gui_PixbufOpener.cpp\
					src/gui/src/Gui_ColourCluster.cpp



opencfu_SOURCES +=  src/gui/headers/Gui_ColourSelector.hpp\
                    src/gui/headers/Gui_ColourWheel.hpp\
					src/gui/headers/Gui_ControlPanel.hpp\
					src/gui/headers/Gui_Decorator.hpp\
					src/gui/headers/Gui_DisplayBaseClass.hpp\
					src/gui/headers/Gui_Display.hpp\
					src/gui/headers/Gui_DisplayRoi.hpp\
					src/gui/headers/Gui_ImgFileSetter.hpp\
					src/gui/headers/Gui_LikFiltSelector.hpp\
					src/gui/headers/Gui_MaskSetter.hpp\
					src/gui/headers/Gui_MouseHandler.hpp\
					src/gui/headers/Gui_MyApplication.hpp\
					src/gui/headers/Gui_MyWindow.hpp\
					src/gui/headers/Gui_OptionSetterBaseClass.hpp\
					src/gui/headers/Gui_ProcessorHandler.hpp\
					src/gui/headers/Gui_RadiusSetter.hpp\
                    src/gui/headers/Gui_ResultDetails.hpp\
					src/gui/headers/Gui_ResultDisplayBaseClass.hpp\
					src/gui/headers/Gui_ResultFileWriter.hpp\
					src/gui/headers/Gui_ResultFrame.hpp\
					src/gui/headers/Gui_ResultLabel.hpp\
					src/gui/headers/Gui_ResultListDisplay.hpp\
					src/gui/headers/Gui_ThresholdSetter.hpp\
					src/gui/headers/Gui_ConfigIO.hpp\
					src/gui/headers/Gui_HelloWindow.hpp\
					src/gui/headers/ResultMap.hpp\
					src/gui/headers/Gui_UserFilterSetter.hpp\
					src/gui/headers/Gui_PixbufOpener.hpp\
					src/gui/headers/Gui_ColourCluster.hpp\
					src/gui/headers/text.hpp

opencfu_CPPFLAGS += ${GTKMM_CFLAGS}
opencfu_CPPFLAGS += -Isrc/gui/headers/
opencfu_LDADD += ${GTKMM_LIBS}

endif

PKG_SCRIPT_DIR=./packagingScripts


MD5_FILE=$(PKG_SCRIPT_DIR)/md5.txt
$(MD5_FILE): dist
	md5sum  $(DIST_ARCHIVES) | cut -f 1 -d ' ' > $(MD5_FILE)

MD5=`cat $(MD5_FILE)`

publish_windows:$(bin_PROGRAMS) $(TRAINED_CLASSIF) $(TRAINED_CLASSIF_PS) $(VERSION_FILE)
	$(eval SETUP_NAME="OpenCFU-$(VERSION)$(BETA)_setup.exe")
	sed s/^AppVerName=.*/AppVerName=$(VERSION)/ $(PKG_SCRIPT_DIR)/OpenCFU_noVersion.iss > $(PKG_SCRIPT_DIR)/OpenCFU.iss
	ISCC.exe $(PKG_SCRIPT_DIR)/OpenCFU.iss
	mv setup.exe $(SETUP_NAME)
	rsync -avP -e ssh $(SETUP_NAME) quentelery,opencfu@frs.sourceforge.net:/home/frs/project/o/op/opencfu/windows



PKGBUILD_FILE = $(PKG_SCRIPT_DIR)/PKGBUILD



$(PKGBUILD_FILE): $(bin_PROGRAMS) $(TRAINED_CLASSIF) $(TRAINED_CLASSIF_PS) $(MD5_FILE)
	sed s/^pkgver=.*/pkgver=$(VERSION)/ $(PKGBUILD_FILE).template > $(PKGBUILD_FILE)
	mv $(PKGBUILD_FILE) $(PKGBUILD_FILE).template
	sed s/"^md5sums=('.*')"/"md5sums=($(MD5))"/ $(PKGBUILD_FILE).template > $(PKGBUILD_FILE)



publish_linux: $(DESKTOP_FILE) $(PKGBUILD_FILE) $(DESKTOP_FILE)
	rsync -avP -e ssh $(DIST_ARCHIVES) quentelery,opencfu@frs.sourceforge.net:/home/frs/project/o/op/opencfu/linux



TMP_DEB:=$(shell mktemp --suffix='.opencfu' -u)
#~ DEB_TAR:=$(TMP_DEB)$(DIST_ARCHIVES)
DEB_TAR:= $(shell echo $(TMP_DEB)/$(DIST_ARCHIVES)| sed s/"-"/"_"/ |  sed s/".tar."/".orig.tar."/)
DEB_DIR:=$(PKG_SCRIPT_DIR)/debian
deb: dist
	mkdir $(TMP_DEB)
	cp $(DIST_ARCHIVES) $(DEB_TAR)
	cp $(PKG_SCRIPT_DIR)/Makefile-deb $(TMP_DEB)/Makefile
	cp $(DEB_DIR) $(TMP_DEB) -r
	make -C  $(TMP_DEB) all
	echo SUCCESS DEBIAN PACKAGE FILES ARE IN '$(TMP_DEB)'

#	mv $(ls $(TMP_DEB))	$(ls $(TMP_DEB) | sed s/"-"/"_"/ |  sed s/".tar."/".orig.tar."/)
#	rm -rf $(TMP_DEB)

ref_results: $(bin_PROGRAMS) $(TRAINED_CLASSIF) $(TRAINED_CLASSIF_PS)
	./test/stressTest.sh make

#check: $(bin_PROGRAMS) $(TRAINED_CLASSIF) $(TRAINED_CLASSIF_PS)
#	./test/stressTest.sh

cleanall: clean-local
	rm -f *.in *.m4 config.* configure *.log Makefile stamp-h1
	rm -rf build-aux config *.cache
