/// @file SolarSystem.cpp
/// @brief Solar-system animation and shader-driven rendering

#include <Ex7/MeshBuilder.hpp>
#include <Ex7/SolarSystem.hpp>

#include <Ex7/OpenGL/OpenGLApi.hpp>
#include <Ex7/OpenGL/OpenGLMesh.hpp>
#include <Ex7/OpenGL/OpenGLShader.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <numbers>

namespace
{

constexpr CS::Float32 TwoPi = 2.F * std::numbers::pi_v<CS::Float32>;

struct FPlanet final
{
    CS::Float32 OrbitRadius;
    CS::Float32 OrbitSpeed;
    CS::Float32 OrbitAngle;
    CS::Float32 OrbitInclination;
    CS::Float32 OrbitNode;
    CS::Float32 Radius;
    CS::Float32 RotationSpeed;
    CS::Float32 RotationAngle;
    CS::Float32 AxialTilt;
    glm::fvec3 BaseColor;
    glm::fvec3 AccentColor;
    CS::Float32 PatternScale;
    CS::Float32 PatternContrast;
    CS::Float32 BandStrength;
    CS::Float32 BandFrequency;
    CS::Float32 SpecularStrength;
    CS::Float32 Shininess;
    CS::Float32 AtmosphereStrength;
};

[[nodiscard]] std::array<FPlanet, 7> MakePlanets () noexcept
{
    return { FPlanet{ 7.5F,
                      0.55F,
                      0.2F,
                      0.04F,
                      0.1F,
                      0.48F,
                      0.70F,
                      0.F,
                      0.03F,
                      { 0.25F, 0.19F, 0.15F },
                      { 0.68F, 0.48F, 0.32F },
                      3.8F,
                      1.6F,
                      0.05F,
                      5.F,
                      0.18F,
                      24.F,
                      0.03F },
             FPlanet{ 10.5F,
                      0.39F,
                      2.4F,
                      -0.06F,
                      0.8F,
                      0.84F,
                      -0.18F,
                      0.F,
                      0.05F,
                      { 0.48F, 0.25F, 0.07F },
                      { 0.95F, 0.73F, 0.32F },
                      3.1F,
                      1.2F,
                      0.48F,
                      8.F,
                      0.26F,
                      36.F,
                      0.24F },
             FPlanet{ 14.F,
                      0.30F,
                      4.1F,
                      0.08F,
                      1.6F,
                      0.94F,
                      0.85F,
                      0.F,
                      0.41F,
                      { 0.025F, 0.12F, 0.34F },
                      { 0.16F, 0.48F, 0.20F },
                      3.4F,
                      1.8F,
                      0.08F,
                      7.F,
                      0.48F,
                      64.F,
                      0.55F },
             FPlanet{ 18.F,
                      0.24F,
                      1.1F,
                      -0.10F,
                      2.1F,
                      0.66F,
                      0.80F,
                      0.F,
                      0.44F,
                      { 0.30F, 0.075F, 0.035F },
                      { 0.78F, 0.30F, 0.12F },
                      4.2F,
                      1.5F,
                      0.12F,
                      9.F,
                      0.20F,
                      28.F,
                      0.12F },
             FPlanet{ 24.F,
                      0.16F,
                      5.2F,
                      0.05F,
                      2.8F,
                      1.72F,
                      1.25F,
                      0.F,
                      0.06F,
                      { 0.32F, 0.18F, 0.10F },
                      { 0.88F, 0.72F, 0.48F },
                      2.4F,
                      1.0F,
                      0.88F,
                      15.F,
                      0.30F,
                      42.F,
                      0.20F },
             FPlanet{ 31.F,
                      0.115F,
                      3.3F,
                      -0.07F,
                      3.6F,
                      1.45F,
                      1.10F,
                      0.F,
                      0.47F,
                      { 0.40F, 0.32F, 0.17F },
                      { 0.90F, 0.79F, 0.52F },
                      2.6F,
                      0.9F,
                      0.82F,
                      18.F,
                      0.28F,
                      38.F,
                      0.18F },
             FPlanet{ 38.F,
                      0.085F,
                      0.7F,
                      0.11F,
                      4.2F,
                      1.20F,
                      0.92F,
                      0.F,
                      0.50F,
                      { 0.025F, 0.08F, 0.30F },
                      { 0.10F, 0.42F, 0.75F },
                      3.0F,
                      1.2F,
                      0.38F,
                      10.F,
                      0.52F,
                      72.F,
                      0.48F } };
}

void SetViewUniforms ( const CS::GL::FOpenGLShader &Shader, const CS::FRenderView &RenderView )
{
    Shader.SetUniform( "ViewMatrix", RenderView.View );
    Shader.SetUniform( "ProjectionMatrix", RenderView.Projection );
}

[[nodiscard]] glm::fmat4 GetOrbitOrientation ( const FPlanet &Planet ) noexcept
{
    const glm::fmat4 NodeRotation = glm::rotate( glm::fmat4{ 1.F }, Planet.OrbitNode, glm::fvec3{ 0.F, 1.F, 0.F } );
    return glm::rotate( NodeRotation, Planet.OrbitInclination, glm::fvec3{ 0.F, 0.F, 1.F } );
}

[[nodiscard]] glm::fmat4 GetPlanetModel ( const FPlanet &Planet ) noexcept
{
    const glm::fvec4 LocalPosition{ std::cos( Planet.OrbitAngle ) * Planet.OrbitRadius, 0.F,
                                    std::sin( Planet.OrbitAngle ) * Planet.OrbitRadius, 1.F };
    const glm::fvec3 WorldPosition = GetOrbitOrientation( Planet ) * LocalPosition;

    glm::fmat4 Model = glm::translate( glm::fmat4{ 1.F }, WorldPosition );
    Model            = glm::rotate( Model, Planet.AxialTilt, glm::fvec3{ 0.F, 0.F, 1.F } );
    Model            = glm::rotate( Model, Planet.RotationAngle, glm::fvec3{ 0.F, 1.F, 0.F } );
    return glm::scale( Model, glm::fvec3{ Planet.Radius } );
}

} // namespace

struct CS::FSolarSystem::FResources final
{
    GL::FOpenGLMesh Sphere = FMeshBuilder::MakeSphere();
    GL::FOpenGLMesh Quad   = FMeshBuilder::MakeQuad();
    GL::FOpenGLMesh Orbit  = FMeshBuilder::MakeOrbit();
    GL::FOpenGLMesh Stars  = FMeshBuilder::MakeStarField( 2400, 220.F, 460.F, 7331U );

    GL::FOpenGLShader StarShader   = GL::FOpenGLShader::Make( "Shaders/Ex7/Star.vert", "Shaders/Ex7/Star.frag" );
    GL::FOpenGLShader OrbitShader  = GL::FOpenGLShader::Make( "Shaders/Ex7/Orbit.vert", "Shaders/Ex7/Orbit.frag" );
    GL::FOpenGLShader SunShader    = GL::FOpenGLShader::Make( "Shaders/Ex7/Sun.vert", "Shaders/Ex7/Sun.frag" );
    GL::FOpenGLShader PlanetShader = GL::FOpenGLShader::Make( "Shaders/Ex7/Planet.vert", "Shaders/Ex7/Planet.frag" );
    GL::FOpenGLShader HaloShader   = GL::FOpenGLShader::Make( "Shaders/Ex7/Halo.vert", "Shaders/Ex7/Halo.frag" );

    std::array<FPlanet, 7> Planets = MakePlanets();

    void Update ( Float32 DeltaTime ) noexcept
    {
        for ( FPlanet &Planet : Planets )
        {
            Planet.OrbitAngle    = std::fmod( Planet.OrbitAngle + Planet.OrbitSpeed * DeltaTime, TwoPi );
            Planet.RotationAngle = std::fmod( Planet.RotationAngle + Planet.RotationSpeed * DeltaTime, TwoPi );
        }
    }

    void RenderStars ( const FRenderView &RenderView ) const noexcept
    {
        GL::Api.Disable( GL_DEPTH_TEST );
        GL::Api.DepthMask( GL_FALSE );
        GL::Api.Enable( GL_BLEND );
        GL::Api.BlendFunc( GL_ONE, GL_ONE );

        StarShader.Use();
        SetViewUniforms( StarShader, RenderView );
        StarShader.SetUniform( "Time", RenderView.TotalTime );
        StarShader.SetUniform( "TwinkleIntensity", 0.72F );
        StarShader.SetUniform( "TwinkleSpeed", 2.6F );
        const Float32 PointSizeScale = std::clamp( static_cast<Float32>( RenderView.Viewport.y ) / 720.F, 0.75F, 2.F );
        StarShader.SetUniform( "PointSizeScale", PointSizeScale );
        StarShader.SetUniform( "StarColorCold", glm::fvec3{ 0.52F, 0.68F, 1.F } );
        StarShader.SetUniform( "StarColorWarm", glm::fvec3{ 1.F, 0.79F, 0.53F } );
        Stars.Draw();
    }

    void RenderOrbits ( const FRenderView &RenderView ) const noexcept
    {
        GL::Api.Enable( GL_DEPTH_TEST );
        GL::Api.DepthFunc( GL_LEQUAL );
        GL::Api.DepthMask( GL_FALSE );
        GL::Api.BlendFunc( GL_SRC_ALPHA, GL_ONE );

        OrbitShader.Use();
        SetViewUniforms( OrbitShader, RenderView );
        OrbitShader.SetUniform( "OrbitColor", glm::fvec3{ 0.26F, 0.34F, 0.48F } );
        OrbitShader.SetUniform( "OrbitOpacity", 0.24F );

        for ( const FPlanet &Planet : Planets )
        {
            const glm::fmat4 Model = glm::scale( GetOrbitOrientation( Planet ), glm::fvec3{ Planet.OrbitRadius } );
            OrbitShader.SetUniform( "ModelMatrix", Model );
            Orbit.Draw();
        }
    }

    void RenderSun ( const FRenderView &RenderView ) const noexcept
    {
        GL::Api.Disable( GL_BLEND );
        GL::Api.DepthMask( GL_TRUE );

        glm::fmat4 Model = glm::rotate( glm::fmat4{ 1.F }, RenderView.TotalTime * 0.04F, glm::fvec3{ 0.F, 1.F, 0.F } );
        Model            = glm::scale( Model, glm::fvec3{ 3.15F } );

        SunShader.Use();
        SetViewUniforms( SunShader, RenderView );
        SunShader.SetUniform( "ModelMatrix", Model );
        SunShader.SetUniform( "CameraPosition", RenderView.Camera );
        SunShader.SetUniform( "EmissionColor", glm::fvec3{ 1.F, 0.24F, 0.015F } );
        SunShader.SetUniform( "HotColor", glm::fvec3{ 1.F, 0.78F, 0.18F } );
        SunShader.SetUniform( "EmissionIntensity", 1.55F );
        SunShader.SetUniform( "GranulationScale", 3.6F );
        SunShader.SetUniform( "GranulationSpeed", 0.11F );
        SunShader.SetUniform( "Time", RenderView.TotalTime );
        Sphere.Draw();
    }

    void RenderPlanets ( const FRenderView &RenderView ) const noexcept
    {
        PlanetShader.Use();
        SetViewUniforms( PlanetShader, RenderView );
        PlanetShader.SetUniform( "CameraPosition", RenderView.Camera );
        PlanetShader.SetUniform( "LightPosition", glm::fvec3{ 0.F } );
        PlanetShader.SetUniform( "LightColor", glm::fvec3{ 1.F, 0.91F, 0.76F } );
        PlanetShader.SetUniform( "LightIntensity", 3.F );
        PlanetShader.SetUniform( "LightFalloff", 0.0018F );
        PlanetShader.SetUniform( "AmbientStrength", 0.055F );

        for ( const FPlanet &Planet : Planets )
        {
            PlanetShader.SetUniform( "ModelMatrix", GetPlanetModel( Planet ) );
            PlanetShader.SetUniform( "BaseColor", Planet.BaseColor );
            PlanetShader.SetUniform( "AccentColor", Planet.AccentColor );
            PlanetShader.SetUniform( "PatternScale", Planet.PatternScale );
            PlanetShader.SetUniform( "PatternContrast", Planet.PatternContrast );
            PlanetShader.SetUniform( "BandStrength", Planet.BandStrength );
            PlanetShader.SetUniform( "BandFrequency", Planet.BandFrequency );
            PlanetShader.SetUniform( "SpecularStrength", Planet.SpecularStrength );
            PlanetShader.SetUniform( "Shininess", Planet.Shininess );
            PlanetShader.SetUniform( "AtmosphereStrength", Planet.AtmosphereStrength );
            Sphere.Draw();
        }
    }

    void RenderHalo ( const FRenderView &RenderView ) const noexcept
    {
        GL::Api.Enable( GL_BLEND );
        GL::Api.BlendFunc( GL_ONE, GL_ONE );
        GL::Api.DepthMask( GL_FALSE );

        HaloShader.Use();
        SetViewUniforms( HaloShader, RenderView );
        HaloShader.SetUniform( "HaloCenter", glm::fvec3{ 0.F } );
        HaloShader.SetUniform( "HaloSize", 9.2F );
        HaloShader.SetUniform( "HaloColor", glm::fvec3{ 1.F, 0.34F, 0.055F } );
        HaloShader.SetUniform( "HaloIntensity", 0.72F );
        HaloShader.SetUniform( "RayCount", 14.F );
        HaloShader.SetUniform( "RayStrength", 0.22F );
        HaloShader.SetUniform( "RayRotationSpeed", 0.05F );
        HaloShader.SetUniform( "Time", RenderView.TotalTime );
        Quad.Draw();
    }

    void Render ( const FRenderView &RenderView ) const noexcept
    {
        GL::Api.Viewport( 0, 0, RenderView.Viewport.x, RenderView.Viewport.y );
        GL::Api.ClearColor( 0.F, 0.F, 0.F, 1.F );
        GL::Api.Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        GL::Api.Enable( GL_MULTISAMPLE );
        GL::Api.Enable( GL_PROGRAM_POINT_SIZE );

        RenderStars( RenderView );
        RenderOrbits( RenderView );
        RenderSun( RenderView );
        RenderPlanets( RenderView );
        RenderHalo( RenderView );

        GL::Api.DepthMask( GL_TRUE );
        GL::Api.Disable( GL_BLEND );
        GL::Api.BindVertexArray( 0 );
    }
};

CS::FSolarSystem::FSolarSystem () : Resources( std::make_unique<FResources>() )
{
    ///
}

CS::FSolarSystem::~FSolarSystem () noexcept = default;

void CS::FSolarSystem::Update ( const Float32 DeltaTime ) noexcept
{
    Resources->Update( DeltaTime );
}

void CS::FSolarSystem::Render ( const FRenderView &RenderView ) const noexcept
{
    Resources->Render( RenderView );
}
