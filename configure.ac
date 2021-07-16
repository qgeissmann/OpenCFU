AC_PREREQ([2.68])
AC_INIT([opencfu],[3.9.1],[opencfu@gmail.com],[opencfu],[http://www.opencfu.sourceforge.net/])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src])

m4_include([m4/ax_cxx_compile_stdcxx_11.m4])
AM_INIT_AUTOMAKE([1.10 -Wall no-define foreign])

#ACLOCAL_AMFLAGS = -I m4

AC_CONFIG_HEADERS([config.h])


# Checks for programs.


AC_PROG_CXX[gcc-mp-4.8  g++]

AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

# We check for openmp)])


AC_OPENMP

AC_SUBST(OPENMP_CFLAGS)

# We check for opencv 3.0.0. if absent, stop!!
PKG_CHECK_MODULES([OPENCV],[opencv4 >= 3.0.0],[],[AC_MSG_ERROR(OpenCV not found. Have you installed the library (devel version)). OpenCFU cannot be built without OpenCV!])

### We add opencv to global AM flags.




AC_ARG_WITH([gui], AS_HELP_STRING([--without-gui], [Build without a Gtkmm GUI]))
AS_IF([test "x$with_gui" != "xno"], [

   PKG_CHECK_MODULES([GTKMM], [gtkmm-2.4 glibmm-2.4 gthread-2.0], [HAVE_GUI=1], [HAVE_GUI=0])

   AC_DEFINE(WITH_GUI, 1, [We build opencv witha GUI])

   AS_IF([test "$HAVE_GUI" -eq 1],[],[AC_MSG_ERROR(Gtkmm not found. Have you installed the library (devel version). You can build without a GUI with "./configure --without-gui")])
])
AM_CONDITIONAL([USE_GUI], [test "$HAVE_GUI" -eq 1])



AC_ARG_WITH([dbgmes], AS_HELP_STRING([--with-dbgmes], [Print debug info in stdout]))

AS_IF([test "x$with_dbgmes" = "xyes"],[AC_DEFINE(DBG_MESSAGE, 1, [Print debug info in stdout])])
AM_CONDITIONAL([DEV_MODE], [test "x$with_dbgmes" = "xyes"])

#######################################################




case `eval uname -o` in
  *-*-msdos* | *-*-go32* | *-*-mingw32* | *-*-cygwin* | *-*-windows* |*Msys*)
    AM_CONDITIONAL([WIN32], true)
    ;;
  *)
    AM_CONDITIONAL([WIN32], false)
    ;;
esac


# Defines here:
AC_DEFINE(TRAINING_SET_IMG, "data/training-set1/", [Location of the training set])
AC_DEFINE(TRAINING_SET_IMG_PS, "data/training-set2/", [Location of the training set post-splitting])
AC_DEFINE(TEST_SET_IMG, "data/training-set1/", [Location of the test set])
#~ AC_DEFINE(PREFIX, "data/trainnedClassifier.xml", [Location of the test set])
AC_DEFINE(TRAINED_CLASSIF_XML_FILE, "data/trainedClassifier.xml", [Location of the classifier])
AC_DEFINE(TRAINED_CLASSIF_PS_XML_FILE, "data/trainedClassifierPS.xml", [Location of the classifier post-splitting])

AC_DEFINE(BANNER_IMG, "data/banner.png", [Location of the banner])
AC_DEFINE(BACKGROUND_NOISE, "data/noise-texture.png", [Location of the banner])
AC_DEFINE(LOGO_IMG, "data/logo.png", [Location of the logo])

AC_DEFINE(ICON128_IMG, "data/icons/128x128/apps/opencfu.png", [Location of the 128px icon])
AC_DEFINE(ICON64_IMG, "data/icons/64x64/apps/opencfu.png", [Location of the 64px icon])
AC_DEFINE(ICON48_IMG, "data/icons/48x48/apps/opencfu.png", [Location of the 48px icon])
AC_DEFINE(ICON24_IMG, "data/icons/24x24/apps/opencfu.png", [Location of the 24px icon])


AC_DEFINE(PROGRAM_NAME,"OpenCFU",[The actual name (Capitals)])


AC_CONFIG_FILES([Makefile])
AC_OUTPUT




# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
# We cheack for compiler compatibility with std+11

