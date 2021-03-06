#include "middleware/node_editor/noiseutils.h"

#include <algorithm>
#include <mathconsts.h>


using namespace noise;
using namespace noise::model;
using namespace noise::module;
using namespace noise::utils;


//////////////////////////////////////////////////////////////////////////////
// RendererImage class

RendererImage::RendererImage() {
    BuildGrayscaleGradient();
}

void RendererImage::AddGradientPoint(double gradientPos, const math::Color& gradientColor) {
    m_gradient.Add(gradientPos, gradientColor);
}

void RendererImage::BuildGrayscaleGradient() {
    ClearGradient ();
    m_gradient.Add(-1.0, math::Color (  0,   0,   0, 255));
    m_gradient.Add( 1.0, math::Color (255, 255, 255, 255));
}

void RendererImage::BuildTerrainGradient() {
    ClearGradient ();
    m_gradient.Add(-1.00, math::Color (  0,   0, 128, 255));
    m_gradient.Add(-0.20, math::Color ( 32,  64, 128, 255));
    m_gradient.Add(-0.04, math::Color ( 64,  96, 192, 255));
    m_gradient.Add(-0.02, math::Color (192, 192, 128, 255));
    m_gradient.Add( 0.00, math::Color (  0, 192,   0, 255));
    m_gradient.Add( 0.25, math::Color (192, 192,   0, 255));
    m_gradient.Add( 0.50, math::Color (160,  96,  64, 255));
    m_gradient.Add( 0.75, math::Color (128, 255, 255, 255));
    m_gradient.Add( 1.00, math::Color (255, 255, 255, 255));
}

math::Color RendererImage::CalcDestColor(const math::Color& sourceColor, double lightValue) const {
    // color = color * (lightValue * lightColor / 255)
    int red   = static_cast<int>(static_cast<double>(sourceColor.red)   * lightValue * static_cast<double>(m_lightColor.red) / 255.0);
    int green = static_cast<int>(static_cast<double>(sourceColor.green) * lightValue * static_cast<double>(m_lightColor.green) / 255.0);
    int blue  = static_cast<int>(static_cast<double>(sourceColor.blue)  * lightValue * static_cast<double>(m_lightColor.blue) / 255.0);

    return math::Color(
        static_cast<uint8_t>(std::max(std::min(red, 255), 0)),
        static_cast<uint8_t>(std::max(std::min(green, 255), 0)),
        static_cast<uint8_t>(std::max(std::min(blue, 255), 0))
        );
}

double RendererImage::CalcLightIntensity(double /* center */, double left, double right, double down, double up) const {
    // Recalculate the sine and cosine of the various light values if
    // necessary so it does not have to be calculated each time this method is
    // called.
    if (m_recalcLightValues) {
        m_cosAzimuth = cos (m_lightAzimuth * DEG_TO_RAD);
        m_sinAzimuth = sin (m_lightAzimuth * DEG_TO_RAD);
        m_cosElev    = cos (m_lightElev    * DEG_TO_RAD);
        m_sinElev    = sin (m_lightElev    * DEG_TO_RAD);
        m_recalcLightValues = false;
    }

    // Now do the lighting calculations.
    const double I_MAX = 1.0;
    double io = I_MAX * SQRT_2 * m_sinElev / 2.0;
    double ix = (I_MAX - io) * m_lightContrast * SQRT_2 * m_cosElev * m_cosAzimuth;
    double iy = (I_MAX - io) * m_lightContrast * SQRT_2 * m_cosElev * m_sinAzimuth;
    double intensity = (ix * (left - right) + iy * (down - up) + io);
    if (intensity < 0.0) {
        intensity = 0.0;
    }
    return intensity;
}

void RendererImage::ClearGradient() {
    m_gradient.Clear();
}

ImageView RendererImage::Render() {
    if ( m_sourceModule == NULL
        || m_upperUBound <= m_lowerUBound
        || m_upperVBound <= m_lowerVBound
        || m_destWidth <= 0
        || m_destHeight <= 0
        || m_gradient.Count () < 2) {
        throw noise::ExceptionInvalidParam ();
    }

    auto header = m_destImage.view.header;
    if ((header.height != m_destHeight) || (header.width != m_destWidth)) {
            m_destImage.Create(ImageHeader(m_destWidth, m_destHeight, PixelFormat::R8G8B8A8));
    }

    double uDelta  = (m_upperUBound - m_lowerUBound) / static_cast<double>(m_destWidth);
    double vDelta  = (m_upperVBound - m_lowerVBound) / static_cast<double>(m_destHeight);
    double scaledU = m_lowerUBound;
    double scaledV = m_lowerVBound;

    uint32_t* pDest = reinterpret_cast<uint32_t*>(m_destImage.view.data);
    for (uint32_t y=0; y!=m_destHeight; ++y) {
        scaledU = m_lowerUBound;
        for (uint32_t x=0; x!=m_destWidth; ++x) {

            double sourceValue = m_sourceModule->GetValue(scaledU, scaledV);
            // Get the color based on the value at the current point in the noise
            // map.
            math::Color destColor = m_gradient.Get(sourceValue);

            // If lighting is enabled, calculate the light intensity based on the
            // rate of change at the current point in the noise map.
            if (m_isLightEnabled) {
                // Calculate the positions of the current point's four-neighbors.
                int xLeftOffset = -1;
                int xRightOffset = 1;
                int yDownOffset = -1;
                int yUpOffset = 1;

                // Get the noise value of the current point in the source noise map
                // and the noise values of its four-neighbors.
                double nc = sourceValue;
                double nl = m_sourceModule->GetValue(scaledU + xLeftOffset * uDelta, scaledV);
                double nr = m_sourceModule->GetValue(scaledU + xRightOffset * uDelta, scaledV);
                double nd = m_sourceModule->GetValue(scaledU, scaledV + yDownOffset * vDelta);
                double nu = m_sourceModule->GetValue(scaledU, scaledV + yUpOffset * vDelta);

                // Now we can calculate the lighting intensity.
                double lightIntensity = CalcLightIntensity(nc, nl, nr, nd, nu);
                lightIntensity *= m_lightBrightness;
                *pDest = CalcDestColor(destColor, lightIntensity).value;
            } else {
                *pDest = destColor.value;
            }

            // Go to the next point.
            ++pDest;
            scaledU += uDelta;
        }
        scaledV += vDelta;
    }

    return m_destImage.view;
}

//////////////////////////////////////////////////////////////////////////////
// RendererNormalMap class

math::Color RendererNormalMap::CalcNormalColor(double nc, double nr, double nu, double bumpHeight) const {
    // Calculate the surface normal.
    nc *= bumpHeight;
    nr *= bumpHeight;
    nu *= bumpHeight;
    double ncr = (nc - nr);
    double ncu = (nc - nu);
    double d = sqrt ((ncu * ncu) + (ncr * ncr) + 1);
    double vxc = (nc - nr) / d;
    double vyc = (nc - nu) / d;
    double vzc = 1.0 / d;

    // Map the normal range from the (-1.0 .. +1.0) range to the (0 .. 255) range.
    return math::Color(
        static_cast<uint8_t>(static_cast<uint>((floor)((vxc + 1.0) * 127.5)) & 0xff),
        static_cast<uint8_t>(static_cast<uint>((floor)((vyc + 1.0) * 127.5)) & 0xff),
        static_cast<uint8_t>(static_cast<uint>((floor)((vzc + 1.0) * 127.5)) & 0xff));
}

ImageView RendererNormalMap::Render() {
    if ( m_sourceModule == NULL
        || m_upperUBound <= m_lowerUBound
        || m_upperVBound <= m_lowerVBound
        || m_destWidth <= 0
        || m_destHeight <= 0) {
        throw noise::ExceptionInvalidParam ();
    }

    auto header = m_destImage.view.header;
    if ((header.height != m_destHeight) || (header.width != m_destWidth)) {
            m_destImage.Create(ImageHeader(m_destWidth, m_destHeight, PixelFormat::R8G8B8A8));
    }

    double uDelta  = (m_upperUBound - m_lowerUBound) / static_cast<double>(m_destWidth);
    double vDelta  = (m_upperVBound - m_lowerVBound) / static_cast<double>(m_destHeight);
    double scaledU = m_lowerUBound;
    double scaledV = m_lowerVBound;

    uint32_t* pDest = reinterpret_cast<uint32_t*>(m_destImage.view.data);
    for (uint32_t y=0; y!=m_destHeight; ++y) {
        scaledU = m_lowerUBound;
        for (uint32_t x=0; x!=m_destWidth; ++x) {

            // Calculate the positions of the current point's right and up neighbors.
            int xRightOffset = 1;
            int yUpOffset = 1;

            // Get the noise value of the current point in the source noise map
            // and the noise values of its right and up neighbors.
            double nc = m_sourceModule->GetValue(scaledU, scaledV);
            double nr = m_sourceModule->GetValue(scaledU + xRightOffset * uDelta, scaledV);
            double nu = m_sourceModule->GetValue(scaledU, scaledV + yUpOffset * vDelta);

            // Calculate the normal product.
            *pDest = CalcNormalColor (nc, nr, nu, m_bumpHeight).value;

            // Go to the next point.
            ++pDest;
            scaledU += uDelta;
        }
        scaledV += vDelta;
    }

    return m_destImage.view;
}
