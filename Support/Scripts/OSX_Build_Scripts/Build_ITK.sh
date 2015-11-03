#!/bin/bash

#------------------------------------------------------------------------------
# Read the configuration file for the SDK Build
shopt -s extglob
configfile="SDK_Configuration.conf" # set the actual path name of your (DOS or Unix) config file
tr -d '\r' < $configfile > $configfile.unix
while IFS='= ' read lhs rhs
do
    if [[ ! $lhs =~ ^\ *# && -n $lhs ]]; then
        rhs="${rhs%%\#*}"    # Del in line right comments
        rhs="${rhs%%*( )}"   # Del trailing spaces
        rhs="${rhs%\"*}"     # Del opening string quotes 
        rhs="${rhs#\"*}"     # Del closing string quotes 
        declare $lhs="$rhs"
    fi
done < $configfile.unix
rm $configfile.unix
#------------------------------------------------------------------------------

SCRIPT_DIR=`pwd`
cd $SDK_INSTALL


HOST_SYSTEM=`uname`
echo "Host System: $HOST_SYSTEM"

WGET=`type -P wget`
CURL=`type -P curl`

if [[ "$WGET" == "" ]];
   then
  if [[ "$CURL" == "" ]];
     then
    echo "wget and curl are NOT present on your machine. One of them is needed to download sources from the internet."
    exit 1
  fi
fi


DOWNLOAD_PROG=""
DOWNLOAD_ARGS=""

if [[ "$WGET" != "" ]];
then
  DOWNLOAD_PROG=$WGET
fi

if [[ "$CURL" != "" ]];
then
  DOWNLOAD_PROG=$CURL
fi



CMAKE=`type -P cmake`
if [[ $CMAKE == "" ]];
  then
  echo "CMake is needed for this script. Please install it on your system and be sure it is on your path."
  exit 1
fi


# Build the ITK libraries we need and set our Environment Variable.

if [ ! -e "${SDK_INSTALL}/${ITK_FOLDER_NAME}.tar.gz" ];
then
  echo "-------------------------------------------"
  echo " Downloading ITK version ${ITK_VERSION}"
  echo "-------------------------------------------"
  
  $DOWNLOAD_PROG  "http://superb-dca2.dl.sourceforge.net/project/itk/itk/4.8/${ITK_FOLDER_NAME}.tar.gz" -o ${ITK_FOLDER_NAME}.tar.gz
fi

if [ ! -e "$SDK_INSTALL/${ITK_FOLDER_NAME}" ];
then
  tar -xvzf ${ITK_FOLDER_NAME}.tar.gz
fi




function ConfigureITK()
{
  # $1 is the HDF5 Installation location
  # $2 is the Build Type
  # Write an initial Cache File
  mkdir ITK-${ITK_VERSION}-$2
  cd ITK-${ITK_VERSION}-$2

  lower=`echo $2 | tr '[:upper:]' '[:lower:]'`
  upper=`echo $2 | tr '[:lower:]' '[:upper:]'`
  cacheFile=CMakeCache.txt
  echo "BUILD_DOCUMENTATION:BOOL=OFF" > $cacheFile
  echo "ITK_USE_SYSTEM_HDF5:BOOL=ON" >> $cacheFile
  echo "BUILD_EXAMPLES:BOOL=OFF" >> $cacheFile
  echo "BUILD_SHARED_LIBS:BOOL=ON" >> $cacheFile
  echo "BUILD_TESTING:BOOL=OFF" >> $cacheFile
  echo "CMAKE_BUILD_TYPE:STRING=$2" >> $cacheFile
  echo "DITK_LEGACY_REMOVE:BOOL=ON" >> $cacheFile
  echo "KWSYS_USE_MD5:BOOL=ON" >> $cacheFile
  echo "Module_ITKReview:BOOL=ON" >> $cacheFile
  echo "ITK_BUILD_DEFAULT_MODULES=OFF" >> $cacheFile
  echo "ITKGroup_Core=ON" >> $cacheFile
  echo "ITKGroup_Filtering:BOOL=ON" >> $cacheFile
  echo "ITKGroup_Registration:BOOL=ON" >> $cacheFile
  echo "ITKGroup_Segmentation:BOOL=ON" >> $cacheFile
  echo "ITK_USE_SYSTEM_HDF5:BOOL=ON" >> $cacheFile
  echo "CMAKE_SKIP_INSTALL_RPATH:BOOL=ON" >> $cacheFile
  echo "CMAKE_SKIP_RPATH:BOOL=ON" >> $cacheFile
  echo "USE_COMPILER_HIDDEN_VISIBILITY:BOOL=OFF" >> $cacheFile
  echo "CMAKE_CXX_STANDARD:STRING=11" >> $cacheFile
  echo "CMAKE_CXX_STANDARD_REQUIRED:BOOL=ON" >> $cacheFile
  echo "CMAKE_OSX_DEPLOYMENT_TARGET=$OSX_DEPLOYMENT_TARGET" >> $cacheFile
  echo "CMAKE_OSX_SYSROOT:Path=$OSX_SDK" >> $cacheFile
  echo "CMAKE_CXX_FLAGS:STRING=-stdlib=libc++ -std=c++11" >> cacheFile

  echo "HDF5_DIR:STRING=$SDK_INSTALL/$1-$2/share/cmake" >> $cacheFile
  echo "HDF5_CXX_COMPILER_EXECUTABLE:FILEPATH=HDF5_CXX_COMPILER_EXECUTABLE-NOTFOUND" >> $cacheFile
  echo "HDF5_CXX_INCLUDE_DIR:PATH=$SDK_INSTALL/$1-$2/include" >> $cacheFile
  echo "HDF5_C_COMPILER_EXECUTABLE:FILEPATH=HDF5_C_COMPILER_EXECUTABLE-NOTFOUND" >> $cacheFile
  echo "HDF5_C_INCLUDE_DIR:PATH=$SDK_INSTALL/$1-$2/include" >> $cacheFile

  echo "HDF5_DIFF_EXECUTABLE:FILEPATH=$SDK_INSTALL/$1-$2/bin/h5diff" >> $cacheFile
  echo "HDF5_DIR:PATH=$SDK_INSTALL/$1-$2/share/cmake" >> $cacheFile
  echo "HDF5_Fortran_COMPILER_EXECUTABLE:FILEPATH=HDF5_Fortran_COMPILER_EXECUTABLE-NOTFOUND" >> $cacheFile
  echo "HDF5_IS_PARALLEL:BOOL=OFF" >> $cacheFile
  if [ "$2" = "Debug" ]; then
    echo "HDF5_CXX_LIBRARY:PATH=$SDK_INSTALL/$1-$2/lib/libhdf5_cpp_${lower}.1.8.15.dylib" >> $cacheFile
    echo "HDF5_C_LIBRARY:PATH=$SDK_INSTALL/$1-$2/lib/libhdf5_${lower}.1.8.15.dylib" >> $cacheFile
    echo "HDF5_hdf5_LIBRARY_${upper}:FILEPATH=$SDK_INSTALL/$1-$2/lib/libhdf5_${lower}.dylib" >> $cacheFile
    echo "HDF5_hdf5_cpp_LIBRARY_${upper}:FILEPATH=$SDK_INSTALL/$1-$2/lib/libhdf5_cpp_${lower}.dylib" >> $cacheFile
  fi

  if [ "$2" = "Release" ]; then
    echo "HDF5_CXX_LIBRARY:PATH=$SDK_INSTALL/$1-$2/lib/libhdf5_cpp.1.8.15.dylib" >> $cacheFile
    echo "HDF5_C_LIBRARY:PATH=$SDK_INSTALL/$1-$2/lib/libhdf5.1.8.15.dylib" >> $cacheFile
    echo "HDF5_hdf5_LIBRARY_${upper}:FILEPATH=$SDK_INSTALL/$1-$2/lib/libhdf5.dylib" >> $cacheFile
    echo "HDF5_hdf5_cpp_LIBRARY_${upper}:FILEPATH=$SDK_INSTALL/$1-$2/lib/libhdf5_cpp.dylib" >> $cacheFile
  fi

  cmake -G Ninja -DCMAKE_CXX_FLAGS="-stdlib=libc++ -std=c++11" ../$ITK_FOLDER_NAME
  ninja
  cd ../
}

ConfigureITK ${HDF5_INSTALL} Debug
$SCRIPT_DIR/FixITK.sh $SDK_INSTALL/${ITK_INSTALL}-Debug 

ConfigureITK ${HDF5_INSTALL} Release
$SCRIPT_DIR/FixITK.sh $SDK_INSTALL/${ITK_INSTALL}-Release 



echo "" >> "$SDK_INSTALL/DREAM3D_SDK.cmake"
echo "#--------------------------------------------------------------------------------------------------" >> "$SDK_INSTALL/DREAM3D_SDK.cmake"
echo "# ITK (www.itk.org) For Image Processing Plugin" >> "$SDK_INSTALL/DREAM3D_SDK.cmake"
echo "set(ITK_DIR \"\${DREAM3D_SDK_ROOT}/${ITK_INSTALL}-\${BUILD_TYPE}\")" >> "$SDK_INSTALL/DREAM3D_SDK.cmake"
