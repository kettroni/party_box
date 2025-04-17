{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
        {
          devShell = with pkgs; mkShell {
            buildInputs = [
              gcc
              gdb

              # X11 dependencies
              pkgs.xorg.libX11
              pkgs.xorg.libX11.dev
              pkgs.xorg.libXcursor
              pkgs.xorg.libXi
              pkgs.xorg.libXinerama
              pkgs.xorg.libXrandr
              pkgs.libGL

            ];
          };
        }
    );
}

