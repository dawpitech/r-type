{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.05";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            libcxx
            gnumake
            cmake
            just
            mold
            ninja
            act

            alsa-lib
            hidapi
            ibus
            jack2
            libdecor
            libGL
            libpulseaudio
            libusb1
            xorg.libXcursor
            xorg.libX11
            xorg.libXext
            xorg.libXinerama
            libxkbcommon
            xorg.libXrandr
            xorg.libXrender
            xorg.libXtst
            mesa
            ninja
            pipewire
            sndio
            wayland
            wayland-protocols
            freetype
          ];
        };

        formatter = pkgs.nixpkgs-fmt;
      });
}
