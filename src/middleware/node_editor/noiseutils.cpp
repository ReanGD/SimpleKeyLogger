#include "middleware/node_editor/noiseutils.h"

#include <interp.h>
#include <mathconsts.h>


using namespace noise;
using namespace noise::model;
using namespace noise::module;

//////////////////////////////////////////////////////////////////////////////
// Miscellaneous functions

namespace noise {
    namespace utils {
        // Performs linear interpolation between two 8-bit channel values.
        inline noise::uint8 BlendChannel (const uint8 channel0, const uint8 channel1, float alpha) {
            float c0 = (float)channel0 / 255.0f;
            float c1 = (float)channel1 / 255.0f;
            return (noise::uint8)(((c1 * alpha) + (c0 * (1.0f - alpha))) * 255.0f);
        }

        // Performs linear interpolation between two colors and stores the result
        // in out.
        inline void LinearInterpColor(const math::Color& color0, const math::Color& color1, float alpha, math::Color& out) {
            out.alpha = BlendChannel(color0.alpha, color1.alpha, alpha);
            out.blue  = BlendChannel(color0.blue , color1.blue , alpha);
            out.green = BlendChannel(color0.green, color1.green, alpha);
            out.red   = BlendChannel(color0.red  , color1.red  , alpha);
        }

        inline uint32_t ClampValue(uint32_t value, uint32_t lowerBound, uint32_t upperBound) {
            if (value < lowerBound) {
                return lowerBound;
            } else if (value > upperBound) {
                return upperBound;
            } else {
                return value;
            }
        }
    }
}

using namespace noise;

using namespace noise::utils;

//////////////////////////////////////////////////////////////////////////////
// GradientColor class

GradientColor::~GradientColor() {
    if (m_pGradientPoints != nullptr) {
        delete[] m_pGradientPoints;
        m_pGradientPoints = nullptr;
    }
}

void GradientColor::AddGradientPoint(double gradientPos, const math::Color& gradientColor) {
    // Find the insertion point for the new gradient point and insert the new
    // gradient point at that insertion point.  The gradient point array will
    // remain sorted by gradient position.
    uint32_t insertionPos = FindInsertionPos(gradientPos);
    InsertAtPos(insertionPos, gradientPos, gradientColor);
}

void GradientColor::Clear() {
    if (m_pGradientPoints != nullptr) {
        delete[] m_pGradientPoints;
        m_pGradientPoints = nullptr;
    }
    m_gradientPointCount = 0;
}

uint32_t GradientColor::FindInsertionPos(double gradientPos) {
    uint32_t insertionPos;
    for (insertionPos=0; insertionPos!=m_gradientPointCount; ++insertionPos) {
        if (gradientPos < m_pGradientPoints[insertionPos].pos) {
            // We found the array index in which to insert the new gradient point.
            // Exit now.
            break;
        } else if (gradientPos == m_pGradientPoints[insertionPos].pos) {
            // Each gradient point is required to contain a unique gradient
            // position, so throw an exception.
            throw noise::ExceptionInvalidParam ();
        }
    }
    return insertionPos;
}

const math::Color& GradientColor::GetColor(double gradientPos) const {
    assert(m_gradientPointCount >= 2);

    // Find the first element in the gradient point array that has a gradient
    // position larger than the gradient position passed to this method.
    uint32_t indexPos;
    for (indexPos=0; indexPos!=m_gradientPointCount; ++indexPos) {
        if (gradientPos < m_pGradientPoints[indexPos].pos) {
            break;
        }
    }

    // Find the two nearest gradient points so that we can perform linear
    // interpolation on the color.
    uint32_t index0 = ClampValue(indexPos - 1, 0, m_gradientPointCount - 1);
    uint32_t index1 = ClampValue(indexPos    , 0, m_gradientPointCount - 1);

    // If some gradient points are missing (which occurs if the gradient
    // position passed to this method is greater than the largest gradient
    // position or less than the smallest gradient position in the array), get
    // the corresponding gradient color of the nearest gradient point and exit
    // now.
    if (index0 == index1) {
        m_workingColor = m_pGradientPoints[index1].color;
        return m_workingColor;
    }

    // Compute the alpha value used for linear interpolation.
    double input0 = m_pGradientPoints[index0].pos;
    double input1 = m_pGradientPoints[index1].pos;
    double alpha = (gradientPos - input0) / (input1 - input0);

    // Now perform the linear interpolation given the alpha value.
    const math::Color& color0 = m_pGradientPoints[index0].color;
    const math::Color& color1 = m_pGradientPoints[index1].color;
    LinearInterpColor(color0, color1, (float)alpha, m_workingColor);
    return m_workingColor;
}

void GradientColor::InsertAtPos(uint32_t insertionPos, double gradientPos, const math::Color& gradientColor) {
    // Make room for the new gradient point at the specified insertion position
    // within the gradient point array.  The insertion position is determined by
    // the gradient point's position; the gradient points must be sorted by
    // gradient position within that array.
    GradientPoint* newGradientPoints;
    newGradientPoints = new GradientPoint[m_gradientPointCount + 1];
    for (uint32_t i=0; i!=m_gradientPointCount; ++i) {
        if (i < insertionPos) {
            newGradientPoints[i] = m_pGradientPoints[i];
        } else {
            newGradientPoints[i + 1] = m_pGradientPoints[i];
        }
    }
    if (m_pGradientPoints) {
        delete[] m_pGradientPoints;
    }
    m_pGradientPoints = newGradientPoints;
    ++m_gradientPointCount;

    // Now that we've made room for the new gradient point within the array, add
    // the new gradient point.
    m_pGradientPoints[insertionPos].pos = gradientPos ;
    m_pGradientPoints[insertionPos].color = gradientColor;
}

//////////////////////////////////////////////////////////////////////////////
// RendererImage class

RendererImage::RendererImage() {
    BuildGrayscaleGradient();
}

void RendererImage::AddGradientPoint (double gradientPos,
    const math::Color& gradientColor)
{
    m_gradient.AddGradientPoint (gradientPos, gradientColor);
}

void RendererImage::BuildGrayscaleGradient ()
{
    ClearGradient ();
    m_gradient.AddGradientPoint (-1.0, math::Color (  0,   0,   0, 255));
    m_gradient.AddGradientPoint ( 1.0, math::Color (255, 255, 255, 255));
}

void RendererImage::BuildTerrainGradient ()
{
    ClearGradient ();
    m_gradient.AddGradientPoint (-1.00, math::Color (  0,   0, 128, 255));
    m_gradient.AddGradientPoint (-0.20, math::Color ( 32,  64, 128, 255));
    m_gradient.AddGradientPoint (-0.04, math::Color ( 64,  96, 192, 255));
    m_gradient.AddGradientPoint (-0.02, math::Color (192, 192, 128, 255));
    m_gradient.AddGradientPoint ( 0.00, math::Color (  0, 192,   0, 255));
    m_gradient.AddGradientPoint ( 0.25, math::Color (192, 192,   0, 255));
    m_gradient.AddGradientPoint ( 0.50, math::Color (160,  96,  64, 255));
    m_gradient.AddGradientPoint ( 0.75, math::Color (128, 255, 255, 255));
    m_gradient.AddGradientPoint ( 1.00, math::Color (255, 255, 255, 255));
}

math::Color RendererImage::CalcDestColor(const math::Color& sourceColor, double lightValue) const {
    double red   = (double)sourceColor.red   / 255.0;
    double green = (double)sourceColor.green / 255.0;
    double blue  = (double)sourceColor.blue  / 255.0;

    if (m_isLightEnabled) {
        // Now calculate the light color.
        double lightRed   = lightValue * (double)m_lightColor.red   / 255.0;
        double lightGreen = lightValue * (double)m_lightColor.green / 255.0;
        double lightBlue  = lightValue * (double)m_lightColor.blue  / 255.0;

        // Apply the light color to the new color.
        red   *= lightRed  ;
        green *= lightGreen;
        blue  *= lightBlue ;
    }

    // Clamp the color channels to the (0..1) range.
    red   = (red   < 0.0)? 0.0: red  ;
    red   = (red   > 1.0)? 1.0: red  ;
    green = (green < 0.0)? 0.0: green;
    green = (green > 1.0)? 1.0: green;
    blue  = (blue  < 0.0)? 0.0: blue ;
    blue  = (blue  > 1.0)? 1.0: blue ;

    // Rescale the color channels to the noise::uint8 (0..255) range and return
    // the new color.
    math::Color newColor (
        (noise::uint8)((noise::uint)(red   * 255.0) & 0xff),
        (noise::uint8)((noise::uint)(green * 255.0) & 0xff),
        (noise::uint8)((noise::uint)(blue  * 255.0) & 0xff));
    return newColor;
}

double RendererImage::CalcLightIntensity (double /* center */, double left,
    double right, double down, double up) const
{
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
    double ix = (I_MAX - io) * m_lightContrast * SQRT_2 * m_cosElev
        * m_cosAzimuth;
    double iy = (I_MAX - io) * m_lightContrast * SQRT_2 * m_cosElev
        * m_sinAzimuth;
    double intensity = (ix * (left - right) + iy * (down - up) + io);
    if (intensity < 0.0) {
        intensity = 0.0;
    }
    return intensity;
}

void RendererImage::ClearGradient () {
    m_gradient.Clear();
}

ImageView RendererImage::Render() {
    if ( m_sourceModule == NULL
        || m_upperUBound <= m_lowerUBound
        || m_upperVBound <= m_lowerVBound
        || m_destWidth <= 0
        || m_destHeight <= 0
        || m_gradient.GetGradientPointCount () < 2) {
        throw noise::ExceptionInvalidParam ();
    }

    auto header = m_destImage.view.header;
    if ((header.height != m_destHeight) || (header.width != m_destWidth)) {
            m_destImage.Create(ImageHeader(m_destWidth, m_destHeight, PixelFormat::R8G8B8A8));
    }

    double uExtent = m_upperUBound - m_lowerUBound;
    double vExtent = m_upperVBound - m_lowerVBound;
    double uDelta  = uExtent / (double)m_destWidth;
    double vDelta  = vExtent / (double)m_destHeight;
    double scaledU = m_lowerUBound;
    double scaledV = m_lowerVBound;

    uint32_t* pDest = reinterpret_cast<uint32_t*>(m_destImage.view.data);
    for (uint32_t y=0; y!=m_destHeight; ++y) {
        scaledU = m_lowerUBound;
        for (uint32_t x=0; x!=m_destWidth; ++x) {

            double sourceValue = m_sourceModule->GetValue(scaledU, scaledV);
            // Get the color based on the value at the current point in the noise
            // map.
            math::Color destColor = m_gradient.GetColor(sourceValue);

            // If lighting is enabled, calculate the light intensity based on the
            // rate of change at the current point in the noise map.
            double lightIntensity;
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
                lightIntensity = CalcLightIntensity(nc, nl, nr, nd, nu);
                lightIntensity *= m_lightBrightness;

            } else {
                // These values will apply no lighting to the destination image.
                lightIntensity = 1;
            }

            // Blend the destination color, background color, and the light
            // intensity together, then update the destination image with that
            // color.
            *pDest = CalcDestColor(destColor, lightIntensity).value;

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

math::Color RendererNormalMap::CalcNormalColor (double nc, double nr, double nu, double bumpHeight) const {
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
    noise::uint8 xc, yc, zc;
    xc = (noise::uint8)((noise::uint)((floor)((vxc + 1.0) * 127.5)) & 0xff);
    yc = (noise::uint8)((noise::uint)((floor)((vyc + 1.0) * 127.5)) & 0xff);
    zc = (noise::uint8)((noise::uint)((floor)((vzc + 1.0) * 127.5)) & 0xff);

    return math::Color (xc, yc, zc);
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

    double uExtent = m_upperUBound - m_lowerUBound;
    double vExtent = m_upperVBound - m_lowerVBound;
    double uDelta  = uExtent / (double)m_destWidth;
    double vDelta  = vExtent / (double)m_destHeight;
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
