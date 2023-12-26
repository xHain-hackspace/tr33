{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/23.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs { inherit system; };
          pname = "tr33";

          python-with-packages = pkgs.python311.withPackages (pp: with pp; [
            protobuf
            setuptools
          ]);
        in
        {
          devShell = pkgs.mkShell
            {
              name = "${pname}";
              buildInputs = with pkgs; [ protobuf python-with-packages beam.packages.erlangR26.elixir_1_15 ];
            };

        });
}




