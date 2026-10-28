{
  pkgs,
  ...
}:
{
  nativeBuildInputs = with pkgs; [
    gnumake
    ninja
    pkg-config
    ccache
    gdb
    valgrind
  ] ++ lib.optionals pkgs.stdenv.hostPlatform.isLinux [
    mold
  ];

  buildInputs = with pkgs; [
    sdl3
    glm
  ];
}
