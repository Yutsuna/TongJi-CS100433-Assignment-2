/// @file Noise.glsl
// @brief Shared Procedural Functions

/// Sine Hash
float Hash11 ( float Value )
{
    return fract( sin( Value * 127.1 ) * 43758.5453123 );
}

/// Dave Hoskins
float Hash13 ( vec3 Point )
{
    Point = fract( Point * 0.1031 );
    Point += dot( Point, Point.zyx + 31.32 );
    return fract( ( Point.x + Point.y ) * Point.z );
}

/// T * T * (3.0 - 2.0 * T)
float ValueNoise ( vec3 Point )
{
    vec3 Cell     = floor( Point );
    vec3 Local    = fract( Point );
    vec3 Smoothed = Local * Local * ( 3.0 - 2.0 * Local );

    float C000 = Hash13( Cell + vec3( 0.0, 0.0, 0.0 ) );
    float C100 = Hash13( Cell + vec3( 1.0, 0.0, 0.0 ) );
    float C010 = Hash13( Cell + vec3( 0.0, 1.0, 0.0 ) );
    float C110 = Hash13( Cell + vec3( 1.0, 1.0, 0.0 ) );
    float C001 = Hash13( Cell + vec3( 0.0, 0.0, 1.0 ) );
    float C101 = Hash13( Cell + vec3( 1.0, 0.0, 1.0 ) );
    float C011 = Hash13( Cell + vec3( 0.0, 1.0, 1.0 ) );
    float C111 = Hash13( Cell + vec3( 1.0, 1.0, 1.0 ) );

    return mix( mix( mix( C000, C100, Smoothed.x ), mix( C010, C110, Smoothed.x ), Smoothed.y ),
                mix( mix( C001, C101, Smoothed.x ), mix( C011, C111, Smoothed.x ), Smoothed.y ), Smoothed.z );
}

/// Fractal noise noramlized to [0, 1]
float FractalNoise ( vec3 Point )
{
    float Sum       = 0.0;
    float Amplitude = 0.5;
    for ( int Octave = 0; Octave < 4; ++Octave )
    {
        Sum += Amplitude * ValueNoise( Point );
        Point = Point * 2.03 + vec3( 11.7 );
        Amplitude *= 0.5;
    }
    return Sum / 0.9375;
}
