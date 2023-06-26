{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-23.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs { inherit system; };
          pname = "tr33";

          python-with-packages = pkgs.python310.withPackages (pp: with pp; [
            protobuf
            setuptools
          ]);
        in
        {
          devShell = pkgs.mkShell
            {
              name = "${pname}";
              buildInputs = with pkgs; [ protobuf python-with-packages beam.packages.erlangR25.elixir_1_14 ];
            };

        });
}




