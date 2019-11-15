#pragma once
// noiseutils.h
//
// Copyright (C) 2003-2005 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//


#include "engine/gui/math.h"
#include "engine/material/image.h"
#include "middleware/node_editor/noise_2d.h"

using namespace noise;

namespace noise {
    namespace utils {
        /// Defines a point used to build a color gradient.
        ///
        /// A color gradient is a list of gradually-changing colors.  A color
        /// gradient is defined by a list of <i>gradient points</i>.  Each
        /// gradient point has a position and a color.  In a color gradient, the
        /// colors between two adjacent gradient points are linearly interpolated.
        ///
        /// The ColorGradient class defines a color gradient by a list of these
        /// objects.
        struct GradientPoint {
            /// The position of this gradient point.
            double pos;
            /// The color of this gradient point.
            math::Color color;
        };

        /// Defines a color gradient.
        ///
        /// A color gradient is a list of gradually-changing colors.  A color
        /// gradient is defined by a list of <i>gradient points</i>.  Each
        /// gradient point has a position and a color.  In a color gradient, the
        /// colors between two adjacent gradient points are linearly interpolated.
        ///
        /// To add a gradient point to the color gradient, pass its position and
        /// color to the AddGradientPoint() method.
        ///
        /// To retrieve a color from a specific position in the color gradient,
        /// pass that position to the GetColor() method.
        ///
        /// This class is a useful tool for coloring height maps based on
        /// elevation.
        ///
        /// <b>Gradient example</b>
        ///
        /// Suppose a gradient object contains the following gradient points:
        /// - -1.0 maps to black.
        /// - 0.0 maps to white.
        /// - 1.0 maps to red.
        ///
        /// If an application passes -0.5 to the GetColor() method, this method
        /// will return a gray color that is halfway between black and white.
        ///
        /// If an application passes 0.25 to the GetColor() method, this method
        /// will return a very light pink color that is one quarter of the way
        /// between white and red.
        class GradientColor {
        public:
                GradientColor() = default;
                ~GradientColor();

                /// Adds a gradient point to this gradient object.
                ///
                /// @param gradientPos The position of this gradient point.
                /// @param gradientColor The color of this gradient point.
                ///
                /// @pre No two gradient points have the same position.
                ///
                /// @throw noise::ExceptionInvalidParam See the precondition.
                ///
                /// It does not matter which order these gradient points are added.
                void AddGradientPoint(double gradientPos, const math::Color& gradientColor);

                /// Deletes all the gradient points from this gradient object.
                ///
                /// @post All gradient points from this gradient object are deleted.
                void Clear();

                /// Returns the color at the specified position in the color gradient.
                ///
                /// @param gradientPos The specified position.
                ///
                /// @returns The color at that position.
                const math::Color& GetColor(double gradientPos) const;

                /// Returns a pointer to the array of gradient points in this object.
                ///
                /// @returns A pointer to the array of gradient points.
                ///
                /// Before calling this method, call GetGradientPointCount() to
                /// determine the number of gradient points in this array.
                ///
                /// It is recommended that an application does not store this pointer
                /// for later use since the pointer to the array may change if the
                /// application calls another method of this object.
                const GradientPoint* GetGradientPointArray() const {
                    return m_pGradientPoints;
                }

                /// Returns the number of gradient points stored in this object.
                ///
                /// @returns The number of gradient points stored in this object.
                uint32_t GetGradientPointCount() const {
                    return m_gradientPointCount;
                }

            private:

                /// Determines the array index in which to insert the gradient point
                /// into the internal gradient-point array.
                ///
                /// @param gradientPos The position of this gradient point.
                ///
                /// @returns The array index in which to insert the gradient point.
                ///
                /// @pre No two gradient points have the same input value.
                ///
                /// @throw noise::ExceptionInvalidParam See the precondition.
                ///
                /// By inserting the gradient point at the returned array index, this
                /// object ensures that the gradient-point array is sorted by input
                /// value.  The code that maps a value to a color requires a sorted
                /// gradient-point array.
                uint32_t FindInsertionPos(double gradientPos);

                /// Inserts the gradient point at the specified position in the
                /// internal gradient-point array.
                ///
                /// @param insertionPos The zero-based array position in which to
                /// insert the gradient point.
                /// @param gradientPos The position of this gradient point.
                /// @param gradientColor The color of this gradient point.
                ///
                /// To make room for this new gradient point, this method reallocates
                /// the gradient-point array and shifts all gradient points occurring
                /// after the insertion position up by one.
                ///
                /// Because this object requires that all gradient points in the array
                /// must be sorted by the position, the new gradient point should be
                /// inserted at the position in which the order is still preserved.
                void InsertAtPos(uint32_t insertionPos, double gradientPos, const math::Color& gradientColor);

                /// Number of gradient points.
                uint32_t m_gradientPointCount;

                /// Array that stores the gradient points.
                GradientPoint* m_pGradientPoints = nullptr;

                /// A color object that is used by a gradient object to store a
                /// temporary value.
                mutable math::Color m_workingColor;
        };

        class RendererImage {
        public:
                RendererImage();

                /// Adds a gradient point to this gradient object.
                ///
                /// @param gradientPos The position of this gradient point.
                /// @param gradientColor The color of this gradient point.
                ///
                /// @pre No two gradient points have the same position.
                ///
                /// @throw noise::ExceptionInvalidParam See the preconditions.
                ///
                /// This object uses a color gradient to calculate the color for each
                /// pixel in the destination image according to the value from the
                /// corresponding position in the noise map.
                ///
                /// The gradient requires a minimum of two gradient points.
                ///
                /// The specified color value passed to this method has an alpha
                /// channel.  This alpha channel specifies how a pixel in the
                /// background image (if specified) is blended with the calculated
                /// color.  If the alpha value is high, this object weighs the blend
                /// towards the calculated color, and if the alpha value is low, this
                /// object weighs the blend towards the color from the corresponding
                /// pixel in the background image.
                void AddGradientPoint(double gradientPos, const math::Color& gradientColor);

                /// Builds a grayscale gradient.
                ///
                /// @post The original gradient is cleared and a grayscale gradient is
                /// created.
                ///
                /// This color gradient contains the following gradient points:
                /// - -1.0 maps to black
                /// - 1.0 maps to white
                void BuildGrayscaleGradient();

                /// Builds a color gradient suitable for terrain.
                ///
                /// @post The original gradient is cleared and a terrain gradient is
                /// created.
                ///
                /// This gradient color at position 0.0 is the "sea level".  Above
                /// that value, the gradient contains greens, browns, and whites.
                /// Below that value, the gradient contains various shades of blue.
                void BuildTerrainGradient();

                /// Clears the color gradient.
                ///
                /// Before calling the Render() method, the application must specify a
                /// new color gradient with at least two gradient points.
                void ClearGradient();

                /// Enables or disables the light source.
                ///
                /// @param enable A flag that enables or disables the light source.
                ///
                /// If the light source is enabled, this object will interpret the
                /// noise map as a bump map.
                void EnableLight(bool enable = true) {
                    m_isLightEnabled = enable;
                }

                /// Returns the azimuth of the light source, in degrees.
                ///
                /// @returns The azimuth of the light source.
                ///
                /// The azimuth is the location of the light source around the
                /// horizon:
                /// - 0.0 degrees is east.
                /// - 90.0 degrees is north.
                /// - 180.0 degrees is west.
                /// - 270.0 degrees is south.
                double GetLightAzimuth() const {
                    return m_lightAzimuth;
                }

                /// Returns the brightness of the light source.
                ///
                /// @returns The brightness of the light source.
                double GetLightBrightness() const {
                    return m_lightBrightness;
                }

                /// Returns the color of the light source.
                ///
                /// @returns The color of the light source.
                math::Color GetLightColor() const {
                    return m_lightColor;
                }

                /// Returns the contrast of the light source.
                ///
                /// @returns The contrast of the light source.
                ///
                /// The contrast specifies how sharp the boundary is between the
                /// light-facing areas and the shadowed areas.
                ///
                /// The contrast determines the difference between areas in light and
                /// areas in shadow.  Determining the correct contrast amount requires
                /// some trial and error, but if your application interprets the noise
                /// map as a height map that has a spatial resolution of @a h meters
                /// and an elevation resolution of 1 meter, a good contrast amount to
                /// use is ( 1.0 / @a h ).
                double GetLightContrast() const {
                    return m_lightContrast;
                }

                /// Returns the elevation of the light source, in degrees.
                ///
                /// @returns The elevation of the light source.
                ///
                /// The elevation is the angle above the horizon:
                /// - 0 degrees is on the horizon.
                /// - 90 degrees is straight up.
                double GetLightElev() const {
                    return m_lightElev;
                }

                /// Returns the intensity of the light source.
                ///
                /// @returns The intensity of the light source.
                double GetLightIntensity() const {
                    return m_lightIntensity;
                }

                /// Determines if the light source is enabled.
                ///
                /// @returns
                /// - @a true if the light source is enabled.
                /// - @a false if the light source is disabled.
                bool IsLightEnabled() const {
                    return m_isLightEnabled;
                }

                /// Renders the destination image using the contents of the source
                /// noise map and an optional background image.
                ///
                /// @pre SetSourceNoiseMap() has been previously called.
                /// @pre There are at least two gradient points in the color gradient.
                /// @pre No two gradient points have the same position.
                /// @pre If a background image was specified, it has the exact same
                /// size as the source height map.
                ///
                /// @post The original contents of the destination image is destroyed.
                ///
                /// @throw noise::ExceptionInvalidParam See the preconditions.
                ///
                /// The background image and the destination image can safely refer to
                /// the same image, although in this case, the destination image is
                /// irretrievably blended into the background image.
                ImageView Render();

                void SetBounds(double lowerUBound, double upperUBound, double lowerVBound, double upperVBound) {
                    if (lowerUBound >= upperUBound || lowerVBound >= upperVBound) {
                        throw noise::ExceptionInvalidParam ();
                    }

                    m_lowerUBound = lowerUBound;
                    m_upperUBound = upperUBound;
                    m_lowerVBound = lowerVBound;
                    m_upperVBound = upperVBound;
                }

                void SetDestSize(uint32_t destWidth, uint32_t destHeight) {
                    m_destWidth = destWidth;
                    m_destHeight = destHeight;
                }

                /// Sets the azimuth of the light source, in degrees.
                ///
                /// @param lightAzimuth The azimuth of the light source.
                ///
                /// The azimuth is the location of the light source around the
                /// horizon:
                /// - 0.0 degrees is east.
                /// - 90.0 degrees is north.
                /// - 180.0 degrees is west.
                /// - 270.0 degrees is south.
                ///
                /// Make sure the light source is enabled via a call to the
                /// EnableLight() method before calling the Render() method.
                void SetLightAzimuth(double lightAzimuth) {
                    m_lightAzimuth = lightAzimuth;
                    m_recalcLightValues = true;
                }

                /// Sets the brightness of the light source.
                ///
                /// @param lightBrightness The brightness of the light source.
                ///
                /// Make sure the light source is enabled via a call to the
                /// EnableLight() method before calling the Render() method.
                void SetLightBrightness(double lightBrightness) {
                    m_lightBrightness = lightBrightness;
                    m_recalcLightValues = true;
                }

                /// Sets the color of the light source.
                ///
                /// @param lightColor The light color.
                ///
                /// Make sure the light source is enabled via a call to the
                /// EnableLight() method before calling the Render() method.
                void SetLightColor(const math::Color& lightColor) {
                    m_lightColor = lightColor;
                }

                /// Sets the contrast of the light source.
                ///
                /// @param lightContrast The contrast of the light source.
                ///
                /// @pre The specified light contrast is positive.
                ///
                /// @throw noise::ExceptionInvalidParam See the preconditions.
                ///
                /// The contrast specifies how sharp the boundary is between the
                /// light-facing areas and the shadowed areas.
                ///
                /// The contrast determines the difference between areas in light and
                /// areas in shadow.  Determining the correct contrast amount requires
                /// some trial and error, but if your application interprets the noise
                /// map as a height map that has a spatial resolution of @a h meters
                /// and an elevation resolution of 1 meter, a good contrast amount to
                /// use is ( 1.0 / @a h ).
                ///
                /// Make sure the light source is enabled via a call to the
                /// EnableLight() method before calling the Render() method.
                void SetLightContrast(double lightContrast) {
                    if (lightContrast <= 0.0) {
                        throw noise::ExceptionInvalidParam ();
                    }

                    m_lightContrast = lightContrast;
                    m_recalcLightValues = true;
                }

                /// Sets the elevation of the light source, in degrees.
                ///
                /// @param lightElev The elevation of the light source.
                ///
                /// The elevation is the angle above the horizon:
                /// - 0 degrees is on the horizon.
                /// - 90 degrees is straight up.
                ///
                /// Make sure the light source is enabled via a call to the
                /// EnableLight() method before calling the Render() method.
                void SetLightElev(double lightElev) {
                    m_lightElev = lightElev;
                    m_recalcLightValues = true;
                }

                /// Returns the intensity of the light source.
                ///
                /// @returns The intensity of the light source.
                ///
                /// A good value for intensity is 2.0.
                ///
                /// Make sure the light source is enabled via a call to the
                /// EnableLight() method before calling the Render() method.
                void SetLightIntensity(double lightIntensity) {
                    if (lightIntensity < 0.0) {
                        throw noise::ExceptionInvalidParam ();
                    }

                    m_lightIntensity = lightIntensity;
                    m_recalcLightValues = true;
                }

                void SetSourceModule(const BaseNoise2DNode* sourceModule) {
                    m_sourceModule = sourceModule;
                }

            private:

                /// Calculates the destination color.
                ///
                /// @param sourceColor The source color generated from the color
                /// gradient.
                /// @param lightValue The intensity of the light at that position.
                ///
                /// @returns The destination color.
                math::Color CalcDestColor(const math::Color& sourceColor, double lightValue) const;

                /// Calculates the intensity of the light given some elevation values.
                ///
                /// @param center Elevation of the center point.
                /// @param left Elevation of the point directly left of the center
                /// point.
                /// @param right Elevation of the point directly right of the center
                /// point.
                /// @param down Elevation of the point directly below the center
                /// point.
                /// @param up Elevation of the point directly above the center point.
                ///
                /// These values come directly from the noise map.
                double CalcLightIntensity(double center, double left, double right, double down, double up) const;

                /// The cosine of the azimuth of the light source.
                mutable double m_cosAzimuth;

                /// The cosine of the elevation of the light source.
                mutable double m_cosElev;

                /// The color gradient used to specify the image colors.
                GradientColor m_gradient;

                /// A flag specifying whether lighting is enabled.
                bool m_isLightEnabled = false;

                /// The azimuth of the light source, in degrees.
                double m_lightAzimuth = 45.0;

                /// The brightness of the light source.
                double m_lightBrightness = 1.0;

                /// The color of the light source.
                math::Color m_lightColor = math::Color(255);

                /// The contrast between areas in light and areas in shadow.
                double m_lightContrast = 1.0;

                /// The elevation of the light source, in degrees.
                double m_lightElev = 45.0;

                /// The intensity of the light source.
                double m_lightIntensity = 1.0;

                /// Lower x boundary of the planar noise map, in units.
                /// Southern boundary of the spherical noise map, in degrees.
                /// Lower angle boundary of the cylindrical noise map, in degrees.
                double m_lowerUBound = 0;

                /// Lower z boundary of the planar noise map, in units.
                /// Western boundary of the spherical noise map, in degrees.
                /// Lower height boundary of the cylindrical noise map, in units.
                double m_lowerVBound = 0;

                /// Upper x boundary of the planar noise map, in units.
                /// Eastern boundary of the spherical noise map, in degrees.
                /// Upper angle boundary of the cylindrical noise map, in degrees.
                double m_upperUBound = 0;

                /// Upper z boundary of the planar noise map, in units.
                /// Northern boundary of the spherical noise map, in degrees.
                /// Upper height boundary of the cylindrical noise map, in units.
                double m_upperVBound = 0;

                /// Height of the destination noise map, in points.
                uint32_t m_destHeight = 0;

                /// Width of the destination noise map, in points.
                uint32_t m_destWidth = 0;

                Image m_destImage;

                /// A pointer to the source noise map.
                const BaseNoise2DNode* m_sourceModule = nullptr;

                /// Used by the CalcLightIntensity() method to recalculate the light
                /// values only if the light parameters change.
                ///
                /// When the light parameters change, this value is set to True.  When
                /// the CalcLightIntensity() method is called, this value is set to
                /// false.
                mutable bool m_recalcLightValues = true;

                /// The sine of the azimuth of the light source.
                mutable double m_sinAzimuth;

                /// The sine of the elevation of the light source.
                mutable double m_sinElev;
        };

        /// Renders a normal map from a noise map.
        ///
        /// This class renders an image containing the normal vectors from a noise
        /// map object.  This image can then be used as a bump map for a 3D
        /// application or game.
        ///
        /// This class encodes the (x, y, z) components of the normal vector into
        /// the (red, green, blue) channels of the image.  Like any 24-bit
        /// true-color image, the channel values range from 0 to 255.  0
        /// represents a normal coordinate of -1.0 and 255 represents a normal
        /// coordinate of +1.0.
        ///
        /// You should also specify the <i>bump height</i> before rendering the
        /// normal map.  The bump height specifies the ratio of spatial resolution
        /// to elevation resolution.  For example, if your noise map has a spatial
        /// resolution of 30 meters and an elevation resolution of one meter, set
        /// the bump height to 1.0 / 30.0.
        ///
        /// <b>Rendering the normal map</b>
        ///
        /// To render the image containing the normal map, perform the following
        /// steps:
        /// - Pass a NoiseMap object to the SetSourceNoiseMap() method.
        /// - Pass an ImageOld object to the SetDestImage() method.
        /// - Call the Render() method.
        class RendererNormalMap {
            public:
                RendererNormalMap () = default;

                /// Returns the bump height.
                ///
                /// @returns The bump height.
                ///
                /// The bump height specifies the ratio of spatial resolution to
                /// elevation resolution.  For example, if your noise map has a
                /// spatial resolution of 30 meters and an elevation resolution of one
                /// meter, set the bump height to 1.0 / 30.0.
                ///
                /// The spatial resolution and elevation resolution are determined by
                /// the application.
                double GetBumpHeight() const {
                    return m_bumpHeight;
                }

                /// Renders the noise map to the destination image.
                ///
                /// @pre SetSourceNoiseMap() has been previously called.
                ///
                /// @post The original contents of the destination image is destroyed.
                ///
                /// @throw noise::ExceptionInvalidParam See the preconditions.
                ImageView Render();

                /// Sets the bump height.
                ///
                /// @param bumpHeight The bump height.
                ///
                /// The bump height specifies the ratio of spatial resolution to
                /// elevation resolution.  For example, if your noise map has a
                /// spatial resolution of 30 meters and an elevation resolution of one
                /// meter, set the bump height to 1.0 / 30.0.
                ///
                /// The spatial resolution and elevation resolution are determined by
                /// the application.
                void SetBumpHeight(double bumpHeight) {
                    m_bumpHeight = bumpHeight;
                }

                void SetBounds(double lowerUBound, double upperUBound, double lowerVBound, double upperVBound) {
                    if (lowerUBound >= upperUBound || lowerVBound >= upperVBound) {
                        throw noise::ExceptionInvalidParam ();
                    }

                    m_lowerUBound = lowerUBound;
                    m_upperUBound = upperUBound;
                    m_lowerVBound = lowerVBound;
                    m_upperVBound = upperVBound;
                }

                void SetDestSize(uint32_t destWidth, uint32_t destHeight) {
                    m_destWidth = destWidth;
                    m_destHeight = destHeight;
                }

                void SetSourceModule(const BaseNoise2DNode& sourceModule) {
                    m_sourceModule = &sourceModule;
                }

            private:

                /// Calculates the normal vector at a given point on the noise map.
                ///
                /// @param nc The height of the given point in the noise map.
                /// @param nr The height of the left neighbor.
                /// @param nu The height of the up neighbor.
                /// @param bumpHeight The bump height.
                ///
                /// @returns The normal vector represented as a color.
                ///
                /// This method encodes the (x, y, z) components of the normal vector
                /// into the (red, green, blue) channels of the returned color.  In
                /// order to represent the vector as a color, each coordinate of the
                /// normal is mapped from the -1.0 to 1.0 range to the 0 to 255 range.
                ///
                /// The bump height specifies the ratio of spatial resolution to
                /// elevation resolution.  For example, if your noise map has a
                /// spatial resolution of 30 meters and an elevation resolution of one
                /// meter, set the bump height to 1.0 / 30.0.
                ///
                /// The spatial resolution and elevation resolution are determined by
                /// the application.
                math::Color CalcNormalColor (double nc, double nr, double nu, double bumpHeight) const;

                /// The bump height for the normal map.
                double m_bumpHeight = 1.0;

                Image m_destImage;

                /// Lower x boundary of the planar noise map, in units.
                /// Southern boundary of the spherical noise map, in degrees.
                /// Lower angle boundary of the cylindrical noise map, in degrees.
                double m_lowerUBound = 0;

                /// Lower z boundary of the planar noise map, in units.
                /// Western boundary of the spherical noise map, in degrees.
                /// Lower height boundary of the cylindrical noise map, in units.
                double m_lowerVBound = 0;

                /// Upper x boundary of the planar noise map, in units.
                /// Eastern boundary of the spherical noise map, in degrees.
                /// Upper angle boundary of the cylindrical noise map, in degrees.
                double m_upperUBound = 0;

                /// Upper z boundary of the planar noise map, in units.
                /// Northern boundary of the spherical noise map, in degrees.
                /// Upper height boundary of the cylindrical noise map, in units.
                double m_upperVBound = 0;

                /// Height of the destination noise map, in points.
                uint32_t m_destHeight = 0;

                /// Width of the destination noise map, in points.
                uint32_t m_destWidth = 0;

                /// A pointer to the source noise map.
                const BaseNoise2DNode* m_sourceModule = nullptr;
        };
    }
}
