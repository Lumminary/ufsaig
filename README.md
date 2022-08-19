This is a step by step configuration, build and run guide for CentOS 7. 
It assumes that the source code of the project is already there.

1. Install development tools 

         sudo yum -y group install "Development Tools"
    
2. Install SCL repository 

         sudo yum -y install centos-release-scl-rh
    
3. Install EPEL repository 

         sudo yum -y install epel-release

4. Install GCC 9 

         sudo yum install -y devtoolset-9-toolchain
         mv /usr/bin/cc /usr/bin/cc-4.8.5;
         ln -s /opt/rh/devtoolset-9/root/usr/bin/cc /usr/bin/cc;
         mv /usr/bin/gcc /usr/bin/gcc-4.8.5;
         ln -s /opt/rh/devtoolset-9/root/usr/bin/gcc /usr/bin/gcc;

5. Install dependency packages 

         sudo yum install -y libev-devel java-1.8.0-openjdk jsoncpp-devel libuuid-devel libpng-devel libjpeg-devel openssl-devel openssl

6. Make your shell use developer toolset 9 

         scl enable devtoolset-9 bash;

7. Install CMake. You need at least 3.19.1 - on CentOS this is a newer version than provided by system repos so you need to do the following:
    Build and install CMake from souce. Open https://cmake.org/download/ in your browser. Copy the most recent Linux/Unix source tarball URL. In the SSH terminal type 'wget <URL from the previous step>'. Extract the downloaded archive: 'tar -xvf <archive-name>' 

         wget https://github.com/Kitware/CMake/releases/download/v3.23.0-rc2/cmake-3.23.0-rc2.tar.gz;
         tar -xvf cmake-3.23.0-rc2.tar.gz;
         cd cmake-3.23.0-rc2;
         ./bootstrap;
         make -j `grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4+1}'`;
         sudo make install;
         cd ..;
        
8. Install libwebp. The default libwebp-devel for CentOS has problems, so it is necessary to build from source. 

         git clone https://github.com/webmproject/libwebp;
         cd libwebp;
         ./autogen.sh;
         ./configure;
         make -j `grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4+1}'`;
         sudo make install;
                        
9. Fix PKG_COFIG_PATH to make build system recognize the custom-built webp library installed from source:
         
        export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
    
10. Build the project
         
        cd <project_root>
         ./build.sh
    
11. Run image server
    11.1 Create a configuration file. 
        
        You can use <project_root>/config_example.json as a template
    
     11.3 If you need to bind to port 80, allow the binary to do so:
         
        sudo setcap CAP_NET_BIND_SERVICE=+eip <project_root>/build/image_server
    
     11.2 Assuming you are in the project root folder, do
         
        build/image_server <path to the configuration file>
