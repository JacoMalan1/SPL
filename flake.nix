{
  description = "NixOS dev shell flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
  let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
    system = "x86_64-linux";
  in
  {
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        gtest
      ];
      
      env = {
        CMAKE_PREFIX_PATH = "${pkgs.gtest}/include";
      };
    };
  };
}
