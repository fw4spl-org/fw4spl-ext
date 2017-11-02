/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __ITKREGISTRATIONOP_RESAMPLER_HPP__
#define __ITKREGISTRATIONOP_RESAMPLER_HPP__

#include "itkRegistrationOp/config.hpp"

#include <fwData/Image.hpp>
#include <fwData/TransformationMatrix3D.hpp>

namespace itkRegistrationOp
{
/**
 * @brief The Resampler class
 */
class ITKREGISTRATIONOP_CLASS_API Resampler
{

public:

    /**
     * @brief resample use itk resample image filter to tranform input image to output image using a transform
     * @param[in] _inImg the input fwData::Image.
     * @param[out] _outImg the resulting transformed image.
     * @param[in] _trf the input fwData::TransformationMatrix3D.
     * @param[in] _targetImg image used to set the desired origin, spacing and size.
     */
    static ITKREGISTRATIONOP_API void resample(const ::fwData::Image::csptr& _inImg,
                                               const ::fwData::Image::sptr& outImg,
                                               const ::fwData::TransformationMatrix3D::csptr& _trf,
                                               const ::fwData::Image::csptr& _targetImg = nullptr);

};

}// namespace itkRegistrationOp

#endif // __ITKREGISTRATIONOP_RESAMPLER_HPP__