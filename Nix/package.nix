{
  pkgs,
  ...
}:
let
  gccStdenv = pkgs.gcc16Stdenv;
  deps = import ./deps.nix { inherit pkgs; };
  version = builtins.head (builtins.split "\n" (builtins.readFile ../VERSION.md));
in
gccStdenv.mkDerivation {
  pname = "CS100433";
  inherit version;
  src = pkgs.lib.cleanSource ../.;

  nativeBuildInputs = deps.nativeBuildInputs;
  buildInputs = deps.buildInputs ++ [ pkgs.gcc16.cc.lib ];

  buildPhase = ''
    runHook preBuild
    make -j$NIX_BUILD_CORES
    runHook postBuild
  '';

  installPhase = ''
    runHook preInstall
    mkdir -p $out/bin
    mv Ex* $out/bin
    cp -r Shaders $out/bin/
    runHook postInstall
  '';

  meta = {
    description = "CS100433";
    platforms = pkgs.lib.platforms.all;
  };
}
