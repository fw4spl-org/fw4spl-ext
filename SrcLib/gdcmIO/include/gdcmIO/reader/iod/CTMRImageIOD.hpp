/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __GDCMIO_READER_DICOM_CTMRIMAGEIOD_HPP__
#define __GDCMIO_READER_DICOM_CTMRIMAGEIOD_HPP__

#include "gdcmIO/reader/iod/InformationObjectDefinition.hpp"

namespace gdcmIO
{
namespace reader
{
namespace iod
{

/**
 * @class CTMRImageIOD
 * @brief CTMRImageIOD class used to read CT & MR Image IODs
 */
class GDCMIO_CLASS_API CTMRImageIOD : public ::gdcmIO::reader::iod::InformationObjectDefinition
{

public :

    /**
     * @brief Constructor
     * @param[in] dicomSeries DicomSeries
     * @param[in] instance Instance
     */
    GDCMIO_API CTMRImageIOD(::fwDicomData::DicomSeries::sptr dicomSeries,
            SPTR(::gdcmIO::container::DicomInstance) instance);

    /// Destructor
    GDCMIO_API ~CTMRImageIOD();

    /**
     * @brief Read a DICOM File
     * @param[in,out] series Series that must be enriched
     */
    GDCMIO_API void read(::fwMedData::Series::sptr series) throw(::gdcmIO::exception::Failed);

private :

    /// Read DICOM image (2D or 3D) from DICOM file(s).
    void readImage(::fwMedData::Series::sptr series) throw(::gdcmIO::exception::Failed);

    /**
     * @brief Read image buffer
     * @return Global raw buffer of the image.
     */
    void* readImageBuffer() throw(::gdcmIO::exception::Failed);

};

}  // namespace iod
}  // namespace reader
}  // namespace gdcmIO

#endif /*__GDCMIO_READER_DICOM_CTMRIMAGEIOD_HPP__*/