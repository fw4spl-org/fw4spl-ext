/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "itkRegistrationOp/Resampler.hpp"

#include <fwItkIO/helper/Transform.hpp>
#include <fwItkIO/itk.hpp>

#include <fwTools/Dispatcher.hpp>
#include <fwTools/DynamicTypeKeyTypeMapping.hpp>
#include <fwTools/IntrinsicTypes.hpp>

#include <itkAffineTransform.h>
#include <itkBSplineInterpolateImageFunction.h>
#include <itkMatrix.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkResampleImageFilter.h>

namespace itkRegistrationOp
{

struct Resampling
{
    struct Parameters
    {
        ::itk::AffineTransform<double, 3>::Pointer i_trf;
        ::fwData::Image::csptr i_image;
        ::fwData::Image::sptr o_image;
        ::fwData::Image::csptr i_targetImage;
    };

    //------------------------------------------------------------------------------

    template<class PIXELTYPE>
    void operator()(Parameters& params)
    {
        typedef typename ::itk::Image< PIXELTYPE, 3 > ImageType;
        const typename ImageType::Pointer itkImage = ::fwItkIO::itkImageFactory< ImageType >(params.i_image);

        typename ::itk::ResampleImageFilter<ImageType, ImageType>::Pointer resampler =
            ::itk::ResampleImageFilter<ImageType, ImageType>::New();

        typename ::itk::MinimumMaximumImageCalculator< ImageType >::Pointer minCalculator =
            ::itk::MinimumMaximumImageCalculator< ImageType >::New();

        minCalculator->SetImage(itkImage);
        minCalculator->ComputeMinimum();
        resampler->SetDefaultPixelValue(minCalculator->GetMinimum());

        resampler->SetTransform(params.i_trf.GetPointer());
        resampler->SetInput(itkImage);

        typename ImageType::SizeType size           = itkImage->GetLargestPossibleRegion().GetSize();
        typename ImageType::PointType origin        = itkImage->GetOrigin();
        typename ImageType::SpacingType spacing     = itkImage->GetSpacing();
        typename ImageType::DirectionType direction = itkImage->GetDirection();

        if(params.i_targetImage)
        {
            for(std::uint8_t i = 0; i < 3; ++i)
            {
                // ITK uses unsigned long to store sizes.
                size[i]    = static_cast<typename ImageType::SizeType::SizeValueType>(params.i_targetImage->getSize()[i]);
                origin[i]  = params.i_targetImage->getOrigin()[i];
                spacing[i] = params.i_targetImage->getSpacing()[i];
            }
        }

        resampler->SetSize(size);
        resampler->SetOutputOrigin(origin);
        resampler->SetOutputDirection(direction);
        resampler->SetOutputSpacing(spacing);

        resampler->Update();

        typename ImageType::Pointer outputImage = resampler->GetOutput();

        ::fwItkIO::itkImageToFwDataImage(outputImage, params.o_image);
    }
};

//-----------------------------------------------------------------------------

void Resampler::resample(const ::fwData::Image::csptr& _inImage,
                         const ::fwData::Image::sptr& _outImage,
                         const ::fwData::TransformationMatrix3D::csptr& _trf,
                         const ::fwData::Image::csptr& _targetImg)
{
    const ::fwTools::DynamicType type          = _inImage->getPixelType();
    const itk::Matrix<double, 4, 4 > itkMatrix = ::fwItkIO::helper::Transform::convertToITK(_trf);

    // We need to extract a 3x3 matrix and a vector to set the affine transform.
    itk::Matrix<double, 3, 3> transformMat;

    for(std::uint8_t i = 0; i < 3; ++i)
    {
        for(std::uint8_t j = 0; j < 3; ++j)
        {
            transformMat(i, j) = itkMatrix(i, j);
        }
    }

    itk::Vector<double, 3> translation;
    for(std::uint8_t i = 0; i < 3; ++i)
    {
        // Extract the last column to get the translation.
        translation.SetElement(i, itkMatrix(i, 3));
    }

    itk::AffineTransform<double, 3>::Pointer transf = itk::AffineTransform<double, 3>::New();
    transf->SetMatrix(transformMat);
    transf->SetTranslation(translation);

    Resampling::Parameters params;
    params.i_image       = _inImage;
    params.o_image       = _outImage;
    params.i_trf         = transf.GetPointer();
    params.i_targetImage = _targetImg;

    ::fwTools::Dispatcher< ::fwTools::IntrinsicTypes, Resampling >::invoke(type, params);
}

//-----------------------------------------------------------------------------

}// namespace itkRegistrationOp