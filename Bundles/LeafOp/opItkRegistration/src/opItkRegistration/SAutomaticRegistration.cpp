/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "opItkRegistration/SAutomaticRegistration.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signal.hxx>

#include <fwData/mt/ObjectReadLock.hpp>
#include <fwData/mt/ObjectWriteLock.hpp>

#include <fwServices/macros.hpp>

#include <itkRegistrationOp/AutomaticRegistration.hpp>
#include <itkRegistrationOp/AutomaticRegistrationV4.hpp>

namespace opItkRegistration
{

fwServicesRegisterMacro(::fwServices::IOperator, ::opItkRegistration::SAutomaticRegistration, ::fwData::Image);

static const ::fwServices::IService::KeyType s_TARGET_IN    = "target";
static const ::fwServices::IService::KeyType s_REFERENCE_IN = "reference";

static const ::fwServices::IService::KeyType s_TRANSFORM_INOUT = "transform";

//------------------------------------------------------------------------------

SAutomaticRegistration::SAutomaticRegistration()
{

}

//------------------------------------------------------------------------------

SAutomaticRegistration::~SAutomaticRegistration()
{

}

//------------------------------------------------------------------------------

void SAutomaticRegistration::configuring()
{
    ::fwServices::IService::ConfigType config = this->getConfigTree();

    m_minStep = config.get< double >("minStep", -1.);
    m_maxStep = config.get< double >("maxStep", -1.);

    OSLM_FATAL_IF("Invalid or missing minStep.", m_minStep <= 0);
    OSLM_FATAL_IF("Invalid or missing maxStep.", m_maxStep <= 0);

    m_maxIterations = config.get< unsigned long >("maxIterations", 0);

    OSLM_FATAL_IF("Invalid or missing number of iterations.", m_maxIterations == 0);

    const std::string metric = config.get< std::string >("metric", "");
    setMetric(metric);

    const std::string legacyMode = config.get_optional< std::string >("legacyMode").get_value_or("off");

    OSLM_FATAL_IF("Invalid legacyMode, must be 'on' or 'off'. Here : " << legacyMode,
                  legacyMode != "on" && legacyMode != "off");

    m_legacyMode = (legacyMode == "on");
}

//------------------------------------------------------------------------------

void SAutomaticRegistration::starting()
{

}

//------------------------------------------------------------------------------

void SAutomaticRegistration::updating()
{
    ::fwData::Image::csptr target    = this->getInput< ::fwData::Image >(s_TARGET_IN);
    ::fwData::Image::csptr reference = this->getInput< ::fwData::Image >(s_REFERENCE_IN);

    ::fwData::mt::ObjectReadLock targetLock(target);
    ::fwData::mt::ObjectReadLock refLock(reference);

    ::fwData::TransformationMatrix3D::sptr transform =
        this->getInOut< ::fwData::TransformationMatrix3D >(s_TRANSFORM_INOUT);

    ::fwData::mt::ObjectWriteLock trfLock(transform);

    SLM_ASSERT("No 'target' found !", target);
    SLM_ASSERT("No 'reference' found !", reference);
    SLM_ASSERT("No 'transform' found !", transform);

    if(m_legacyMode)
    {
        ::itkRegistrationOp::AutomaticRegistration::registerImage(target, reference, transform, m_metric, m_minStep,
                                                                  m_maxStep, m_maxIterations);

    }
    else
    {
        ::itkRegistrationOp::AutomaticRegistrationV4::registerImage(target, reference, transform, m_metric, m_minStep,
                                                                    m_maxIterations);
    }

    m_sigComputed->asyncEmit();

    auto transfoModifiedSig = transform->signal< ::fwData::TransformationMatrix3D::ModifiedSignalType >
                                  (::fwData::TransformationMatrix3D::s_MODIFIED_SIG);

    transfoModifiedSig->asyncEmit();
}

//------------------------------------------------------------------------------

void SAutomaticRegistration::stopping()
{

}

//------------------------------------------------------------------------------

::fwServices::IService::KeyConnectionsMap SAutomaticRegistration::getAutoConnections() const
{
    ::fwServices::IService::KeyConnectionsMap connections;
    connections.push(s_TARGET_IN, ::fwData::Image::s_MODIFIED_SIG, s_UPDATE_SLOT);
    connections.push(s_TARGET_IN, ::fwData::Image::s_BUFFER_MODIFIED_SIG, s_UPDATE_SLOT);
    connections.push(s_REFERENCE_IN, ::fwData::Image::s_MODIFIED_SIG, s_UPDATE_SLOT);
    connections.push(s_REFERENCE_IN, ::fwData::Image::s_BUFFER_MODIFIED_SIG, s_UPDATE_SLOT);
    connections.push(s_TRANSFORM_INOUT, ::fwData::TransformationMatrix3D::s_MODIFIED_SIG, s_UPDATE_SLOT);

    return connections;
}

//------------------------------------------------------------------------------

void SAutomaticRegistration::setEnumParameter(std::string val, std::string key)
{
    if(key == "metric")
    {
        setMetric(val);
    }
    else
    {
        OSLM_FATAL("Key must be 'metric', unknown key :" << key);
    }
}

//------------------------------------------------------------------------------

void SAutomaticRegistration::setDoubleParameter(double val, std::string key)
{
    if(key == "minStep")
    {
        m_minStep = val;
    }
    else if(key == "maxStep")
    {
        m_maxStep = val;

        OSLM_WARN_IF("'maxStep' is useless in non-legacy (v4) mode.", !m_legacyMode);
    }
    else
    {
        OSLM_FATAL("Unknown key : " << key);
    }
}

//------------------------------------------------------------------------------

void SAutomaticRegistration::setIntParameter(int val, std::string key)
{
    if(key == "maxIterations")
    {
        OSLM_FATAL_IF("The number of iterations must be greater than 0 !!", val <= 0);
        m_maxIterations = static_cast<unsigned long>(val);
    }
}

//------------------------------------------------------------------------------

void SAutomaticRegistration::setMetric(const std::string& metricName)
{
    if(metricName == "MeanSquares")
    {
        m_metric = ::itkRegistrationOp::MEAN_SQUARES;
    }
    else if(metricName == "NormalizedCorrelation")
    {
        m_metric = ::itkRegistrationOp::NORMALIZED_CORRELATION;
    }
    else if(metricName == "MutualInformation")
    {
        m_metric = ::itkRegistrationOp::MUTUAL_INFORMATION;
    }
    else
    {
        OSLM_FATAL("Unknown metric: " << metricName);
    }
}

//------------------------------------------------------------------------------

} // namespace opItkRegistration