FROM fedora:latest
MAINTAINER Moses Apostol email: mapo3126@gmail.com


# INSTALL STEP

RUN yum groupinstall 'Development Tools' -y \
    && yum install opencv-devel -y \
    && yum install wget -y \
    && dnf install git-all -y \
    && sudo dnf install gcc-c++ -y

# FETCH STEP 
RUN git clone https://github.com/qgeissmann/OpenCFU \
    &&  wget http://sourceforge.net/projects/opencfu/files/linux/opencfu-3.9.0.tar.gz \
    &&  tar -xvf opencfu-3.9.0.tar.gz

# THEN THE DELETE-UPDATE step
# we will enter into opencfu-3.9.0, and update the code inside.

RUN cd opencfu-3.9.0
WORKDIR "opencfu-3.9.0/"
RUN sudo rm -r src/ && sudo cp -a ../OpenCFU/src/. src && sudo cp -a ../OpenCFU/samples/. samples

# delete 
RUN cd .. \
    && sudo rm -r OpenCFU/


# install gcc
RUN ./configure --without-gui
RUN make
