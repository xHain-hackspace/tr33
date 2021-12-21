{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  buildInputs = with pkgs.python39Packages; [ setuptools protobuf ];
  NIX_SHELL_NAME = "tr33";
  OTA_SECRET = "diesisteinsicherestrommelpasswort";

  shellHook = ''
    zsh
  '';
}

