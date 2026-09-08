{
  description = "XDG Base Directory Specification implementation in C++";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        xdgcpp = pkgs.stdenv.mkDerivation {
          pname = "xdgcpp";
          version = "0.1.0";

          src = ./.;

          cmakeFlags = [
            "-DXDG_BUILD_TESTS=ON"
          ];

          # The Boost unit-test suite is built and then executed via ctest
          # during the check phase.
          doCheck = true;

          nativeBuildInputs = with pkgs; [
            cmake
            # boost is required at configure/build time because the test
            # binary is compiled when XDG_BUILD_TESTS=ON (before check).
            boost
          ];
        };
      in
      rec {
        packages = {
          default = xdgcpp;
          inherit xdgcpp;
        };

        # `nix flake check` builds every attribute under checks.*.
        # Building the package already runs the full ctest suite via
        # doCheck, so exposing the package here is sufficient and keeps
        # failures visible under `nix flake check`.
        checks = {
          inherit xdgcpp;
        };

        apps = rec {
          default = xdgcpp-info;

          xdgcpp-info = flake-utils.lib.mkApp {
            drv = packages.xdgcpp;
            exePath = "/bin/xdgcpp-info";
          };
        };
      }
    );
}
