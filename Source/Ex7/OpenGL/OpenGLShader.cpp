/// @file OpenGLShader.cpp
/// @brief Shader file preprocessing, compilation and linking

#include <Ex7/OpenGLShader.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace
{

[[nodiscard]] std::string ReadShaderSource ( const std::filesystem::path &Path )
{
    std::ifstream Input( Path );
    if ( not Input )
    {
        throw std::runtime_error( "Unable to open shader file: " + Path.string() );
    }

    std::ostringstream Source;
    std::string Line;
    while ( std::getline( Input, Line ) )
    {
        constexpr std::string_view IncludePrefix = "#include";
        if ( Line.starts_with( IncludePrefix ) )
        {
            const std::size_t FirstQuote  = Line.find( '"' );
            const std::size_t SecondQuote = Line.find( '"', FirstQuote + 1 );
            if ( FirstQuote == std::string::npos or SecondQuote == std::string::npos )
            {
                throw std::runtime_error( "Malformed shader include in: " + Path.string() );
            }

            const std::filesystem::path IncludePath =
                Path.parent_path() / Line.substr( FirstQuote + 1, SecondQuote - FirstQuote - 1 );
            Source << ReadShaderSource( IncludePath ) << '\n';
        }
        else
        {
            Source << Line << '\n';
        }
    }

    return Source.str();
}

[[nodiscard]] CS::UInt32 CompileShader ( const CS::UInt32 Stage, const std::filesystem::path &Path, const std::string &Source )
{
    const CS::UInt32 ShaderId = CS::GL::Api.CreateShader( Stage );
    const CS::AnsiChar *Data  = Source.data();
    CS::GL::Api.ShaderSource( ShaderId, 1, &Data, nullptr );
    CS::GL::Api.CompileShader( ShaderId );

    CS::Int32 bSucceeded = 0;
    CS::GL::Api.GetShaderParameters( ShaderId, GL_COMPILE_STATUS, &bSucceeded );
    if ( bSucceeded == GL_TRUE )
    {
        return ShaderId;
    }

    CS::Int32 LogLength = 0;
    CS::GL::Api.GetShaderParameters( ShaderId, GL_INFO_LOG_LENGTH, &LogLength );
    std::vector<CS::AnsiChar> Log( static_cast<CS::USize>( LogLength ), '\0' );
    if ( LogLength > 1 )
    {
        CS::GL::Api.GetShaderInfoLog( ShaderId, LogLength, nullptr, Log.data() );
    }
    const std::string Message( Log.data() );
    CS::GL::Api.DeleteShader( ShaderId );
    throw std::runtime_error( "Shader compilation failed (" + Path.string() + "):\n" + Message );
}

} // namespace

CS::GL::FOpenGLShader::FOpenGLShader ( FOpenGLShader &&Other ) noexcept
    : ProgramId( std::exchange( Other.ProgramId, 0 ) ), UniformLocations( std::move( Other.UniformLocations ) )
{
}

CS::GL::FOpenGLShader &CS::GL::FOpenGLShader::operator=( FOpenGLShader &&Other ) noexcept
{
    if ( this != &Other )
    {
        Release();
        ProgramId        = std::exchange( Other.ProgramId, 0 );
        UniformLocations = std::move( Other.UniformLocations );
    }
    return *this;
}

CS::GL::FOpenGLShader::~FOpenGLShader () noexcept
{
    Release();
}

CS::GL::FOpenGLShader CS::GL::FOpenGLShader::Make ( const std::filesystem::path &VertexPath,
                                                    const std::filesystem::path &FragmentPath )
{
    const std::string VertexSource   = ReadShaderSource( VertexPath );
    const std::string FragmentSource = ReadShaderSource( FragmentPath );
    const UInt32 VertexShader        = CompileShader( GL_VERTEX_SHADER, VertexPath, VertexSource );

    UInt32 FragmentShader = 0;
    try
    {
        FragmentShader = CompileShader( GL_FRAGMENT_SHADER, FragmentPath, FragmentSource );
    }
    catch ( ... )
    {
        Api.DeleteShader( VertexShader );
        throw;
    }

    FOpenGLShader Shader;
    Shader.ProgramId = Api.CreateProgram();
    Api.AttachShader( Shader.ProgramId, VertexShader );
    Api.AttachShader( Shader.ProgramId, FragmentShader );
    Api.LinkProgram( Shader.ProgramId );
    Api.DeleteShader( VertexShader );
    Api.DeleteShader( FragmentShader );

    Int32 bSucceeded = 0;
    Api.GetProgramParameters( Shader.ProgramId, GL_LINK_STATUS, &bSucceeded );
    if ( bSucceeded == GL_TRUE )
    {
        return Shader;
    }

    Int32 LogLength = 0;
    Api.GetProgramParameters( Shader.ProgramId, GL_INFO_LOG_LENGTH, &LogLength );
    std::vector<AnsiChar> Log( static_cast<USize>( LogLength ), '\0' );
    if ( LogLength > 1 )
    {
        Api.GetProgramInfoLog( Shader.ProgramId, LogLength, nullptr, Log.data() );
    }
    const std::string Message( Log.data() );
    throw std::runtime_error( "Shader linking failed:\n" + Message );
}

void CS::GL::FOpenGLShader::Use () const noexcept
{
    Api.UseProgram( ProgramId );
}

CS::Int32 CS::GL::FOpenGLShader::FindUniformLocation ( const std::string_view Name ) const noexcept
{
    const auto Existing = UniformLocations.find( Name );
    if ( Existing != UniformLocations.end() )
    {
        return Existing->second;
    }

    const Int32 Location = Api.GetUniformLocation( ProgramId, std::string( Name ).c_str() );
    UniformLocations.emplace( Name, Location );
    return Location;
}

void CS::GL::FOpenGLShader::Release () noexcept
{
    if ( ProgramId != 0 )
    {
        Api.DeleteProgram( ProgramId );
        ProgramId = 0;
    }
    UniformLocations.clear();
}
