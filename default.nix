{ pkgs ? import <nixpkgs> {} }:

pkgs.stdenv.mkDerivation {
  pname = "my-cpp-project";
  version = "0.1.0";

  src = ./.; # your project dir

  nativeBuildInputs = [ pkgs.git pkgs.cmake pkgs.pkg-config ];
  buildInputs = [ pkgs.zlib pkgs.openssl ];

  # optional, if tests are run with ctest
  doCheck = true;

  # if you want to run cmake explicitly:
}
