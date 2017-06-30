# Install script for directory: C:/Libraries/taglib1_10/taglib

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/taglib")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "CMAKE_BUILD_TYPE")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/taglib/lib/libtag.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/taglib/lib" TYPE STATIC_LIBRARY FILES "C:/Libraries/taglib1_10/taglib/libtag.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/taglib/include/taglib/tag.h;C:/Program Files (x86)/taglib/include/taglib/fileref.h;C:/Program Files (x86)/taglib/include/taglib/audioproperties.h;C:/Program Files (x86)/taglib/include/taglib/taglib_export.h;C:/Program Files (x86)/taglib/include/taglib/taglib_config.h;C:/Program Files (x86)/taglib/include/taglib/taglib.h;C:/Program Files (x86)/taglib/include/taglib/tstring.h;C:/Program Files (x86)/taglib/include/taglib/tlist.h;C:/Program Files (x86)/taglib/include/taglib/tlist.tcc;C:/Program Files (x86)/taglib/include/taglib/tstringlist.h;C:/Program Files (x86)/taglib/include/taglib/tbytevector.h;C:/Program Files (x86)/taglib/include/taglib/tbytevectorlist.h;C:/Program Files (x86)/taglib/include/taglib/tbytevectorstream.h;C:/Program Files (x86)/taglib/include/taglib/tiostream.h;C:/Program Files (x86)/taglib/include/taglib/tfile.h;C:/Program Files (x86)/taglib/include/taglib/tfilestream.h;C:/Program Files (x86)/taglib/include/taglib/tmap.h;C:/Program Files (x86)/taglib/include/taglib/tmap.tcc;C:/Program Files (x86)/taglib/include/taglib/tpropertymap.h;C:/Program Files (x86)/taglib/include/taglib/trefcounter.h;C:/Program Files (x86)/taglib/include/taglib/tdebuglistener.h;C:/Program Files (x86)/taglib/include/taglib/mpegfile.h;C:/Program Files (x86)/taglib/include/taglib/mpegproperties.h;C:/Program Files (x86)/taglib/include/taglib/mpegheader.h;C:/Program Files (x86)/taglib/include/taglib/xingheader.h;C:/Program Files (x86)/taglib/include/taglib/id3v1tag.h;C:/Program Files (x86)/taglib/include/taglib/id3v1genres.h;C:/Program Files (x86)/taglib/include/taglib/id3v2extendedheader.h;C:/Program Files (x86)/taglib/include/taglib/id3v2frame.h;C:/Program Files (x86)/taglib/include/taglib/id3v2header.h;C:/Program Files (x86)/taglib/include/taglib/id3v2synchdata.h;C:/Program Files (x86)/taglib/include/taglib/id3v2footer.h;C:/Program Files (x86)/taglib/include/taglib/id3v2framefactory.h;C:/Program Files (x86)/taglib/include/taglib/id3v2tag.h;C:/Program Files (x86)/taglib/include/taglib/attachedpictureframe.h;C:/Program Files (x86)/taglib/include/taglib/commentsframe.h;C:/Program Files (x86)/taglib/include/taglib/eventtimingcodesframe.h;C:/Program Files (x86)/taglib/include/taglib/generalencapsulatedobjectframe.h;C:/Program Files (x86)/taglib/include/taglib/ownershipframe.h;C:/Program Files (x86)/taglib/include/taglib/popularimeterframe.h;C:/Program Files (x86)/taglib/include/taglib/privateframe.h;C:/Program Files (x86)/taglib/include/taglib/relativevolumeframe.h;C:/Program Files (x86)/taglib/include/taglib/synchronizedlyricsframe.h;C:/Program Files (x86)/taglib/include/taglib/textidentificationframe.h;C:/Program Files (x86)/taglib/include/taglib/uniquefileidentifierframe.h;C:/Program Files (x86)/taglib/include/taglib/unknownframe.h;C:/Program Files (x86)/taglib/include/taglib/unsynchronizedlyricsframe.h;C:/Program Files (x86)/taglib/include/taglib/urllinkframe.h;C:/Program Files (x86)/taglib/include/taglib/chapterframe.h;C:/Program Files (x86)/taglib/include/taglib/tableofcontentsframe.h;C:/Program Files (x86)/taglib/include/taglib/oggfile.h;C:/Program Files (x86)/taglib/include/taglib/oggpage.h;C:/Program Files (x86)/taglib/include/taglib/oggpageheader.h;C:/Program Files (x86)/taglib/include/taglib/xiphcomment.h;C:/Program Files (x86)/taglib/include/taglib/vorbisfile.h;C:/Program Files (x86)/taglib/include/taglib/vorbisproperties.h;C:/Program Files (x86)/taglib/include/taglib/oggflacfile.h;C:/Program Files (x86)/taglib/include/taglib/speexfile.h;C:/Program Files (x86)/taglib/include/taglib/speexproperties.h;C:/Program Files (x86)/taglib/include/taglib/opusfile.h;C:/Program Files (x86)/taglib/include/taglib/opusproperties.h;C:/Program Files (x86)/taglib/include/taglib/flacfile.h;C:/Program Files (x86)/taglib/include/taglib/flacpicture.h;C:/Program Files (x86)/taglib/include/taglib/flacproperties.h;C:/Program Files (x86)/taglib/include/taglib/flacmetadatablock.h;C:/Program Files (x86)/taglib/include/taglib/apefile.h;C:/Program Files (x86)/taglib/include/taglib/apeproperties.h;C:/Program Files (x86)/taglib/include/taglib/apetag.h;C:/Program Files (x86)/taglib/include/taglib/apefooter.h;C:/Program Files (x86)/taglib/include/taglib/apeitem.h;C:/Program Files (x86)/taglib/include/taglib/mpcfile.h;C:/Program Files (x86)/taglib/include/taglib/mpcproperties.h;C:/Program Files (x86)/taglib/include/taglib/wavpackfile.h;C:/Program Files (x86)/taglib/include/taglib/wavpackproperties.h;C:/Program Files (x86)/taglib/include/taglib/trueaudiofile.h;C:/Program Files (x86)/taglib/include/taglib/trueaudioproperties.h;C:/Program Files (x86)/taglib/include/taglib/rifffile.h;C:/Program Files (x86)/taglib/include/taglib/aifffile.h;C:/Program Files (x86)/taglib/include/taglib/aiffproperties.h;C:/Program Files (x86)/taglib/include/taglib/wavfile.h;C:/Program Files (x86)/taglib/include/taglib/wavproperties.h;C:/Program Files (x86)/taglib/include/taglib/infotag.h;C:/Program Files (x86)/taglib/include/taglib/asffile.h;C:/Program Files (x86)/taglib/include/taglib/asfproperties.h;C:/Program Files (x86)/taglib/include/taglib/asftag.h;C:/Program Files (x86)/taglib/include/taglib/asfattribute.h;C:/Program Files (x86)/taglib/include/taglib/asfpicture.h;C:/Program Files (x86)/taglib/include/taglib/mp4file.h;C:/Program Files (x86)/taglib/include/taglib/mp4atom.h;C:/Program Files (x86)/taglib/include/taglib/mp4tag.h;C:/Program Files (x86)/taglib/include/taglib/mp4item.h;C:/Program Files (x86)/taglib/include/taglib/mp4properties.h;C:/Program Files (x86)/taglib/include/taglib/mp4coverart.h;C:/Program Files (x86)/taglib/include/taglib/modfilebase.h;C:/Program Files (x86)/taglib/include/taglib/modfile.h;C:/Program Files (x86)/taglib/include/taglib/modtag.h;C:/Program Files (x86)/taglib/include/taglib/modproperties.h;C:/Program Files (x86)/taglib/include/taglib/itfile.h;C:/Program Files (x86)/taglib/include/taglib/itproperties.h;C:/Program Files (x86)/taglib/include/taglib/s3mfile.h;C:/Program Files (x86)/taglib/include/taglib/s3mproperties.h;C:/Program Files (x86)/taglib/include/taglib/xmfile.h;C:/Program Files (x86)/taglib/include/taglib/xmproperties.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files (x86)/taglib/include/taglib" TYPE FILE FILES
    "C:/Libraries/taglib1_10/taglib/tag.h"
    "C:/Libraries/taglib1_10/taglib/fileref.h"
    "C:/Libraries/taglib1_10/taglib/audioproperties.h"
    "C:/Libraries/taglib1_10/taglib/taglib_export.h"
    "C:/Libraries/taglib1_10/taglib/../taglib_config.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/taglib.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tstring.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tlist.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tlist.tcc"
    "C:/Libraries/taglib1_10/taglib/toolkit/tstringlist.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tbytevector.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tbytevectorlist.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tbytevectorstream.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tiostream.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tfile.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tfilestream.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tmap.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tmap.tcc"
    "C:/Libraries/taglib1_10/taglib/toolkit/tpropertymap.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/trefcounter.h"
    "C:/Libraries/taglib1_10/taglib/toolkit/tdebuglistener.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/mpegfile.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/mpegproperties.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/mpegheader.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/xingheader.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v1/id3v1tag.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v1/id3v1genres.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/id3v2extendedheader.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/id3v2frame.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/id3v2header.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/id3v2synchdata.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/id3v2footer.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/id3v2framefactory.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/id3v2tag.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/attachedpictureframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/commentsframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/eventtimingcodesframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/ownershipframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/popularimeterframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/privateframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/relativevolumeframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/synchronizedlyricsframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/textidentificationframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/unknownframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/urllinkframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/chapterframe.h"
    "C:/Libraries/taglib1_10/taglib/mpeg/id3v2/frames/tableofcontentsframe.h"
    "C:/Libraries/taglib1_10/taglib/ogg/oggfile.h"
    "C:/Libraries/taglib1_10/taglib/ogg/oggpage.h"
    "C:/Libraries/taglib1_10/taglib/ogg/oggpageheader.h"
    "C:/Libraries/taglib1_10/taglib/ogg/xiphcomment.h"
    "C:/Libraries/taglib1_10/taglib/ogg/vorbis/vorbisfile.h"
    "C:/Libraries/taglib1_10/taglib/ogg/vorbis/vorbisproperties.h"
    "C:/Libraries/taglib1_10/taglib/ogg/flac/oggflacfile.h"
    "C:/Libraries/taglib1_10/taglib/ogg/speex/speexfile.h"
    "C:/Libraries/taglib1_10/taglib/ogg/speex/speexproperties.h"
    "C:/Libraries/taglib1_10/taglib/ogg/opus/opusfile.h"
    "C:/Libraries/taglib1_10/taglib/ogg/opus/opusproperties.h"
    "C:/Libraries/taglib1_10/taglib/flac/flacfile.h"
    "C:/Libraries/taglib1_10/taglib/flac/flacpicture.h"
    "C:/Libraries/taglib1_10/taglib/flac/flacproperties.h"
    "C:/Libraries/taglib1_10/taglib/flac/flacmetadatablock.h"
    "C:/Libraries/taglib1_10/taglib/ape/apefile.h"
    "C:/Libraries/taglib1_10/taglib/ape/apeproperties.h"
    "C:/Libraries/taglib1_10/taglib/ape/apetag.h"
    "C:/Libraries/taglib1_10/taglib/ape/apefooter.h"
    "C:/Libraries/taglib1_10/taglib/ape/apeitem.h"
    "C:/Libraries/taglib1_10/taglib/mpc/mpcfile.h"
    "C:/Libraries/taglib1_10/taglib/mpc/mpcproperties.h"
    "C:/Libraries/taglib1_10/taglib/wavpack/wavpackfile.h"
    "C:/Libraries/taglib1_10/taglib/wavpack/wavpackproperties.h"
    "C:/Libraries/taglib1_10/taglib/trueaudio/trueaudiofile.h"
    "C:/Libraries/taglib1_10/taglib/trueaudio/trueaudioproperties.h"
    "C:/Libraries/taglib1_10/taglib/riff/rifffile.h"
    "C:/Libraries/taglib1_10/taglib/riff/aiff/aifffile.h"
    "C:/Libraries/taglib1_10/taglib/riff/aiff/aiffproperties.h"
    "C:/Libraries/taglib1_10/taglib/riff/wav/wavfile.h"
    "C:/Libraries/taglib1_10/taglib/riff/wav/wavproperties.h"
    "C:/Libraries/taglib1_10/taglib/riff/wav/infotag.h"
    "C:/Libraries/taglib1_10/taglib/asf/asffile.h"
    "C:/Libraries/taglib1_10/taglib/asf/asfproperties.h"
    "C:/Libraries/taglib1_10/taglib/asf/asftag.h"
    "C:/Libraries/taglib1_10/taglib/asf/asfattribute.h"
    "C:/Libraries/taglib1_10/taglib/asf/asfpicture.h"
    "C:/Libraries/taglib1_10/taglib/mp4/mp4file.h"
    "C:/Libraries/taglib1_10/taglib/mp4/mp4atom.h"
    "C:/Libraries/taglib1_10/taglib/mp4/mp4tag.h"
    "C:/Libraries/taglib1_10/taglib/mp4/mp4item.h"
    "C:/Libraries/taglib1_10/taglib/mp4/mp4properties.h"
    "C:/Libraries/taglib1_10/taglib/mp4/mp4coverart.h"
    "C:/Libraries/taglib1_10/taglib/mod/modfilebase.h"
    "C:/Libraries/taglib1_10/taglib/mod/modfile.h"
    "C:/Libraries/taglib1_10/taglib/mod/modtag.h"
    "C:/Libraries/taglib1_10/taglib/mod/modproperties.h"
    "C:/Libraries/taglib1_10/taglib/it/itfile.h"
    "C:/Libraries/taglib1_10/taglib/it/itproperties.h"
    "C:/Libraries/taglib1_10/taglib/s3m/s3mfile.h"
    "C:/Libraries/taglib1_10/taglib/s3m/s3mproperties.h"
    "C:/Libraries/taglib1_10/taglib/xm/xmfile.h"
    "C:/Libraries/taglib1_10/taglib/xm/xmproperties.h"
    )
endif()

