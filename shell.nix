{ pkgs ? import <nixpkgs> { } }:
with pkgs;

stdenv.mkDerivation {
    name = "c-env";
    nativeBuildInputs = [ 
        cmake 
        xorg.libXrandr
        xorg.libXinerama
        xorg.libXcursor
        xorg.libXi
        xorg.libXext
    ];
    buildInputs = [ 
        zeromq
        mesa
        libcxx
        libGL
        driversi686Linux.mesa
        glew
        freetype
        freeglut
    ];
}